
/*
 * cmpimacs.h
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
 * CMPI convenious macros.
 *
*/


#ifndef _CMCIMACS_H_
#define _CMCIMACS_H_

#ifndef DOC_ONLY
#include "cmcidt.h"
#include "cmcift.h"
#endif

#ifdef DOC_ONLY
#define CMPI_INLINE
#endif

#ifdef DOC_ONLY
  /** This macro builds a CMPIStatus object with &lt;rc&gt; as return code and returns
      to the Broker.
      @param rc the CMPI return code
      @return This macro contains a return statement and leaves the function.
   */
 noReturn CMReturn(CMPIrc rc);
#else
#define CMReturn(rc) \
      { CMPIStatus stat={(rc),NULL}; \
         return stat; }
#endif

#ifdef CMPI_INLINE
  /** Initializes status object with rc and NULL message.
      @param st Address of status object
      @param rcp CMPI return code
  */
inline static  void CMSetStatus(CMPIStatus* st, CMPIrc rcp)
      { if (st) { (st)->rc = (rcp); (st)->msg=NULL; }}
#else
#define CMSetStatus(st,rcp) \
      { if (st) {(st)->rc=(rcp); (st)->msg=NULL; }}
#endif

#ifdef CMPI_INLINE
  /** Initializes status object with rc and message.
      @param mb Broker this pointer
      @param st Address of status object
      @param rcp CMPI return code
      @param chars Message character string
  */
inline static   void CMSetStatusWithChars(CMPIStatus* st, CMPIrc rcp,
                                 char* chars)
      { if (st) {
          (st)->rc=(rcp);
          (st)->msg=newCMPIString((chars),NULL); 
      } }
#else
#define CMSetStatusWithChars(st,rcp,chars) \
      { if (st) { (st)->rc=(rcp); \
        (st)->msg=newCMPIString((chars),NULL); }}
#endif

#ifdef DOC_ONLY
  /** This macro builds a CMPIStatus object with &lt;rc&gt; as return code and &lt;str&gt; as
      message and returns to the Broker.
//       @param rc the CMPI return code
      @param str the message as String object
      @return This macro contains a return statement and leaves the function.
   */
 noReturn CMReturnWithString(CMPIrc rc, CMPIString *str);
#else
#define CMReturnWithString(rc,str) \
      { CMPIStatus stat={(rc),(str)}; \
         return stat; }
#endif

#ifdef DOC_ONLY
  /** This macro builds a CMPIStatus object with &lt;rc&gt; as return code and &lt;msg&gt; as
      message and returns to the Broker.
      @param mb Broker this pointer
      @param rc the CMPI return code
      @param msg the message as character string
      @return This macro contains a return statement and leaves the function.
   */
 noReturn CMReturnWithChars(CMPIrc rc, char *msg);
#else
#define CMReturnWithChars(rc,chars) \
      { CMPIStatus stat={(rc),NULL}; \
         stat.msg=newCMPIString((chars),NULL); \
         return stat; }
#endif



#ifndef DOC_ONLY
  #ifdef CMPI_PLATFORM_WIN32_IX86_MSVC
	#define  CMPI_EXTERN_C __declspec(dllexport)
  #else
	#ifdef __cplusplus
      #define  CMPI_EXTERN_C extern "C"
    #else
      #define CMPI_EXTERN_C
    #endif
  #endif
#endif

#ifdef CMPI_INLINE
  /** Tests for encapsulated NULL object.
      @param obj CMPI Object pointer
  */
  inline static   CMPIBoolean CMIsNullObject(void* obj)
       { return ((obj)==NULL || *((void**)(obj))==NULL); }
#else
  #define CMIsNullObject(o)           ((o)==NULL || *((void**)(o))==NULL)
#endif

#ifdef CMPI_INLINE
  /** Tests for nullValue data item.
      @param val Value object
  */
  inline static   CMPIBoolean CMIsNullValue(CMPIData val)
       { return ((val.state) & CMPI_nullValue); }
#else
  #define CMIsNullValue(v)                   ((v.state) & CMPI_nullValue)
#endif

#ifdef CMPI_INLINE
  /** Tests for keyValue data item.
      @param val Value object
  */
  inline static   CMPIBoolean CMIsKeyValue(CMPIData val)
       { return ((val.state) & CMPI_keyValue); }
#else
  #define CMIsKeyValue(v)                     ((v.state) & CMPI_keyValue)
#endif

#ifdef CMPI_INLINE
  /** Tests for keyValue data item.
      @param val Value object
  */
  inline static   CMPIBoolean CMIsArray(CMPIData val)
       { return ((val.type) & CMPI_ARRAY); }
#else
  #define CMIsArray(v)                            ((v.type) & CMPI_ARRAY)
#endif


    // Life-cycle macros

  #define CMClone(o,rc)                        ((o)->ft->clone((o),(rc)))
  #define CMRelease(o)                            ((o)->ft->release((o)))
  #define CMGetCharPtr(s)                                 ((char*)s->hdl)
  #define CMReleaseData(d) \
       if (((d)->type >= CMPI_instance  && (d)->type <= CMPI_dateTime) || \
           ((d)->type >= CMPI_instanceA && (d)->type <= CMPI_dateTimeA)) \
                                                    CMRelease((d)->value);

    // CMPIInstance macros


#ifdef CMPI_INLINE
       /** Gets a named property value.
	 @param inst Instance this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
   inline static   CMPIData CMGetProperty
              (CMPIInstance* inst, char* name, CMPIStatus* rc)
	{ return ((inst)->ft->getProperty((inst),(name),(rc))); }
#else
  #define CMGetProperty(i,n,rc)      ((i)->ft->getProperty((i),(n),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Gets a Property value defined by its index.
	 @param inst Instance this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
   inline static   CMPIData CMGetPropertyAt
              (CMPIInstance* inst, unsigned int index, CMPIString** name,
	       CMPIStatus* rc)
	{ return ((inst)->ft->getPropertyAt((inst),(index),(name),(rc))); }
#else
 #define CMGetPropertyAt(i,num,s,rc) \
                  ((i)->ft->getPropertyAt((i),(num),(s),(rc)))
#endif

#ifdef CMPI_INLINE
      /** Adds/replaces a named Property.
	 @param inst Instance this pointer.
         @param name Entry name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
  inline static   CMPIStatus CMSetProperty
              (CMPIInstance* inst, char* name,
               void* value, CMPIType type)
	{ return ((inst)->ft->setProperty((inst),(name),(CMPIValue*)(value),(type))); }
#else
  #define CMSetProperty(i,n,v,t) \
                      ((i)->ft->setProperty((i),(n),(CMPIValue*)(v),(t)))
#endif

#ifdef CMPI_INLINE
      /** Gets the number of properties contained in this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
     inline static   unsigned int CMGetPropertyCount
              (CMPIInstance* inst, CMPIStatus* rc)
	 { return ((inst)->ft->getPropertyCount((inst),(rc))); }
#else
  #define CMGetPropertyCount(i,rc)   ((i)->ft->getPropertyCount((i),(rc)))
#endif

#ifdef CMPI_INLINE
      /** Generates an ObjectPath out of the nameSpace, classname and
	  key propeties of this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
         @return the generated ObjectPath.
      */
     inline static   CMPIObjectPath* CMGetObjectPath
              (CMPIInstance* inst, CMPIStatus* rc)
	{ return ((inst)->ft->getObjectPath((inst),(rc))); }
#else
  #define CMGetObjectPath(i,rc)        ((i)->ft->getObjectPath((i),(rc)))
#endif

#ifdef CMPI_INLINE
      /** Directs CMPI to ignore any setProperty operations for this
	  instance for any properties not in this list.
	 @param inst Instance this pointer.
	 @param propertyList If not NULL, the members of the array define one
	     or more Property names to be accepted by setProperty operations.
	 @param keys array of key properties. This array must be specified.
	 @return Service return status.
      */
     inline static   CMPIStatus CMSetPropertyFilter
              (CMPIInstance* inst, char** propertyList, char **keys)
	{ return ((inst)->ft->setPropertyFilter((inst),(propertyList),(keys))); }
#else
  #define CMSetPropertyFilter(i,pl,k) ((i)->ft->setPropertyFilter((i),(pl),(k)))
#endif



   // CMPIObjectPath macros


#ifdef CMPI_INLINE
       /** Set/replace the hostname component.
	 @param op ObjectPath this pointer.
	 @param hn The hostname string
	 @return Service return status.
      */
     inline static   CMPIStatus CMSetHostname
              (CMPIObjectPath* op, char* hn)
	{ return ((op)->ft->setHostname((op),(hn))); }
#else
  #define CMSetHostname(p,n)              ((p)->ft->setHostname((p),(n)))
#endif

#ifdef CMPI_INLINE
       /** Get the hostname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The hostname component.
      */
   inline static   CMPIString* CMGetHostname
              (CMPIObjectPath* op,CMPIStatus* rc)
	{ return ((op)->ft->getHostname((op),(rc))); }
#else
  #define CMGetHostname(p,rc)            ((p)->ft->getHostname((p),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Set/replace the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param ns The nameSpace string
	 @return Service return status.
      */
   inline static   CMPIStatus CMSetNameSpace
              (CMPIObjectPath* op,char* ns)
	 { return ((op)->ft->setNameSpace((op),(ns))); }
#else
  #define CMSetNameSpace(p,n)            ((p)->ft->setNameSpace((p),(n)))
#endif

#ifdef CMPI_INLINE
       /** Get the nameSpace component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The nameSpace component.
      */
   inline static   CMPIString* CMGetNameSpace
              (CMPIObjectPath* op, CMPIStatus* rc)
	{ return ((op)->ft->getNameSpace((op),(rc))); }
#else
  #define CMGetNameSpace(p,rc)          ((p)->ft->getNameSpace((p),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Set/replace the classname component.
	 @param op ObjectPath this pointer.
	 @param cn The hostname string
	 @return Service return status.
      */
   inline static   CMPIStatus CMSetClassName
                 (CMPIObjectPath* op, char* cn)
	{ return ((op)->ft->setClassName((op),(cn))); }
#else
  #define CMSetClassName(p,n)            ((p)->ft->setClassName((p),(n)))
#endif

#ifdef CMPI_INLINE
       /** Get the classname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The classname component.
      */
   inline static   CMPIString* CMGetClassName
              (CMPIObjectPath* op,CMPIStatus* rc)
	{ return ((op)->ft->getClassName((op),(rc))); }
#else
  #define CMGetClassName(p,rc)          ((p)->ft->getClassName((p),(rc)))
#endif

#ifdef CMPI_INLINE
      /** Adds/replaces a named key property.
	 @param op ObjectPath this pointer.
         @param name Key property name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
   inline static   CMPIStatus CMAddKey
              (CMPIObjectPath* op, char* name,
               void* value, CMPIType type)
	{ return ((op)->ft->addKey((op),(name),(CMPIValue*)(value),(type))); }
#else
  #define CMAddKey(p,n,v,t) \
                           ((p)->ft->addKey((p),(n),(CMPIValue*)(v),(t)))
#endif

#ifdef CMPI_INLINE
       /** Gets a named key property value.
	 @param op ObjectPath this pointer.
	 @param name Key property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Entry value.
      */
   inline static   CMPIData CMGetKey
              (CMPIObjectPath* op, char* name, CMPIStatus* rc)
	{ return ((op)->ft->getKey((op),(name),(rc))); }
#else
  #define CMGetKey(p,n,rc)                ((p)->ft->getKey((p),(n),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Gets a key property value defined by its index.
	 @param op ObjectPath this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Data value.
      */
   inline static   CMPIData CMGetKeyAt
              (CMPIObjectPath* op,unsigned int index, CMPIString** name,
	       CMPIStatus* rc)
	{ return ((op)->ft->getKeyAt((op),(index),(name),(rc))); }
#else
  #define CMGetKeyAt(p,i,n,rc)          ((p)->ft->getKeyAt((p),(i),(n),(rc)))
#endif


#ifdef CMPI_INLINE
      /** Gets the number of key properties contained in this ObjectPath.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
   inline static   unsigned int CMGetKeyCount
              (CMPIObjectPath* op, CMPIStatus* rc)
	{ return ((op)->ft->getKeyCount((op),(rc))); }
#else
  #define CMGetKeyCount(p,rc)            ((p)->ft->getKeyCount((p),(rc)))
#endif

#ifdef CMPI_INLINE
      /** Set/replace nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
   inline static   CMPIStatus CMSetNameSpaceFromObjectPath
              (CMPIObjectPath* op, CMPIObjectPath* src)
	{ return ((op)->ft->setNameSpaceFromObjectPath((op),(src))); }
#else
  #define CMSetNameSpaceFromObjectPath(p,s) \
                           ((p)->ft->setNameSpaceFromObjectPath((p),(s)))
#endif

#ifdef CMPI_INLINE
      /** Set/replace hostname, nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
   inline static   CMPIStatus CMSetHostAndNameSpaceFromObjectPath
              (CMPIObjectPath* op,
               CMPIObjectPath* src)
	{ return ((op)->ft->setHostAndNameSpaceFromObjectPath((op),(src))); }
#else
  #define CMSetHostAndNameSpaceFromObjectPath(p,s) \
                     ((p)->ft->setHostAndNameSpaceFromObjectPath((p),(s)))
#endif

#if defined(CMPI_VER_86)
#ifdef CMPI_INLINE
      /** Set/replace hostname, nameSpace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
   inline static   CMPIString* CMObjectPathToString
              (CMPIObjectPath* op, CMPIStatus* rc)
	{ return ((op)->ft->toString(op),(rc))); }
#else
  #define CMObjectPathToString(p,rc) \
                     ((p)->ft->toString((p),(rc)))
#endif
#endif

    // CMPIArray macros


#ifdef CMPI_INLINE
      /** Gets the number of elements contained in this Array.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
   inline static   CMPICount CMGetArrayCount
             (CMPIArray* ar, CMPIStatus* rc)
	{ return ((ar)->ft->getSize((ar),(rc))); }
#else
   #define CMGetArrayCount(a,rc)             ((a)->ft->getSize((a),(rc)))
#endif

#ifdef CMPI_INLINE
      /** Gets the element type.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
   inline static   CMPIType CMGetArrayType
             (CMPIArray* ar, CMPIStatus* rc)
	{ return ((ar)->ft->getSimpleType((ar),(rc))); }
#else
   #define CMGetArrayType(a,rc)        ((a)->ft->getSimpleType((a),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Gets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
   inline static   CMPIData CMGetArrayElementAt
             (CMPIArray* ar, CMPICount index, CMPIStatus* rc)
	{ return ((ar)->ft->getElementAt((ar),(index),(rc))); }
#else
   #define CMGetArrayElementAt(a,n,rc) \
                                    ((a)->ft->getElementAt((a),(n),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Sets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
   inline static   CMPIStatus CMSetArrayElementAt
             (CMPIArray* ar, CMPICount index, CMPIValue* value, CMPIType type)
	{ return ((ar)->ft->setElementAt((ar),(index),(value),(type))); }
#else
   #define CMSetArrayElementAt(a,n,v,t) \
                     ((a)->ft->setElementAt((a),(n),(CMPIValue*)(v),(t)))
#endif



    // CMPIARgs macros


#ifdef CMPI_INLINE
      /** Adds/replaces a named argument.
	 @param as Args this pointer.
         @param name Argument name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
   inline static   CMPIStatus CMAddArg
              (CMPIArgs* as, char* name ,void* value,
               CMPIType type)
	{ return  ((as)->ft->addArg((as),(name),(CMPIValue*)(value),(type))); }
#else
  #define CMAddArg(a,n,v,t) \
                           ((a)->ft->addArg((a),(n),(CMPIValue*)(v),(t)))
#endif

#ifdef CMPI_INLINE
       /** Gets a named argument value.
	 @param as Args this pointer.
	 @param name Argument name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
  inline static   CMPIData CMGetArg
              (CMPIArgs* as, char* name, CMPIStatus* rc)
	{ return  ((as)->ft->getArg((as),(name),(rc))); }
#else
  #define CMGetArg(a,n,rc)                ((a)->ft->getArg((a),(n),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Gets a Argument value defined by its index.
	 @param as Args this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned argument name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
   inline static   CMPIData CMGetArgAt
              (CMPIArgs* as, unsigned int index, CMPIString** name,
	       CMPIStatus* rc)
	{ return  ((as)->ft->getArgAt((as),(index),(name),(rc))); }
#else
  #define CMGetArgAt(a,p,n,rc)       ((a)->ft->getArgAt((a),(p),(n),(rc)))
#endif

#ifdef CMPI_INLINE
      /** Gets the number of arguments contained in this Args.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
   inline static   unsigned int CMGetArgCount
              (CMPIArgs* as, CMPIStatus* rc)
	{ return  ((as)->ft->getArgCount((as),(rc))); }
#else
  #define CMGetArgCount(a,rc)            ((a)->ft->getArgCount((a),(rc)))
#endif



    // CMPIString Macros


#ifdef CMPI_INLINE
       /** Get a pointer to a C char* representation of this String.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to char* representation.
      */
   inline static   char* CMGetCharsPtr
             (CMPIString* st, CMPIStatus* rc)
	{ return  ((st)->ft->getCharPtr((st),(rc))); }
#else
  #define CMGetCharsPtr(st,rc)              ((st)->ft->getCharPtr((st),(rc)))
#endif



    // CMPIDateTime macros


#ifdef CMPI_INLINE
       /** Get DateTime setting in UTC string format.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime as UTC string.
      */
   inline static   CMPIString* CMGetStringFormat
             (CMPIDateTime* dt, CMPIStatus* rc)
	{ return ((dt)->ft->getStringFormat((dt),(rc))); }
#else
  #define CMGetStringFormat(d,rc)    ((d)->ft->getStringFormat((d),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Get DateTime setting in binary format (in microsecods
	       starting since 00:00:00 GMT, Jan 1,1970).
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime in binary.
      */
   inline static   CMPIUint64 CMGetBinaryFormat
             (CMPIDateTime* dt, CMPIStatus* rc)
	{ return ((dt)->ft->getBinaryFormat((dt),(rc))); }
#else
  #define CMGetBinaryFormat(d,rc)    ((d)->ft->getBinaryFormat((d),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Tests whether DateTime is an interval value.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True if interval value.
      */
   inline static   CMPIBoolean CMIsInterval
              (CMPIDateTime* dt, CMPIStatus* rc)
	{ return ((dt)->ft->isInterval((dt),(rc))); }
#else
  #define CMIsInterval(d,rc)              ((d)->ft->isInterval((d),(rc)))
#endif


    // CMPIEnumeration Macros


#ifdef CMPI_INLINE
       /** Get the next element of this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
   inline static   CMPIData CMGetNext
             (CMPIEnumeration* en, CMPIStatus* rc)
	{ return ((en)->ft->getNext((en),(rc))); }
#else
  #define CMGetNext(n,rc)                    ((n)->ft->getNext((n),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Test for any elements left in this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True or false.
      */
   inline static   CMPIBoolean CMHasNext
             (CMPIEnumeration* en, CMPIStatus* rc)
	{ return ((en)->ft->hasNext((en),(rc))); }
#else
  #define CMHasNext(n,rc)                    ((n)->ft->hasNext((n),(rc)))
#endif

#ifdef CMPI_INLINE
       /** Convert this Enumeration into an Array.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Array.
      */
   inline static   CMPIArray* CMToArray
             (CMPIEnumeration* en, CMPIStatus* rc)
	{ return ((en)->ft->toArray((en),(rc))); }
#else
  #define CMToArray(n,rc)                   ((n)->ft->toArray((n),(rc)))
#endif



#endif // _CMCIMACS_H_
