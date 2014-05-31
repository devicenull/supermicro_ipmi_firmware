
/*
 * $Id: cimc.h,v 1.4 2007/12/21 15:10:43 sschuetz Exp $
 *
 * © Copyright IBM Corp. 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Authors: Adrian Schuur <schuur@de.ibm.com>        
 *          Viktor Mihajlovski <mihajlov@de.ibm.com>
 *
 * Description:
 *
 * CIM C API Functions for Environment and Client Operations
 *
 */

#ifndef _CIMC_H_
#define _CIMC_H_

#include "cimcdt.h"
#include "cimcft.h"

#ifdef __cplusplus
extern "C" {
#endif


  
  /*
   * _CIMCClientFt Function Table
   */


  struct _CIMCClient;
  typedef struct _CIMCClient CIMCClient;
  
  typedef struct _CIMCClientFT {
    
    /** Function table version
     */
    int ftVersion;
    
    /** The Client object will not be used any further and may be freed by
	cimc run time system.
	@param cl Client this pointer.
	@return Service return status.
    */
    CIMCStatus (*release)
      (CIMCClient* cl);
    
    /** The clone operation is not supported. 
	Inserted here to conform with other cimc function table layouts.
	@param cl Client this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Always NULL.
    */
    CIMCClient *(*clone) ( CIMCClient * cl, CIMCStatus * st );
    
    
    /** Get Class using &lt;op&gt; as reference. Class structure can be
	controled using the flags parameter.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace and classname components.
	@param flags Any combination of the following flags are supported: 
	CIMC_FLAG_LocalOnly, CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	@param properties If not NULL, the members of the array define one or more Property
	names. Each returned Object MUST NOT include elements for any Properties
	missing from this list
	@param rc Output: Service return status (suppressed when NULL).
	@return The Class.
    */
    CIMCClass* (*getClass)
      (CIMCClient* cl,
       CIMCObjectPath* op, CIMCFlags flags, char** properties, CIMCStatus* rc);
    
    
    /** Enumerate Class Names (and subclass names) in the nameSpace defined by &lt;op&gt;. Inheritance scope can be controled using the
	flags parameter.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace component.
	@param flags The following flag is supported: CIMC_FLAG_DeepInheritance.
	@param rc Output: Service return status (suppressed when NULL).
	@return Enumeration of ObjectPathes.
    */
    
    CIMCEnumeration* (*enumClassNames)
      (CIMCClient* cl,
       CIMCObjectPath* op, CIMCFlags flags, CIMCStatus* rc);
    
    /** Enumerate Classes (and subclasses) in the nameSpace  defined by &lt;op&gt;.
	Class structure and inheritance scope can be controled using the &lt;flags&gt; parameter.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace and classname components.
	@param flags Any combination of the following flags are supported: CIMC_FLAG_LocalOnly, 
	CIMC_FLAG_DeepInheritance, CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	@param rc Output: Service return status (suppressed when NULL).
	@return Enumeration of Instances.
    */
    
    CIMCEnumeration* (*enumClasses)
      (CIMCClient* cl,
       CIMCObjectPath* op, CIMCFlags flags, CIMCStatus* rc);
    
    /** Get Instance using &lt;op&gt; as reference. Instance structure can be
	controled using the &lt;flags&gt; parameter.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace, classname and key components.
	@param flags Any combination of the following flags are supported: 
	CIMC_FLAG_LocalOnly, CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	@param properties If not NULL, the members of the array define one or more Property
	names. Each returned Object MUST NOT include elements for any Properties
	missing from this list
	@param rc Output: Service return status (suppressed when NULL).
	@return The Instance.
    */
    CIMCInstance* (*getInstance)
      (CIMCClient* cl,
       CIMCObjectPath* op, CIMCFlags flags, char** properties, CIMCStatus* rc);
    
    /** Create Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace, classname and key components.
	@param inst Complete instance.
	@param rc Output: Service return status (suppressed when NULL).
	@return The assigned instance reference.
    */
    CIMCObjectPath* (*createInstance)
      (CIMCClient* cl,
       CIMCObjectPath* op, CIMCInstance* inst, CIMCStatus* rc);
    
    /** Replace an existing Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace, classname and key components.
	@param inst Complete instance.
	@param flags The following flag is supported: CIMC_FLAG_IncludeQualifiers.
	@param properties If not NULL, the members of the array define one or more Property
	names, only those properties will be updated. Else, all properties will be updated. 
	@return Service return status.
    */
    CIMCStatus (*setInstance)
      (CIMCClient* cl,
       CIMCObjectPath* op, CIMCInstance* inst, CIMCFlags flags, char ** properties);
    
    /** Delete an existing Instance using &lt;op&gt; as reference.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace, classname and key components.
	@return Service return status.
    */
    CIMCStatus (*deleteInstance)
      (CIMCClient* cl,
       CIMCObjectPath* op);

    /** Query the enumeration of instances of the class (and subclasses) defined
	by &lt;op&gt; using &lt;query&gt; expression.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace and classname components.
	@param query Query expression
	@param lang Query Language
	@param rc Output: Service return status (suppressed when NULL).
	@return Resulting eumeration of Instances.
    */
    CIMCEnumeration* (*execQuery)
      (CIMCClient* cl,
       CIMCObjectPath* op, const char *query, const char *lang, CIMCStatus* rc);
    
    /** Enumerate Instance Names of the class (and subclasses) defined by &lt;op&gt;.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace and classname components.
	@param rc Output: Service return status (suppressed when NULL).
	@return Enumeration of ObjectPathes.
    */
    CIMCEnumeration* (*enumInstanceNames)
      (CIMCClient* cl,
       CIMCObjectPath* op, CIMCStatus* rc);
    
    /** Enumerate Instances of the class (and subclasses) defined by &lt;op&gt;.
	Instance structure and inheritance scope can be controled using the
	&lt;flags&gt; parameter.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace and classname components.
	@param flags Any combination of the following flags are supported: CIMC_FLAG_LocalOnly, 
	CIMC_FLAG_DeepInheritance, CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	@param properties If not NULL, the members of the array define one or more Property
	names. Each returned Object MUST NOT include elements for any Properties
	missing from this list
	@param rc Output: Service return status (suppressed when NULL).
	@return Enumeration of Instances.
    */
    CIMCEnumeration* (*enumInstances)
      (CIMCClient* cl,
       CIMCObjectPath* op, CIMCFlags flags, char** properties, CIMCStatus* rc);

    /** Enumerate instances associated with the Instance defined by the &lt;op&gt;.
	@param cl Client this pointer.
	@param op Source ObjectPath containing nameSpace, classname and key components.
	@param assocClass If not NULL, MUST be a valid Association Class name.
	It acts as a filter on the returned set of Objects by mandating that
	each returned Object MUST be associated to the source Object via an
	Instance of this Class or one of its subclasses.
	@param resultClass If not NULL, MUST be a valid Class name.
	It acts as a filter on the returned set of Objects by mandating that
	each returned Object MUST be either an Instance of this Class (or one
	of its subclasses).
	@param role If not NULL, MUST be a valid Property name.
	It acts as a filter on the returned set of Objects by mandating
	that each returned Object MUST be associated to the source Object
	via an Association in which the source Object plays the specified role
	(i.e. the name of the Property in the Association Class that refers
	to the source Object MUST match the value of this parameter).
	@param resultRole If not NULL, MUST be a valid Property name.
	It acts as a filter on the returned set of Objects by mandating
	that each returned Object MUST be associated to the source Object
	via an Association in which the returned Object plays the specified role
	(i.e. the name of the Property in the Association Class that refers to
	the returned Object MUST match the value of this parameter).
	@param flags Any combination of the following flags are supported: 
	CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	@param properties If not NULL, the members of the array define one or more Property
	names. Each returned Object MUST NOT include elements for any Properties
	missing from this list
	@param rc Output: Service return status (suppressed when NULL).
	@return Enumeration of Instances.
    */
    CIMCEnumeration* (*associators)
      (CIMCClient* cl,
       CIMCObjectPath* op, const char *assocClass, const char *resultClass,
       const char *role, const char *resultRole, CIMCFlags flags, 
       char** properties, CIMCStatus* rc);

    /** Enumerate ObjectPaths associated with the Instance defined by &lt;op&gt;.
	@param cl Client this pointer.
	@param op Source ObjectPath containing nameSpace, classname and key components.
	@param assocClass If not NULL, MUST be a valid Association Class name.
	It acts as a filter on the returned set of Objects by mandating that
	each returned Object MUST be associated to the source Object via an
	Instance of this Class or one of its subclasses.
	@param resultClass If not NULL, MUST be a valid Class name.
	It acts as a filter on the returned set of Objects by mandating that
	each returned Object MUST be either an Instance of this Class (or one
	of its subclasses).
	@param role If not NULL, MUST be a valid Property name.
	It acts as a filter on the returned set of Objects by mandating
	that each returned Object MUST be associated to the source Object
	via an Association in which the source Object plays the specified role
	(i.e. the name of the Property in the Association Class that refers
	to the source Object MUST match the value of this parameter).
	@param resultRole If not NULL, MUST be a valid Property name.
	It acts as a filter on the returned set of Objects by mandating
	that each returned Object MUST be associated to the source Object
	via an Association in which the returned Object plays the specified role
	(i.e. the name of the Property in the Association Class that refers to
	the returned Object MUST match the value of this parameter).
	@param rc Output: Service return status (suppressed when NULL).
	@return Enumeration of ObjectPaths.
    */
    CIMCEnumeration* (*associatorNames)
      (CIMCClient* cl,
       CIMCObjectPath* op, const char *assocClass, const char *resultClass,
       const char *role, const char *resultRole, CIMCStatus* rc);

    /** Enumerates the association instances that refer to the instance defined by
	&lt;op&gt;.
	@param cl Client this pointer.
	@param op Source ObjectPath containing nameSpace, classname and key components.
	@param resultClass If not NULL, MUST be a valid Class name.
	It acts as a filter on the returned set of Objects by mandating that
	each returned Object MUST be either an Instance of this Class (or one
	of its subclasses).
	@param role If not NULL, MUST be a valid Property name.
	It acts as a filter on the returned set of Objects by mandating
	that each returned Object MUST be associated to the source Object
	via an Association in which the source Object plays the specified role
	(i.e. the name of the Property in the Association Class that refers
	to the source Object MUST match the value of this parameter).
	@param flags Any combination of the following flags are supported: 
	CIMC_FLAG_IncludeQualifiers and CIMC_FLAG_IncludeClassOrigin.
	@param properties If not NULL, the members of the array define one or more Property
	names. Each returned Object MUST NOT include elements for any Properties
	missing from this list
	@param rc Output: Service return status (suppressed when NULL).
	@return Enumeration of ObjectPaths.
    */
    CIMCEnumeration* (*references)
      (CIMCClient* cl,
       CIMCObjectPath* op, const char *resultClass ,const char *role ,
       CIMCFlags flags, char** properties, CIMCStatus* rc);

    /** Enumerates the association ObjectPaths that refer to the instance defined by
	&lt;op&gt;.
	@param cl Client this pointer.
	@param op Source ObjectPath containing nameSpace, classname and key components.
	@param resultClass If not NULL, MUST be a valid Class name.
	It acts as a filter on the returned set of Objects by mandating that
	each returned Object MUST be either an Instance of this Class (or one
	of its subclasses).
	@param role If not NULL, MUST be a valid Property name.
	It acts as a filter on the returned set of Objects by mandating
	that each returned Object MUST be associated to the source Object
	via an Association in which the source Object plays the specified role
	(i.e. the name of the Property in the Association Class that refers
	to the source Object MUST match the value of this parameter).
	@param rc Output: Service return status (suppressed when NULL).
	@return Enumeration of ObjectPaths.
    */
    CIMCEnumeration* (*referenceNames)
      (CIMCClient* cl,
       CIMCObjectPath* op, const char *resultClass ,const char *role,
       CIMCStatus* rc);

    /** Invoke a named, extrinsic method of an Instance
	defined by the &lt;op&gt; parameter.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace, classname and key components.
	@param method Method name
	@param in Input parameters.
	@param out Output parameters.
	@param rc Output: Service return status (suppressed when NULL).
	@return Method return value.
    */
    CIMCData (*invokeMethod)
      (CIMCClient* cl,
       CIMCObjectPath* op,const char *method,
       CIMCArgs* in, CIMCArgs* out, CIMCStatus* rc);

    /** Set the named property value of an Instance defined by the &lt;op&gt; parameter.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace, classname and key components.
	@param name Property name
	@param value Value.
	@param type Value type.
	@return Service return status.
    */
    CIMCStatus (*setProperty)
      (CIMCClient* cl,
       CIMCObjectPath* op, const char *name , CIMCValue* value,
       CIMCType type);

    /** Get the named property value of an Instance defined by the &lt;op&gt; parameter.
	@param cl Client this pointer.
	@param op ObjectPath containing nameSpace, classname and key components.
	@param name Property name
	@param rc Output: Service return status (suppressed when NULL).
	@return Property value.
    */
    CIMCData (*getProperty)
      (CIMCClient *cl, 
       CIMCObjectPath *op, const char *name, CIMCStatus *rc);


  } CIMCClientFT;

  struct _CIMCClient {
    void *hdl;
    CIMCClientFT *ft;
  };


  /*
   * CIMCEnv function table definition
   */
   
  struct _CIMCEnv;
  typedef struct _CIMCEnv CIMCEnv;
  
  typedef struct _CIMCEnvFT {
    char *env;
    void* (*release)(CIMCEnv *ce);
    CIMCClient* (*connect)
      (CIMCEnv *ce, const char *hn, const char *scheme, const char *port, 
       const char *user, const char *pwd,CIMCStatus *rc);
    CIMCClient* (*connect2)
      (CIMCEnv *ce, const char *hn, const char *scheme, const char *port, 
       const char *user, const char *pwd, 
       int verifyMode, const char * trustStore,
       const char * certFile, const char * keyFile,
       CIMCStatus *rc);
    CIMCInstance* (*newInstance)
      (CIMCEnv *ce, const CIMCObjectPath* op, CIMCStatus* rc);
    CIMCObjectPath* (*newObjectPath)
      (CIMCEnv *ce, const char *ns, const char *cn, CIMCStatus* rc);
    CIMCArgs* (*newArgs)
      (CIMCEnv *ce, CIMCStatus* rc);
    CIMCString* (*newString)
      (CIMCEnv *ce, const char *data, CIMCStatus* rc);
    CIMCArray* (*newArray)
      (CIMCEnv *ce, CIMCCount max, CIMCType type, CIMCStatus* rc);
    CIMCDateTime* (*newDateTime)
      (CIMCEnv *ce, CIMCStatus* rc);
    CIMCDateTime* (*newDateTimeFromBinary)
      (CIMCEnv *ce, CIMCUint64 binTime, CIMCBoolean interval, CIMCStatus* rc);
    CIMCDateTime* (*newDateTimeFromChars)
      (CIMCEnv *ce, const char *utcTime, CIMCStatus* rc);
    CIMCIndicationListener* (*newIndicationListener)
      (CIMCEnv *ce, int sslMode, int *portNumber, char **socketName, 
       void (*fp) (CIMCInstance *indInstance), CIMCStatus* rc);
  } CIMCEnvFT;

  struct _CIMCEnv {
    void *hdl;
    CIMCEnvFT *ft;
  };


  CIMCEnv* NewCIMCEnv(const char *id, unsigned int options, int *rc, char **msg);
  void ReleaseCIMCEnv(CIMCEnv* env);

  /*
   * Prototype for CIMC Implementation Factory Function
   */
  typedef CIMCEnv *(*InitCimcEnv) (const char *id, unsigned int options, int *rc, char **msg);


#ifdef __cplusplus
 };
#endif

#endif // _cimcC_H_
