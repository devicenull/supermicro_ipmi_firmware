/**
 * $Id: backend_print.c,v 1.3 2006/10/27 13:14:21 sschuetz Exp $
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
 * Description: MOF Compiler Simple Backend
 *              This backend prints out a pseudo-data textual representation
 *              of the parsed class tree.
 *
 */

#include "backend.h"
#include "symtab.h"
#include "mofdecl.h" 
#include <stdio.h>
#include <string.h>

static void print_value(FILE * f, value_chain * va_ch)
{
  int arr = 0;
  fprintf(f,"      = "); 
  if (va_ch && va_ch -> val_next) {
    arr = 1;
    fprintf(f,"{ "); 
  }
  if (va_ch && va_ch -> val_value) {
    fprintf(f,"%s ", va_ch -> val_value); 
    va_ch = va_ch -> val_next;
  }
  while (va_ch && va_ch -> val_value) {
    fprintf(f,", %s ", va_ch -> val_value); 
    va_ch = va_ch -> val_next;
  }
  if (arr) {
  fprintf(f,"\n     }"); 
  }
  fprintf(f,";\n");
}

static void print_qualifier_chain(FILE * f,qual_chain * qu_ch)
{
  if (qu_ch) {
    fprintf(f,"  Qualifiers = {\n");
    fprintf(f,"      %s\n",qu_ch->qual_id);
    print_value(f,qu_ch->qual_vals);
    while (qu_ch -> qual_next) {
      fprintf(f,"    , %s\n",qu_ch -> qual_next -> qual_id);
      print_value(f,qu_ch->qual_next->qual_vals);
      qu_ch = qu_ch -> qual_next;
    }
    fprintf(f,"  }\n");
  }  
}

static void print_property_chain(FILE * f,prop_chain * pr_ch)
{
  if (pr_ch) {
    fprintf(f,"  Properties = {\n");
    fprintf(f,"      %s <%s>\n",pr_ch->prop_id,pr_ch->prop_class);
    print_value(f,pr_ch->prop_value);
    print_qualifier_chain(f,pr_ch->prop_quals);
    while (pr_ch -> prop_next) {
      fprintf(f,"    , %s <%s>\n",pr_ch -> prop_next -> prop_id, 
	     pr_ch -> prop_next -> prop_class);
      print_value(f,pr_ch->prop_next->prop_value);
      print_qualifier_chain(f,pr_ch->prop_next->prop_quals);
      pr_ch = pr_ch -> prop_next;
    }
    fprintf(f,"  }\n");
  }  
}

static void print_method_chain(FILE * f,method_chain * me_ch)
{
  if (me_ch) {
    fprintf(f,"  Methods = {\n");
    fprintf(f,"      %s <%s>\n",me_ch->method_id,me_ch->method_class);
    while (me_ch -> method_next) {
      fprintf(f,"    , %s <%s>\n",me_ch -> method_next -> method_id,
	     me_ch -> method_next -> method_class);
      me_ch = me_ch -> method_next;
    }
    fprintf(f,"  }\n");
  }  
}

static void print_parent_chain(FILE * f,class_entry * ce)
{
  if (ce -> class_parent) {
    fprintf(f,", Parents(");
    ce = ce -> class_parent;
    fprintf(f,"%s",ce->class_id);
    while (ce -> class_parent) {
      fprintf(f,",%s",ce -> class_parent -> class_id);
      ce = ce -> class_parent;
    }
    fprintf(f,") ");
  }  
}

static void print_class_attrs(FILE * f,class_entry * ce)
{
  if (ce -> class_attr) {
    if (ce -> class_attr & CLASS_ASSOCIATION) {
      fprintf(f,"[ASSOCIATION] ");
    }
    if (ce -> class_attr & CLASS_INDICATION) {
      fprintf(f,"[INDICATION] ");
    }
  }
}

static void print_instance(const char * outdir, const char * ns, class_entry * ie)
{
	FILE * f;
	char outfile[3000];
	
	strcpy(outfile, outdir);
	strcat(outfile, "/");
	strcat(outfile, ns);
	strcat(outfile, "/");	
	strcat(outfile, ie->class_id);
	
	f = fopen(outfile, "a");

	class_entry * ce = get_class_def_for_instance(ie);

	if(check_for_keys(ce, ie) && check_valid_props(ce, ie)) {
		
		fprintf(f,"Instance %s ",ie -> class_id);
		fprintf(f,"{\n");
		print_property_chain(f,ie -> class_props);
		fprintf(f,"}\n");
		fprintf(f,"\n");
	}
}

static void print_qualifier(FILE * f, qual_entry * q)
{		
	fprintf(f,"Qualifier %s ",q -> qual_id);
	fprintf(f,"{\n");
	print_value(f,q -> qual_defval);
	fprintf(f, "\tisArray: %d\t type: %d\n", q->qual_array, q->qual_type.type_base);
	fprintf(f,"}\n");
	fprintf(f,"\n");			
}

static void print_class(FILE * f,class_entry * ce)
{
  /* postfix processing - recursive */
  if ( ce -> class_parent) {
    print_class( f, ce -> class_parent); 
  }	
  fprintf(f,"Class %s ",ce -> class_id);
  print_class_attrs(f,ce);
  print_parent_chain(f,ce);
  fprintf(f,"{\n");
  print_qualifier_chain(f,ce -> class_quals);
  print_property_chain(f,ce -> class_props);
  print_method_chain(f,ce -> class_methods);
  fprintf(f,"}\n");
}

int backend_print(class_chain * cls_chain, class_chain * inst_chain, qual_chain * qual_chain, 
			const char * out_file, const char * out_dir,
			const char * ns, unsigned options, const char * extraopts)
{
  FILE * outfile = fopen(out_file,"w");
  if (outfile == NULL) {
    fprintf(stderr,"Failed to open %s\n",out_file);
    return 1;
  }
  
  FILE * qualfile;
  char qual_file[3000];
	
  strcpy(qual_file, out_dir);
  strcat(qual_file, "/");
  strcat(qual_file, ns);
  strcat(qual_file, "/");  
  strcat(qual_file, "qualifiers");
  qualfile = fopen(qual_file, "w");
  if (qualfile == NULL) {
    fprintf(stderr,"Failed to open %s\n",qual_file);
    return 1;
  }  
  
  while (cls_chain && cls_chain->class_item) {
    print_class(outfile, cls_chain->class_item);
    cls_chain = cls_chain -> class_next;
  }
  while (inst_chain && inst_chain->class_item) {
  	print_instance(out_dir, ns, inst_chain->class_item);
    inst_chain = inst_chain -> class_next;
  }
  while (qual_chain && qual_chain->qual_qual) {
  	print_qualifier(qualfile, qual_chain->qual_qual);
    qual_chain = qual_chain -> qual_next;
  }
  fclose(outfile);
  return 0;
}

backend_type * backend_ptr = backend_print;
