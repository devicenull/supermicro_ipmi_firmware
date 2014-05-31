
/*
 * cmcift.h
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
 * CMCI function tables.
 *
*/

#ifndef _CMCIFT_H_
#define _CMCIFT_H_

#include "cmcidt.h"

#ifdef __cplusplus
extern "C" {
#endif


 
   //---------------------------------------------------
   //--
   //	_CMPIConstClass Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to ConstClass
       support sevices.
   */
typedef struct _CMPIConstClassFT {

       /** Function table version
       */
     int ftVersion;

       /** The ConstClass object will not be used any further and may be freed by
           CMPI run time system.
	 @param ccls ConstClass this pointer.
	 @return Service return status.
      */
     CMPIStatus (*release)
              (CMPIConstClass* ccls);

       /** Create an independent copy of this CpnstClass object. The resulting
           object must be released explicitly.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied ConstClass object.
      */
     CMPIConstClass* (*clone)
              (CMPIConstClass* ccls, CMPIStatus* rc);

       /** Gets the classname.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return classname.
      */     
     CMPIString *(*getClassName) 
              ( CMPIConstClass * ccls, CMPIStatus * rc );
                
       /** Gets a named property value.
	 @param ccls ConstClass this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     CMPIData (*getProperty)
              (CMPIConstClass* ccls, const char *name, CMPIStatus* rc);

       /** Gets a Property value defined by its index.
	 @param ccls ConstClass this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     CMPIData (*getPropertyAt)
              (CMPIConstClass* ccls, unsigned int index, CMPIString** name,
	       CMPIStatus* rc);

      /** Gets the number of properties contained in this Instance.
	 @param ccls ConstClass this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getPropertyCount)
              (CMPIConstClass* ccls, CMPIStatus* rc);

              
     CMPIData (*getQualifier)
              (CMPIConstClass * ccls,const char *name, CMPIStatus* rc);
     CMPIData (*getQualifierAt)
              (CMPIConstClass * ccls,unsigned int index, CMPIString** name,
	       CMPIStatus* rc);
     unsigned int (*getQualifierCount)
              (CMPIConstClass * ccls,CMPIStatus* rc);
              
              
     CMPIData (*getPropertyQualifier)
              (CMPIConstClass * ccls,const char *pname, const char *qname, CMPIStatus* rc);
     CMPIData (*getPropertyQualifierAt)
              (CMPIConstClass * ccls,const char *pname, unsigned int index, CMPIString** name,
	       CMPIStatus* rc);
     unsigned int (*getPropertyQualifierCount)
              (CMPIConstClass * ccls,const char *pname, CMPIStatus* rc);
              
} CMPIConstClassFT;


struct _CMPIConstClass {
   void *hdl;
   CMPIConstClassFT *ft;
};


  //---------------------------------------------------
   //--
   //	_CMPIInstance Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Instance object.
   */
   struct _CMPIInstance {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the Instance Function Table.
       */
     CMPIInstanceFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_CMPIInstance Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Instance
       support sevices.
   */
   struct _CMPIInstanceFT {

       /** Function table version
       */
     int ftVersion;

       /** The Instance object will not be used any further and may be freed by
           CMPI run time system.
	 @param inst Instance this pointer.
	 @return Service return status.
      */
     CMPIStatus (*release)
              (CMPIInstance* inst);

       /** Create an independent copy of this Instance object. The resulting
           object must be released explicitly.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Instance object.
      */
     CMPIInstance* (*clone)
              (CMPIInstance* inst, CMPIStatus* rc);

       /** Gets a named property value.
	 @param inst Instance this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     CMPIData (*getProperty)
              (CMPIInstance* inst, const char *name, CMPIStatus* rc);

       /** Gets a Property value defined by its index.
	 @param inst Instance this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
     CMPIData (*getPropertyAt)
              (CMPIInstance* inst, unsigned int index, CMPIString** name,
	       CMPIStatus* rc);

      /** Gets the number of properties contained in this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getPropertyCount)
              (CMPIInstance* inst, CMPIStatus* rc);

      /** Adds/replaces a named Property.
	 @param inst Instance this pointer.
         @param name Entry name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
     CMPIStatus (*setProperty)
              (CMPIInstance* inst, const char *name,
               CMPIValue* value, CMPIType type);

      /** Generates an ObjectPath out of the nameSpace, classname and
	  key propeties of this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
         @return the generated ObjectPath.
      */
     CMPIObjectPath* (*getObjectPath)
              (CMPIInstance* inst, CMPIStatus* rc);

      /** Directs CMPI to ignore any setProperty operations for this
	  instance for any properties not in this list.
	 @param inst Instance this pointer.
	 @param propertyList If not NULL, the members of the array define one
	     or more Property names to be accepted by setProperty operations.
	 @param keys Array of key property names of this instance. This array
	     must be specified.
	 @return Service return status.
      */
     CMPIStatus (*setPropertyFilter)
              (CMPIInstance* inst, char **propertyList, char **keys);
              
              
     CMPIData (*getQualifier)
              (CMPIInstance* inst, const char *name, CMPIStatus* rc);
     CMPIData (*getQualifierAt)
              (CMPIInstance* inst, unsigned int index, CMPIString** name,
	       CMPIStatus* rc);
     unsigned int (*getQualifierCount)
              (CMPIInstance* inst, CMPIStatus* rc);
              
              
     CMPIData (*getPropertyQualifier)
              (CMPIInstance* inst, const char *pname, const char *qname, CMPIStatus* rc);
     CMPIData (*getPropertyQualifierAt)
              (CMPIInstance* inst, const char *pname, unsigned int index, CMPIString** name,
	       CMPIStatus* rc);
     unsigned int (*getPropertyQualifierCount)
              (CMPIInstance* inst, const char *pname, CMPIStatus* rc);
              
   };




   //---------------------------------------------------
   //--
   //	_CMPIObjectPath Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Instance object.
   */
   struct _CMPIObjectPath {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the ObjectPath Function Table.
       */
     CMPIObjectPathFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_CMPIObjectPath Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to ObjectPath
       support sevices.
   */
   struct _CMPIObjectPathFT {

       /** Function table version
       */
     int ftVersion;

       /** The ObjectPath object will not be used any further and may be freed by
           CMPI run time system.
	 @param op ObjectPath this pointer.
	 @return Service return status.
      */
     CMPIStatus (*release)
              (CMPIObjectPath* op);

       /** Create an independent copy of this ObjectPath object. The resulting
           object must be released explicitly.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied ObjectPath object.
      */
     CMPIObjectPath* (*clone)
              (CMPIObjectPath* op, CMPIStatus* rc);

       /** Set/replace the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param ns The nameSpace string
	 @return Service return status.
      */
     CMPIStatus (*setNameSpace)
              (CMPIObjectPath* op, const char *ns);

       /** Get the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The nameSpace component.
      */
     CMPIString* (*getNameSpace)
              (CMPIObjectPath* op, CMPIStatus* rc);

       /** Set/replace the hostname component.
	 @param op ObjectPath this pointer.
	 @param hn The hostname string
	 @return Service return status.
      */
     CMPIStatus (*setHostname)
              (CMPIObjectPath* op, const char *hn);

       /** Get the hostname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The hostname component.
      */
     CMPIString* (*getHostname)
              (CMPIObjectPath* op, CMPIStatus* rc);

       /** Set/replace the classname component.
	 @param op ObjectPath this pointer.
	 @param cn The hostname string
	 @return Service return status.
      */
     CMPIStatus (*setClassName)
              (CMPIObjectPath* op, const char *cn);

       /** Get the classname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The classname component.
      */
     CMPIString* (*getClassName)
              (CMPIObjectPath* op, CMPIStatus* rc);

      /** Adds/replaces a named key property.
	 @param op ObjectPath this pointer.
         @param name Key property name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
     CMPIStatus (*addKey)
              (CMPIObjectPath* op, const char *name,
               CMPIValue* value, CMPIType type);

       /** Gets a named key property value.
	 @param op ObjectPath this pointer.
	 @param name Key property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Entry value.
      */
     CMPIData (*getKey)
              (CMPIObjectPath* op, const char *name, CMPIStatus* rc);

       /** Gets a key property value defined by its index.
	 @param op ObjectPath this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Data value.
      */
     CMPIData (*getKeyAt)
              (CMPIObjectPath* op, unsigned int index, CMPIString** name,
	       CMPIStatus* rc);

      /** Gets the number of key properties contained in this ObjectPath.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getKeyCount)
              (CMPIObjectPath* op, CMPIStatus* rc);

      /** Set/replace nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
     CMPIStatus (*setNameSpaceFromObjectPath)
              (CMPIObjectPath* op, CMPIObjectPath* src);

      /** Set/replace hostname, nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
     CMPIStatus (*setHostAndNameSpaceFromObjectPath)
              (CMPIObjectPath* op,
               CMPIObjectPath* src);
	


		// optional qualifier support


       /** Get class qualifier value.
	 @param op ObjectPath this pointer.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
     CMPIData (*getClassQualifier)
              (CMPIObjectPath* op,
               const char *qName,
               CMPIStatus *rc);

       /** Get property qualifier value.
	 @param op ObjectPath this pointer.
	 @param pName Property name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
     CMPIData (*getPropertyQualifier)
              (CMPIObjectPath* op,
               const char *pName,
               const char *qName,
               CMPIStatus *rc);

       /** Get method qualifier value.
	 @param op ObjectPath this pointer.
	 @param mName Method name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
     CMPIData (*getMethodQualifier)
              (CMPIObjectPath* op,
               const char *methodName,
               const char *qName,
               CMPIStatus *rc);

       /** Get method parameter quailifier value.
	 @param op ObjectPath this pointer.
	 @param mName Method name.
	 @param pName Parameter name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
     CMPIData (*getParameterQualifier)
              (CMPIObjectPath* op,
               const char *mName,
               const char *pName,
               const char *qName,
               CMPIStatus *rc);
	
   #if defined(CMPI_VER_86)
      /** Generates a well formed string representation of this ObjectPath
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return String representation.
      */
     CMPIString *(*toString)
              (CMPIObjectPath* op, CMPIStatus *rc);
    #endif

   };







   //---------------------------------------------------
   //--
   //	_CMPIArgs Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Args object.
   */
   struct _CMPIArgs {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the Args Function Table.
       */
     CMPIArgsFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_CMPIArgsFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Args
       support sevices.
   */
   struct _CMPIArgsFT{

       /** Function table version
       */
     int ftVersion;

       /** The Args object will not be used any further and may be freed by
           CMPI run time system.
	 @param as Args this pointer.
	 @return Service return status.
      */
     CMPIStatus (*release)
              (CMPIArgs* as);

       /** Create an independent copy of this Args object. The resulting
           object must be released explicitly.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Args object.
      */
     CMPIArgs* (*clone)
              (CMPIArgs* as, CMPIStatus* rc);

      /** Adds/replaces a named argument.
	 @param as Args this pointer.
         @param name Argument name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
     CMPIStatus (*addArg)
              (CMPIArgs* as, const char *name ,CMPIValue* value,
               CMPIType type);

       /** Gets a named argument value.
	 @param as Args this pointer.
	 @param name Argument name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
     CMPIData (*getArg)
              (CMPIArgs* as, const char *name, CMPIStatus* rc);

       /** Gets a Argument value defined by its index.
	 @param as Args this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned argument name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
     CMPIData (*getArgAt)
              (CMPIArgs* as, unsigned int index, CMPIString** name,
	       CMPIStatus* rc);

      /** Gets the number of arguments contained in this Args.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     unsigned int (*getArgCount)
              (CMPIArgs* as, CMPIStatus* rc);
   };




   //---------------------------------------------------
   //--
   //	_CMPIString Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated String object.
   */
   struct _CMPIString {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the String Function Table.
       */
     CMPIStringFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_CMPIStringFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to String
       support sevices.
   */
   struct _CMPIStringFT {

       /** Function table version
       */
     int ftVersion;

       /** The String object will not be used any further and may be freed by
           CMPI run time system.
	 @param st String this pointer.
	 @return Service return status.
       */
     CMPIStatus (*release)
             (CMPIString* st);

       /** Create an independent copy of this String object. The resulting
           object must be released explicitly.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied String object.
      */
     CMPIString* (*clone)
             (CMPIString* st, CMPIStatus* rc);

       /** Get a pointer to a C char *representation of this String.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to char *representation.
      */
     char *(*getCharPtr)
             (CMPIString* st, CMPIStatus* rc);
   };




   //---------------------------------------------------
   //--
   //	_CMPIArray Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Array object.
   */
   struct _CMPIArray {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the Array Function Table.
       */
     CMPIArrayFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_CMPIArrayFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Array
       support sevices.
   */
   struct _CMPIArrayFT {

       /** Function table version
       */
     int ftVersion;

       /** The Array object will not be used any further and may be freed by
           CMPI run time system.
	 @param ar Array this pointer.
	 @return Service return status.
       */
     CMPIStatus (*release)
             (CMPIArray* ar);

       /** Create an independent copy of this Array object. The resulting
           object must be released explicitly.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Array object.
      */
     CMPIArray* (*clone)
             (CMPIArray* ar, CMPIStatus* rc);

      /** Gets the number of elements contained in this Array.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
     CMPICount (*getSize)
             (CMPIArray* ar, CMPIStatus* rc);

      /** Gets the element type.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
     CMPIType (*getSimpleType)
             (CMPIArray* ar, CMPIStatus* rc);

       /** Gets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
     CMPIData (*getElementAt)
             (CMPIArray* ar, CMPICount index, CMPIStatus* rc);

       /** Sets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
     CMPIStatus (*setElementAt)
             (CMPIArray* ar, CMPICount index, CMPIValue* value, CMPIType type);
   };





   //---------------------------------------------------
   //--
   //	_CMPIEnumeration Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the Encapsulated Enumeration object.
   */
   struct _CMPIEnumeration {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the Enumeration Function Table.
       */
     CMPIEnumerationFT* ft;
   };



   //---------------------------------------------------
   //--
   //	_CMPIEnumerationFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to Enumeration
       support sevices.
   */
   struct _CMPIEnumerationFT {

       /** Function table version
       */
     int ftVersion;

       /** The Enumeration object will not be used any further and may be freed by
           CMPI run time system.
	 @param en Enumeration this pointer.
	 @return Service return status.
       */
     CMPIStatus (*release)
             (CMPIEnumeration* en);

       /** Create an independent copy of this Enumeration object. The resulting
           object must be released explicitly.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Enumeration object.
      */
     CMPIEnumeration* (*clone)
             (CMPIEnumeration* en, CMPIStatus* rc);

       /** Get the next element of this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
     CMPIData (*getNext)
             (CMPIEnumeration* en, CMPIStatus* rc);

       /** Test for any elements left in this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True or false.
      */
     CMPIBoolean (*hasNext)
             (CMPIEnumeration* en, CMPIStatus* rc);

       /** Convert this Enumeration into an Array.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Array.
      */
     CMPIArray* (*toArray)
             (CMPIEnumeration* en, CMPIStatus* rc);
  };





   //---------------------------------------------------
   //--
   //	_CMPIDateTime Encapsulated object
   //--
   //---------------------------------------------------


   /** This structure represents the DateTime object.
   */
  struct _CMPIDateTime {

       /** Opaque pointer to MB specific implementation data.
       */
     void *hdl;

       /** Pointer to the DateTime Function Table.
       */
     CMPIDateTimeFT *ft;
   };



   //---------------------------------------------------
   //--
   //	_CMPIDateTimeFT Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers providing access to DateTime
       support sevices.
   */
   struct _CMPIDateTimeFT {

       /** Function table version
       */
     int ftVersion;

       /** The DateTime object will not be used any further and may be freed by
           CMPI run time system.
	 @param dt DateTime this pointer.
	 @return Service return status.
       */
     CMPIStatus (*release)
             (CMPIDateTime* dt);

       /** Create an independent copy of this DateTime object. The resulting
           object must be released explicitly.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied DateTime object.
      */
     CMPIDateTime* (*clone)
             (CMPIDateTime* dt, CMPIStatus* rc);

       /** Get DateTime setting in binary format (in microsecods
	       starting since 00:00:00 GMT, Jan 1,1970).
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime in binary.
      */
     CMPIUint64 (*getBinaryFormat)
             (CMPIDateTime* dt, CMPIStatus* rc);

       /** Get DateTime setting in UTC string format.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime as UTC string.
      */
     CMPIString* (*getStringFormat)
             (CMPIDateTime* dt, CMPIStatus* rc);

       /** Tests whether DateTime is an interval value.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True if interval value.
      */
     CMPIBoolean (*isInterval)
              (CMPIDateTime* dt, CMPIStatus* rc);
  };



#include "cmcimacs.h"

#ifdef __cplusplus
 };
#endif

#endif // _CMCIFT_H_
