
/*
 * $Id: cimcft.h,v 1.5 2007/12/21 15:10:44 sschuetz Exp $
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
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMCI function tables.
 *
*/

#ifndef _CIMCFT_H_
#define _CIMCFT_H_

#include "cimcdt.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct _CIMCObject {
    void *hdl;
    void *ft;
    int   refCount;
  } CIMCObject;   


  /* -------------------------------------------------------------------*/

  /** This structure is a table of pointers providing access to Class
   *   support sevices.
   */
  
  typedef struct _CIMCClassFT { 
    int version;
    CIMCStatus(*release) (CIMCClass * cc);
    CIMCClass *(*clone) (CIMCClass * cc, CIMCStatus * rc); 
    CIMCString *(*getClassName)(CIMCClass* cc, CIMCStatus * rc);
   
    CIMCData (*getProperty) (CIMCClass * cc, const char *prop, CIMCStatus * rc);
    CIMCData (*getPropertyAt) (CIMCClass * cc, CIMCCount i,
			       CIMCString ** name,  CIMCStatus * rc);
    CIMCCount (*getPropertyCount) (CIMCClass * cc, CIMCStatus * rc);
   
    CIMCData (*getQualifier) (CIMCClass * cc, const char *qual, CIMCStatus * rc);
    CIMCData (*getQualifierAt) (CIMCClass * cc, CIMCCount i,
				CIMCString ** name, CIMCStatus * rc);
    CIMCCount (*getQualifierCount) (CIMCClass * cc, CIMCStatus * rc);
   
    CIMCData (*getPropQualifier) (CIMCClass * cc, const char *prop,
				  const char *qual, CIMCStatus * rc);
    CIMCData (*getPropQualifierAt) (CIMCClass * cc, const char *prop,
				    CIMCCount i, CIMCString ** name,
				    CIMCStatus * rc);
    CIMCCount (*getPropQualifierCount) (CIMCClass * cc, const char * prop, CIMCStatus * rc);
   
    CIMCArray *(*getKeyList) (CIMCClass * cc);
    char *(*toString) (CIMCClass * cc);     
    void (*relocate) (CIMCClass * cc);
    const char *(*getCharClassName) (CIMCClass * br);
    const char *(*getCharSuperClassName) (CIMCClass * br);
    CIMCBoolean(*isAssociation) (CIMCClass * cc);
    CIMCBoolean(*isAbstract) (CIMCClass * cc);
    CIMCBoolean(*isIndication) (CIMCClass * cc);
    CIMCData(*getPropQualAt) (CIMCClass * cc, CIMCCount p, CIMCCount i, CIMCString ** name,
			      CIMCStatus * rc);
  } CIMCClassFT;

  struct _CIMCClass {
    void *hdl;
    CIMCClassFT *ft;
  };

  /* -------------------------------------------------------------------*/

  /** This structure represents the Encapsulated Instance object.
   */
  struct _CIMCInstance {

    /** Opaque pointer to MB specific implementation data.
     */
    void *hdl;

    /** Pointer to the Instance Function Table.
     */
    CIMCInstanceFT* ft;
  };


  /** This structure is a table of pointers providing access to Instance
      support sevices.
  */
  struct _CIMCInstanceFT {

    /** Function table version
     */
    int ftVersion;

    /** The Instance object will not be used any further and may be freed by
	CIMC run time system.
	@param inst Instance this pointer.
	@return Service return status.
    */
    CIMCStatus (*release)
      (CIMCInstance* inst);

    /** Create an independent copy of this Instance object. The resulting
	object must be released explicitly.
	@param inst Instance this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Pointer to copied Instance object.
    */
    CIMCInstance* (*clone)
      (CIMCInstance* inst, CIMCStatus* rc);

    /** Gets a named property value.
	@param inst Instance this pointer.
	@param name Property name.
	@param rc Output: Service return status (suppressed when NULL).
	@return Property value.
    */
    CIMCData (*getProperty)
      (CIMCInstance* inst, const char *name, CIMCStatus* rc);

    /** Gets a Property value defined by its index.
	@param inst Instance this pointer.
	@param index Position in the internal Data array.
	@param name Output: Returned property name (suppressed when NULL).
	@param rc Output: Service return status (suppressed when NULL).
	@return Property value.
    */
    CIMCData (*getPropertyAt)
      (CIMCInstance* inst, unsigned int index, CIMCString** name,
       CIMCStatus* rc);

    /** Gets the number of properties contained in this Instance.
	@param inst Instance this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Number of properties.
    */
    unsigned int (*getPropertyCount)
      (CIMCInstance* inst, CIMCStatus* rc);

    /** Adds/replaces a named Property.
	@param inst Instance this pointer.
	@param name Entry name.
	@param value Address of value structure.
	@param type Value type.
	@return Service return status.
    */
    CIMCStatus (*setProperty)
      (CIMCInstance* inst, const char *name,
       CIMCValue* value, CIMCType type);

    /** Generates an ObjectPath out of the nameSpace, classname and
	key propeties of this Instance.
	@param inst Instance this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return the generated ObjectPath.
    */
    CIMCObjectPath* (*getObjectPath)
      (CIMCInstance* inst, CIMCStatus* rc);

    /** Directs CIMC to ignore any setProperty operations for this
	instance for any properties not in this list.
	@param inst Instance this pointer.
	@param propertyList If not NULL, the members of the array define one
	or more Property names to be accepted by setProperty operations.
	@param keys Array of key property names of this instance. This array
	must be specified.
	@return Service return status.
    */
    CIMCStatus (*setPropertyFilter)
      (CIMCInstance* inst, char **propertyList, char **keys);
              
              
    CIMCData (*getQualifier)
      (CIMCInstance* inst, const char *name, CIMCStatus* rc);
    CIMCData (*getQualifierAt)
      (CIMCInstance* inst, unsigned int index, CIMCString** name,
       CIMCStatus* rc);
    unsigned int (*getQualifierCount)
      (CIMCInstance* inst, CIMCStatus* rc);
              
              
    CIMCData (*getPropertyQualifier)
      (CIMCInstance* inst, const char *pname, const char *qname, CIMCStatus* rc);
    CIMCData (*getPropertyQualifierAt)
      (CIMCInstance* inst, const char *pname, unsigned int index, CIMCString** name,
       CIMCStatus* rc);
    unsigned int (*getPropertyQualifierCount)
      (CIMCInstance* inst, const char *pname, CIMCStatus* rc);
              
  };


  /* -------------------------------------------------------------------*/


  /** This structure represents the Encapsulated Instance object.
   */
  struct _CIMCObjectPath {

    /** Opaque pointer to MB specific implementation data.
     */
    void *hdl;

    /** Pointer to the ObjectPath Function Table.
     */
    CIMCObjectPathFT* ft;
  };



  /** This structure is a table of pointers providing access to ObjectPath
      support sevices.
  */
  struct _CIMCObjectPathFT {

    /** Function table version
     */
    int ftVersion;

    /** The ObjectPath object will not be used any further and may be freed by
	CIMC run time system.
	@param op ObjectPath this pointer.
	@return Service return status.
    */
    CIMCStatus (*release)
      (CIMCObjectPath* op);

    /** Create an independent copy of this ObjectPath object. The resulting
	object must be released explicitly.
	@param op ObjectPath this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Pointer to copied ObjectPath object.
    */
    CIMCObjectPath* (*clone)
      (CIMCObjectPath* op, CIMCStatus* rc);

    /** Set/replace the nameSpace component.
	@param op ObjectPath this pointer.
	@param ns The nameSpace string
	@return Service return status.
    */
    CIMCStatus (*setNameSpace)
      (CIMCObjectPath* op, const char *ns);

    /** Get the nameSpace component.
	@param op ObjectPath this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return The nameSpace component.
    */
    CIMCString* (*getNameSpace)
      (CIMCObjectPath* op, CIMCStatus* rc);

    /** Set/replace the hostname component.
	@param op ObjectPath this pointer.
	@param hn The hostname string
	@return Service return status.
    */
    CIMCStatus (*setHostname)
      (CIMCObjectPath* op, const char *hn);

    /** Get the hostname component.
	@param op ObjectPath this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return The hostname component.
    */
    CIMCString* (*getHostname)
      (CIMCObjectPath* op, CIMCStatus* rc);

    /** Set/replace the classname component.
	@param op ObjectPath this pointer.
	@param cn The hostname string
	@return Service return status.
    */
    CIMCStatus (*setClassName)
      (CIMCObjectPath* op, const char *cn);

    /** Get the classname component.
	@param op ObjectPath this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return The classname component.
    */
    CIMCString* (*getClassName)
      (CIMCObjectPath* op, CIMCStatus* rc);

    /** Adds/replaces a named key property.
	@param op ObjectPath this pointer.
	@param name Key property name.
	@param value Address of value structure.
	@param type Value type.
	@return Service return status.
    */
    CIMCStatus (*addKey)
      (CIMCObjectPath* op, const char *name,
       CIMCValue* value, CIMCType type);

    /** Gets a named key property value.
	@param op ObjectPath this pointer.
	@param name Key property name.
	@param rc Output: Service return status (suppressed when NULL).
	@return Entry value.
    */
    CIMCData (*getKey)
      (CIMCObjectPath* op, const char *name, CIMCStatus* rc);

    /** Gets a key property value defined by its index.
	@param op ObjectPath this pointer.
	@param index Position in the internal Data array.
	@param name Output: Returned property name (suppressed when NULL).
	@param rc Output: Service return status (suppressed when NULL).
	@return Data value.
    */
    CIMCData (*getKeyAt)
      (CIMCObjectPath* op, unsigned int index, CIMCString** name,
       CIMCStatus* rc);

    /** Gets the number of key properties contained in this ObjectPath.
	@param op ObjectPath this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Number of properties.
    */
    unsigned int (*getKeyCount)
      (CIMCObjectPath* op, CIMCStatus* rc);

    /** Set/replace nameSpace and classname components from &lt;src&gt;.
	@param op ObjectPath this pointer.
	@param src Source input.
	@return Service return status.
    */
    CIMCStatus (*setNameSpaceFromObjectPath)
      (CIMCObjectPath* op, CIMCObjectPath* src);

    /** Set/replace hostname, nameSpace and classname components from &lt;src&gt;.
	@param op ObjectPath this pointer.
	@param src Source input.
	@return Service return status.
    */
    CIMCStatus (*setHostAndNameSpaceFromObjectPath)
      (CIMCObjectPath* op,
       CIMCObjectPath* src);
	


    /** Get class qualifier value.
	@param op ObjectPath this pointer.
	@param qName Qualifier name.
	@param rc Output: Service return status (suppressed when NULL).
	@return Qualifier value.
    */
    CIMCData (*getClassQualifier)
      (CIMCObjectPath* op,
       const char *qName,
       CIMCStatus *rc);

    /** Get property qualifier value.
	@param op ObjectPath this pointer.
	@param pName Property name.
	@param qName Qualifier name.
	@param rc Output: Service return status (suppressed when NULL).
	@return Qualifier value.
    */
    CIMCData (*getPropertyQualifier)
      (CIMCObjectPath* op,
       const char *pName,
       const char *qName,
       CIMCStatus *rc);

    /** Get method qualifier value.
	@param op ObjectPath this pointer.
	@param mName Method name.
	@param qName Qualifier name.
	@param rc Output: Service return status (suppressed when NULL).
	@return Qualifier value.
    */
    CIMCData (*getMethodQualifier)
      (CIMCObjectPath* op,
       const char *methodName,
       const char *qName,
       CIMCStatus *rc);

    /** Get method parameter quailifier value.
	@param op ObjectPath this pointer.
	@param mName Method name.
	@param pName Parameter name.
	@param qName Qualifier name.
	@param rc Output: Service return status (suppressed when NULL).
	@return Qualifier value.
    */
    CIMCData (*getParameterQualifier)
      (CIMCObjectPath* op,
       const char *mName,
       const char *pName,
       const char *qName,
       CIMCStatus *rc);
	
    /** Generates a well formed string representation of this ObjectPath
	@param op ObjectPath this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return String representation.
    */
    CIMCString *(*toString)
      (CIMCObjectPath* op, CIMCStatus *rc);

  };

  /* -------------------------------------------------------------------*/

  /** This structure represents the Encapsulated Args object.
   */
  struct _CIMCArgs {

    /** Opaque pointer to MB specific implementation data.
     */
    void *hdl;

    /** Pointer to the Args Function Table.
     */
    CIMCArgsFT* ft;
  };

  /** This structure represents the Encapsulated String object.
   */
  struct _CIMCString {

    /** Opaque pointer to MB specific implementation data.
     */
    void *hdl;

    /** Pointer to the String Function Table.
     */
    CIMCStringFT* ft;
  };


  /** This structure is a table of pointers providing access to String
      support sevices.
  */
  struct _CIMCStringFT {

    /** Function table version
     */
    int ftVersion;

    /** The String object will not be used any further and may be freed by
	CIMC run time system.
	@param st String this pointer.
	@return Service return status.
    */
    CIMCStatus (*release)
      (CIMCString* st);

    /** Create an independent copy of this String object. The resulting
	object must be released explicitly.
	@param st String this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Pointer to copied String object.
    */
    CIMCString* (*clone)
      (CIMCString* st, CIMCStatus* rc);

    /** Get a pointer to a C char *representation of this String.
	@param st String this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Pointer to char *representation.
    */
    char *(*getCharPtr)
      (CIMCString* st, CIMCStatus* rc);
  };

  /* -------------------------------------------------------------------*/


  /** This structure represents the Encapsulated Array object.
   */
  struct _CIMCArray {

    /** Opaque pointer to MB specific implementation data.
     */
    void *hdl;

    /** Pointer to the Array Function Table.
     */
    CIMCArrayFT* ft;
  };

  /** This structure is a table of pointers providing access to Array
      support sevices.
  */
  struct _CIMCArrayFT {

    /** Function table version
     */
    int ftVersion;

    /** The Array object will not be used any further and may be freed by
	CIMC run time system.
	@param ar Array this pointer.
	@return Service return status.
    */
    CIMCStatus (*release)
      (CIMCArray* ar);

    /** Create an independent copy of this Array object. The resulting
	object must be released explicitly.
	@param ar Array this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Pointer to copied Array object.
    */
    CIMCArray* (*clone)
      (CIMCArray* ar, CIMCStatus* rc);

    /** Gets the number of elements contained in this Array.
	@param ar Array this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Number of elements.
    */
    CIMCCount (*getSize)
      (CIMCArray* ar, CIMCStatus* rc);

    /** Gets the element type.
	@param ar Array this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Number of elements.
    */
    CIMCType (*getSimpleType)
      (CIMCArray* ar, CIMCStatus* rc);

    /** Gets an element value defined by its index.
	@param ar Array this pointer.
	@param index Position in the internal Data array.
	@param rc Output: Service return status (suppressed when NULL).
	@return Element value.
    */
    CIMCData (*getElementAt)
      (CIMCArray* ar, CIMCCount index, CIMCStatus* rc);

    /** Sets an element value defined by its index.
	@param ar Array this pointer.
	@param index Position in the internal Data array.
	@param value Address of value structure.
	@param type Value type.
	@return Service return status.
    */
    CIMCStatus (*setElementAt)
      (CIMCArray* ar, CIMCCount index, CIMCValue* value, CIMCType type);
  };


  /* -------------------------------------------------------------------*/


  /** This structure represents the Encapsulated Enumeration object.
   */
  struct _CIMCEnumeration {

    /** Opaque pointer to MB specific implementation data.
     */
    void *hdl;

    /** Pointer to the Enumeration Function Table.
     */
    CIMCEnumerationFT* ft;
  };

  /** This structure is a table of pointers providing access to Enumeration
      support sevices.
  */
  struct _CIMCEnumerationFT {

    /** Function table version
     */
    int ftVersion;

    /** The Enumeration object will not be used any further and may be freed by
	CIMC run time system.
	@param en Enumeration this pointer.
	@return Service return status.
    */
    CIMCStatus (*release)
      (CIMCEnumeration* en);

    /** Create an independent copy of this Enumeration object. The resulting
	object must be released explicitly.
	@param en Enumeration this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Pointer to copied Enumeration object.
    */
    CIMCEnumeration* (*clone)
      (CIMCEnumeration* en, CIMCStatus* rc);

    /** Get the next element of this Enumeration.
	@param en Enumeration this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Element value.
    */
    CIMCData (*getNext)
      (CIMCEnumeration* en, CIMCStatus* rc);

    /** Test for any elements left in this Enumeration.
	@param en Enumeration this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return True or false.
    */
    CIMCBoolean (*hasNext)
      (CIMCEnumeration* en, CIMCStatus* rc);

    /** Convert this Enumeration into an Array.
	@param en Enumeration this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return The Array.
    */
    CIMCArray* (*toArray)
      (CIMCEnumeration* en, CIMCStatus* rc);
  };

  /* -------------------------------------------------------------------*/


  /** This structure represents the DateTime object.
   */
  struct _CIMCDateTime {

    /** Opaque pointer to MB specific implementation data.
     */
    void *hdl;

    /** Pointer to the DateTime Function Table.
     */
    CIMCDateTimeFT *ft;
  };

  /** This structure is a table of pointers providing access to DateTime
      support sevices.
  */
  struct _CIMCDateTimeFT {

    /** Function table version
     */
    int ftVersion;

    /** The DateTime object will not be used any further and may be freed by
	CIMC run time system.
	@param dt DateTime this pointer.
	@return Service return status.
    */
    CIMCStatus (*release)
      (CIMCDateTime* dt);

    /** Create an independent copy of this DateTime object. The resulting
	object must be released explicitly.
	@param dt DateTime this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return Pointer to copied DateTime object.
    */
    CIMCDateTime* (*clone)
      (CIMCDateTime* dt, CIMCStatus* rc);

    /** Get DateTime setting in binary format (in microsecods
	starting since 00:00:00 GMT, Jan 1,1970).
	@param dt DateTime this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return DateTime in binary.
    */
    CIMCUint64 (*getBinaryFormat)
      (CIMCDateTime* dt, CIMCStatus* rc);

    /** Get DateTime setting in UTC string format.
	@param dt DateTime this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return DateTime as UTC string.
    */
    CIMCString* (*getStringFormat)
      (CIMCDateTime* dt, CIMCStatus* rc);

    /** Tests whether DateTime is an interval value.
	@param dt DateTime this pointer.
	@param rc Output: Service return status (suppressed when NULL).
	@return True if interval value.
    */
    CIMCBoolean (*isInterval)
      (CIMCDateTime* dt, CIMCStatus* rc);
  };

  /** This structure represents the indication listener object.
   */
  struct _CIMCIndicationListener {

    /** Opaque pointer to MB specific implementation data.
     */
    void *hdl;

    /** Pointer to the indication listener Function Table.
     */
    CIMCIndicationListenerFT* ft;
  };


  /** This structure is a table of pointers providing access to indication
      listener support sevices.
  */
  struct _CIMCIndicationListenerFT {

    /** Function table version
     */
    int ftVersion;

    /** The indication listener object will not be used any further and may 
        be freed by CIMC run time system.
	@param il pointer to this indication listener
	@return Service return status.
    */
    CIMCStatus (*release)
      (CIMCIndicationListener* il);

    /** Create an independent copy of this indication listener object. The
	resulting object must be released explicitly.
	@param il pointer to this indication listener
	@param rc Output: Service return status (suppressed when NULL).
	@return Pointer to copied indication listener object.
    */
    CIMCIndicationListener* (*clone)
      (CIMCIndicationListener* il, CIMCStatus* rc);

    /** Start listening.
	@param il pointer to this indication listener.
    */
    CIMCStatus (*start)
      (CIMCIndicationListener* il);
      
    /** Stop listening.
    @param il pointer to this indication listener.
    */
    CIMCStatus (*stop)
      (CIMCIndicationListener* il);      
  };
#ifdef __cplusplus
};
#endif

#endif 
