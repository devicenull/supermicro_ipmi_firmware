/**
 * $Id: mofc.y,v 1.6 2007/01/03 11:44:08 sschuetz Exp $
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
 * Description: Grammar for DMTF MOF Syntax
 *
 */

%{

/*
 * passhthru
 */
# include <stdio.h>
# include <string.h>
# include <hash.h>
# include <mofdecl.h>
# include "symtab.h"

extern class_chain  * cls_chain_current;
extern class_chain  * inst_chain_current;
extern qual_chain  * qual_chain_current;

%}


/*
 * lexical value types
 */

%union  {
        char *                lval_id;
        type_type             lval_type;
        char *                lval_literal;
        class_entry         * lval_class;
        prop_or_method_list * lval_props;
        qual_chain          * lval_quals;
        qual_entry          * lval_qual;
        value_chain         * lval_vals;
        param_chain         * lval_params;
        int                   lval_int;
        qual_quals			  lval_qual_quals;
        }

/*
 * terminals / types
 */

%token <lval_id> Identifier
%token <lval_type> BaseTypeUINT8
%token <lval_type> BaseTypeSINT8
%token <lval_type> BaseTypeUINT16
%token <lval_type> BaseTypeSINT16
%token <lval_type> BaseTypeUINT32
%token <lval_type> BaseTypeSINT32
%token <lval_type> BaseTypeUINT64
%token <lval_type> BaseTypeSINT64
%token <lval_type> BaseTypeREAL32
%token <lval_type> BaseTypeREAL64
%token <lval_type> BaseTypeSTRING
%token <lval_type> BaseTypeCHAR16
%token <lval_type> BaseTypeDATETIME
%token <lval_type> BaseTypeBOOLEAN
%token <lval_literal> IntLiteral
%token <lval_literal> FloatLiteral
%token <lval_literal> StringLiteral
%token <lval_literal> CharLiteral
%token <lval_literal> BoolLiteral
%token <lval_literal> NullLiteral
%token AS
%token <lval_id> CLASS
%token INSTANCE
%token OF
%token QUALIFIER
%token REF
%token FLAVOR
%token SCOPE

/*
 * nonterminal types
 */

%type <lval_class> class_definition
%type <lval_class> instance_definition
%type <lval_class> opt_superclass
%type <lval_props> opt_property_or_method_definitionlist
%type <lval_props> property_or_method_definitionlist
%type <lval_props> property_or_method_definition
%type <lval_props> opt_property_initializer_list
%type <lval_props> property_initializer_list
%type <lval_props> property_initializer
%type <lval_quals> opt_qualifier_list
%type <lval_quals> qualifier_list
%type <lval_quals> qualifier
%type <lval_qual>  qualifier_definition
%type <lval_params> opt_parameter_list parameter_list parameter
%type <lval_literal> opt_base_literal base_literal string_literal_list
%type <lval_literal> opt_array_spec 
%type <lval_vals> opt_qualifier_initializer opt_literal_list literal_list
%type <lval_vals> opt_initializer initial_value
%type <lval_type> full_type base_type
%type <lval_int> flavor_list
%type <lval_int> scope_list
%type <lval_id> ext_identifier
%type <lval_qual_quals> opt_qualifier_extension_list

/*
 * precedence ?
 */

%start mof

%%

mof : /* empty */
| definition_list 
  {
#ifndef ONEPASS
    fix_forward_decls(cls_chain_current);
#endif
  };
;

definition_list : definition 
                | definition_list definition
;

definition : qualifier_definition
             {
               add_qual_list(qual_chain_current,$1);
             }
           | class_definition 
             {
               add_class_list(cls_chain_current,$1);
             }  
           | instance_definition
             {
               add_class_list(inst_chain_current,$1);
             }
;

/*
 * qualifier definitions
 */

qualifier_definition : QUALIFIER  Identifier ':' 
                       base_type opt_array_spec opt_initializer
                       opt_qualifier_extension_list ';'
                       {
                         $$ = make_qualifier_definition(current_qualtab, $2, $4,
                                                   $5,$6,$7);
                       }
;

opt_qualifier_extension_list : /* empty */
							 {
                             	$$.scope = 0;
                             	$$.flavor = 0;                             	
							 }
                             | ',' SCOPE '(' scope_list ')'
                             {
                             	$$.scope = $4;
                             	$$.flavor = 0;
                             }
                             | ',' FLAVOR '(' flavor_list ')'
                             {
                             	$$.scope = 0;
                             	$$.flavor = $4;                             
                             }
                             | ',' SCOPE '(' scope_list ')' ',' FLAVOR '(' flavor_list ')'
                             {
                             	$$.scope = $4;
                             	$$.flavor = $9;                       	
                             }
;
			
scope_list : ext_identifier
			{
				$$=make_scope($1);
			}
			| scope_list ',' ext_identifier
			{
				$$|=make_scope($3);			
			}
;

flavor_list: ext_identifier
			{
				$$=make_flavor($1);
			}
		    | flavor_list ',' ext_identifier
			{
				$$|=make_flavor($3);
			}
;

ext_identifier : Identifier
               | CLASS
;

/*
 * classes and instances 
 */

class_definition : opt_qualifier_list CLASS Identifier opt_superclass
                   '{'
                        opt_property_or_method_definitionlist 
                   '}' ';'
                   { $$ = make_class(current_symtab,$1,$3,$4,$6); }
;

instance_definition : opt_qualifier_list INSTANCE OF Identifier opt_alias
                   '{'
                        opt_property_initializer_list
                   '}' ';'
                   {
                   	 $$ = make_instance(current_symtab,$1,$4,$7);
                   	 add_class_list(cls_chain_current,get_class_def(current_symtab,$4));
                   }
;

opt_qualifier_list : /* empty */            {$$=NULL;}
                   | '[' qualifier_list ']' {$$=$2;}
;

qualifier_list : qualifier                    {$$=$1;}
               | qualifier_list ',' qualifier {qualifier_list_add($1,$3);}
;

qualifier : Identifier opt_qualifier_initializer 
            {$$=make_qualifier(current_qualtab,$1,$2);}
;

opt_qualifier_initializer : /* empty */              {$$=NULL;}
                          | '(' opt_base_literal ')' {$$=make_value_list($2);}
                          | '{' opt_literal_list '}' {$$=$2;}
;

opt_literal_list : /* empty */  {$$=NULL;}
                 | literal_list  {$$=$1;}
;

literal_list : base_literal                  {$$=make_value_list($1);}
             | literal_list ',' base_literal {value_list_add($1,$3);}
;

opt_superclass : /* empty */    {$$=NULL;}
               | ':' Identifier {$$=get_class_def(current_symtab,$2);}
;

opt_property_or_method_definitionlist : /* empty */ {$$=NULL;}
                                      | property_or_method_definitionlist
                                        {$$=$1;}
;

property_or_method_definitionlist : property_or_method_definition {$$=$1;}
                                  | property_or_method_definitionlist
                                    property_or_method_definition
                                    {pom_list_add($1,$2);}
;

property_or_method_definition : opt_qualifier_list
                                full_type Identifier
                                opt_array_spec
                                opt_parameter_list
                                opt_initializer
                                ';'
                                {$$=make_pom_list($1,$2,$3,$4,$5,$6);}
;

full_type : base_type       {$$=$1;}
          | Identifier REF {$$=make_ref_type(current_symtab,$1);}
;

base_type : BaseTypeSINT8 {$$.type_base = BaseTypeSINT8; /* must be a smarter way */}
          | BaseTypeUINT8 {$$.type_base = BaseTypeUINT8;}
          | BaseTypeSINT16 {$$.type_base = BaseTypeSINT16;}
          | BaseTypeUINT16 {$$.type_base = BaseTypeUINT16;}
          | BaseTypeSINT32 {$$.type_base = BaseTypeSINT32;}
          | BaseTypeUINT32 {$$.type_base = BaseTypeUINT32;}
          | BaseTypeSINT64 {$$.type_base = BaseTypeSINT64;}
          | BaseTypeUINT64 {$$.type_base = BaseTypeUINT64;}
          | BaseTypeREAL32 {$$.type_base = BaseTypeREAL32;}
          | BaseTypeREAL64 {$$.type_base = BaseTypeREAL64;}
          | BaseTypeSTRING {$$.type_base = BaseTypeSTRING;}
          | BaseTypeCHAR16 {$$.type_base = BaseTypeCHAR16;}
          | BaseTypeBOOLEAN {$$.type_base = BaseTypeBOOLEAN;}
          | BaseTypeDATETIME {$$.type_base = BaseTypeDATETIME;}
;

opt_array_spec : /* empty */ {$$=NULL;}
               | '[' ']' {$$="0"; /*unspecified length*/} 
               | '[' base_literal ']' {$$=$2;}
;

opt_base_literal : /* empty */ {$$=NULL;}
                 | base_literal {$$=$1;}
;

opt_parameter_list : /* empty */ {$$=NULL;}
                   | '(' ')' {$$=make_param_list(NULL,(type_type)0,NULL,NULL);}
                   | '(' parameter_list ')' {$$=$2;}
;

parameter_list : parameter                     {$$=$1;}
               | parameter_list ',' parameter  {param_list_add($1,$3);}
;

parameter : opt_qualifier_list full_type Identifier opt_array_spec
            {$$=make_param_list($1,$2,$3,$4);}
;

opt_initializer : /* empty */ {$$=NULL;}
                | '=' initial_value {$$ = $2;}
;

initial_value : base_literal {$$=make_value_list($1);}
              | '{' opt_literal_list '}' {$$=$2;}
;

base_literal : IntLiteral
             | FloatLiteral
             | CharLiteral {$$=make_char($1);}
             | string_literal_list
             | BoolLiteral
             | NullLiteral
;

string_literal_list : StringLiteral                     {$$=make_string($1);}
                    | string_literal_list StringLiteral {$$=append_string($1,make_string($2));}
;

opt_alias : /* empty */
          | AS Identifier
;

opt_property_initializer_list : /* empty */ {$$=NULL;}
                              | property_initializer_list
							{
								$$=$1;
							}
;

property_initializer_list : property_initializer
						{
							$$=$1;
						}
                          | property_initializer_list property_initializer
						{
							pom_list_add($1,$2);
						}
;

property_initializer : Identifier opt_initializer ';'
						{
							$$=make_pom_list(NULL,(type_type)0,$1,NULL,NULL,$2);
						}
;
