
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
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Copyright (c) 2006 The Open Group                                         */
/*                                                                           */
/* Permission is hereby granted, free of charge, to any person obtaining a   */
/* copy of this software (the "Software"), to deal in the Software without   */
/* restriction, including without limitation the rights to use, copy,        */
/* modify, merge, publish, distribute, sublicense, and/or sell copies of     */
/* the Software, and to permit persons to whom the Software is furnished     */
/* to do so, subject to the following conditions:                            */
/*                                                                           */
/* The above copyright notice and this permission notice shall be included   */
/* in all copies or substantial portions of the Software.                    */
/*                                                                           */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.    */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY      */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT */
/* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  */
/* THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                */
/*                                                                           */
/* ------------------------------------------------------------------------- */

#ifndef _CMPIDT_H_
#   define _CMPIDT_H_

#   include "cmpipl.h"

#   ifdef __cplusplus
extern "C"
{
#   endif

// defintion of version numbers to be used by providers using CMBrokerVersion()
// They indicate what CMPI version is supported by both the broker and its adapter

#   define CMPIVersion100 100
  //  1.00
#   define CMPIVersion200 200
  //  2.00

// CMPI_VERSION compile switch should be used during MI compilation only.
// It is used define minimal version support needed from Management Broker.
// This value will be set in <mi-name>_Create<mi-type>MI.mi_version

#   ifdef CMPI_VERSION
#      if (CMPI_VERSION==200)
#         define CMPI_VER_200 1
#      elif (CMPI_VERSION==100)
#         define CMPI_VER_100 1
// Please note that if the CMPI implementations supports "historical" CMPI
// levels (90, 86, etc) this would be the place to add the checks for the
// version.
//
#      else
#         error Unsupported CMPI_VERSION defined
#      endif
#   else
#      define CMPI_VER_200
#   endif


// CMPI_VER_x switch is used by Management Broker implementations only.

// It defines the CMPI version supported by the Management Broker.
// This value must be set in the ftVersion field of all functions tables.

// Version definitions are cumulative.
// A new version definition must #define all previous definitions.

// During MI loading MBs must ensure that
//  <mi-name>_Create<mi-type>MI.miVersion<=<mi-name>_Create<mi-type>MI.ftVersion
// If this is not the case, the MI might require higher version MB support.
#   if defined (CMPI_VER_200) || defined(CMPI_VER_ALL)
#      define CMPI_VER_100
#      define CMPICurrentVersion CMPIVersion200
#   elif   defined (CMPI_VER_100) || defined(CMPI_VER_ALL)
#      define CMPICurrentVersion CMPIVersion100

// Please note that if the CMPI implementations supports "historical" CMPI
// levels (90, 86, etc) this would be the place to add the checks for the
// version.

#   else                        // default version
#      define CMPI_VER_200
#      define CMPI_VER_100
#      define CMPICurrentVersion CMPIVersion200
#   endif

/** A platform independent CMPI data type, usually an unsigned int */
  typedef unsigned int CMPICount;


  struct _CMPIBroker;
  struct _CMPIInstance;
  struct _CMPIObjectPath;
  struct _CMPIArgs;
  struct _CMPISelectExp;
  struct _CMPISelectCond;
  struct _CMPISubCond;
  struct _CMPIPredicate;
  struct _CMPIEnumeration;
  struct _CMPIArray;
  struct _CMPIString;
  struct _CMPIResult;
  struct _CMPIContext;
  struct _CMPIDateTime;

  typedef struct _CMPIBroker CMPIBroker;
  typedef struct _CMPIInstance CMPIInstance;
  typedef struct _CMPIObjectPath CMPIObjectPath;
  typedef struct _CMPIArgs CMPIArgs;
  typedef struct _CMPISelectExp CMPISelectExp;
  typedef struct _CMPISelectCond CMPISelectCond;
  typedef struct _CMPISubCond CMPISubCond;
  typedef struct _CMPIPredicate CMPIPredicate;
  typedef struct _CMPIEnumeration CMPIEnumeration;
  typedef struct _CMPIArray CMPIArray;
  typedef struct _CMPIString CMPIString;
  typedef struct _CMPIResult CMPIResult;
  typedef struct _CMPIContext CMPIContext;
  typedef struct _CMPIDateTime CMPIDateTime;

#   ifdef CMPI_VER_200
  struct _CMPIError;

  typedef struct _CMPIError CMPIError;
#   endif

  struct _CMPIBrokerFT;
  struct _CMPIBrokerEncFT;
  struct _CMPIInstanceFT;
  struct _CMPIObjectPathFT;
  struct _CMPIArgsFT;
  struct _CMPISelectExpFT;
  struct _CMPISelectCondFT;
  struct _CMPISelectCondDocFT;
  struct _CMPISelectCondCodFT;
  struct _CMPISubCondFT;
  struct _CMPIPredicateFT;
  struct _CMPIEnumerationFT;
  struct _CMPIArrayFT;
  struct _CMPIStringFT;
  struct _CMPIresultFT;
  struct _CMPIContextFT;
  struct _CMPIDateTimeFT;
#   ifdef CMPI_VER_200
  struct _CMPIBrokerMemFT;
  struct _CMPIErrorFT;

  typedef struct _CMPIBrokerMemFT CMPIBrokerMemFT;
  typedef struct _CMPIErrorFT CMPIErrorFT;
#   endif
  typedef struct _CMPIBrokerFT CMPIBrokerFT;
  typedef struct _CMPIBrokerEncFT CMPIBrokerEncFT;
  typedef struct _CMPIBrokerExtFT CMPIBrokerExtFT;
  typedef struct _CMPIInstanceFT CMPIInstanceFT;
  typedef struct _CMPIObjectPathFT CMPIObjectPathFT;
  typedef struct _CMPIArgsFT CMPIArgsFT;
  typedef struct _CMPISelectExpFT CMPISelectExpFT;
  typedef struct _CMPISelectCondFT CMPISelectCondFT;
  typedef struct _CMPISubCondFT CMPISubCondFT;
  typedef struct _CMPIPredicateFT CMPIPredicateFT;
  typedef struct _CMPIEnumerationFT CMPIEnumerationFT;
  typedef struct _CMPIArrayFT CMPIArrayFT;
  typedef struct _CMPIStringFT CMPIStringFT;
  typedef struct _CMPIResultFT CMPIResultFT;
  typedef struct _CMPIContextFT CMPIContextFT;
  typedef struct _CMPIDateTimeFT CMPIDateTimeFT;


  typedef unsigned char CMPIBoolean;
  typedef unsigned short CMPIChar16;
  typedef unsigned char CMPIUint8;
  typedef unsigned short CMPIUint16;
  typedef unsigned int CMPIUint32;
#   ifndef CMPI_PLATFORM_WIN32_IX86_MSVC
  typedef unsigned long long CMPIUint64;
#   else
  typedef unsigned __int64 CMPIUint64;
#   endif
  typedef signed char CMPISint8;
  typedef short CMPISint16;
  typedef signed int CMPISint32;
#   ifndef CMPI_PLATFORM_WIN32_IX86_MSVC
  typedef long long CMPISint64;
#   else
  typedef __int64 CMPISint64;
#   endif
  typedef float CMPIReal32;
  typedef double CMPIReal64;

  typedef struct _CMPIValuePtr
  {
    void *ptr;
    CMPICount length;
  } CMPIValuePtr;


  typedef union _CMPIValue
  {
    CMPIUint64 uint64;
    CMPIUint32 uint32;
    CMPIUint16 uint16;
    CMPIUint8 uint8;
    CMPISint64 sint64;
    CMPISint32 sint32;
    CMPISint16 sint16;
    CMPISint8 sint8;
    CMPIReal64 real64;
    CMPIReal32 real32;
    CMPIBoolean boolean;
    CMPIChar16 char16;

    CMPIInstance *inst;
    CMPIObjectPath *ref;
    CMPIArgs *args;
    CMPISelectExp *filter;
    CMPIEnumeration *Enum;
    CMPIArray *array;
    CMPIString *string;
    char *chars;
    CMPIDateTime *dateTime;
    CMPIValuePtr dataPtr;

    CMPISint8 Byte;
    CMPISint16 Short;
    CMPISint32 Int;
    CMPISint64 Long;
    CMPIReal32 Float;
    CMPIReal64 Double;
  } CMPIValue;


  typedef unsigned short CMPIType;

#   define CMPI_null         0

#   define CMPI_SIMPLE       (2)
#   define CMPI_boolean      (2+0)
#   define CMPI_char16       (2+1)

#   define CMPI_REAL         ((2)<<2)
#   define CMPI_real32       ((2+0)<<2)
#   define CMPI_real64       ((2+1)<<2)

#   define CMPI_UINT         ((8)<<4)
#   define CMPI_uint8        ((8+0)<<4)
#   define CMPI_uint16       ((8+1)<<4)
#   define CMPI_uint32       ((8+2)<<4)
#   define CMPI_uint64       ((8+3)<<4)
#   define CMPI_SINT         ((8+4)<<4)
#   define CMPI_sint8        ((8+4)<<4)
#   define CMPI_sint16       ((8+5)<<4)
#   define CMPI_sint32       ((8+6)<<4)
#   define CMPI_sint64       ((8+7)<<4)
#   define CMPI_INTEGER      ((CMPI_UINT | CMPI_SINT))

#   define CMPI_ENC          ((16)<<8)
#   define CMPI_instance     ((16+0)<<8)
#   define CMPI_ref          ((16+1)<<8)
#   define CMPI_args         ((16+2)<<8)
#   define CMPI_class        ((16+3)<<8)
#   define CMPI_filter       ((16+4)<<8)
#   define CMPI_enumeration  ((16+5)<<8)
#   define CMPI_string       ((16+6)<<8)
#   define CMPI_chars        ((16+7)<<8)
#   define CMPI_dateTime     ((16+8)<<8)
#   define CMPI_ptr          ((16+9)<<8)
#   define CMPI_charsptr     ((16+10)<<8)

#   define CMPI_ARRAY        ((1)<<13)
#   define CMPI_SIMPLEA      (CMPI_ARRAY | CMPI_SIMPLE)
#   define CMPI_booleanA     (CMPI_ARRAY | CMPI_boolean)
#   define CMPI_char16A      (CMPI_ARRAY | CMPI_char16)

#   define CMPI_REALA        (CMPI_ARRAY | CMPI_REAL)
#   define CMPI_real32A      (CMPI_ARRAY | CMPI_real32)
#   define CMPI_real64A      (CMPI_ARRAY | CMPI_real64)

#   define CMPI_UINTA        (CMPI_ARRAY | CMPI_UINT)
#   define CMPI_uint8A       (CMPI_ARRAY | CMPI_uint8)
#   define CMPI_uint16A      (CMPI_ARRAY | CMPI_uint16)
#   define CMPI_uint32A      (CMPI_ARRAY | CMPI_uint32)
#   define CMPI_uint64A      (CMPI_ARRAY | CMPI_uint64)
#   define CMPI_SINTA        (CMPI_ARRAY | CMPI_SINT)
#   define CMPI_sint8A       (CMPI_ARRAY | CMPI_sint8)
#   define CMPI_sint16A      (CMPI_ARRAY | CMPI_sint16)
#   define CMPI_sint32A      (CMPI_ARRAY | CMPI_sint32)
#   define CMPI_sint64A      (CMPI_ARRAY | CMPI_sint64)
#   define CMPI_INTEGERA     (CMPI_ARRAY | CMPI_INTEGER)

#   define CMPI_ENCA         (CMPI_ARRAY | CMPI_ENC)
#   define CMPI_stringA      (CMPI_ARRAY | CMPI_string)
#   define CMPI_charsA       (CMPI_ARRAY | CMPI_chars)
#   define CMPI_dateTimeA    (CMPI_ARRAY | CMPI_dateTime)
#   define CMPI_instanceA    (CMPI_ARRAY | CMPI_instance)
#   define CMPI_refA         (CMPI_ARRAY | CMPI_ref)
#   define CMPI_charsptrA    (CMPI_ARRAY | CMPI_charsptr)

  // the following are CMPIObjectPath key-types synonyms
  // and are valid only when CMPI_keyValue of CMPIValueState is set

#   define CMPI_keyInteger   (CMPI_sint64)
#   define CMPI_keyString    (CMPI_string)
#   define CMPI_keyBoolean   (CMPI_boolean)
#   define CMPI_keyRef       (CMPI_ref)

  // the following are predicate types only

#   define CMPI_charString      (CMPI_string)
#   define CMPI_integerString   (CMPI_string | CMPI_sint64)
#   define CMPI_realString      (CMPI_string | CMPI_real64)
#   define CMPI_numericString   (CMPI_string | CMPI_sint64 | CMPI_real64)
#   define CMPI_booleanString   (CMPI_string | CMPI_boolean)
#   define CMPI_dateTimeString  (CMPI_string | CMPI_dateTime)
#   define CMPI_classNameString (CMPI_string | CMPI_class)
#   define CMPI_nameString      (CMPI_string | ((16+10)<<8))

/** Indicates the state of a CMPI value.
    <UL>
	 <LI>CMPI_goodValue - Value embedded in CMPIData object is valid.
	 <LI>CMPI_nullValue - Value embedded in CMPIData object is NULL.
	 <LI>CMPI_keyValue - Value embedded in CMPIData object is valid &amp; is a key
	 <LI>CMPI_notFound -
	 <LI>CMPI_badValue - Value embedded in CMPIData object is not valid.
	</UL>
*/
  typedef unsigned short CMPIValueState;
#   define CMPI_goodValue (0)
#   define CMPI_nullValue (1<<8)
#   define CMPI_keyValue  (2<<8)
#   define CMPI_notFound  (4<<8)
#   define CMPI_badValue  (0x80<<8)

/** Values transferred from CMPI functions to the MI return three
    components: the value, its state, and its type as defined by the schema.
    All three components are bundled into this one structure.
*/
  typedef struct _CMPIData
  {
    /** An unsigned short representing the type of the CMPIData object */
    CMPIType type;
	/** An unsigned short representing whether this CMPIData object is
        valid or not.
	*/
    CMPIValueState state;
	/** A union which holds the actual underlying value of the data object */
    CMPIValue value;
  } CMPIData;

  typedef CMPIData CMPIAccessor (const char *, void *parm);

#   ifndef CMPI_NO_SYNONYM_SUPPORT
#      define CMPI_Byte    CMPI_sint8
#      define CMPI_Short   CMPI_sint16
#      define CMPI_Int     CMPI_sint32
#      define CMPI_Long    CMPI_sint64
#      define CMPI_Float   CMPI_real32
#      define CMPI_Double  CMPI_real64

#      define CMPI_ByteA   CMPI_sint8A
#      define CMPI_ShortA  CMPI_sint16A
#      define CMPI_IntA    CMPI_sint32A
#      define CMPI_LongA   CMPI_sint64A
#      define CMPI_FloatA  CMPI_real32A
#      define CMPI_DoubleA CMPI_real64A
#   endif                       // CMPI_NO_SYNONYM_SUPPORT

  typedef void* CMPIMsgFileHandle;
  typedef void CMPIGcStat;

/** The CMPIFlags type is used to inform MI functions about options specified
    by the client and passed on to the MI for certain requests. CMPIFlags are
	not passed to MIs directly. MIs must use the CMPIContext services to gain
	access under the name CMPIInvocationFlags.
	<UL>
	 <LI>CMPI_FLAG_LocalOnly - Local only  attributes
	 <LI>CMPI_FLAG_DeepInheritance - Providers should provide deep inhertiance
	  information
	 <LI>CMPI_FLAG_IncludeQualifiers - <I>deprecated</I> Indicates that a
	  MI's should return qualifiers
	 <LI>CMPI_FLAG_IncludeClassOrigin - MI should return class origin info
	</UL>
*/
  typedef unsigned int CMPIFlags;

#   define CMPI_FLAG_LocalOnly          1
#   define CMPI_FLAG_DeepInheritance    2
#   define CMPI_FLAG_IncludeQualifiers  4
#   define CMPI_FLAG_IncludeClassOrigin 8

	/* Authenticated ID of the user requesting this MI invocation. */
#   define CMPIPrincipal       "CMPIPrincipal"
	/* CMPIFlags -  invocation flags as specified by the client. */
#   define CMPIInvocationFlags "CMPIInvocationFlags"
	/* Namespace for which the MI is started. */
#   define CMPIInitNameSpace   "CMPIInitNameSpace"
	/* The role assumed by the current authenticated user. */
#   define CMPIRole            "CMPIRole"
	/* The accept language from the request */
#   define CMPIAcceptLanguage  "CMPIAcceptLanguage"
	/* The content language of the request */
#   define CMPIContentLanguage "CMPIContentLanguage"

/** Enum which indicates success or failure, usually accessed as part
	of the CMPIStatus structure
*/
  typedef enum _CMPIrc
  {
	/** Success */
    CMPI_RC_OK = 0,
	/** Generic failure */
    CMPI_RC_ERR_FAILED = 1,
	/** Specified user does not have access to perform the requested action */
    CMPI_RC_ERR_ACCESS_DENIED = 2,
	/** invalid namespace specified */
    CMPI_RC_ERR_INVALID_NAMESPACE = 3,
	/** invalid parameter specified */
    CMPI_RC_ERR_INVALID_PARAMETER = 4,
	/** Invalid class specified */
    CMPI_RC_ERR_INVALID_CLASS = 5,
	/** Item was not found */
    CMPI_RC_ERR_NOT_FOUND = 6,
	/** Operation not supported */
    CMPI_RC_ERR_NOT_SUPPORTED = 7,
	/** Object has child objects */
    CMPI_RC_ERR_CLASS_HAS_CHILDREN = 8,
	/** Object has instances */
    CMPI_RC_ERR_CLASS_HAS_INSTANCES = 9,
	/** Invalid super class specified */
    CMPI_RC_ERR_INVALID_SUPERCLASS = 10,
	/** specified object already exists */
    CMPI_RC_ERR_ALREADY_EXISTS = 11,
	/** Property does not exist */
    CMPI_RC_ERR_NO_SUCH_PROPERTY = 12,
	/** This is a type mismatch */
    CMPI_RC_ERR_TYPE_MISMATCH = 13,
	/** Query language not supported */
    CMPI_RC_ERR_QUERY_LANGUAGE_NOT_SUPPORTED = 14,
	/** Invalid query */
    CMPI_RC_ERR_INVALID_QUERY = 15,
	/** Method is not available */
    CMPI_RC_ERR_METHOD_NOT_AVAILABLE = 16,
	/** could not find the specified method */
    CMPI_RC_ERR_METHOD_NOT_FOUND = 17,
	/** Returned by a MI to indicate that it should not be unloaded, only
		returned via a cleanup() call
	*/
    CMPI_RC_DO_NOT_UNLOAD = 50,
	/** Returned by a MI to indicate that it should never be unloaded, only
		returned via a cleanup() call
	*/
    CMPI_RC_NEVER_UNLOAD = 51,
    /* Internal CMPI return codes. */
    CMPI_RC_ERR_INVALID_HANDLE = 60,
    CMPI_RC_ERR_INVALID_DATA_TYPE = 61,
    /* Hosting OS errors. */
    CMPI_RC_ERROR_SYSTEM = 100,
    CMPI_RC_ERROR = 200
  } CMPIrc;

/** The status structure is used to indicate success or failure of a call */
  typedef struct _CMPIStatus
  {
	  /** The CMPIrc value.
		  @see _CMPIrc
	  */
    CMPIrc rc;
	  /** A text string representing the error message
		  @see CMPIString
	  */
    CMPIString *msg;
  } CMPIStatus;


  /* Management Broker capabilities and feature support */

#   define CMPI_MB_Class_0     0x00000001
#   define CMPI_MB_Class_1     0x00000003
#   define CMPI_MB_Class_2     0x00000007

#   define CMPI_MB_Supports_PropertyMI         0x00000100
#   define CMPI_MB_Supports_IndicationMI       0x00000200
#   define CMPI_MB_Supports_IndicationPolling  0x00000400
#   define CMPI_MB_Supports_QueryNormalization 0x00000800
#   define CMPI_MB_Supports_Qualifier          0x00001000
#   define CMPI_MB_Supports_Schema             0x00003000

#   ifdef CMPI_VER_200
#      define CMPI_MB_Supports_MemEnhancements   0x00004000
#      define CMPI_MB_Supports_Extended_Error    0x00008000
#   endif

#   define CMPI_MB_BasicRead 0x00000001
#   define CMPI_MB_BasicWrite 0x00000003
#   define CMPI_MB_InstanceManipulation 0x00000007
#   define CMPI_MB_AssociationTraversal 0x00000009
#   define CMPI_MB_QueryExecution 0x00000011
#   define CMPI_MB_QueryNormalization 0x00000031
#   define CMPI_MB_Indications 0x00000081
#   define CMPI_MB_BasicQualifierSupport 0x00000047
#   define CMPI_MB_OSEncapsulationSupport 0x00000100


  /* Query Predicate operations */

  typedef enum _CMPIPredOp
  {
    CMPI_PredOp_Equals = 1,
    CMPI_PredOp_NotEquals = 2,
    CMPI_PredOp_LessThan = 3,
    CMPI_PredOp_GreaterThanOrEquals = 4,
    CMPI_PredOp_GreaterThan = 5,
    CMPI_PredOp_LessThanOrEquals = 6,
    CMPI_PredOp_Isa = 7,
    CMPI_PredOp_NotIsa = 8,
    CMPI_PredOp_Like = 9,
    CMPI_PredOp_NotLike = 10
#   ifdef CMPI_VER_200
    ,
	CMPI_PredOp_Not_Null = 11,
    CMPI_PredOp_Null = 12,
    CMPI_PredOp_And = 13,
    CMPI_PredOp_Or = 14
#   endif
  } CMPIPredOp;

  /** Severity levels for logging functions */
  typedef enum _CMPISeverity
  {
	/** Error */
    CMPI_SEV_ERROR = 1,
	/** General info */
    CMPI_SEV_INFO = 2,
	/** Warning message */
    CMPI_SEV_WARNING = 3,
	/** Debug message */
    CMPI_DEV_DEBUG = 4
  } CMPISeverity;

  /** Logging level for trace functions*/
  typedef enum _CMPILevel
  {
	/** Generic information */
    CMPI_LEV_INFO = 1,
	/** warnings */
    CMPI_LEV_WARNING = 2,
	/** detailed/specific information */
    CMPI_LEV_VERBOSE = 3
  } CMPILevel;

  /** Type of query expression it is normalized to. */
  typedef enum _CMPISelectCondType
  {
	/** Disjuntion Of Conjunctions */
    CMPI_COND_DOC = 0,
	/** Conjuction of disjunctions */
    CMPI_COND_COD = 1
  } CMPISelectCondType;

#ifdef CMPI_VER_200
	/** Possible values an Error object can use in its type property
	    @version 2.0
	*/
typedef enum _CMPIErrorType {
	/** Unkown */
    UnknownErrorType = 0,
	/** Other */
    OtherErrorType = 1,
	/** Communications error */
    CommunicationsError = 2,
	/** QoS error */
    QualityOfServiceError = 3,
	/** Software error */
    SoftwareError = 4,
	/** Hardware error */
    HardwareError = 5,
	/** Environmental error */
    EnvironmentalError = 6,
	/** Security error */
    SecurityError = 7,
	/** over subscription error */
    Oversubscription_Error = 8,
	/** Unavailable resource */
    UnavailableResourceError = 9,
	/** Unsupported operation */
    UnsupportedOperationError = 10
} CMPIErrorType;

/** Possible values an Error object can use to indicate severity */
typedef enum _CMPIErrorSeverity {
	/** Unknown */
    ErrorSevUnknown = 0,
	/** Low severity */
    ErrorSevLow = 2,
	/** Medium Severity */
    ErrorSevMedium = 3,
	/** High severity */
    ErrorSevHigh = 4,
	/** Fatal error */
    ErrorSevFatal = 5,
} CMPIErrorSeverity;

/** Possible values an Error object can use to indicate the probable cause */
typedef enum _CMPIErrorProbableCause {
	/** Unknown */
	ErrorProbCauseUnknown = 0,
	/** Other cause */
	ErrorProbCauseOther = 1,
	/** Adpater card failure */
	Adapter_Card_Error = 2,
	/** Subsystem failure */
	Application_Subsystem_Failure = 3,
	/** Reduced bandwidth */
	Bandwidth_Reduced = 4,
	/** Could not establish connection */
	Connection_Establishment_Error = 5,
	/** protocol error */
	Communications_Protocol_Error = 6,
	/** Subsystem failure */
	Communications_Subsystem_Failure = 7,
	/** Configuration error */
	ConfigurationCustomization_Error = 8,
	/** Congested */
	Congestion = 9,
	/** Data is corrupt */
	Corrupt_Data = 10,
	/** CPU cycles exceeded */
	CPU_Cycles_Limit_Exceeded = 11,
	/* Dataset modem error */
	DatasetModem_Error = 12,
	/** Degraded signal */
	Degraded_Signal = 13,
	/** STE/DCE Interface Error */
	DTE_DCE_Interface_Error = 14,
	/** Door open */
	Enclosure_Door_Open = 15,
	/** Equipment malfunction */
	Equipment_Malfunction = 16,
	/** Excessive Vibration */
	Excessive_Vibration = 17,
	/** File format error */
	File_Format_Error = 18,
	/** Fire detected */
	Fire_Detected = 19,
	/** Flood detected */
	Flood_Detected = 20,
	/** framing error */
	Framing_Error = 21,
	/** HVAC problem */
	HVAC_Problem = 22,
	/* Humidity unacceptable */
	Humidity_Unacceptable = 23,
	/** IO device error */
	IO_Device_Error = 24,
	/** Input device error */
	Input_Device_Error = 25,
	/** LAN Error */
	LAN_Error = 26,
	/** Non-toxic leak detected */
	Non_Toxic_Leak_Detected = 27,
	/* Local node transmission error */
	Local_Node_Transmission_Error = 28,
	/** loss of frame */
	Loss_of_Frame = 29,
	/** loss of signal */
	Loss_of_Signal = 30,
	/** Material supply exhausted */
	Material_Supply_Exhausted = 31,
	/** Multiplexer problem */
	Multiplexer_Problem = 32,
	/** Out of memory */
	Out_of_Memory = 33,
	/** Output device error */
	Output_Device_Error = 34,
	/** Performance degraded */
	Performance_Degraded = 35,
	/** Power problem */
	Power_Problem = 36,
	/** Pressure unacceptable */
	Pressure_Unacceptable = 37,
	/** Processor problem */
	Processor_Problem = 38,
	/** Pump failure */
	Pump_Failure = 39,
	/** Queue size exceeded */
	Queue_Size_Exceeded = 40,
	/** Receive failure */
	Receive_Failure = 41,
	/** Receiver failure */
	Receiver_Failure = 42,
	/** Remote node transmission error */
	Remote_Node_Transmission_Error = 43,
	/** Resource at or nearing capacity */
	Resource_at_or_Nearing_Capacity = 44,
	/** Response time excessive */
	Response_Time_Excessive = 45,
	/** Retransmission rate excessive */
	Retransmission_Rate_Excessive = 46,
	/** Software Error */
	Software_Error = 47,
	/** Software terminated abnormally */
	Software_Program_Abnormally_Terminated = 48,
	/** Program error */
	Software_Program_Error = 49,
	/** Storage capacity problem */
	Storage_Capacity_Problem = 50,
	/** Temperature_Unacceptable */
	Temperature_Unacceptable = 51,
	/** Threshold_Crossed */
	Threshold_Crossed = 52,
	/** Timing_Problem */
	Timing_Problem = 53,
	/** Toxic_Leak_Detected */
	Toxic_Leak_Detected = 54,
	/** Transmit_Failure */
	Transmit_Failure = 55,
	/** Transmitter_Failure */
	Transmitter_Failure = 56,
	/** Underlying_Resource_Unavailable */
	Underlying_Resource_Unavailable = 57,
	/** Version_Mismatch */
	Version_Mismatch = 58,
	/** Previous_Alert_Cleared */
	Previous_Alert_Cleared = 59,
	/** Login_Attempts_Failed */
	Login_Attempts_Failed = 60,
	/** Software_Virus_Detected */
	Software_Virus_Detected = 61,
	/** Hardware_Security_Breached */
	Hardware_Security_Breached = 62,
	/** Denial_of_Service_Detected */
	Denial_of_Service_Detected = 63,
	/** Security_Credential_Mismatch */
	Security_Credential_Mismatch = 64,
	/** Unauthorized_Access */
	Unauthorized_Access = 65,
	/** Alarm_Received */
	Alarm_Received = 66,
	/** Loss_of_Pointer */
	Loss_of_Pointer = 67,
	/** Payload_Mismatch */
	Payload_Mismatch = 68,
	/** Transmission_Error */
	Transmission_Error = 69,
	/** Excessive_Error_Rate */
	Excessive_Error_Rate = 70,
	/** Trace_Problem */
	Trace_Problem = 71,
	/** Element_Unavailable */
	Element_Unavailable = 72,
	/** Element_Missing */
	Element_Missing = 73,
	/** Loss_of_Multi_Frame */
	Loss_of_Multi_Frame = 74,
	/** Broadcast_Channel_Failure */
	Broadcast_Channel_Failure = 75,
	/** Invalid_Message_Received */
	Invalid_Message_Received = 76,
	/** Routing_Failure */
	Routing_Failure = 77,
	/** Backplane_Failure */
	Backplane_Failure = 78,
	/** Identifier_Duplication */
	Identifier_Duplication = 79,
	/** Protection_Path_Failure */
	Protection_Path_Failure = 80,
	/** Sync_Loss_or_Mismatch */
	Sync_Loss_or_Mismatch = 81,
	/** Terminal_Problem */
	Terminal_Problem = 82,
	/** Real_Time_Clock_Failure */
	Real_Time_Clock_Failure = 83,
	/** Antenna_Failure */
	Antenna_Failure = 84,
	/** Battery_Charging_Failure */
	Battery_Charging_Failure = 85,
	/** Disk_Failure */
	Disk_Failure = 86,
	/** Frequency_Hopping_Failure */
	Frequency_Hopping_Failure = 87,
	/** Loss_of_Redundancy */
	Loss_of_Redundancy = 88,
	/** Power_Supply_Failure */
	Power_Supply_Failure = 89,
	/** Signal_Quality_Problem */
	Signal_Quality_Problem = 90,
	/** Battery_Discharging */
	Battery_Discharging = 91,
	/** Battery_Failure */
	Battery_Failure = 92,
	/** Commercial_Power_Problem */
	Commercial_Power_Problem = 93,
	/** Fan_Failure */
	Fan_Failure = 94,
	/** Engine_Failure */
	Engine_Failure = 95,
	/** Sensor_Failure */
	Sensor_Failure = 96,
	/** Fuse_Failure */
	Fuse_Failure = 97,
	/** Generator_Failure */
	Generator_Failure = 98,
	/** Low_Battery */
	Low_Battery = 99,
	/** Low_Fuel */
	Low_Fuel = 100,
	/** Low_Water */
	Low_Water = 101,
	/** Explosive_Gas */
	Explosive_Gas = 102,
	/** High_Winds */
	High_Winds = 103,
	/** Ice_Buildup */
	Ice_Buildup = 104,
	/** Smoke */
	Smoke = 105,
	/** Memory_Mismatch */
	Memory_Mismatch = 106,
	/** Out_of_CPU_Cycles */
	Out_of_CPU_Cycles = 107,
	/** Software_Environment_Problem */
	Software_Environment_Problem = 108,
	/** Software_Download_Failure */
	Software_Download_Failure = 109,
	/** Element_Reinitialized */
	Element_Reinitialized = 110,
	/** Timeout */
	Timeout = 111,
	/** Logging_Problems */
	Logging_Problems = 112,
	/** Leak_Detected */
	Leak_Detected_113,
	/** Protection_Mechanism_Failure */
	Protection_Mechanism_Failure = 114,
	/** Protecting_Resource_Failure */
	Protecting_Resource_Failure = 115,
	/** Database_Inconsistency */
	Database_Inconsistency = 116,
	/** Authentication_Failure */
	Authentication_Failure = 117,
	/** Breach_of_Confidentiality */
	Breach_of_Confidentiality = 118,
	/** Cable_Tamper */
	Cable_Tamper = 119,
	/** Delayed_Information */
	Delayed_Information = 120,
	/** Duplicate_Information */
	Duplicate_Information = 121,
	/** Information_Missing */
	Information_Missing = 122,
	/** Information_Modification */
	Information_Modification = 123,
	/** Information_Out_of_Sequence */
	Information_Out_of_Sequence = 124,
	/** Key_Expired */
	Key_Expired = 125,
	/** Non_Repudiation_Failure */
	Non_Repudiation_Failure = 126,
	/** Out_of_Hours_Activity */
	Out_of_Hours_Activity = 127,
	/** Out_of_Service */
	Out_of_Service = 128,
	/** Procedural_Error */
	Procedural_Error = 129,
	/** Unexpected_Information */
	Unexpected_Information = 130,
} CMPIErrorProbableCause;

/** Possible values an Error object can have for the error src format */
typedef enum _CMPIErrorSrcFormat {
	/** Unknown source */
    CMPIErrSrcUnknown = 0,
	/** Other source */
    CMPIErrSrcOther = 1,
	/* Object handle */
    CIMObjectHandle = 2,
} CMPIErrorSrcFormat;

#endif /* CMPI_VER_200 */

#   ifdef __cplusplus
};
#   endif

#endif // _CMPIDT_H_
