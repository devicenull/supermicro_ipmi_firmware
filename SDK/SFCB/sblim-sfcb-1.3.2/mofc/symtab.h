/**
 * $Id: symtab.h,v 1.6 2007/01/03 11:44:08 sschuetz Exp $
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

# if ! defined SYMTABH
# define SYMTABH

# include <hash.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define CLASS_ASSOCIATION 0x0001
#define CLASS_INDICATION  0x0002
#define CLASS_FORWARDDECL 0x0100
#define CLASS_COMPLETED   0x0200

#define PROPERTY_KEY      0x0001

#define PARAMETER_IN      0x0001
#define PARAMETER_OUT     0x0002

//Qualifier Flavor
#define Qual_F_DisableOverride  1 //default is Overriddable,different default than objectImpl
#define Qual_F_Restricted       2 //default is ToSubclass,different default than objectImpl
#define Qual_F_ToInstance       4 //deprecated
#define Qual_F_Translatable     8 //default is no

//Qualifier Scope
#define Qual_S_Class         1
#define Qual_S_Association   2
#define Qual_S_Reference     4
#define Qual_S_Property      8
#define Qual_S_Method       16
#define Qual_S_Parameter    32
#define Qual_S_Indication   64
#define Qual_S_Any         255

extern void yyerror( char * );  /* introduce YACC error function */

typedef struct class_struct class_entry;

typedef struct class_ch_struct {
  class_entry            * class_item;
  struct class_ch_struct * class_next;
} class_chain;

typedef union {
  int           type_base;
  class_entry * type_ref;
} type_type;

typedef struct value_struct {
  char                * val_value;
  struct value_struct * val_next;   /* link pointer */
} value_chain;

typedef struct qual_quals_struct {
  unsigned char scope;
  unsigned char flavor;
} qual_quals;

typedef struct qual_def_struct {
  char                * qual_id;
  type_type             qual_type;
  int                   qual_array;
  value_chain         * qual_defval;
  qual_quals			qual_attrs; //scope&flavor
} qual_entry;

typedef struct qual_struct {
  char               * qual_id;
  qual_entry         * qual_qual;
  value_chain        * qual_vals;
  struct qual_struct * qual_next;   /* link pointer */
} qual_chain;

typedef struct param_struct {
  char                * param_id;
  unsigned              param_attr;
  type_type             param_type;
  int                   param_array;
  value_chain         * param_value;
  qual_chain          * param_quals; 
  struct param_struct * param_next;   /* link pointer */
} param_chain;

typedef struct method_struct {
  char                 * method_id;
  char                 * method_class;
  type_type              method_type;
  int                    method_array;
  qual_chain           * method_quals; 
  param_chain          * method_params; 
  struct method_struct * method_next;   /* link pointer */
} method_chain;

typedef struct prop_struct {
  char               * prop_id;
  char               * prop_class;
  unsigned             prop_attr;
  type_type            prop_type;
  int                  prop_array;
  value_chain        * prop_value;
  qual_chain         * prop_quals; 
  struct prop_struct * prop_next;   /* link pointer */
} prop_chain;

typedef struct prop_or_method_struct {
  prop_chain   * pom_props;
  method_chain * pom_methods; 
} prop_or_method_list;

struct class_struct {
  char                * class_id;
  unsigned              class_attr;
  struct class_struct * class_parent;
  qual_chain          * class_quals; 
  prop_chain          * class_props; 
  method_chain        * class_methods; 
};

typedef struct symtab_struct {
  enum {
    SYM_TOKEN,
    SYM_CLASS,
    SYM_QUAL,
  } sym_type;
  union {
    int            sym_token;
    class_entry  * sym_class;
    qual_entry   * sym_qual;
  } sym_union;
} symtab_entry;

symtab_entry * make_token( int token_value );
void add_class_list(class_chain * cl_ch, class_entry * cls);
void add_qual_list(qual_chain * qu_ch, qual_entry * q);
class_entry * get_class_from_list(class_chain * cl_ch, char * name);

#ifndef ONEPASS
void fix_forward_decls(class_chain * cl_ch);
#endif

class_entry * make_class(hashentry * he, 
			 qual_chain * qu_ch,
			 const char * name, 
			 class_entry * parent,
			 prop_or_method_list * pom_li);
class_entry * get_class_def( hashentry * he, const char * name );
qual_entry * make_qualifier_definition(hashentry * he, 
				       const char * name, 
				       type_type typeid,
				       char * arrayspec,
				       value_chain * pr_ch,
				       qual_quals sf);
type_type make_ref_type( hashentry * he, const char * name );
qual_chain * make_qualifier(hashentry * he, 
			    const char * name, 
			     value_chain * val_ch );
qual_entry * get_qual_def( hashentry * he, const char * name );
void qualifier_list_add( qual_chain * qu_ch1, qual_chain * qu_ch2 );
value_chain * make_value_list(const char * value);
void value_list_add(value_chain * val_ch1, const char * val);
char * make_string(const char * string);
char * make_char(const char * string);
char * append_string(char * string1, const char * string2);
prop_or_method_list * make_pom_list(qual_chain * qu_ch,
				    type_type typeid,
				    const char * name,
				    const char * arrayspec,
				    param_chain * pa_ch,
				    value_chain * va_ch
				    );
void pom_list_add(prop_or_method_list * pom_li1, prop_or_method_list * pom_li2);
prop_chain * make_property_chain(qual_chain * qu_ch,
				 type_type typeid,
				 const char * name,
				 const char * arrayspec,
				 value_chain * va_ch
				 );
method_chain * make_method_chain(qual_chain * qu_ch,
				 type_type typeid,
				 const char * name,
				 const char * arrayspec,
				 param_chain * pa_ch
				 );
param_chain * make_param_list(qual_chain * qu_ch,
			      type_type typeid,
			      const char * name,
			      const char * arrayspec
			      );
void param_list_add(param_chain * pa_ch1, param_chain * pa_ch2);
class_entry * make_instance(hashentry * he, 
			 qual_chain * qu_ch,
			 const char * name,
			 prop_or_method_list * pom_li);
prop_chain * check_for_prop(class_entry * e, char * prop_id);
int check_for_keys(class_entry * ce, class_entry * ie);
class_entry * get_class_def_for_instance(class_entry * ie);
int check_valid_props(class_entry * ce, class_entry * ie);
int make_scope(char * scopeVal);
int make_flavor(char * flavorVal);

# endif
