
/*
 * cmpic.h
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPI Client function tables.
 *
*/

#ifndef _CMPIC_H_
#define _CMPIC_H_

#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"

#ifdef __cplusplus
extern "C" {
#endif



   //---------------------------------------------------
   //--
   //	_CMCIClientFt Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers to client CIMOM services.
   */

struct _CMCIClient;
typedef struct _CMCIClient CMCIClient;
   
struct _CMCIConnection;
typedef struct _CMCIConnection CMCIConnection;
   
struct _ClientEnc;
typedef struct _ClientEnc ClientEnc;

typedef struct _CMCIClientFT {

       /** Function table version
       */
     int ftVersion;

      /** The Client object will not be used any further and may be freed by
           CMPI run time system.
	 @param cl Client this pointer.
	 @return Service return status.
      */
     CMPIStatus (*release)
              (CMCIClient* cl);
   
      /** The clone operation is not supported. 
           Inserted here to conform with other CMCI function table layouts.
	 @param cl Client this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Always NULL.
      */
     CMCIClient *(*clone) ( CMCIClient * cl, CMPIStatus * st );
     
     
      /** Get Class using &lt;op&gt; as reference. Class structure can be
         controled using the flags parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: 
	    CMPI_FLAG_LocalOnly, CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Class.
     */
     CMPIConstClass* (*getClass)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);


       /** Enumerate Class Names (and subclass names) in the nameSpace defined by &lt;op&gt;. Inheritance scope can be controled using the
	        flags parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace component.
	 @param flags The following flag is supported: CMPI_FLAG_DeepInheritance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */
     
     CMPIEnumeration* (*enumClassNames)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, CMPIStatus* rc);

     /** Enumerate Classes (and subclasses) in the nameSpace  defined by &lt;op&gt;.
         Class structure and inheritance scope can be controled using the &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: CMPI_FLAG_LocalOnly, 
	     CMPI_FLAG_DeepInheritance, CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     
     CMPIEnumeration* (*enumClasses)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, CMPIStatus* rc);

      /** Get Instance using &lt;op&gt; as reference. Instance structure can be
         controled using the &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param flags Any combination of the following flags are supported: 
	    CMPI_FLAG_LocalOnly, CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Instance.
     */
     CMPIInstance* (*getInstance)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);

      /** Create Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param inst Complete instance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The assigned instance reference.
     */
     CMPIObjectPath* (*createInstance)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIInstance* inst, CMPIStatus* rc);

      /** Replace an existing Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param inst Complete instance.
	 @param flags The following flag is supported: CMPI_FLAG_IncludeQualifiers.
	 @param properties If not NULL, the members of the array define one or more Property
	     names, only those properties will be updated. Else, all properties will be updated. 
	 @return Service return status.
     */
     CMPIStatus (*setInstance)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIInstance* inst, CMPIFlags flags, char ** properties);

      /** Delete an existing Instance using &lt;op&gt; as reference.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @return Service return status.
     */
     CMPIStatus (*deleteInstance)
                (CMCIClient* cl,
                 CMPIObjectPath* op);

      /** Query the enumeration of instances of the class (and subclasses) defined
         by &lt;op&gt; using &lt;query&gt; expression.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param query Query expression
	 @param lang Query Language
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Resulting eumeration of Instances.
     */
     CMPIEnumeration* (*execQuery)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *query, const char *lang, CMPIStatus* rc);

       /** Enumerate Instance Names of the class (and subclasses) defined by &lt;op&gt;.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */
     CMPIEnumeration* (*enumInstanceNames)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIStatus* rc);

     /** Enumerate Instances of the class (and subclasses) defined by &lt;op&gt;.
         Instance structure and inheritance scope can be controled using the
	      &lt;flags&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace and classname components.
	 @param flags Any combination of the following flags are supported: CMPI_FLAG_LocalOnly, 
	     CMPI_FLAG_DeepInheritance, CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     CMPIEnumeration* (*enumInstances)
                (CMCIClient* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);

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
	    CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
     CMPIEnumeration* (*associators)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *assocClass, const char *resultClass,
		 const char *role, const char *resultRole, CMPIFlags flags, 
                 char** properties, CMPIStatus* rc);

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
     CMPIEnumeration* (*associatorNames)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *assocClass, const char *resultClass,
		 const char *role, const char *resultRole, CMPIStatus* rc);

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
	    CMPI_FLAG_IncludeQualifiers and CMPI_FLAG_IncludeClassOrigin.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPaths.
     */
     CMPIEnumeration* (*references)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *resultClass ,const char *role ,
		 CMPIFlags flags, char** properties, CMPIStatus* rc);

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
     CMPIEnumeration* (*referenceNames)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *resultClass ,const char *role,
                 CMPIStatus* rc);

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
     CMPIData (*invokeMethod)
                (CMCIClient* cl,
                 CMPIObjectPath* op,const char *method,
		 CMPIArgs* in, CMPIArgs* out, CMPIStatus* rc);

       /** Set the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param name Property name
	 @param value Value.
	 @param type Value type.
	 @return Service return status.
      */
     CMPIStatus (*setProperty)
                (CMCIClient* cl,
                 CMPIObjectPath* op, const char *name , CMPIValue* value,
                 CMPIType type);

       /** Get the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param cl Client this pointer.
	 @param op ObjectPath containing nameSpace, classname and key components.
	 @param name Property name
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     CMPIData (*getProperty)
                (CMCIClient *cl, 
                 CMPIObjectPath *op, const char *name, CMPIStatus *rc);


} CMCIClientFT;


typedef struct clientData {
   char *hostName;
   char *port;
   char *user;
   char *pwd;
   char *scheme;
   int  status;
} CMCIClientData;
  
#define CMCI_VERIFY_NONE 0
#define CMCI_VERIFY_PEER 1

typedef struct credentialData {
  int    verifyMode;
  char * trustStore;
  char * certFile;
  char * keyFile;
} CMCICredentialData;

struct _CMCIClient {
   void *hdl;
   CMCIClientFT *ft;
};   



CMCIClient *cmciConnect(const char *hn, const char *scheme, const char *port, 
                        const char *user, const char *pwd, CMPIStatus *rc);   

CMCIClient *cmciConnect2(const char *hn, const char *scheme, const char *port, 
			 const char *user, const char *pwd, 
			 int verifyMode, const char * trustStore,
			 const char * certFile, const char * keyFile,
			 CMPIStatus *rc);   

#define native_new_CMPIObjectPath   newCMPIObjectPath

CMPIObjectPath * newCMPIObjectPath ( const char * my_nameSpace, 
					     const char * classname,
					     CMPIStatus * rc );
int sameCMPIObjectPath ( const CMPIObjectPath *cop1, const CMPIObjectPath *cop2);
   
#ifdef __cplusplus
 };
#endif

#endif // _CMPIC_H_
