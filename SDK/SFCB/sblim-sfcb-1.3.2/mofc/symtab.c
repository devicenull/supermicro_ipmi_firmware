/**
 * $Id: symtab.c,v 1.11 2007/01/04 11:43:08 sschuetz Exp $
 *
 * (C) Copyright IBM Corp. 2004
 * 
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Viktor Mihajlovski <mihajlov@de.ibm.com>
 * Contributors: 
 *
 * Description: MOF Compiler Symbol Table
 *
 */

# include "symtab.h"
# include "mofc.h"
# include "mofdecl.h" 
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

extern hashentry * current_symtab;
static char symerrstr[300];
static void do_inheritance( class_entry * ce );

symtab_entry * make_token( int token_value )
{
  symtab_entry * symtabptr = calloc( sizeof( symtab_entry ), 1 );

  symtabptr -> sym_type = SYM_TOKEN;
  symtabptr -> sym_union.sym_token = token_value;

  return symtabptr;
}


void add_class_list(class_chain * cl_ch, class_entry * cls)
{
  while (cl_ch -> class_next) cl_ch = cl_ch -> class_next;
  cl_ch -> class_next = calloc( sizeof(class_entry), 1 );
  cl_ch -> class_item = cls;
}

void add_qual_list(qual_chain * qu_ch, qual_entry * q)
{
  while (qu_ch -> qual_next) qu_ch = qu_ch -> qual_next;
  qu_ch -> qual_next = calloc( sizeof(qual_entry), 1 );
  qu_ch -> qual_qual = q;
}

class_entry * get_class_from_list(class_chain * cl_ch, char * name)
{
  class_chain * cl_ch_local = cl_ch;
  while (cl_ch_local) {
  	if(strcmp(cl_ch_local->class_item->class_id, name))
  		cl_ch_local = cl_ch_local->class_next;
  	else
  		return cl_ch_local->class_item; 
  }
  return 0;
}

#ifndef ONEPASS
void fix_forward_decls(class_chain * cl_ch)
{
  class_chain * cl_ch_local = cl_ch;
  while (cl_ch_local -> class_next) {
    if (cl_ch_local -> class_item) {
      do_inheritance(cl_ch_local -> class_item);
    }
    cl_ch_local = cl_ch_local -> class_next;
  }  
}
#endif

qual_entry * make_qualifier_definition(hashentry * he, 
				       const char * name, 
				       type_type typeid,
				       char * arrayspec,
				       value_chain * va_ch,
				       qual_quals sf)
{
  qual_entry * qe = calloc ( sizeof(qual_entry), 1 );
  symtab_entry * se = calloc ( sizeof(symtab_entry ), 1 );

  qe -> qual_type = typeid;
  qe -> qual_array = arrayspec ? atoi(arrayspec) : -1;
  qe -> qual_defval = va_ch;
  qe -> qual_id = strdup(name);
  qe -> qual_attrs = sf; //scope&flavor
  se -> sym_type = SYM_QUAL;
  se -> sym_union.sym_qual = qe;
  if (htinsert( he, upstrdup( name, strlen( name) ), 
		strlen( name ) , se )==0) {
    sprintf(symerrstr,"redefinition of qualifier %s attempted",name);
    yyerror(symerrstr);
    qe = NULL;
  }
  return qe;
}

static qual_chain * merge_qualifiers(qual_chain * qu_ch1, qual_chain * qu_ch2)
{
  qual_chain * qu_top = qu_ch1;
  qual_chain * qu_search;
  qual_chain * qu_help;
  
  while(qu_ch2) {
    /* Special handling for ABSTRACT */
    if (strcasecmp(qu_ch2 -> qual_id,"abstract") == 0) {
      qu_ch2 = qu_ch2 -> qual_next;
      continue;
    }
    qu_search = qu_ch1;
    while (qu_search) {
      if (strcasecmp(qu_ch2 -> qual_id, qu_search -> qual_id)==0) {
	break; /* already in list */
      }
      qu_search = qu_search -> qual_next;
    }
    if (qu_search == NULL) {
      /* not found in list -- add */
      /* beware: may not modify list 2 chaining */
      qu_help = calloc( sizeof(qual_chain), 1);
      *qu_help = *qu_ch2;
      qu_help -> qual_next = qu_top;
      qu_top = qu_help;
    }
    qu_ch2 = qu_ch2 -> qual_next;
  }
  return qu_top;
}

static method_chain * merge_methods(method_chain * me_ch1, method_chain * me_ch2)
{
  method_chain * me_top = me_ch1;
  method_chain * me_search;
  method_chain * me_help;
  
  while(me_ch2) {
    me_search = me_ch1;
    while (me_search) {
      if (strcasecmp(me_ch2 -> method_id, me_search -> method_id)==0) {
	me_search -> method_quals = merge_qualifiers(me_search -> method_quals,
						     me_ch2 -> method_quals);
	break; /* already in list */
      }
      me_search = me_search -> method_next;
    }
    if (me_search == NULL) {
      /* not found in list -- add */
      /* beware: may not modify list 2 chaining */
      me_help = calloc( sizeof(method_chain), 1);
      *me_help = *me_ch2;
      me_help -> method_next = me_top;
      me_top = me_help;
    }
    me_ch2 = me_ch2 -> method_next;
  }
  return me_top;
}

static prop_chain * merge_properties(prop_chain * pr_ch1, prop_chain * pr_ch2)
{
  prop_chain * pr_top = pr_ch1;
  prop_chain * pr_search;
  prop_chain * pr_help;
  
  while(pr_ch2) {
    pr_search = pr_ch1;
    while (pr_search) {
      if (strcasecmp(pr_ch2 -> prop_id, pr_search -> prop_id)==0) {
	pr_search -> prop_quals = merge_qualifiers(pr_search -> prop_quals,
						   pr_ch2 -> prop_quals);
	pr_search->prop_attr |= pr_ch2->prop_attr;
	break; /* already in list */
      }
      pr_search = pr_search -> prop_next;
    }
    if (pr_search == NULL) {
      /* not found in list -- add */
      /* beware: may not modify list 2 chaining */
      pr_help = calloc( sizeof(prop_chain), 1);
      *pr_help = *pr_ch2;
      pr_help -> prop_next = pr_top;
      pr_top = pr_help;
    }
    pr_ch2 = pr_ch2 -> prop_next;
  }
  return pr_top;
}

static void do_inheritance( class_entry * ce ) 
{
  qual_chain * qu_ch = ce -> class_quals;
  if (ce -> class_attr & CLASS_COMPLETED) {
    return;
  }
#ifndef RELAXED_MOF 
  if (ce -> class_attr & CLASS_FORWARDDECL) {
    ce -> class_attr &= ~CLASS_FORWARDDECL; /* reset to avoid multiple msgs */
    sprintf(symerrstr,"class %s was referenced but never defined.",
	      ce -> class_id);
    yyerror(symerrstr);    
  }
#endif
  if (ce -> class_parent) {
      do_inheritance(ce -> class_parent);
  }
  /* check for association/indication qualifiers */
  while ( qu_ch ) {
    if (strcasecmp("ASSOCIATION",qu_ch -> qual_id)==0) {
#ifndef RELAXED_MOF
      if (ce -> class_parent && 
	  (ce -> class_parent -> class_attr & CLASS_ASSOCIATION) == 0) {
	sprintf(symerrstr,
		"class %s cannot be association, since parent %s is none",
		ce -> class_id , ce -> class_parent -> class_id );
	yyerror(symerrstr);
      }
#endif
      ce -> class_attr |= CLASS_ASSOCIATION;
    } else if (strcasecmp("INDICATION",qu_ch -> qual_id)==0) {
#ifndef RELAXED_MOF
      if (ce -> class_parent && 
	  (ce -> class_parent -> class_attr & CLASS_INDICATION) == 0) {
	sprintf(symerrstr,
		"class %s cannot be indication, since parent %s is none",
		ce -> class_id, ce -> class_parent -> class_id );
	yyerror(symerrstr);
      }
#endif
      ce -> class_attr |= CLASS_INDICATION;
    }
    qu_ch = qu_ch -> qual_next;
  }
  if (ce -> class_parent) {
    ce -> class_attr |= (ce -> class_parent -> class_attr);
    ce -> class_quals = merge_qualifiers(ce -> class_quals, 
					 ce -> class_parent -> class_quals);
    ce -> class_props = merge_properties(ce -> class_props, 
					 ce -> class_parent -> class_props);
    ce -> class_methods = merge_methods(ce -> class_methods, 
					ce -> class_parent -> class_methods);
  }
  ce -> class_attr |= CLASS_COMPLETED;
}

static char * get_version_qual(qual_chain * qc)
{
  qual_chain * qc_help = qc;
  while (qc_help) {
    if (strcasecmp(qc_help->qual_id,"version") == 0) {
      return qc_help->qual_vals->val_value;
    }
    qc_help = qc_help -> qual_next;
  }
  return NULL;
}

static int version_cmp(const char * v1, const char *v2)
{
  char * v1_cp, * v1_pt, * v1_fcp;
  char * v2_cp, * v2_pt, * v2_fcp;
  int  v1_num, v2_num;
  int result = 0;

  v1_fcp = v1_cp = v1 ? strdup(v1) : NULL;
  v2_fcp = v2_cp = v2 ? strdup(v2) : NULL;
  do {
    if (v1_cp) {
      if (v2_cp) {
	v1_pt = strchr(v1_cp,'.');
	v2_pt = strchr(v2_cp,'.');
	if (v1_pt) {
	  *v1_pt = '\0';
	  v1_num = atoi(v1_cp);
	  v1_cp = v1_pt + 1;
	} else {
	  v1_num = atoi(v1_cp);
	  v1_cp = NULL;
	}
	if (v2_pt) {
	  *v2_pt = '\0';
	  v2_num = atoi(v2_cp);
	  v2_cp = v2_pt + 1;
	} else {
	  v2_num = atoi(v2_cp);
	  v2_cp = NULL;
	}
	if (v1_num > v2_num) {
	  result = 1;
	  break;
	}
	if (v1_num < v2_num) {
	  result = -1;
	  break;
	};
      } else {
	/* v2_cp NULL , v1 > v2 */
	result = 1;
	break;
      }
    } else if (v2_cp) {
      /*  v1_cp NULL , v1 < v2 */
      result = -1;
      break;
    }
  } while (v1_cp || v2_cp);
  free(v1_fcp);
  free(v2_fcp);
  return result;
}

class_entry * make_class( hashentry * he,
			  qual_chain * qu_ch,
			  const char * name, 
			  class_entry * parent,
			  prop_or_method_list * pom_li)
{
  class_entry * ce = NULL;
  prop_chain   * props;
  method_chain * methods;
  char         * version_this;
  char         * version_symtab;


#ifndef RELAXED_MOF
#ifdef NAMESPACE_KLUDGE
  /* allow to define an old-style __Namespace class */
  if (strcasecmp(name,"__namespace")) {
#endif
    if (strchr(name,'_') == NULL || strchr(name,'_')==(char*)name) {
      sprintf(symerrstr,"class name %s invalid (missing schema prefix)",name);
      yyerror(symerrstr);
    }
#ifdef NAMESPACE_KLUDGE
  }
#endif
#endif
  ce = get_class_def(he,name);
  if (ce) { 
    if (ce -> class_attr & CLASS_FORWARDDECL) {
      /* remove forward declaration flag as we are defining the class */
      ce -> class_attr &= ~CLASS_FORWARDDECL;
    } else {
      /* code for upgrade check */
      version_symtab = get_version_qual(ce -> class_quals);
      version_this = get_version_qual(qu_ch);
      if (version_symtab && version_this) {
	if (version_cmp(version_symtab,version_this) >= 0) {
	  /* we found newer/same class version in symtab, keep it */
	  return ce;
	}
      } else {
	sprintf(symerrstr,"redefinition of class %s attempted (version qualifier missing)",name);
	yyerror(symerrstr);
	return NULL;
      }
    }
    /* complete class definition */
    ce -> class_quals = qu_ch;
    ce -> class_parent = parent;
    if (pom_li) { 
      ce -> class_props = pom_li -> pom_props;
      ce -> class_methods = pom_li -> pom_methods;
    }
    props = ce -> class_props;
    while (props) {
      props -> prop_class = ce -> class_id;
      props = props -> prop_next;
    }
    methods = ce -> class_methods;
    while (methods) {
      methods -> method_class = ce -> class_id;
      methods = methods -> method_next;
    }
    
  }
#ifdef ONEPASS
  do_inheritance(ce);
#endif
  return ce;
}

class_entry * make_instance( hashentry * he,
			  qual_chain * qu_ch,
			  const char * name,
			  prop_or_method_list * pom_li)
{
  class_entry * ce = malloc(sizeof(class_entry));
  prop_chain   * props;
 
  ce -> class_id = strdup(name); 
	if (pom_li) { 
	  ce -> class_props = pom_li -> pom_props;
	}
	props = ce -> class_props;
	while (props) {
	  props = props -> prop_next;
	}
    
  return ce;
}

class_entry * get_class_def( hashentry * he, const char * name )
{
  symtab_entry * se = htlookup(he, upstrdup(name,strlen(name)), strlen(name));
  class_entry * ce = NULL;
  if ( se ) {
    if ( se -> sym_type == SYM_CLASS ) {
      ce = se -> sym_union.sym_class;
    } else {
     sprintf(symerrstr,"%s is not a valid class name",name);
     yyerror(symerrstr);
    }
  } else {
      /* -- we support forward decls now - enter into hashtable !*/
    se = calloc( sizeof(symtab_entry),1 );
    ce = calloc( sizeof(class_entry),1 );
    ce -> class_id = strdup(name);
    ce -> class_attr = CLASS_FORWARDDECL;
    se -> sym_type = SYM_CLASS;
    se -> sym_union.sym_class = ce;
    if (htinsert( he, upstrdup( ce -> class_id, strlen( ce -> class_id ) ), 
		  strlen( ce -> class_id ) , se )==0) {
      sprintf(symerrstr,"internal error adding class %s to symbol table",name);
      yyerror(symerrstr);
      ce = NULL;
    }  
  }
  return ce;
}

type_type make_ref_type( hashentry * he, const char * name )
{
  return (type_type) get_class_def( he, name );
}

qual_entry * get_qual_def( hashentry * he, const char * name )
{
  symtab_entry * se = htlookup( he, upstrdup(name,strlen(name)), strlen(name) );
  if ( se && se -> sym_type == SYM_QUAL ) {
    return se -> sym_union.sym_qual;
  }
  sprintf(symerrstr,"%s is not a known qualifier",name);
  yyerror(symerrstr);
  return NULL;
}

qual_chain * make_qualifier(hashentry * he, 
			    const char * name, 
			    value_chain * val_ch )
{
  qual_chain * qu_ch = calloc( sizeof(qual_chain), 1 );

  qu_ch -> qual_id = strdup( name );
  qu_ch -> qual_qual = get_qual_def(he,name);
  qu_ch -> qual_vals = val_ch;
  if ( qu_ch -> qual_qual && qu_ch -> qual_vals == NULL) {
    if (qu_ch -> qual_qual -> qual_type.type_base == BaseTypeBOOLEAN) {
      /* BOOLEAN: if specified, it is TRUE */
      qu_ch -> qual_vals = make_value_list("true");
    }  else {
      qu_ch -> qual_vals = qu_ch -> qual_qual -> qual_defval;
    }    
  }
  return qu_ch;
}

void qualifier_list_add( qual_chain * qu_ch1, qual_chain * qu_ch2 )
{
  while (qu_ch1 -> qual_next) qu_ch1 = qu_ch1 -> qual_next;
  qu_ch1 -> qual_next = qu_ch2;
}

value_chain * make_value_list(const char * value)
{
  value_chain * val_ch = calloc( sizeof(value_chain), 1 );

  val_ch -> val_value = strdup( value );
  return val_ch;
}

void value_list_add(value_chain * val_ch, const char * val)
{
  while (val_ch -> val_next) val_ch = val_ch -> val_next;
  val_ch -> val_next = calloc( sizeof(value_chain), 1 );
  val_ch -> val_next -> val_value = strdup( val );
}

char * make_string(const char * string)
{
  char * dups;
  char *strstart = strchr(string,'"');
  char *strend = strrchr(string,'"');
  dups = calloc( strend - strstart, 1 );
  
  int length = strlen(string);
  int a,i;
  for(i = 1, a=0; i < length-1; i++) {
  	//eliminate escaping backslashs
  	if(!(string[i] == '\\' && string[i+1] == '"')) {
  		dups[a++] = string[i];
  	}
  }
  //strncpy(dups,strstart + 1,strend-strstart - 1);
  return dups;
}

char * make_char(const char * string)
{
  char * dups;
  char *strstart = strchr(string,'\'');
  char *strend = strrchr(string,'\'');
  dups = calloc( strend - strstart, 1 );
  strncpy(dups,strstart + 1,strend-strstart - 1);
  return dups;
}

char * append_string(char * string1, const char * string2)
{
  char * newstring  = malloc ( strlen( string1 ) + strlen( string2 ) + 1 );
  sprintf(newstring,"%s%s",string1,string2);
  return newstring;
}

prop_or_method_list * make_pom_list(qual_chain * qu_ch,
				    type_type typeid,
				    const char * name,
				    const char * arrayspec,
				    param_chain * pa_ch,
				    value_chain * va_ch
				    )
{
  prop_or_method_list * pom_li = calloc( sizeof(prop_or_method_list), 1 );
  
  if (pa_ch) {
    /* this item is a method */
    pom_li -> pom_methods = make_method_chain(qu_ch,typeid,name,arrayspec,
					      pa_ch);
  } else {
    pom_li -> pom_props = make_property_chain(qu_ch,typeid,name,arrayspec,
					      va_ch);
  }
  return pom_li;
}
  
prop_chain * make_property_chain(qual_chain * qu_ch,
				 type_type typeid,
				 const char * name,
				 const char * arrayspec,
				 value_chain * va_ch
				 )
{
  prop_chain * pr_ch = calloc( sizeof(prop_chain), 1);

  pr_ch -> prop_quals = qu_ch;
  pr_ch -> prop_type = typeid;
  pr_ch -> prop_id = strdup(name);
  pr_ch -> prop_array = arrayspec ? atoi(arrayspec) : -1;
  pr_ch -> prop_value = va_ch;
  
  while ( qu_ch ) {
    if (strcasecmp("KEY",qu_ch -> qual_id)==0) {
      pr_ch -> prop_attr |= PROPERTY_KEY;
    }
    qu_ch = qu_ch -> qual_next;
  }
  return pr_ch;
}

method_chain * make_method_chain(qual_chain * qu_ch,
				 type_type typeid,
				 const char * name,
				 const char * arrayspec,
				 param_chain * pa_ch
				 )
{
  method_chain * me_ch = calloc( sizeof(method_chain), 1);

  me_ch -> method_quals = qu_ch;
  me_ch -> method_type = typeid;
  me_ch -> method_id = strdup(name);
  me_ch -> method_array = arrayspec ? atoi(arrayspec) : -1;
  me_ch -> method_params = pa_ch;
  
  return me_ch;
}

void pom_list_add(prop_or_method_list * pom_li1, prop_or_method_list * pom_li2)
{
  method_chain * me_ch1 = pom_li1 -> pom_methods;
  prop_chain * pr_ch1 = pom_li1 -> pom_props;
  if (pr_ch1) {
    while (pr_ch1 -> prop_next) pr_ch1 = pr_ch1 -> prop_next;
    pr_ch1 -> prop_next = pom_li2 -> pom_props;
  } else {
    pom_li1 -> pom_props = pom_li2 -> pom_props;
  }
  if (me_ch1) {
    while (me_ch1 -> method_next) me_ch1 = me_ch1 -> method_next;
    me_ch1 -> method_next = pom_li2 -> pom_methods;
  } else {
    pom_li1 -> pom_methods = pom_li2 -> pom_methods;
  }
}

param_chain * make_param_list(qual_chain * qu_ch,
			      type_type typeid,
			      const char * name,
			      const char * arrayspec
			      )
{
  param_chain * pa_ch = calloc( sizeof(param_chain), 1 );
  
  if (name == NULL) {
    /* empty parameter -- needed to identify methods */
    return pa_ch;
  }
  pa_ch -> param_quals = qu_ch;
  pa_ch -> param_type = typeid;
  pa_ch -> param_id = strdup(name);
  pa_ch -> param_array = arrayspec ? atoi(arrayspec) : -1;
  
  while ( qu_ch ) {
    if (strcasecmp("IN",qu_ch -> qual_id)==0) {
      pa_ch -> param_attr |= PARAMETER_IN;
    } else if (strcasecmp("OUT",qu_ch -> qual_id)==0) {
      pa_ch -> param_attr |= PARAMETER_OUT;
    }
    qu_ch = qu_ch -> qual_next;
  }
  return pa_ch;
}

void param_list_add(param_chain * pa_ch1, param_chain * pa_ch2)
{
  while (pa_ch1 -> param_next) pa_ch1 = pa_ch1 -> param_next;
  pa_ch1 -> param_next = pa_ch2;
}

prop_chain * check_for_prop(class_entry * e, char * prop_id)
{
	prop_chain * props = e->class_props;
	
	while (props) {
		if(strcmp(props->prop_id, prop_id)) 
			props = props->prop_next;
		else
			return props;
	}
	return 0;
} 

int check_for_keys(class_entry * ce, class_entry * ie)
{
	prop_chain * classprop = ce->class_props;
	prop_chain * tempprop = NULL;
	
	while(classprop) {
		if(classprop->prop_attr & PROPERTY_KEY) {
			//found key - see if it's defined in the instance
			tempprop = check_for_prop(ie, classprop->prop_id);
			if (!tempprop) {
				sprintf(symerrstr,"not all keys defined for %s",ce->class_id);
				yyerror(symerrstr);
				return 0;
			}
		}
		classprop = classprop->prop_next;
	}
	return 1;
}

int check_valid_props(class_entry * ce, class_entry * ie)
{
	prop_chain * instprop = ie->class_props;
	
	while(instprop) {
		if(!check_for_prop(ce, instprop->prop_id)) {
			sprintf(symerrstr,"invalid property %s defined for %s", instprop->prop_id,ce->class_id);
			yyerror(symerrstr);
			return 0;				
		}
		instprop = instprop->prop_next;
	}
	return 1;
}

class_entry * get_class_def_for_instance(class_entry * ie)
{
	char * className = ie->class_id;
	
	symtab_entry * se = htlookup(current_symtab, upstrdup(className,strlen(className)), strlen(className));
	if(se && !(se->sym_union.sym_class->class_attr & CLASS_FORWARDDECL)) {
		return se->sym_union.sym_class;
	}
	else {
		sprintf(symerrstr,"class definition for %s not found",className);
		yyerror(symerrstr);		
	}
	return NULL;
}

int make_scope(char * scopeVal)
{
	if(!strcasecmp(scopeVal, "class")) return Qual_S_Class;
	if(!strcasecmp(scopeVal, "association")) return Qual_S_Association;
	if(!strcasecmp(scopeVal, "reference")) return Qual_S_Reference;
	if(!strcasecmp(scopeVal, "property")) return Qual_S_Property;
	if(!strcasecmp(scopeVal, "method")) return Qual_S_Method;
	if(!strcasecmp(scopeVal, "parameter")) return Qual_S_Parameter;
	if(!strcasecmp(scopeVal, "indication")) return Qual_S_Indication;
	if(!strcasecmp(scopeVal, "any")) return Qual_S_Any;

	sprintf(symerrstr,"invalid scope %s", scopeVal);
	yyerror(symerrstr);	
	return 0;
}

int make_flavor(char * flavorVal)
{
	if(!strcasecmp(flavorVal, "enableoverride")) return 0; // because it's default
	if(!strcasecmp(flavorVal, "disableoverride")) return Qual_F_DisableOverride;
	if(!strcasecmp(flavorVal, "tosubclass")) return 0; // because it's default
	if(!strcasecmp(flavorVal, "restricted")) return Qual_F_Restricted;
	if(!strcasecmp(flavorVal, "translatable")) return Qual_F_Translatable;

	sprintf(symerrstr,"invalid flavor %s", flavorVal);
	yyerror(symerrstr);	
	return 0;
}
