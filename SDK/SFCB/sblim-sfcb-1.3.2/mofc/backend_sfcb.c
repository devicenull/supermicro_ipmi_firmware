/**
 * $Id: backend_sfcb.c,v 1.19 2007/08/28 08:37:13 sschuetz Exp $
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
 * Description: MOF Compiler sfcb Backend
 *              This backend produces Small Footprint CIM Broker
 *              objects for the parsed CIM classes.
 *
 */

#include "cmpidt.h"
#include "objectImpl.h"
#include "fileRepository.h"
#include "mofdecl.h"
#include "symtab.h"
#include "mofc.h"
#include "hash.h"
#include "backend.h"
#include "objectpath.h"
#include <sys/utsname.h>


extern CMPIStatus sfcb_simpleArrayAdd(CMPIArray * array, CMPIValue * val, CMPIType type);
extern CMPIObjectPath *getObjectPath(char *path, char **msg);

extern CMPIBroker *Broker;

static unsigned sfcb_options = BACKEND_DEFAULT;
static int endianMode = SFCB_LOCAL_ENDIAN;

typedef void *(*SwapClass)(ClClass * cls, int *size);
typedef void *(*SwapInstance)(ClInstance * inst, int *size);
typedef void *(*SwapQualifier)(ClQualifierDeclaration * inst, int *size);
static void *swapLib;
static SwapClass swapEntryClass;
static SwapInstance swapEntryInstance;
static SwapQualifier swapEntryQualifier;
static int swapMode=0;

extern  char * repfn; //in fileRepository - rep location

#define BACKEND_SFCB_NO_QUALIFIERS      0x0100
#define BACKEND_SFCB_REDUCED_QUALIFIERS 0x0200

CMPIType make_cmpi_type(type_type lextype, int arrayspec)
{
  int array_flag = arrayspec == -1 ? 0 : CMPI_ARRAY;
  switch (lextype.type_base) {
  case BaseTypeUINT8:
    return CMPI_uint8 | array_flag;
  case BaseTypeSINT8:
    return CMPI_sint8 | array_flag;
  case BaseTypeUINT16:
    return CMPI_uint16 | array_flag;
  case BaseTypeSINT16:
    return CMPI_sint16 | array_flag;
  case BaseTypeUINT32:
    return CMPI_uint32 | array_flag;
  case BaseTypeSINT32:
    return CMPI_sint32 | array_flag;
  case BaseTypeUINT64:
    return CMPI_uint64 | array_flag;
  case BaseTypeSINT64:
    return CMPI_sint64 | array_flag;
  case BaseTypeREAL32:
    return CMPI_real32 | array_flag;
  case BaseTypeREAL64:
    return CMPI_real64 | array_flag;
  case BaseTypeCHAR16:
    return CMPI_char16 | array_flag;
  case BaseTypeSTRING:
    return CMPI_string | array_flag;
  case BaseTypeBOOLEAN:
    return CMPI_boolean | array_flag;
  case BaseTypeDATETIME:
    return CMPI_dateTime | array_flag;
  default:
    return CMPI_ref | array_flag;
  }  
}

static CMPIData make_cmpi_data( type_type lextype, int arrayspec, 
				value_chain * vals )
{
  CMPIData data;
  CMPIData arr_data;
  int i = 0;
  CMPIStatus st;
  
  data.type = make_cmpi_type(lextype,arrayspec);
  data.value.uint64 = 0L;        /* set to binary zeros */
  
  if ( vals == NULL )  {
    data.state = CMPI_nullValue;
  } else if (vals -> val_value == NULL) {
    fprintf (stderr,"*** fatal error in backend: NULL value recieved.\n");
    abort();  /* paranoia */
  }else { 
    data.state = CMPI_goodValue;
  }    
  
  if ( data.state == CMPI_goodValue ) {
    if (data.type & CMPI_ARRAY) {
      /* process array entries */
      data.value.array = 
	CMNewArray(Broker,0,data.type&~CMPI_ARRAY,NULL);
      while (vals && vals -> val_value) {
	arr_data = make_cmpi_data(lextype,-1,vals);
     st=sfcb_simpleArrayAdd(data.value.array, &arr_data.value, data.type&~CMPI_ARRAY);
	i++;
	vals = vals -> val_next;
      }
    } else {
      switch (data.type & ~CMPI_ARRAY) {
      case CMPI_uint8:
	sscanf(vals -> val_value, "%hhu", &data.value.uint8 );
	break;
      case CMPI_sint8:
	sscanf(vals -> val_value, "%hhd", &data.value.sint8 );
	break;
      case CMPI_uint16:
	sscanf(vals -> val_value, "%hu", &data.value.uint16 );
	break;
      case CMPI_sint16:
	sscanf(vals -> val_value, "%hd", &data.value.sint16 );
	break;
      case CMPI_uint32:
	sscanf(vals -> val_value, "%u", &data.value.uint32 );
	break;
      case CMPI_sint32:
	sscanf(vals -> val_value, "%d", &data.value.sint32 );
	break;
      case CMPI_uint64:
	sscanf(vals -> val_value, "%llu", &data.value.uint64);
	break;
      case CMPI_sint64:
	sscanf(vals -> val_value, "%lld", &data.value.uint64 );
	break;
      case CMPI_real32:
	sscanf(vals -> val_value, "%f", &data.value.real32 );
	break;
      case CMPI_real64:
	sscanf(vals -> val_value, "%lf", &data.value.real64 );
	break;
      case CMPI_char16:
	/* this one is suspect to produce garbage */
	sscanf(vals -> val_value, "%c", &data.value.uint8 );
	break;
      case CMPI_string:
	data.value.string = CMNewString(Broker,vals -> val_value,NULL);
	break;
      case CMPI_boolean:
	if (strcasecmp("true",vals -> val_value) == 0) {
	  data.value.boolean = 1;
	} else {
	  data.value.boolean = 0;
	}
	break;
      case CMPI_dateTime:
	data.value.dateTime = 
	  CMNewDateTimeFromChars(Broker,vals -> val_value,NULL);
	if (data.value.dateTime == NULL) {
	  fprintf(stderr,"failed to build datetime from %s", vals -> val_value);
	  data.state = CMPI_nullValue;
	}
	break;
      default:  {
			char *msg=NULL;
			data.value.ref = getObjectPath(vals -> val_value, &msg);
			if(msg) {
				fprintf(stderr, "Problem with Objectpath %s: %s\n", vals -> val_value, msg);
				exit(0);
			}
	    }
      }
    }
  }
  return data;
}

static void sfcb_add_version(FILE * f, unsigned short opt, int endianMode)
{
   ClVersionRecord rec;
   long size;
   
   rec = ClBuildVersionRecord(opt, endianMode, &size);
   fwrite(&rec,size,1,f);
}

static int sfcb_add_class(FILE * f, hashentry * he, class_entry * ce, int endianMode)
{
  /* SFCB related */
  ClClass * sfcbClass;
  ClClass * sfcbClassRewritten;
  ClProperty * sfcbProp;
  ClMethod * sfcbMeth;
  ClParameter * sfcbParam;
  CMPIParameter param;
  CMPIData data;
  int prop_id;
  int qual_id;
  int meth_id;
  int meth_param_id;
  int size;
  /* Symtab related */
  qual_chain * quals = ce -> class_quals;
  prop_chain * props = ce -> class_props;
  method_chain * meths = ce -> class_methods;
  param_chain * meth_params;
  

  /* postfix processing - recursive */
  if ( ce -> class_parent) {
    sfcb_add_class( f, he, ce -> class_parent, endianMode ); 
  }
  if ( htlookup( he, 
		 upstrdup(ce -> class_id, 
			  strlen(ce -> class_id)), 
		 strlen(ce -> class_id))
       == NULL ) {
    if (sfcb_options & BACKEND_VERBOSE) {
      fprintf(stderr,"  adding class %s \n", ce -> class_id );
    }
    /* remember we did this class already */
    htinsert( he, 
	      upstrdup(ce -> class_id, 
		       strlen(ce -> class_id)), strlen(ce -> class_id),
	      (void *)1); 
    sfcbClass = ClClassNew( ce -> class_id, 
			    ce -> class_parent ? 
			    ce -> class_parent -> class_id : NULL );
    if (sfcbClass == NULL) {
      fprintf(stderr,"Error: could not create SFCB class for %s\n",ce -> class_id);
      return 1;
    }
    while (quals) {
      if (sfcb_options & BACKEND_VERBOSE) {
	fprintf(stderr,"  adding qualifier %s for class %s \n", 
		quals -> qual_id, ce -> class_id );
      }
      qual_id =  
	ClClassAddQualifier(&sfcbClass->hdr, &sfcbClass->qualifiers,
			    quals->qual_id, 
			    make_cmpi_data(quals->qual_qual->qual_type,
					   quals->qual_qual->qual_array,
					   quals->qual_vals));
      quals = quals -> qual_next;
    }
    while (props) {
      if (sfcb_options & BACKEND_VERBOSE) {
	fprintf(stderr,"  adding property %s for class %s \n", 
		props -> prop_id, ce -> class_id );
      }
      data = make_cmpi_data(props->prop_type, props->prop_array, props->prop_value);
      prop_id = ClClassAddProperty(sfcbClass, props->prop_id, 
                                 data, data.type == CMPI_ref ? props->prop_type.type_ref->class_id : NULL);
      if (prop_id == 0) {
	fprintf(stderr,"Error: could not add SFCB class property %s for %s\n",
		props -> prop_id, ce -> class_id);
	return 1;
      }
      quals = props -> prop_quals;
      sfcbProp=((ClProperty*)ClObjectGetClSection(&sfcbClass->hdr,&sfcbClass->properties))+prop_id-1;
      while (quals) {
	if ( (sfcb_options & BACKEND_SFCB_REDUCED_QUALIFIERS) == 0 ||
	    (strcasecmp("description",quals->qual_id) &&
	     strcasecmp("valuemap",quals->qual_id) &&
	     strcasecmp("values",quals->qual_id) ) ) {
	  if (sfcb_options & BACKEND_VERBOSE) {
	    fprintf(stderr,"  adding qualifier %s for property %s in class %s\n", 
		    quals -> qual_id, props -> prop_id, ce -> class_id );
	  }
	  qual_id = 
	    ClClassAddPropertyQualifier(&sfcbClass->hdr, 
					sfcbProp,
					quals->qual_id, 
					make_cmpi_data(quals->qual_qual->qual_type,
						       quals->qual_qual->qual_array,
						       quals->qual_vals));
	}
	quals = quals -> qual_next;
      }
      props = props -> prop_next;
    }
    while (meths) {
    	meth_id = ClClassAddMethod(sfcbClass, meths->method_id, 
    		make_cmpi_type(meths->method_type, meths->method_array));
    	quals = meths->method_quals;
    	sfcbMeth=((ClMethod*)ClObjectGetClSection(&sfcbClass->hdr,&sfcbClass->methods))+meth_id-1;
    	while(quals) {
    		ClClassAddMethodQualifier(&sfcbClass->hdr,
    				sfcbMeth,
    				quals->qual_id,
    				make_cmpi_data(quals->qual_qual->qual_type,
					       quals->qual_qual->qual_array,
					       quals->qual_vals));
			quals = quals->qual_next;
    	}
    	meth_params = meths->method_params;
    	while(meth_params && meth_params->param_id) {
    		param.type = make_cmpi_type(meth_params->param_type, meth_params->param_array);
    		param.arraySize = meth_params->param_array;
    		if((param.type == CMPI_ref || param.type == CMPI_refA) && meth_params->param_type.type_ref) {
    			param.refName = meth_params->param_type.type_ref->class_id;
    		} else {
    			param.refName = NULL;
    		}
    		meth_param_id = ClClassAddMethParameter(&sfcbClass->hdr,
    							sfcbMeth,
    							meth_params->param_id,
    							param);
    		quals = meth_params->param_quals;
    		sfcbParam=((ClParameter*)ClObjectGetClSection(&sfcbClass->hdr,&sfcbMeth->parameters))+meth_param_id-1;
    		while(quals) {
    			ClClassAddMethParamQualifier(&sfcbClass->hdr,
    								sfcbParam,
    								quals->qual_id,
    								make_cmpi_data(quals->qual_qual->qual_type,
					       				quals->qual_qual->qual_array,
					       				quals->qual_vals));
				quals = quals->qual_next;   								
    						
    		}
    		meth_params = meth_params->param_next;
    	}
    	meths = meths->method_next;
    }    
    
    sfcbClassRewritten = ClClassRebuildClass(sfcbClass,NULL);
    size=sfcbClassRewritten->hdr.size;

    if (swapMode) {
       void *tmp=sfcbClassRewritten;
       sfcbClassRewritten = swapEntryClass(sfcbClassRewritten,&size);
       free(tmp);
    }
       
    fwrite(sfcbClassRewritten,size,1,f);

    free(sfcbClassRewritten);
  }
  return 0;
}

CMPIObjectPath * mofc_getObjectPath(class_entry * ce, class_entry * ie, const char * ns)
{
	prop_chain * classprop = ce->class_props;
	prop_chain * tempprop = NULL;
	CMPIObjectPath *path;
	CMPIData data;
	
	path = NewCMPIObjectPath(ns, ie->class_id, NULL);
	
	while(classprop) {
		if(classprop->prop_attr & PROPERTY_KEY) { //found key - see if it's defined in the instance
			tempprop = check_for_prop(ie, classprop->prop_id);
			if (tempprop) {
				data = make_cmpi_data(classprop->prop_type, classprop->prop_array, tempprop->prop_value);
				path->ft->addKey(path, tempprop->prop_id, &data.value, data.type);
			} else {
				//should not happen since keys have been checked before
				fprintf(stderr,"  not all keys defined for %s\n", ce->class_id);				
			}
		}
		classprop = classprop->prop_next;
	}
	return path;
}

int sfcb_add_qualifier(qual_entry * q, const char * ns)
{
	ClQualifierDeclaration * qual;
	CMPIQualifierDecl * cmpi_qual = malloc(sizeof(CMPIQualifierDecl));
	void * blob;
	int len, size;
	CMPIData d;
	
	qual = ClQualifierDeclarationNew(ns, q->qual_id);
	qual->type = make_cmpi_type(q->qual_type, q->qual_array);
	d = make_cmpi_data(q->qual_type, q->qual_array, q->qual_defval);
	ClQualifierAddQualifier(&qual->hdr, &qual->qualifierData, q->qual_id, d);
	
	//negate the 2 to match obejctImpl's representation
	if(q->qual_attrs.flavor & Qual_F_DisableOverride)
		q->qual_attrs.flavor &= ~ Qual_F_DisableOverride;
	else
		q->qual_attrs.flavor |= Qual_F_DisableOverride;
	if(q->qual_attrs.flavor & Qual_F_Restricted)
		q->qual_attrs.flavor &= ~ Qual_F_Restricted;
	else
		q->qual_attrs.flavor |= Qual_F_Restricted;
		
	qual->flavor = q->qual_attrs.flavor;
	qual->scope = q->qual_attrs.scope;

	cmpi_qual->hdl=qual;
 	len=getQualifierSerializedSize(cmpi_qual);
 	blob=malloc(len);
	getSerializedQualifier(cmpi_qual,blob);
	
    if (swapMode) {
		size=((ClQualifierDeclaration *)((CMPIQualifierDecl * )blob)->hdl)->hdr.size;
		int offset = len-size;
		qual = swapEntryQualifier(blob+offset,&size);
		len = offset + size;
		blob = realloc(blob, len);
		memcpy(blob+offset, qual, size);
		free(qual);
    }

	if (addBlob((char*)ns, "qualifiers", q->qual_id, blob,(int)len)) {
		fprintf(stderr,"could not write qualifier %s\n",q->qual_id);
		return 1;
	}
	free(blob);		
	return 0;
}

int sfcb_add_instance(class_entry * ie, const char * ns)
{	
	void * blob;
	int len, size;
	CMPIData data;
	class_entry * ce=NULL; //class definition for ie
	CMPIObjectPath * path;
	ClInstance * inst;
	CMPIInstance* cmpi_instance = malloc(sizeof(CMPIInstance));
	prop_chain * class_prop;
	prop_chain * inst_props = ie -> class_props;
	
	
	ce = get_class_def_for_instance(ie);
	if(!ce) {
		//class def not found
		fprintf(stderr,"class definition for %s not found\n",
			ie->class_id);
		return 1;
	}

	path = mofc_getObjectPath(ce, ie, ns);
	inst = ClInstanceNew(ns, ie->class_id);

    while (inst_props) {
		if (sfcb_options & BACKEND_VERBOSE) {
			fprintf(stderr,"  adding property %s for instance %s \n", 
			inst_props -> prop_id, ie -> class_id );
		}
		class_prop = check_for_prop(ce, inst_props->prop_id);
		data = make_cmpi_data(class_prop->prop_type, class_prop->prop_array, inst_props->prop_value);
		ClInstanceAddProperty(inst, inst_props->prop_id, data);
		inst_props = inst_props->prop_next;
    }

	cmpi_instance->hdl=inst;
 	len=getInstanceSerializedSize(cmpi_instance);
 	blob=malloc(len);
	getSerializedInstance(cmpi_instance,blob);

    if (swapMode) {
		size=((ClInstance *)((CMPIInstance * )blob)->hdl)->hdr.size;
		int offset = len-size;
		inst = swapEntryInstance(blob+offset,&size);
		len = offset + size;
		blob = realloc(blob, len);
		memcpy(blob+offset, inst, size);
		free(inst);
    }
	
	if (addBlob((char*)ns,ie->class_id,normalizeObjectPathChars(path),blob,(int)len)) {
		fprintf(stderr,"could not write instance for class definition %s\n",ie->class_id);
		return 1;
	}
	free(blob);
	return 0;
}

int backend_sfcb(class_chain * cls_chain, class_chain * inst_chain, qual_chain * qual_chain,
			const char * outfile, const char * outdir,
			const char * ns, unsigned options, const char * extraopts)
{
  hashentry * classes_done = htcreate("SFCB");
  short test = 1;
  char *tp = (char*)&test;
  struct utsname uName;
  static int ix86=1,first=1;
  FILE      * class_file = NULL;// = fopen(outfile, "w");
  
  if(cls_chain && cls_chain->class_item) {
  	class_file = fopen(outfile, "w");
  }
 
  if(outdir)
  	repfn = strdup(outdir);
  else
  	repfn = strdup("./");
   
  if (first) {
     uname(&uName);
     if (uName.machine[0]!='i' || strcmp(uName.machine+2,"86")!=0) ix86=0;
     first=0;
  }
    
  /*if (class_file == NULL) {
    return 1;
  }*/
  sfcb_options = options;
  
  if (strchr(extraopts,'Q')) {
    sfcb_options |= BACKEND_SFCB_NO_QUALIFIERS;
    sfcb_options |= BACKEND_SFCB_REDUCED_QUALIFIERS;
  } else if (strchr(extraopts,'q')) {
    sfcb_options |= BACKEND_SFCB_REDUCED_QUALIFIERS;
    if (sfcb_options & BACKEND_VERBOSE) {
      fprintf(stderr,"  information: omitting selected qualifiers.\n");
    }
  }
  
  if (tp[0]==1) endianMode = SFCB_LITTLE_ENDIAN;
  else if (tp[1]==1) endianMode = SFCB_BIG_ENDIAN;
  
  if (strstr(extraopts,"P32")) {
    if (ix86) {
       char libName[]="libsfcObjectImplSwapI32toP32.so";
       char entryNameClass[]="swapI32toP32Class";
       char entryNameInstance[]="swapI32toP32Instance";
       char entryNameQualifier[]="swapI32toP32QualifierDeclaration";
       char *error;
       
       endianMode=SFCB_BIG_ENDIAN;
       swapLib=dlopen(libName, RTLD_LAZY);
       if (swapLib==NULL) {
          fprintf (stderr, "--- swap library not found - %s\n", dlerror());
          exit(16);
       }
       dlerror();
       swapEntryClass = dlsym(swapLib, entryNameClass);
       swapEntryInstance = dlsym(swapLib, entryNameInstance);
       swapEntryQualifier = dlsym(swapLib, entryNameQualifier);       
       if ((error = dlerror()) != NULL)  {
          fprintf (stderr, "--- swap library entry not found - %s\n", error);
          exit(16);
       }
       swapMode=1;
    }
    else {
       fprintf(stderr,"--- backend_sfcb: P32 option can only be used on ix86 class machines\n");
       exit(16);
    }
  }
  
  if(class_file)
  	sfcb_add_version(class_file, ClTypeClassRep, endianMode);

  while (cls_chain && cls_chain->class_item) {
    if (sfcb_add_class(class_file, classes_done, cls_chain->class_item,endianMode)) {
      return 1;
    }
    cls_chain = cls_chain -> class_next;
  }
  while (inst_chain && inst_chain->class_item) {
  	if (sfcb_add_instance(inst_chain->class_item, ns)) {
  	  return 1;
  	}
    inst_chain = inst_chain -> class_next;
  }
  while (qual_chain && qual_chain->qual_qual) {
  	if(sfcb_add_qualifier(qual_chain->qual_qual, ns)) {
  		return 1;
  	}
    qual_chain = qual_chain -> qual_next;
  }
  
  if(class_file) fclose(class_file);
  return 0;
}

backend_type * backend_ptr = backend_sfcb;
