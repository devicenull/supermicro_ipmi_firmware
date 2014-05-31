
/*
 * cmpidt.h
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
 * CMPI data types.
 *
*/

#ifndef _CMCIDT_H_
#define _CMCIDT_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// defintion of version numbers to be used by providers using CMBrokerVersion()
// They indicate what CMPI version is supported by both the broker and its adapter

   #define CMPIVersion051 51     //  0.51
   #define CMPIVersion060 60     //  0.60
   #define CMPIVersion070 70     //  0.70
   #define CMPIVersion080 80     //  0.80
   #define CMPIVersion085 85     //  0.85
   #define CMPIVersion086 86     //  0.86
   #define CMPIVersion087 87     //  0.87
   #define CMPIVersion090 90     //  0.90


// CMPI_VERSION compile switch should be used during MI compilation only.
// It is used define minimal version support needed from Management Broker.
// This value will be set in <mi-name>_Create<mi-type>MI.mi_version

#ifdef CMPI_VERSION
  #if (CMPI_VERSION==80)
     #define CMPI_VER_80 1
  #elif (CMPI_VERSION==85)
     #define CMPI_VER_85 1
  #elif (CMPI_VERSION==86)
     #define CMPI_VER_86 1
   #elif (CMPI_VERSION==87)
     #define CMPI_VER_87 1
   #elif (CMPI_VERSION==90)
     #define CMPI_VER_90 1
  #else
     #error Unsupported CMPI_VERSION defined
  #endif
#else
  #define CMPI_VER_86
#endif


// CMPI_VER_x switche is used by Management Broker implementations only.

// It defines the CMPI version supported by the Managmeent Broker.
// This value must be set in the ftVersion field of all functions tables.

// Version definitions are cummulative.
// A new version definition must #define all previous definitions.

// During MI loading MBs must ensure that
//  <mi-name>_Create<mi-type>MI.miVersion<=<mi-name>_Create<mi-type>MI.ftVersion
// If this is not the case, the MI might require higher version MB support.

#if   defined (CMPI_VER_90) || defined(CMPI_VER_ALL)
   // added Ext function table and getKeyList
  #define CMPI_VER_87
  #define CMPI_VER_86
  #define CMPI_VER_85
  #define CMPI_VER_80
  #define CMPICurrentVersion CMPIVersion090
#elif   defined (CMPI_VER_87) || defined(CMPI_VER_ALL)
   // added evaluateUsingAccessor in _CMPISelectExp
  #define CMPI_VER_86
  #define CMPI_VER_85
  #define CMPI_VER_80
  #define CMPICurrentVersion CMPIVersion087
#elif defined (CMPI_VER_86) || defined(CMPI_VER_ALL)
   // enable() disable() support in _CMPIIndicationMIFT
   // toString() in _CMPIObjectPathFT
   // support for NULL return from <mi-name>_Create<mi-type>MI
  #define CMPI_VER_85
  #define CMPI_VER_80
  #define CMPICurrentVersion CMPIVersion086
#elif   defined (CMPI_VER_85) || defined(CMPI_VER_ALL)
   // getMessage() globalization support in _CMPIBrokerEncFT
  #define CMPI_VER_80
  #define CMPICurrentVersion CMPIVersion085
#elif defined (CMPI_VER_80) || defined(CMPI_VER_ALL)
  #define CMPICurrentVersion CMPIVersion080
#else  // default version
  #define CMPI_VER_90
  #define CMPI_VER_87
  #define CMPI_VER_86
  #define CMPI_VER_85
  #define CMPI_VER_80
  #define CMPICurrentVersion CMPIVersion090
#endif


   struct _CMPIInstance;
   struct _CMPIObjectPath;
   struct _CMPIArgs;
   struct _CMPIEnumeration;
   struct _CMPIArray;
   struct _CMPIString;
   struct _CMPIDateTime;
   struct _CMPIConstClass;
   
   typedef struct _CMPIInstance       CMPIInstance;
   typedef struct _CMPIObjectPath     CMPIObjectPath;
   typedef struct _CMPIArgs           CMPIArgs;
   typedef struct _CMPIEnumeration    CMPIEnumeration;
   typedef struct _CMPIArray          CMPIArray;
   typedef struct _CMPIString         CMPIString;
   typedef struct _CMPIDateTime       CMPIDateTime;
   typedef struct _CMPIConstClass     CMPIConstClass;

   struct _CMPIInstanceFT;
   struct _CMPIObjectPathFT;
   struct _CMPIArgsFT;
   struct _CMPIEnumerationFT;
   struct _CMPIArrayFT;
   struct _CMPIStringFT;
   struct _CMPIDateTimeFT;

   typedef struct _CMPIInstanceFT      CMPIInstanceFT;
   typedef struct _CMPIObjectPathFT    CMPIObjectPathFT;
   typedef struct _CMPIArgsFT          CMPIArgsFT;
   typedef struct _CMPIEnumerationFT   CMPIEnumerationFT;
   typedef struct _CMPIArrayFT         CMPIArrayFT;
   typedef struct _CMPIStringFT        CMPIStringFT;
   typedef struct _CMPIDateTimeFT      CMPIDateTimeFT;


   typedef unsigned char              CMPIBoolean;
   typedef unsigned short             CMPIChar16;
   typedef unsigned char              CMPIUint8;
   typedef unsigned short             CMPIUint16;
   typedef unsigned long              CMPIUint32;
#ifndef CMPI_PLATFORM_WIN32_IX86_MSVC
   typedef unsigned long long         CMPIUint64;
#else
   typedef unsigned __int64           CMPIUint64;
#endif
   typedef signed char                CMPISint8;
   typedef short                      CMPISint16;
   typedef long                       CMPISint32;
#ifndef CMPI_PLATFORM_WIN32_IX86_MSVC
   typedef long long                  CMPISint64;
#else
   typedef __int64                    CMPISint64;
#endif
   typedef float                      CMPIReal32;
   typedef double                     CMPIReal64;

   typedef struct _CMPIValuePtr {
     void *ptr;
     unsigned int length;
   } CMPIValuePtr;

   typedef union _CMPIValue {
        CMPIUint64           uint64;
        CMPIUint32           uint32;
        CMPIUint16           uint16;
        CMPIUint8            uint8;
        CMPISint64           sint64;
        CMPISint32           sint32;
        CMPISint16           sint16;
        CMPISint8            sint8;
        CMPIReal64           real64;
        CMPIReal32           real32;
        CMPIBoolean          boolean;
        CMPIChar16           char16;

        CMPIConstClass*      cls;
        CMPIInstance*        inst;
        CMPIObjectPath*      ref;
        CMPIArgs*            args;
        CMPIEnumeration*     Enum;
        CMPIArray*           array;
        CMPIString*          string;
        char*                chars;
        CMPIDateTime*        dateTime;
        CMPIValuePtr         dataPtr;

        CMPISint8            Byte;
        CMPISint16           Short;
        CMPISint32           Int;
        CMPISint64           Long;
        CMPIReal32           Float;
        CMPIReal64           Double;
   } CMPIValue;


   typedef unsigned short CMPIType;

        #define CMPI_null         0

        #define CMPI_SIMPLE       (2)
        #define CMPI_boolean      (2+0)
        #define CMPI_char16       (2+1)

        #define CMPI_REAL         ((2)<<2)
        #define CMPI_real32       ((2+0)<<2)
        #define CMPI_real64       ((2+1)<<2)

        #define CMPI_UINT         ((8)<<4)
        #define CMPI_uint8        ((8+0)<<4)
        #define CMPI_uint16       ((8+1)<<4)
        #define CMPI_uint32       ((8+2)<<4)
        #define CMPI_uint64       ((8+3)<<4)
        #define CMPI_SINT         ((8+4)<<4)
        #define CMPI_sint8        ((8+4)<<4)
        #define CMPI_sint16       ((8+5)<<4)
        #define CMPI_sint32       ((8+6)<<4)
        #define CMPI_sint64       ((8+7)<<4)
        #define CMPI_INTEGER      ((CMPI_UINT | CMPI_SINT))

        #define CMPI_ENC          ((16)<<8)
        #define CMPI_instance     ((16+0)<<8)
        #define CMPI_ref          ((16+1)<<8)
        #define CMPI_args         ((16+2)<<8)
        #define CMPI_class        ((16+3)<<8)
        #define CMPI_filter       ((16+4)<<8)
        #define CMPI_enumeration  ((16+5)<<8)
        #define CMPI_string       ((16+6)<<8)
        #define CMPI_chars        ((16+7)<<8)
        #define CMPI_dateTime     ((16+8)<<8)
        #define CMPI_ptr          ((16+9)<<8)
        #define CMPI_charsptr     ((16+10)<<8)

        #define CMPI_ARRAY        ((1)<<13)
        #define CMPI_SIMPLEA      (CMPI_ARRAY | CMPI_SIMPLE)
        #define CMPI_booleanA     (CMPI_ARRAY | CMPI_boolean)
        #define CMPI_char16A      (CMPI_ARRAY | CMPI_char16)

        #define CMPI_REALA        (CMPI_ARRAY | CMPI_REAL)
        #define CMPI_real32A      (CMPI_ARRAY | CMPI_real32)
        #define CMPI_real64A      (CMPI_ARRAY | CMPI_real64)

        #define CMPI_UINTA        (CMPI_ARRAY | CMPI_UINT)
        #define CMPI_uint8A       (CMPI_ARRAY | CMPI_uint8)
        #define CMPI_uint16A      (CMPI_ARRAY | CMPI_uint16)
        #define CMPI_uint32A      (CMPI_ARRAY | CMPI_uint32)
        #define CMPI_uint64A      (CMPI_ARRAY | CMPI_uint64)
        #define CMPI_SINTA        (CMPI_ARRAY | CMPI_SINT)
        #define CMPI_sint8A       (CMPI_ARRAY | CMPI_sint8)
        #define CMPI_sint16A      (CMPI_ARRAY | CMPI_sint16)
        #define CMPI_sint32A      (CMPI_ARRAY | CMPI_sint32)
        #define CMPI_sint64A      (CMPI_ARRAY | CMPI_sint64)
        #define CMPI_INTEGERA     (CMPI_ARRAY | CMPI_INTEGER)

        #define CMPI_ENCA         (CMPI_ARRAY | CMPI_ENC)
        #define CMPI_stringA      (CMPI_ARRAY | CMPI_string)
        #define CMPI_charsA       (CMPI_ARRAY | CMPI_chars)
        #define CMPI_dateTimeA    (CMPI_ARRAY | CMPI_dateTime)
        #define CMPI_instanceA    (CMPI_ARRAY | CMPI_instance)
        #define CMPI_refA         (CMPI_ARRAY | CMPI_ref)
        #define CMPI_ptrA         (CMPI_ARRAY | CMPI_ptr)
        #define CMPI_charsptrA    (CMPI_ARRAY | CMPI_charsptr)

	// the following are CMPIObjectPath key-types synonyms
	// and are valid only when CMPI_keyValue of CMPIValueState is set

        #define CMPI_keyInteger   (CMPI_sint64)
	#define CMPI_keyString    (CMPI_string)
	#define CMPI_keyBoolean   (CMPI_boolean)
	#define CMPI_keyRef       (CMPI_ref)

	// the following are predicate types only

        #define CMPI_charString      (CMPI_string)
        #define CMPI_integerString   (CMPI_string | CMPI_sint64)
        #define CMPI_realString      (CMPI_string | CMPI_real64)
        #define CMPI_numericString   (CMPI_string | CMPI_sint64 | CMPI_real64)
	#define CMPI_booleanString   (CMPI_string | CMPI_boolean)
	#define CMPI_dateTimeString  (CMPI_string | CMPI_dateTime)
        #define CMPI_classNameString (CMPI_string | CMPI_class)
        #define CMPI_nameString      (CMPI_string | ((16+10)<<8))

   typedef unsigned short CMPIValueState;
        #define CMPI_goodValue (0)
        #define CMPI_nullValue (1<<8)
        #define CMPI_keyValue  (2<<8)
        #define CMPI_notFound  (4<<8)
        #define CMPI_badValue  (0x80<<8)

   typedef struct _CMPIData {
      CMPIType type;
      CMPIValueState state;
      CMPIValue value;
   } CMPIData;


#ifdef CMPI_VER_87
   typedef CMPIData CMPIAccessor(const char*, void* parm);
#endif


#ifndef CMPI_NO_SYNONYM_SUPPORT
   #define CMPI_Byte    CMPI_sint8
   #define CMPI_Short   CMPI_sint16
   #define CMPI_Int     CMPI_sint32
   #define CMPI_Long    CMPI_sint64
   #define CMPI_Float   CMPI_real32
   #define CMPI_Double  CMPI_real64

   #define CMPI_ByteA   CMPI_sint8A
   #define CMPI_ShortA  CMPI_sint16A
   #define CMPI_IntA    CMPI_sint32A
   #define CMPI_LongA   CMPI_sint64A
   #define CMPI_FloatA  CMPI_real32A
   #define CMPI_DoubleA CMPI_real64A
#endif // CMPI_NO_SYNONYM_SUPPORT

   typedef unsigned int CMPICount;


   typedef unsigned int CMPIFlags;

   #define CMPI_FLAG_LocalOnly          1
   #define CMPI_FLAG_DeepInheritance    2
   #define CMPI_FLAG_IncludeQualifiers  4
   #define CMPI_FLAG_IncludeClassOrigin 8

   #define CMPIInvocationFlags "CMPIInvocationFlags"
   #define CMPIPrincipal "CMPIPrincipal"

   typedef enum _CMPIrc {
     CMPI_RC_OK                               =0,
     CMPI_RC_ERR_FAILED                       =1,
     CMPI_RC_ERR_ACCESS_DENIED                =2,
     CMPI_RC_ERR_INVALID_NAMESPACE            =3,
     CMPI_RC_ERR_INVALID_PARAMETER            =4,
     CMPI_RC_ERR_INVALID_CLASS                =5,
     CMPI_RC_ERR_NOT_FOUND                    =6,
     CMPI_RC_ERR_NOT_SUPPORTED                =7,
     CMPI_RC_ERR_CLASS_HAS_CHILDREN           =8,
     CMPI_RC_ERR_CLASS_HAS_INSTANCES          =9,
     CMPI_RC_ERR_INVALID_SUPERCLASS           =10,
     CMPI_RC_ERR_ALREADY_EXISTS               =11,
     CMPI_RC_ERR_NO_SUCH_PROPERTY             =12,
     CMPI_RC_ERR_TYPE_MISMATCH                =13,
     CMPI_RC_ERR_QUERY_LANGUAGE_NOT_SUPPORTED =14,
     CMPI_RC_ERR_INVALID_QUERY                =15,
     CMPI_RC_ERR_METHOD_NOT_AVAILABLE         =16,
     CMPI_RC_ERR_METHOD_NOT_FOUND             =17,
     CMPI_RC_DO_NOT_UNLOAD                    =50,
     CMPI_RC_NEVER_UNLOAD                     =51,
     CMPI_RC_ERROR_SYSTEM                     =100,
     CMPI_RC_ERROR                            =200
   } CMPIrc;

   typedef struct _CMPIStatus {
      CMPIrc rc;
      CMPIString *msg;
   } CMPIStatus;


   /* Management Broker classification and feature support */

   #define CMPI_MB_Class_0     0x00000001
   #define CMPI_MB_Class_1     0x00000003
   #define CMPI_MB_Class_2     0x00000007

   #define CMPI_MB_Supports_PropertyMI         0x00000100
   #define CMPI_MB_Supports_IndicationMI       0x00000200
   #define CMPI_MB_Supports_IndicationPolling  0x00000400
   #define CMPI_MB_Supports_QueryNormalization 0x00000800
   #define CMPI_MB_Supports_Qualifier          0x00001000
   #define CMPI_MB_Supports_Schema             0x00003000


#ifdef __cplusplus
 };
#endif

#endif // _CMCIDT_H_



