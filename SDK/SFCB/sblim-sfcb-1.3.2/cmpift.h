
/*
 * cmpift.h
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
 * CMPI function tables.
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

#ifndef _CMPIFT_H_
#   define _CMPIFT_H_

#   include "cmpidt.h"
#   include "cmpios.h"

#   ifdef __cplusplus
extern "C"
{
#   endif


  //---------------------------------------------------
  //--
  //   _CMPIBrokerEncFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers to broker and factory services
       of encapsulated CMPIObjects. This table is made available
       by the Management Broker, aka CIMOM, whenever a provider
       is loaded and initialized.
   */
  struct _CMPIBrokerEncFT
  {

     /** Function table version
     */
    int ftVersion;

     /** Instance factory service.
         @param mb Pointer to the broker.
	     @param op ObjectPath containing namespace and classname.
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created Instance.
     */
    CMPIInstance *(*newInstance)
      (const CMPIBroker * mb, const CMPIObjectPath * op, CMPIStatus * rc);

     /** ObjectPath factory service.
         @param mb Pointer to the broker.
	     @param ns Namespace
	     @param cn Classname.
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created ObjectPath.
     */
    CMPIObjectPath *(*newObjectPath)
      (const CMPIBroker * mb, const char *ns, const char *cn,
       CMPIStatus * rc);

     /** Args container factory service.
         @param mb Pointer to the broker.
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created Args container.
     */
    CMPIArgs *(*newArgs) (const CMPIBroker * mb, CMPIStatus * rc);

     /** String container factory service.
         @param mb Pointer to the broker.
	     @param data String data
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created String.
     */
    CMPIString *(*newString)
      (const CMPIBroker * mb, const char *data, CMPIStatus * rc);

     /** Array container factory service.
         @param mb Pointer to the broker.
	     @param max Maximum number of elements
	     @param type Element type
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created Array.
     */
    CMPIArray *(*newArray)
      (const CMPIBroker * mb, CMPICount max, CMPIType type, CMPIStatus * rc);

     /** DateTime factory service. Initialized with the time of day.
         @param mb Pointer to the broker.
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created DateTime.
     */
    CMPIDateTime *(*newDateTime) (const CMPIBroker * mb, CMPIStatus * rc);

     /** DateTime factory service. Initialized from &lt;binTime&gt;.
         @param mb Pointer to the broker.
	     @param binTime Date/Time definition in binary format in microsecods
	       starting since 00:00:00 GMT, Jan 1,1970.
 	     @param interval Wenn true, defines Date/Time definition to be an interval value
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created DateTime.
     */
    CMPIDateTime *(*newDateTimeFromBinary)
      (const CMPIBroker * mb, CMPIUint64 binTime, CMPIBoolean interval,
       CMPIStatus * rc);

     /** DateTime factory service. Is initialized from &lt;utcTime&gt;.
         @param mb Pointer to the broker.
	     @param utcTime Date/Time definition in UTC format
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created DateTime.
     */
    CMPIDateTime *(*newDateTimeFromChars)
      (const CMPIBroker * mb, const char *utcTime, CMPIStatus * rc);

     /** SelectExp factory service.
         @param mb Pointer to the broker.
	     @param query The select expression.
	     @param lang The query language.
	     @param projection Output: Projection specification (suppressed when NULL).
	     @param rc Output: Service return status (suppressed when NULL).
         @return The newly created SelectExp.
     */
    CMPISelectExp *(*newSelectExp)
      (const CMPIBroker * mb, const char *query, const char *lang,
       CMPIArray ** projection, CMPIStatus * st);

     /** Function to determine whether a CIM class is of &lt;type&gt; or any of
         &lt;type&gt; subclasses.
         @param mb Pointer to the broker.
	     @param op The class path (namespace and classname components).
	     @param type The type to tested for.
	     @param rc Output: Service return status (suppressed when NULL).
         @return True if test successful.
     */
      CMPIBoolean (*classPathIsA)
      (const CMPIBroker * mb, const CMPIObjectPath * op, const char *type,
       CMPIStatus * rc);

     /** Attempts to transforms an CMPI object to a broker specific string format.
         Intended for debugging purposes only.
         @param mb Pointer to the broker.
	     @param object A valid CMPI object.
	     @param rc Output: Service return status (suppressed when NULL).
         @return String from representation of &lt;object&gt;.
     */
    CMPIString *(*toString)
      (const CMPIBroker * mb, const void *object, CMPIStatus * rc);

     /** Verifies whether &lt;object&gt; is of CMPI type &lt;type&gt;.
         Intended for debugging purposes only.
         @param mb Pointer to the broker.
	     @param object A valid CMPI object.
	     @param type A string specifying a valid CMPI Object type
	         ("CMPIInstance", "CMPIObjectPath", etc).
	     @param rc Output: Service return status (suppressed when NULL).
         @return True if test successful.
     */
      CMPIBoolean (*isOfType)
      (const CMPIBroker * mb, const void *object, const char *type,
       CMPIStatus * rc);

     /** Retrieves the CMPI type of &lt;object&gt;.
         Intended for debugging purposes only.
         @param mb Pointer to the broker.
	     @param object A valid CMPI object.
	     @param rc Output: Service return status (suppressed when NULL).
         @return CMPI object type.
     */
    CMPIString *(*getType)
      (const CMPIBroker * mb, const void *object, CMPIStatus * rc);

     /** Retrieves translated message.
         @param mb Pointer to the broker.
	     @param msgId The message identifier.
	     @param defMsg The default message.
	     @param rc Output: Service return status (suppressed when NULL).
	     @param count The number of message substitution values.
         @return the trabslated message.
     */
    CMPIString *(*getMessage)
      (const CMPIBroker * mb, const char *msgId, const char *defMsg,
       CMPIStatus * rc, CMPICount count, ...);

        /** Logs a diagnostic message.
		@param mb The mb argument points to a CMPIBroker structure.
		@param severity  The level argument describes the level of log message. Levels are defined in Section 4.9.
		@param id The component argument, if not NULL, is the component ID.
		@param text  The text argument, if not NULL, is the message text to be logged.
		@param string The string argument, if not NULL, is the message text to be logged. string will be
			 ignored when text is not NULL.
	    @return Service return status.
      */

      CMPIStatus (*logMessage)
      (const CMPIBroker *, int severity, const char *id,
       const char *text, const CMPIString * string);

        /** Logs a trace entry. Intended for debugging purposes.
		@param mb The mb argument points to a CMPIBroker structure.
		@param level  The level argument describes the level of log message. Levels are defined in Section 4.9.
		@param component The component argument, if not NULL, is the component ID.
		@param text  The text argument, if not NULL, is the message text to be logged.
		@param string The string argument, if not NULL, is the message text to be logged. string will be
			 ignored when text is not NULL.
	    @return Service return status.
      */
      CMPIStatus (*trace)
      (const CMPIBroker *, int level, const char *component,
       const char *text, const CMPIString * string);

#	ifdef CMPI_VER_200
     /** Error factory service.
         @param mb Pointer to the broker.
	     @param msgID A string which uniquely identifies, within the scope of the 'owner' argument, the format of the message.
		 @param msg A string which represenst the formatted message.
		 @parem sev The percieved severity of the error.
		 @param pc The probably cause of this error
	     @param status Service return status.
         @return The newly created Error.
     */
	  CMPIError* (*newCMPIError)
	  (const CMPIBroker*, const char*, const char*, const char*,
	   const CMPIErrorSeverity, const CMPIErrorProbableCause,const CMPIrc,
	   CMPIStatus *rc);
     /** Opens a message file.
         @param mb Broker this pointer
         @param msgFile The message file identifier.
         @param msgFileHandle Output: The handle representing the open message file.
         @return Service return status.
     */
     CMPIStatus (*openMessageFile)
      (const CMPIBroker* mb, const char *msgFile,
       CMPIMsgFileHandle* msgFileHandle);

     /** Closes a message file.
         @param mb Broker this pointer
         @param msgFileHandle The handle representing the open message file.
         @return Service return status.
     */
     CMPIStatus (*closeMessageFile)
      (const CMPIBroker* mb, const CMPIMsgFileHandle msgFileHandle);

     /** Retrieves translated message.
         @param mb Broker this pointer
         @param msgId The message identifier.
         @param msgFileHandle The handle representing the open message file.
         @param defMsg The default message.
         @param rc Output: Service return status (suppressed when NULL).
         @param count The number of message substitution values.
         @return the translated message.
     */
     CMPIString* (*getMessage2)
      (const CMPIBroker* mb, const char *msgId,
       const CMPIMsgFileHandle msgFileHandle, const char *defMsg,
       CMPIStatus* rc, CMPICount count, ...);
#	endif /* CMPI_VER_200 */

  };



  //---------------------------------------------------
  //--
  //   _CMPIBrokerFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers to broker CIMOM services
       (up-calls). This table is made available by the Management Broker,
       whenever a provider is loaded and initialized.
   */
  struct _CMPIBrokerFT
  {

     /** 32 bits describing CMPI features supported by this CIMOM.
         See CMPI_MB_Class_x and CMPI_MB_Supports_xxx flags.
     */
    unsigned int brokerCapabilities;
     /** CIMOM version as defined by CIMOM
     */
    unsigned int brokerVersion;
     /** CIMOM name
     */
    const char *brokerName;

     /** This function prepares the CMPI run time system to accept
         a thread that will be using CMPI services. The returned
	 CMPIContext object must be used by the subsequent attachThread()
	 and detachThread() invocations.
	 @param mb Pointer to the broker.
	 @param ctx Old Context object
	 @return New Context object to be used by thread to be attached.
     */
    CMPIContext *(*prepareAttachThread)
      (const CMPIBroker * mb, const CMPIContext * ctx);

      /** This function informs the CMPI run time system that the current
         thread with Context will begin using CMPI services.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @return Service return status.
     */
      CMPIStatus (*attachThread) (const CMPIBroker *, const CMPIContext *);

      /** This function informs the CMPI run time system that the current thread
         will not be using CMPI services anymore. The Context object will be
	 freed during this operation.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @return Service return status.
     */
      CMPIStatus (*detachThread)
      (const CMPIBroker * mb, const CMPIContext * ctx);

    // class 0 services

      /** This function requests delivery of an Indication. The CIMOM will
         locate pertinent subscribers and notify them about the event.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param ns Namespace
	 @param ind Indication Instance
	 @return Service return status.
     */
      CMPIStatus (*deliverIndication)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const char *ns, const CMPIInstance * ind);
    // class 1 services

      /** Enumerate Instance Names of the class (and subclasses) defined by &lt;op&gt;.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace and classname components.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPathes.
     */
    CMPIEnumeration *(*enumerateInstanceNames)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, CMPIStatus * rc);

      /** Get Instance using &lt;op&gt; as reference. Instance structure can be
         controled using the CMPIInvocationFlags entry in &lt;ctx&gt;.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace, classname and key components.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Instance.
     */
    CMPIInstance *(*getInstance)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char **properties, CMPIStatus * rc);

    // class 2 services

      /** Create Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace, classname and key components.
	 @param inst Complete instance.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The assigned instance reference.
     */
    CMPIObjectPath *(*createInstance)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const CMPIInstance * inst, CMPIStatus * rc);

      /** Replace an existing Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace, classname and key components.
	 @param inst Complete instance.
	 @return Service return status.
     */
      CMPIStatus (*modifyInstance)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const CMPIInstance * inst,
       const char **properties);

      /** Delete an existing Instance using &lt;op&gt; as reference.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace, classname and key components.
	 @return Service return status.
     */
      CMPIStatus (*deleteInstance)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op);

      /** Query the enumeration of instances of the class (and subclasses) defined
         by &lt;op&gt; using &lt;query&gt; expression.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace and classname components.
	 @param query Query expression
	 @param lang Query Language
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Resulting eumeration of Instances.
     */
    CMPIEnumeration *(*execQuery)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char *query, const char *lang,
       CMPIStatus * rc);

      /** Enumerate Instances of the class (and subclasses) defined by &lt;op&gt;.
         Instance structure and inheritance scope can be controled using the
	 CMPIInvocationFlags entry in &lt;ctx&gt;.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace and classname components.
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
    CMPIEnumeration *(*enumerateInstances)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char **properties, CMPIStatus * rc);

      /** Enumerate instances associated with the Instance defined by the &lt;op&gt;.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op Source ObjectPath containing namespace, classname and key components.
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
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of Instances.
     */
    CMPIEnumeration *(*associators)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char *assocClass,
       const char *resultClass, const char *role, const char *resultRole,
       const char **properties, CMPIStatus * rc);

      /** Enumerate ObjectPaths associated with the Instance defined by &lt;op&gt;.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op Source ObjectPath containing namespace, classname and key components.
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
    CMPIEnumeration *(*associatorNames)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char *assocClass,
       const char *resultClass, const char *role, const char *resultRole,
       CMPIStatus * rc);

       /** Enumerates the association instances that refer to the instance defined by
           &lt;op&gt;.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op Source ObjectPath containing namespace, classname and key components.
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
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Enumeration of ObjectPaths.
     */
    CMPIEnumeration *(*references)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char *resultClass, const char *role,
       const char **properties, CMPIStatus * rc);

       /** Enumerates the association ObjectPaths that refer to the instance defined by
           &lt;op&gt;.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op Source ObjectPath containing namespace, classname and key components.
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
    CMPIEnumeration *(*referenceNames)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char *resultClass, const char *role,
       CMPIStatus * rc);

       /** Invoke a named, extrinsic method of an Instance
         defined by the &lt;op&gt; parameter.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace, classname and key components.
	 @param method Method name
	 @param in Input parameters.
	 @param out Output parameters.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Method return value.
      */
      CMPIData (*invokeMethod)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char *method,
       const CMPIArgs * in, CMPIArgs * out, CMPIStatus * rc);

       /** Set the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace, classname and key components.
	 @param name Property name
	 @param value Value.
	 @param type Value type.
	 @return Service return status.
      */
      CMPIStatus (*setProperty)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char *name, const CMPIValue * value,
       CMPIType type);

       /** Get the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param mb Pointer to the broker.
	 @param ctx Context object
	 @param op ObjectPath containing namespace, classname and key components.
	 @param name Property name
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
      CMPIData (*getProperty)
      (const CMPIBroker * mb, const CMPIContext * ctx,
       const CMPIObjectPath * op, const char *name, CMPIStatus * rc);
  };


  //---------------------------------------------------
  //--
  //   _CMPIBrokerExtFT Function Table
  //--
  //---------------------------------------------------


  struct timespec;

   /** This structure is a table of pointers to extended broker CIMOM
       services This table is made available by the Management Broker,
       whenever a provider is loaded and initialized.
       This is an extension used by Pegasus to support platform dependencies.
   */
  struct _CMPIBrokerExtFT
  {
     /** Function table version
     */
    int ftVersion;
	/**
	This function complements a generic dynamic library nameto its OS-dependent native format.
	@param filename Pointer to the generic library name,
	@return The returned char* pointer points to the complemented library name in native OS format.
		Space for this string has been obtained using malloc() and must be released using free() by the caller.
		 In case no storage could be obtained for the complemented library name, NULL will be
		 returned.
	*/
    char *(*resolveFileName) (const char *filename);


       /** Start a new thread using the POSIX threading semantics.
	 @param start Pointer to the function to be started as a thread.
	 @param param Pointer to the function to be started as a thread.
	 @param detached If not zero, defines that the thread should run in detached mode.
	 @return The return value is the thread ID.
	 */
      CMPI_THREAD_TYPE (*newThread)
      (CMPI_THREAD_RETURN (CMPI_THREAD_CDECL * start) (void *), void *parm,
       int detached);

        /** Waits until the specified thread ends using the POSIX threading semantics.
 	@param thread The thread ID of the thread waiting for completion.
 	@param retval Pointer to the return value of the thread.
	@return Completion code as defined by POSIX threading semantics (pthread_join)
	*/
    int (*joinThread) (CMPI_THREAD_TYPE thread, CMPI_THREAD_RETURN * retval);

        /** Causes the current thread to exit with the passed in return code using POSIX threading semantics.
    @param return_code Is the return code that should be used for the thread.
	@return The function never exits.
	*/
    int (*exitThread) (CMPI_THREAD_RETURN return_code);

        /** Cancel the thread using the POSIX threading semantics.
 	 @param thread  The thread to be canceled.
	 @return Completion code as defined by POSIX threading semantics (pthread_cancel)
	*/
    int (*cancelThread) (CMPI_THREAD_TYPE thread);

        /** Suspends the execution of the current thread for the specified duration.
     @param msec The suspend duration in milliseconds.
	@return Completion code as defined by POSIX threading semantics (pthread_sleep)
	*/
    int (*threadSleep) (CMPIUint32 msec);

        /** Executes the specified function procedure only once during the lifetime of the thread.
     @param once The pointer to the counter.
     @param The function to be called
	@return Completion code as defined by POSIX threading semantics (pthread_once)
    */
    int (*threadOnce) (int *once, void (*init) (void));

    /* Create a POSIX threading conformant thread key. This key can be used as a key to access the thread local store.
       @param key The address for the key to be returned.
       @param cleanup Function to be invoked during thread local store cleanup.
       @return Completion code as defined by POSIX threading semantics.
     */
    int (*createThreadKey)
      (CMPI_THREAD_KEY_TYPE * key, void (*cleanup) (void *));

        /** Destroy a POSIX threading conformant thread key.
    @param key The key to be destroyed.
	@return Completion code as defined by POSIX threading semantics.
	*/
    int (*destroyThreadKey) (CMPI_THREAD_KEY_TYPE key);

        /** Return data from the thread local store using a thread key.
    @param key The key to be used to retrieve the data.
	@return Completion code as defined by POSIX threading semantics.
	*/
    void *(*getThreadSpecific) (CMPI_THREAD_KEY_TYPE key);

        /** Set a pointer to data in the therad local store using a thread key.
    @param key The key to be used.
	@param value The pointer to the data.
	@return Completion code as defined by POSIX threading semantics.
	*/
    int (*setThreadSpecific) (CMPI_THREAD_KEY_TYPE key, void *value);


        /** Create a POSIX threading conformant mutex.
    @param opt The POSIX options. If not options are to be defined the 0 values must be used.
	@return Handle of newly created mutex.
	*/
      CMPI_MUTEX_TYPE (*newMutex) (int opt);

        /** Destroy a POSIX threading conformant mutex.
    @param mutex The mutex to be destroyed.
	*/
    void (*destroyMutex) (CMPI_MUTEX_TYPE mutex);

        /** Attempt to get control of the mutex  and must wait until released when not available.
 	@param mutex The mutex to be locked.
	*/
    void (*lockMutex) (CMPI_MUTEX_TYPE mutex);

        /** Release control of the mutex.
    @param mutex The mutex to be unlocked.
    */
    void (*unlockMutex) (CMPI_MUTEX_TYPE mutex);

        /** Create a new POSIX threading-conformant condition variable.
    @param opt The POSIX options. If not options are to be defined the 0 values must be used.
	@return Handle of newly created condition variable.
	*/
      CMPI_COND_TYPE (*newCondition) (int opt);

        /** Destroy a condition variable.
    @param cond The condition variable to be destroyed.
	*/
    void (*destroyCondition) (CMPI_COND_TYPE cond);

        /** Wait until condition is signalled.  This function returns when condition has been
    signalled already and otherwise must wait for the signal and then return.
	@param cond  The handle of the condition variable to be used.
 	@param mutex  The handle of a locked mutex guarding this condition variable.
	@return Return value As defined by POSIX threading specifications.
	*/
    int (*condWait) (CMPI_COND_TYPE cond, CMPI_MUTEX_TYPE mutex);

        /** Wait until the condition is signalled using a timeout value. This
	function shall return when condition has been signalled already and otherwise
	must wait for the signal and then return. The function shall return when the
	timeout specification elapses before the condition is signalled.
	@param cond Specifies the handle of the condition variable to be used.
	@param mutex Specifies the handle of a locked mutex guarding this condition variable.
	@param wait Specifies the timeout value.
	@return As defined by POSIX threading specifications.
	*/
    int (*timedCondWait)
      (CMPI_COND_TYPE cond, CMPI_MUTEX_TYPE mutex, struct timespec * wait);

        /** Sends a signal to a condition variable.
 	@param cond Specifies the handle of the condition variable to send the signal.
	@return As defined by POSIX threading specifications.
	*/
    int (*signalCondition) (CMPI_COND_TYPE cond);
  };



#   if defined (CMPI_VER_200)
  //---------------------------------------------------
  //--
  //   _CMPIBroker Memory Function Table
  //--
  //---------------------------------------------------

      /** This structure is a table of pointers to memory specific Â CIMOM
		  services. This table is made available by the Management Broker,
		  whenever a provider is loaded and initialized.
		  This is an extension used by CIMOMs to support memory management
		enhancements.
	*/
  struct _CMPIBrokerMemFT
  {

    const int ftVersion;

    /** Returns a marker. Invoking this function marks subsequent newly
	created CMPI objects to be released when release() function is invoked.
	Note: mark() functions can be stacked.
	@param mb The broker.
	@param rc Output: Service return status (suppressed when NULL).
	@return Handle to be provided to releae() function.
   */
    CMPIGcStat *(*mark) (const CMPIBroker * mb, CMPIStatus * rc);

        /** Release all CMPI objects created since last mark() operation
 	represented by the parameter. release() functions can be stacked.
 	@param mb The broker.
	@param gc The handle returned from the mark() operation.
 	@return Service return status.
 	*/
      CMPIStatus (*release) (const CMPIBroker * mb, const CMPIGcStat * gc);

    /** Allocates uninitalized memory of the specified size.
 	@param mb Specifies the broker.
	@param size Specifies the amount of memory to allocate.
	@return Returns a pointer to the allocated memory, or NULL if the memory
	could not be allocated
	*/
    void *(*cmpiMalloc) (const CMPIBroker * mb, size_t size);

    /** This function shall return a pointer to the allocated memory, the
	memory will be initialized to zero.
	@param mb The broker.
	@param nElems The number of elements to allocate.
	@param sizeElem The number of elements to allocate.
	@return Returns a pointer to the allocated memory, or NULL if the memory
	could not be allocated
	*/
    void *(*cmpiCalloc) (const CMPIBroker * mb, size_t, size_t);

    /** This function changes the size of the memory block pointed to by
		ptr which must have been returned by a previous call to cmpiMalloc or
		cmpiCalloc. See the ANSI-C function realloc for more information
		@param mb the broker.
		@param ptr Pointer to previosuly allocated memory.  Passing a pointer
		to this function which was not allocated explicitly by cmpiMalloc or
		cmpiCalloc is undefined.
		@param size The new size of the memory block.
		@return Returns a pointer to the newly allocated memory block, or NULL
		if the new memory is not allcoated. If the function fals nothing is
		done with the original ptr argument.
	*/
    void *(*cmpiRealloc) (const CMPIBroker * mb, void *, size_t);


    /** This function returns a pointer to a new string which is a
		duplicate of the string src.
		@param mb The broker
		@param src The string to duplicate
		@return a pointer to the duplicated string, or NULL if insufficient
		memory was available.
		*/
    char *(*cmpiStrDup) (const CMPIBroker * mb, const char *);

    /** This function frees memory allocated via the cmpiMalloc, cmpiCalloc
		or cmpiRealloc functions.
		@param mb The broker.
		@param ptr The memory to free. This memory MUST have been allocated via
		the cmpiMalloc, cmpiCalloc or cmpiRealloc functions.
		@return None
	*/
    void (*cmpiFree) (const CMPIBroker * mb, void *);

    /**  Allows a MI to free memory associated to a CMPIinstance which was
		 allocated via CMPIBrokerEncFT.newInstance. this function should be
		 called when an instance is no longer being used by the MI. This
		 function will free all contained objects (e.g. properties).
		 @param mb the broker.
		 @parma inst The instance to free.
		 @return None
	*/
    void (*freeInstance) (const CMPIBroker * mb, CMPIInstance * inst);

    /** Allows a MI to free memory associated to a CMPIArgs which was
		allocated via CMPIBrokerEncFT.newArgs. this function should be called
		when an instance is no longer being used by the MI. This function will
		free all contained objects.
		@param mb the broker.
		@param obj The object path to free.
		@return None
	*/
    void (*freeObjectPath) (const CMPIBroker * mb, CMPIObjectPath * obj);

    /** Allows a MI to free memory associated to a CMPIArgs which was
		allocated via CMPIBrokerEncFT.newArgs. this function should be called
		when an instance is no longer being used by the MI. This function will
		free all contained objects.
		@param mb the broker.
		@param args The argument to free.
		@return None.
	*/
    void (*freeArgs) (const CMPIBroker * mb, CMPIArgs * args);

    /**  Allows a MI to free memory associated to a CMPIString which was
		 allocated via CMPIBrokerEncFT.newString. this function should be
		 called when an instance is no longer being used by the MI. This
		 function will free all contained objects.
		 @param mb the broker.
		 @param args The string to free.
		 @return None.
	*/
    void (*freeString) (const CMPIBroker * mb, CMPIString * str);

    /** Allows a MI to free memory associated to a CMPIArray which was
		allocated via CMPIBrokerEncFT.newArray. this function should be called
		when an instance is no longer being used by the MI. This function will
		free all contained objects (e.g. the array elements).
		@param mb the broker.
		@param args The string to free.
		@return None.
	*/
    void (*freeArray) (const CMPIBroker * mb, CMPIArray * array);

    /** Allows a MI to free memory associated to a CMPIDateTime which was
		allocated via CMPIBrokerEncFT.newDateTime functions. this function
		should be called when an instance is no longer being used by the MI.
		This function will free all contained objects.
		@param mb the broker.
		@param args The string to free.
		@return None.
	*/
    void (*freeDateTime) (const CMPIBroker * mb, CMPIDateTime * date);

    /** Allows a MI to free memory associated to a CMPISelectExp which was
		allocated via CMPIBrokerEncFT.newSelectExp functions. this function
		should be called when an instance is no longer being used by the MI.
		This function will free all contained objects.
		@param mb the broker.
		@param args The string to free.
		@return None.
	*/
    void (*freeSelectExp) (const CMPIBroker * mb, CMPISelectExp * se);
  };

#   endif

  //---------------------------------------------------
  //--
  //   _CMPIBroker Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Management Broker (CIM Object Manager).
   */
  struct _CMPIBroker
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to MB service routines function table.
       */
    CMPIBrokerFT *bft;

       /** Pointer to MB factory service routines function table.
       */
    CMPIBrokerEncFT *eft;

       /** Pointer to MB extended services function table.
       */
    CMPIBrokerExtFT *xft;

#   ifdef CMPI_VER_200
       /** Pointer to MB memory enhancements function table.
       */
    CMPIBrokerMemFT *mft;
#   endif

  };



  //---------------------------------------------------
  //--
  //   _CMPIContext Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Context
       support sevices.
   */
  struct _CMPIContextFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The Context object will not be used any further and may be freed by
           CMPI run time system.
	 @param ctx Context this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPIContext * ctx);

       /** Create an independent copy of the Context object.
	 @param ctx Context this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Context object.
      */
    CMPIContext *(*clone) (const CMPIContext * ctx, CMPIStatus * rc);


       /** Gets a named Context entry value.
	 @param ctx Context this pointer.
	 @param name Context entry name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Entry value.
      */
      CMPIData (*getEntry)
      (const CMPIContext * ctx, const char *name, CMPIStatus * rc);

       /** Gets a Context entry value defined by its index.
	 @param ctx Context this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned Context entry name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Entry value.
      */
      CMPIData (*getEntryAt)
      (const CMPIContext * ctx, CMPICount index, CMPIString ** name,
       CMPIStatus * rc);

      /** Gets the number of entries contained in this Context.
	 @param ctx Context this pointer.
	 @return Number of entries.
      */
      CMPICount (*getEntryCount) (const CMPIContext * ctx, CMPIStatus * rc);

      /** Adds/replaces a named Context entry.
	 @param ctx Context this pointer.
         @param name Entry name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
      CMPIStatus (*addEntry)
      (const CMPIContext * ctx, const char *name, const CMPIValue * value,
       const CMPIType type);
  };




  //---------------------------------------------------
  //--
  //   _CMPIContext Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated Context object.
   */
  struct _CMPIContext
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the Context Function Table.
       */
    CMPIContextFT *ft;
  };




  //---------------------------------------------------
  //--
  //   _CMPIResult Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated Result object.
   */
  struct _CMPIResult
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the Result Function Table.
       */
    CMPIResultFT *ft;
  };


  //---------------------------------------------------
  //--
  //   _CMPIResult Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Result
       support sevices. Result support services are used to explicity return
       data produced by provider functions.
   */
  struct _CMPIResultFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The Result object will not be used any further and may be freed by
           CMPI run time system.
	 @param rslt Result this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPIResult * rslt);

       /** Create an independent copy of this Result object.
	 @param rslt Result this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Result object.
      */
    CMPIResult *(*clone) (const CMPIResult * rslt, CMPIStatus * rc);

       /** Return a value/type pair.
	 @param rslt Result this pointer.
	 @param value Address of a Value object.
	 @param type Type of the Value object.
	 @return Service return status.
      */
      CMPIStatus (*returnData)
      (const CMPIResult * rslt, const CMPIValue * value, const CMPIType type);

       /** Return a Instance object.
	 @param rslt Result this pointer.
	 @param inst Instance to be returned.
	 @return Service return status.
      */
      CMPIStatus (*returnInstance)
      (const CMPIResult * rslt, const CMPIInstance * inst);

       /** Return a ObjectPath object.
	 @param rslt Result this pointer.
	 @param ref ObjectPath to be returned.
	 @return Service return status.
      */
      CMPIStatus (*returnObjectPath)
      (const CMPIResult * rslt, const CMPIObjectPath * ref);

       /** Indicates no further data to be returned.
	 @param rslt Result this pointer.
	 @return Service return status.
      */
      CMPIStatus (*returnDone) (const CMPIResult * rslt);

#	ifdef CMPI_VER_200
	  /** Return a CMPIError object instance
	 @param rslt Result this pointer.
	 @param er Error to be returned.
	 @return Service return status.
	  */
	CMPIStatus (*returnError)(const CMPIResult* rslt,
		const CMPIError* er);
#	endif
  };

#	ifdef CMPI_VER_200
	struct _CMPIError {
		void *hdl;
		CMPIErrorFT *ft;
	};

  //---------------------------------------------------
  //--
  //   _CMPIErrorFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Error
       support services.
   */
struct _CMPIErrorFT {
       /** Function table version
       */
    CMPISint32 ftVersion;
       /** The Error object will not be used any further and may be freed by
           CMPI run time system.
	 @param er Error this pointer.
	 @return Service return status.
      */
    CMPIStatus (*release)(CMPIError*);
       /** Create an independent copy of this Error object.
	 @param er Error this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Error object.
      */
    CMPIError* (*clone)(const CMPIError*, CMPIStatus*);
	/** Gets the type of this Error
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return the error type this Error object conatins
	*/
    CMPIErrorType (*getErrorType)(const CMPIError*, CMPIStatus*);
	/** Returns a string which describes the alternate error type.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A string, which can be NULL
	*/
    CMPIString* (*getOtherErrorType)(const CMPIError*, CMPIStatus*);
	/** Returns a string which describes the owneing entity
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A string, which can be NULL
	*/
    CMPIString* (*getOwningEntity)(const CMPIError*, CMPIStatus*);
	/** Returns a string which is the message ID.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A string, which can be NULL
	*/
    CMPIString* (*getMessageID)(const CMPIError*, CMPIStatus*);
	/** Returns a string comnating an error message.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A string, which can be NULL
	*/
    CMPIString* (*getMessage)(const CMPIError*, CMPIStatus*);
	/** Returns the perceieved severity of this error.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return the perceived severity
	*/
    CMPIErrorSeverity (*getPerceivedSeverity)(const CMPIError*, CMPIStatus*);
	/** Returns the probable cause of this error.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A probable cause value
	*/
    CMPIErrorProbableCause (*getProbableCause)(const CMPIError*, CMPIStatus*);
	/** Returns a string which describes the probable cause.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A string, which can be NULL
	*/
    CMPIString* (*getProbableCauseDescription)(const CMPIError*, CMPIStatus*);
	/** Returns an array of strings which describes recomended actions.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A array of strings, which can be NULL
	*/
    CMPIArray* (*getRecommendedActions)(const CMPIError*, CMPIStatus*);
	/** Returns a string which describes the Error source.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A string, which can be NULL
	*/
    CMPIString* (*getErrorSource)(const CMPIError*, CMPIStatus*);
	/** Returns a the format that the error src is in.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A error source format  code
	*/
    CMPIErrorSrcFormat (*getErrorSourceFormat)(const CMPIError*, CMPIStatus*);
	/** Returns a string which describes the 'other' format, only available if the error source is OTHER.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A string, which can be NULL
	*/
    CMPIString* (*getOtherErrorSourceFormat)(const CMPIError*, CMPIStatus*);
	/** Returns the status code of this error.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A CMPI Status code
	*/
    CMPIrc (*getCIMStatusCode)(const CMPIError*, CMPIStatus*);
	/** Returns a string which describes the status code error.
		@param er Error this pointer
		@param rc Output: Service return status (suppressed when NULL).
		@return A string, which can be NULL
	*/
    CMPIString* (*getCIMStatusCodeDescription)(const CMPIError*, CMPIStatus*);
	/** Returns an array which contains the dynamic content of the message.
		@param er The Error this pointer
		@param rc Output: Serbice return status (surpressed when NULL)
		@return An array of CMPIStrings which represents the dynamic values
	*/
	CMPIArray* (*getMessageArguments)(const CMPIError*, CMPIStatus*);
	/** Sets the error type of this error object.
		@param er Error this pointer
		@param et The error type
		@return Output: Service return status
	*/
    CMPIStatus (*setErrorType)(CMPIError*, const CMPIErrorType);
	/** Sets the 'other' error type of this error object.
		@param er Error this pointer
		@param oet A string which describes the error type, it is only valis when error type is "OTHER"
		@return Output: Service return status
	*/
    CMPIStatus (*setOtherErrorType)(CMPIError*, const char *);
	/** Sets the description of the probable cause.
		@param er Error this pointer
		@param pc The probable cause string
		@return Output: Service return status
	*/
    CMPIStatus (*setProbableCauseDescription)(CMPIError*, const char *);
	/** Sets the recomended actions array.
		@param er Error this pointer
		@param ar An array of strings describing actions that shoudl be taken to deal with this error
		@return Output: Service return status
	*/
    CMPIStatus (*setRecommendedActions)(CMPIError*, const CMPIArray*);
	/** Specifies a string which specifes The identifying information of the entity (i.e., the instance) generating the error..
		@param er Error this pointer
		@param es the string which describes the source
		@return Output: Service return status
	*/
    CMPIStatus (*setErrorSource)(CMPIError*, const char*);
	/** Sets the source format of the error object
		@param er Error this pointer
		@param esf the string which describes the source format
		@return Output: Service return status
	*/
    CMPIStatus (*setErrorSourceFormat)(CMPIError*, const CMPIErrorSrcFormat );
	/** specifies A string defining "Other" values for ErrorSourceFormat
		@param er Error this pointer
		@param oef the string which describes the other source format
		@return Output: Service return status
	*/
    CMPIStatus (*setOtherErrorSourceFormat)(CMPIError*, const char*);
	/** Sets the description of the status code.
		@param er Error this pointer
		@param scd A string whcih describes the status code.
		@return Output: Service return status
	*/
    CMPIStatus (*setCIMStatusCodeDescription)(CMPIError*, const char*);
	/** Sets an array of strings for the dynamic content of the message
		@param er Error this pointer
		@param values Specifies an array of CMPIStrings containing the dynamic
		content of the message.
		@return Service return status
	*/
	CMPIStatus (*setMessageArguments)(CMPIError*, CMPIArray*);
};
#	endif /* CMPI_VER_200 */


  //---------------------------------------------------
  //--
  //   _CMPIInstance Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated Instance object.
   */
  struct _CMPIInstance
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the Instance Function Table.
       */
    CMPIInstanceFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPIInstance Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Instance
       support sevices.
   */
  struct _CMPIInstanceFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The Instance object will not be used any further and may be freed by
           CMPI run time system. This will also release the contained objects.
	 @param inst Instance this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPIInstance * inst);

       /** Create an independent copy of this Instance object. The resulting
           object must be released explicitly.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Instance object.
      */
    CMPIInstance *(*clone) (const CMPIInstance * inst, CMPIStatus * rc);

       /** Gets a named property value.
	 @param inst Instance this pointer.
	 @param name Property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
      CMPIData (*getProperty)
      (const CMPIInstance * inst, const char *name, CMPIStatus * rc);

       /** Gets a Property value defined by its index.
	 @param inst Instance this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Property value.
      */
      CMPIData (*getPropertyAt)
      (const CMPIInstance * inst, CMPICount index, CMPIString ** name,
       CMPIStatus * rc);
      /** Gets the number of properties contained in this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
      CMPICount (*getPropertyCount)
      (const CMPIInstance * inst, CMPIStatus * rc);

      /** Adds/replaces a named Property.
	 @param inst Instance this pointer.
         @param name Entry name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
      CMPIStatus (*setProperty)
      (const CMPIInstance * inst, const char *name,
       const CMPIValue * value, CMPIType type);

      /** Generates an ObjectPath out of the namespace, classname and
	  key propeties of this Instance.
	 @param inst Instance this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
         @return the generated ObjectPath.
      */
    CMPIObjectPath *(*getObjectPath)
      (const CMPIInstance * inst, CMPIStatus * rc);

      /** Directs CMPI to ignore any setProperty operations for this
	  instance for any properties not in this list.
	 @param inst Instance this pointer.
	 @param propertyList If not NULL, the members of the array define one
	 or more Property names to be accepted by setProperty operations.
	 @param keys Deprecated, ignored by MB, maintained here for compatibility.
	 @return Service return status.
      */
      CMPIStatus (*setPropertyFilter)
      (CMPIInstance * inst, const char **propertyList, const char **keys);

      /** Set/replace the ObjectPath component in an instance.
		  @param inst  The CMPIInstance structure containing a
		  complete instance.
		  @parmm  op The CMPIObjectPath structure. This objectpath
                  shall contain the namespace,classname, as well as all
                  keys for the specified instance.
		  @return Service return status.
	  */

      CMPIStatus (*setObjectPath) (CMPIInstance * inst,
                                   const CMPIObjectPath * op);
#ifdef CMPI_VER_200
	  /** add/replace a named Property value and origin
		  @param inst is a pointer to the CMPIInstance structure.
		  @param name is a string containing the Property name.
		  @param value points to a CMPIValue structure containing the value
		  to be assigned to the Property.
		  @param type is a CMPIType structure defining the type of the value.
		  @param origin specifies the instance origin.  If NULL, then
		  no origin is attached to  the property
		  @return Service return status
	  */
	  CMPIStatus (*setPropertyWithOrigin)(const CMPIInstance*, const char*,
	            const CMPIValue*, const CMPIType, const char*);
#endif /* CMPI_VER_200 */

  };




  //---------------------------------------------------
  //--
  //   _CMPIObjectPath Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated Instance object.
   */
  struct _CMPIObjectPath
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the ObjectPath Function Table.
       */
    CMPIObjectPathFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPIObjectPath Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to ObjectPath
       support sevices.
   */
  struct _CMPIObjectPathFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The ObjectPath object will not be used any further and may be freed by
           CMPI run time system.
	 @param op ObjectPath this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPIObjectPath * op);

       /** Create an independent copy of this ObjectPath object. The resulting
           object must be released explicitly.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied ObjectPath object.
      */
    CMPIObjectPath *(*clone) (const CMPIObjectPath * op, CMPIStatus * rc);

       /** Set/replace the namespace component.
	 @param op ObjectPath this pointer.
	 @param ns The namespace string
	 @return Service return status.
      */
      CMPIStatus (*setNameSpace) (CMPIObjectPath * op, const char *ns);

       /** Get the namespace component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The namespace component.
      */
    CMPIString *(*getNameSpace) (const CMPIObjectPath * op, CMPIStatus * rc);

       /** Set/replace the hostname component.
	 @param op ObjectPath this pointer.
	 @param hn The hostname string
	 @return Service return status.
      */
      CMPIStatus (*setHostname) (CMPIObjectPath * op, const char *hn);

       /** Get the hostname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The hostname component.
      */
    CMPIString *(*getHostname) (const CMPIObjectPath * op, CMPIStatus * rc);

       /** Set/replace the classname component.
	 @param op ObjectPath this pointer.
	 @param cn The hostname string
	 @return Service return status.
      */
      CMPIStatus (*setClassName) (CMPIObjectPath * op, const char *cn);

       /** Get the classname component.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The classname component.
      */
    CMPIString *(*getClassName) (const CMPIObjectPath * op, CMPIStatus * rc);

      /** Adds/replaces a named key property.
	 @param op ObjectPath this pointer.
         @param name Key property name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
      CMPIStatus (*addKey)
      (CMPIObjectPath * op, const char *name,
       const CMPIValue * value, const CMPIType type);

       /** Gets a named key property value.
	 @param op ObjectPath this pointer.
	 @param name Key property name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Entry value.
      */
      CMPIData (*getKey)
      (const CMPIObjectPath * op, const char *name, CMPIStatus * rc);

       /** Gets a key property value defined by its index.
	 @param op ObjectPath this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned property name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Data value.
      */
      CMPIData (*getKeyAt)
      (const CMPIObjectPath * op, CMPICount index, CMPIString ** name,
       CMPIStatus * rc);
      /** Gets the number of key properties contained in this ObjectPath.
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
      CMPICount (*getKeyCount) (const CMPIObjectPath * op, CMPIStatus * rc);
      /** Set/replace namespace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
      CMPIStatus (*setNameSpaceFromObjectPath)
      (CMPIObjectPath * op, const CMPIObjectPath * src);

      /** Set/replace hostname, namespace and classname components from &lt;src&gt;.
	 @param op ObjectPath this pointer.
	 @param src Source input.
	 @return Service return status.
      */
      CMPIStatus (*setHostAndNameSpaceFromObjectPath)
      (CMPIObjectPath * op, const CMPIObjectPath * src);



    // optional qualifier support


       /** Get class qualifier value.
	 @param op ObjectPath this pointer.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
      CMPIData (*getClassQualifier)
      (const CMPIObjectPath * op, const char *qName, CMPIStatus * rc);

       /** Get property qualifier value.
	 @param op ObjectPath this pointer.
	 @param pName Property name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
      CMPIData (*getPropertyQualifier)
      (const CMPIObjectPath * op,
       const char *pName, const char *qName, CMPIStatus * rc);

       /** Get method qualifier value.
	 @param op ObjectPath this pointer.
	 @param mName Method name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
      CMPIData (*getMethodQualifier)
      (const CMPIObjectPath * op,
       const char *methodName, const char *qName, CMPIStatus * rc);

       /** Get method parameter quailifier value.
	 @param op ObjectPath this pointer.
	 @param mName Method name.
	 @param pName Parameter name.
	 @param qName Qualifier name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Qualifier value.
      */
      CMPIData (*getParameterQualifier)
      (const CMPIObjectPath * op,
       const char *mName,
       const char *pName, const char *qName, CMPIStatus * rc);

      /** Generates a well formed string representation of this ObjectPath
	 @param op ObjectPath this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return String representation.
      */
    CMPIString *(*toString) (const CMPIObjectPath * op, CMPIStatus * rc);

  };




  //---------------------------------------------------
  //--
  //   _CMPISelectExp Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated SelectExp object.
   */
  struct _CMPISelectExp
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the SelExp Function Table.
       */
    CMPISelectExpFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPISelectExpFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to SelectExp
       support sevices.
   */
  struct _CMPISelectExpFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The SelectExp object will not be used any further and may be freed by
           CMPI run time system.
	 @param se SelectExp this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPISelectExp * se);

       /** Create an independent copy of this SelectExp object. The resulting
           object must be released explicitly.
	 @param se SelectExp this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied SelectExp object.
      */
    CMPISelectExp *(*clone) (const CMPISelectExp * se, CMPIStatus * rc);

       /** Evaluate the instance using this select expression.
	 @param se SelectExp this pointer.
	 @param inst Instance to be evaluated.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True or false incicator.
      */
      CMPIBoolean (*evaluate)
      (const CMPISelectExp * se, const CMPIInstance * inst, CMPIStatus * rc);

       /** Return the select expression in string format.
	 @param se SelectExp this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The select expression.
      */
    CMPIString *(*getString) (const CMPISelectExp * se, CMPIStatus * rc);

       /** Return the select expression as disjunction of conjunctions.
	 @param se SelectExp this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The disjunction.
      */
    CMPISelectCond *(*getDOC) (const CMPISelectExp * se, CMPIStatus * rc);

       /** Return the select expression as conjunction of disjunctions.
	 @param se SelectExp this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The conjunction.
      */
    CMPISelectCond *(*getCOD) (const CMPISelectExp * se, CMPIStatus * rc);

       /** Evaluate this select expression by using a data value accessor routine.
	 @param se SelectExp this pointer.
	 @param accessor Address of data accessor routine.
	 @param parm Data accessor routine parameter.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True or false incicator.
      */
      CMPIBoolean (*evaluateUsingAccessor)
      (const CMPISelectExp * se, CMPIAccessor * accessor, void *parm,
       CMPIStatus * rc);
  };



  //---------------------------------------------------
  //--
  //   _CMPISelectCond Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated SelectCond object.
   */
  struct _CMPISelectCond
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the SelCond Function Table.
       */
    CMPISelectCondFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPISelectCondFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to SelectCond
       support sevices.
   */
  struct _CMPISelectCondFT
  {

       /** Function table version
       */
    const int ftVersion;

       /** The SelectCond object will not be used any further and may be freed by
           CMPI run time system.
	 @param sc SelectCond this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPISelectCond * sc);

       /** Create an independent copy of this SelectCond object. The resulting
           object must be released explicitly.
	 @param sc SelectCond this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied SelectExp object.
      */
    CMPISelectCond *(*clone) (const CMPISelectCond * sc, CMPIStatus * rc);

       /** Return the number of sub conditions that are part of this SelectCond.
           Optionally, the SelectCond type (COD or DOC) will be returned.
	 @param sc SelectCond this pointer.
	 @param type Output: SelectCond type (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of SubCond elements.
      */
      CMPICount (*getCountAndType)
      (const CMPISelectCond * sc, int* type, CMPIStatus * rc);

       /** Return a SubCond element based on its index.
	 @param sc SelectCond this pointer.
	 @param index Position in the internal SubCoind array.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The indexed SubCond element.
      */
    CMPISubCond *(*getSubCondAt)
      (const CMPISelectCond * sc, CMPICount index, CMPIStatus * rc);
  };




  //---------------------------------------------------
  //--
  //   _CMPISubCond Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated SubCond object.
   */
  struct _CMPISubCond
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the SubCond Function Table.
       */
    CMPISubCondFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPISubCondFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to SubCond
       support sevices.
   */
  struct _CMPISubCondFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The SubCond object will not be used any further and may be freed by
           CMPI run time system.
	 @param sc SubCond this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPISubCond * sc);

       /** Create an independent copy of this SubCond object. The resulting
           object must be released explicitly.
	 @param se SubCond this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied SelectExp object.
      */
    CMPISubCond *(*clone) (const CMPISubCond * sc, CMPIStatus * rc);

       /** Return the number of predicates that are part of sub condition.
	 @param sc SubCond this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of Predicate elements.
      */
      CMPICount (*getCount) (const CMPISubCond * sc, CMPIStatus * rc);

       /** Return a Predicate element based on its index.
	 @param sc SubCond this pointer.
	 @param index Position in the internal Predicate array.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The indexed Predicate element.
      */
    CMPIPredicate *(*getPredicateAt)
      (const CMPISubCond * sc, CMPICount index, CMPIStatus * rc);

       /** Return a named Predicate element.
	 @param sc SubCond this pointer.
	 @param name Predicate name (property name).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The named Predicate element.
      */
    CMPIPredicate *(*getPredicate)
      (const CMPISubCond * sc, const char *name, CMPIStatus * rc);
  };




  //---------------------------------------------------
  //--
  //   _CMPIPredicate Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated Predicate object.
   */
  struct _CMPIPredicate
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the Predicate Function Table.
       */
    CMPIPredicateFT *ft;
  };




  //---------------------------------------------------
  //--
  //   _CMPIPredicateFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to SubCond
       support sevices.
   */
  struct _CMPIPredicateFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The Predicate object will not be used any further and may be freed by
           CMPI run time system.
	 @param pr Predicate this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPIPredicate * pr);

       /** Create an independent copy of this Predicate object. The resulting
           object must be released explicitly.
	 @param pr Predicate this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Predicate object.
      */
    CMPIPredicate *(*clone) (const CMPIPredicate * pr, CMPIStatus * rc);

       /** Get the predicate components.
	 @param pr Predicate this pointer.
	 @param type Property type.
	 @param op Predicate operation.
	 @param lhs Left hand side of predicate.
	 @param rhs Right hand side of predicate.
	 @return Service return status.
      */
      CMPIStatus (*getData)
      (const CMPIPredicate * pr, CMPIType * type,
       CMPIPredOp * op, CMPIString ** lhs, CMPIString ** rhs);

       /** Evaluate the predicate using a property data accessor function.
	 @param pr Predicate this pointer.
	 @param accessorFnc Pointer to a property value accessor function. The  evaluation
		process will invoke this function to request a CMPIData structure for a particular
		property. The signature of the accessor function is:
		CMPIData CMPIAccessor(const char* propertyName, void* parm);
	 @param parm Parameter that will be passed to the accessor function and can be
		used for providing context data to the accessor function.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Evaluation result.
      */

      CMPIBoolean (*evaluateUsingAccessor)
      (const CMPIPredicate * pr, CMPIAccessor * accessorFnc, void *parm,
       CMPIStatus * rc);
  };




  //---------------------------------------------------
  //--
  //   _CMPIArgs Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated Args object.
   */
  struct _CMPIArgs
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the Args Function Table.
       */
    CMPIArgsFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPIArgsFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Args
       support sevices.
   */
  struct _CMPIArgsFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The Args object will not be used any further and may be freed by
           CMPI run time system.
	 @param as Args this pointer.
	 @return Service return status.
      */
      CMPIStatus (*release) (CMPIArgs * as);

       /** Create an independent copy of this Args object. The resulting
           object must be released explicitly.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Args object.
      */
    CMPIArgs *(*clone) (const CMPIArgs * as, CMPIStatus * rc);

      /** Adds/replaces a named argument.
	 @param as Args this pointer.
         @param name Argument name.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
      CMPIStatus (*addArg)
      (const CMPIArgs * as, const char *name, const CMPIValue * value,
       const CMPIType type);

       /** Gets a named argument value.
	 @param as Args this pointer.
	 @param name Argument name.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
      CMPIData (*getArg)
      (const CMPIArgs * as, const char *name, CMPIStatus * rc);

       /** Gets a Argument value defined by its index.
	 @param as Args this pointer.
	 @param index Position in the internal Data array.
	 @param name Output: Returned argument name (suppressed when NULL).
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Argument value.
      */
      CMPIData (*getArgAt)
      (const CMPIArgs * as, CMPICount index, CMPIString ** name,
       CMPIStatus * rc);

      /** Gets the number of arguments contained in this Args.
	 @param as Args this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of properties.
      */
      CMPICount (*getArgCount) (const CMPIArgs * as, CMPIStatus * rc);
  };




  //---------------------------------------------------
  //--
  //   _CMPIString Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated String object.
   */
  struct _CMPIString
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the String Function Table.
       */
    CMPIStringFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPIStringFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to String
       support sevices.
   */
  struct _CMPIStringFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The String object will not be used any further and may be freed by
           CMPI run time system.
	 @param st String this pointer.
	 @return Service return status.
       */
      CMPIStatus (*release) (CMPIString * st);

       /** Create an independent copy of this String object. The resulting
           object must be released explicitly.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied String object.
      */
    CMPIString *(*clone) (const CMPIString * st, CMPIStatus * rc);

       /** Get a pointer to a C char *representation of this String.
	 @param st String this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to char *representation.
      */
    char *(*getCharPtr) (const CMPIString * st, CMPIStatus * rc);
  };




  //---------------------------------------------------
  //--
  //   _CMPIArray Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated Array object.
   */
  struct _CMPIArray
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the Array Function Table.
       */
    CMPIArrayFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPIArrayFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Array
       support sevices.
   */
  struct _CMPIArrayFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The Array object will not be used any further and may be freed by
           CMPI run time system.
	 @param ar Array this pointer.
	 @return Service return status.
       */
      CMPIStatus (*release) (CMPIArray * ar);

       /** Create an independent copy of this Array object. The resulting
           object must be released explicitly.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Array object.
      */
    CMPIArray *(*clone) (const CMPIArray * ar, CMPIStatus * rc);

      /** Gets the number of elements contained in this Array.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
      CMPICount (*getSize) (const CMPIArray * ar, CMPIStatus * rc);

      /** Gets the element type.
	 @param ar Array this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Number of elements.
      */
      CMPIType (*getSimpleType) (const CMPIArray * ar, CMPIStatus * rc);

       /** Gets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
      CMPIData (*getElementAt)
      (const CMPIArray * ar, CMPICount index, CMPIStatus * rc);

       /** Sets an element value defined by its index.
	 @param ar Array this pointer.
	 @param index Position in the internal Data array.
         @param value Address of value structure.
         @param type Value type.
	 @return Service return status.
      */
      CMPIStatus (*setElementAt)
      (CMPIArray * ar, CMPICount index, const CMPIValue * value,
       CMPIType type);
  };





  //---------------------------------------------------
  //--
  //   _CMPIEnumeration Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the Encapsulated Enumeration object.
   */
  struct _CMPIEnumeration
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the Enumeration Function Table.
       */
    CMPIEnumerationFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPIEnumerationFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Enumeration
       support sevices.
   */
  struct _CMPIEnumerationFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The Enumeration object will not be used any further and may be freed by
           CMPI run time system.
	 @param en Enumeration this pointer.
	 @return Service return status.
       */
      CMPIStatus (*release) (CMPIEnumeration * en);

       /** Create an independent copy of this Enumeration object. The resulting
           object must be released explicitly.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied Enumeration object.
      */
    CMPIEnumeration *(*clone) (const CMPIEnumeration * en, CMPIStatus * rc);

       /** Get the next element of this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Element value.
      */
      CMPIData (*getNext) (const CMPIEnumeration * en, CMPIStatus * rc);

       /** Test for any elements left in this Enumeration.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True or false.
      */
      CMPIBoolean (*hasNext) (const CMPIEnumeration * en, CMPIStatus * rc);

       /** Convert this Enumeration into an Array.
	 @param en Enumeration this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return The Array.
      */
    CMPIArray *(*toArray) (const CMPIEnumeration * en, CMPIStatus * rc);
  };





  //---------------------------------------------------
  //--
  //   _CMPIDateTime Encapsulated object
  //--
  //---------------------------------------------------


   /** This structure represents the DateTime object.
   */
  struct _CMPIDateTime
  {

       /** Opaque pointer to MB specific implementation data.
       */
    void *hdl;

       /** Pointer to the DateTime Function Table.
       */
    CMPIDateTimeFT *ft;
  };



  //---------------------------------------------------
  //--
  //   _CMPIDateTimeFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to DateTime
       support sevices.
   */
  struct _CMPIDateTimeFT
  {

       /** Function table version
       */
    int ftVersion;

       /** The DateTime object will not be used any further and may be freed by
           CMPI run time system.
	 @param dt DateTime this pointer.
	 @return Service return status.
       */
      CMPIStatus (*release) (CMPIDateTime * dt);

       /** Create an independent copy of this DateTime object. The resulting
           object must be released explicitly.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return Pointer to copied DateTime object.
      */
    CMPIDateTime *(*clone) (const CMPIDateTime * dt, CMPIStatus * rc);

       /** Get DateTime setting in binary format (in microsecods
	       starting since 00:00:00 GMT, Jan 1,1970).
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime in binary.
      */
      CMPIUint64 (*getBinaryFormat) (const CMPIDateTime * dt,
                                     CMPIStatus * rc);

       /** Get DateTime setting in UTC string format.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return DateTime as UTC string.
      */
    CMPIString *(*getStringFormat) (const CMPIDateTime * dt, CMPIStatus * rc);

       /** Tests whether DateTime is an interval value.
	 @param dt DateTime this pointer.
	 @param rc Output: Service return status (suppressed when NULL).
	 @return True if interval value.
      */
      CMPIBoolean (*isInterval) (const CMPIDateTime * dt, CMPIStatus * rc);
  };






  //---------------------------------------------------
  //--
  //   _CMPIInstanceMI Instance Provider object
  //--
  //---------------------------------------------------


   /** This structure represents an Instance provider.
   */
  typedef struct _CMPIInstanceMIFT CMPIInstanceMIFT;
  typedef struct _CMPIInstanceMI
  {

       /** Opaque pointer to Provider specific implementation data.
       */
    const void *hdl;

       /** Pointer to the Instance Provider Function Table.
       */
    CMPIInstanceMIFT *ft;
  } CMPIInstanceMI;



  //---------------------------------------------------
  //--
  //   _CMPIInstanceMIFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Instance
       provider functions. This table must be returend during initialization
       by the provider.
   */
  struct _CMPIInstanceMIFT
  {

       /** Function table version
       */
    int ftVersion;

       /** Provider version
       */
    int miVersion;

       /** Provider name
       */
    const char *miName;

       /** The CMPIInstanceMIFT.cleanup() function shall perform any necessary cleanup
	   operation prior to the unloading of the library of which this MI group is part.
	   This function is called prior to the unloading of the provider.

	   @param mi The mi argument is a pointer to a CMPIInstanceMI structure.
	   @param ctx The ctx argument is a pointer to a CMPIContext structure containing the Invocation
	   Context.
	   @param terminating When true, the terminating argument indicates that the MB is in the process of
	      terminating and that cleanup must be done. When set to false, the MI may respond with
	      CMPI_IRC_DO_NOT_UNLOAD, or CMPI_IRC_NEVER_UNLOAD, indicating that unload will
	      interfere with current MI processing.
	   @return Function return status.
	   The following CMPIrc codes shall be recognized:
	      CMPI_RC_OK Operation successful.
	      CMPI_RC_ERR_FAILED Unspecific error occurred.
	      CMPI_RC_DO_NOT_UNLOAD Operation successful - do not unload now.
	      CMPI_RC_NEVER_UNLOAD Operation successful -  never unload.o
      */
      CMPIStatus (*cleanup)
      (CMPIInstanceMI * mi, const CMPIContext * ctx, CMPIBoolean terminating);

       /** Enumerate ObjectPaths of Instances serviced by this provider.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context.
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace and classname components.
	 @return Function return status.
	  The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
      */
      CMPIStatus (*enumerateInstanceNames)
      (CMPIInstanceMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op);

       /** Enumerate the Instances serviced by this provider.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context.
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace and classname components.
	 @param properties If not NULL, the members of the array define one or
	     more Property names. Each returned Object MUST NOT include elements
	     for any Properties missing from this list.
	 @return Function return status.
	  The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
      */
      CMPIStatus (*enumerateInstances)
      (CMPIInstanceMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op, const char **properties);

       /** Get the Instances defined by &lt;op&gt;.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context.
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace, classname and key components.
	 @param properties If not NULL, the members of the array define one or
	     more Property names. Each returned Object MUST NOT include elements
	     for any Properties missing from this list.
	 @return Function return status.
	  The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
      */
      CMPIStatus (*getInstance)
      (CMPIInstanceMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op, const char **properties);

       /** Create Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context.
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace, classname and key components.
	 @param inst The Instance.
	 @return Function return status.
	  The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ALREADY_EXISTS Instance already exists.
      */
      CMPIStatus (*createInstance)
      (CMPIInstanceMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op, const CMPIInstance * inst);

       /** Replace an existing Instance from &lt;inst&gt; using &lt;op&gt; as reference.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context.
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace, classname and key components.
	 @param inst The Instance.
	 @param properties If not NULL, the members of the array define one or
	     more Property names. The process MUST NOT replace elements
	     for any Properties missing from this list. If NULL all properties
	     will be replaced.
	 @return Function return status.
      */
      CMPIStatus (*modifyInstance)
      (CMPIInstanceMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op, const CMPIInstance * inst,
       const char **properties);

       /** Delete an existing Instance defined by &lt;op&gt;.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context.
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace, classname and key components.
	 @return Function return status.
	  The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
      */
      CMPIStatus (*deleteInstance)
      (CMPIInstanceMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op);

      /** Query the enumeration of instances of the class (and subclasses) defined
         by &lt;op&gt; using &lt;query&gt; expression.
	 @param mi Provider this pointer.
	 @param ctx Context object
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace and classname components.
	 @param query Query expression
	 @param lang Query language
	 @return Function return status.
	   The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_QUERY_LANGUAGE_NOT_SUPPORTED Query language not supported.
		CMPI_RC_ERR_INVALID_QUERY Invalid query.
      */
      CMPIStatus (*execQuery)
      (CMPIInstanceMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op, const char *query, const char *lang);
  };






  //---------------------------------------------------
  //--
  //   _CMPIAssociationMI Association Provider object
  //--
  //---------------------------------------------------


   /** This structure represents an Association provider.
   */
  typedef struct _CMPIAssociationMIFT CMPIAssociationMIFT;
  typedef struct _CMPIAssociationMI
  {

       /** Opaque pointer to Provider specific implementation data.
       */
    void *hdl;

       /** Pointer to the Association Provider Function Table.
       */
    CMPIAssociationMIFT *ft;
  } CMPIAssociationMI;


  //---------------------------------------------------
  //--
  //   _CMPIAssociationMIFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Association
       provider functions. This table must be returend during initialization
       by the provider.
   */
  struct _CMPIAssociationMIFT
  {

       /** Function table version
       */
    int ftVersion;

       /** Provider version
       */
    const CMPISint32 miVersion;

       /** Provider name
       */
    const char *miName;

       /** Cleanup is called prior to unloading of the provider.
	   This function shall perform any necessary cleanup
	   operations prior to the unloading of the library of which this MI group is part.

	 @param mi This argument is a pointer to a CMPIAssociationMI structure.
	 @param ctx This argument is a pointer to a CMPIContext structure containing the Invocation Context.
	 @param terminating When true, the terminating argument indicates that the MB is in the process of
	 terminating and that cleanup must be done. When set to false, the MI may respond with
	 CMPI_IRC_DO_NOT_UNLOAD, or CMPI_IRC_NEVER_UNLOAD, indicating that unload will
	 interfere with current MI processing.
	 @return Function return status. The following CMPIrc codes shall be recognized:
	    CMPI_RC_OK Operation successful.
	    CMPI_RC_ERR_FAILED Unspecific error occurred.
	    CMPI_RC_DO_NOT_UNLOAD Operation successful - do not unload now.
	    CMPI_RC_NEVER_UNLOAD Operation successful - never unload.

      */
      CMPIStatus (*cleanup)
      (CMPIAssociationMI * mi, const CMPIContext * ctx,
       CMPIBoolean terminating);

      /** Enumerate ObjectPaths associated with the Instance defined by &lt;op&gt;.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context
	 @param rslt Result data container.
	 @param op Source ObjectPath containing namespace, classname and key components.
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
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list. If NULL all properties must be returned.
	    the returned Object MUST match the value of this parameter).
	 @return Function return status.
	   The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
     */
      CMPIStatus (*associators)
      (CMPIAssociationMI * mi, const CMPIContext * ctx,
       const CMPIResult * rslt, const CMPIObjectPath * op,
       const char *asscClass, const char *resultClass, const char *role,
       const char *resultRole, const char **properties);

      /** Enumerate ObjectPaths associated with the Instance defined by &lt;op&gt;.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context
	 @param rslt Result data container.
	 @param op Source ObjectPath containing namespace, classname and key components.
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
	 @return Function return status.
	   The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
     */
      CMPIStatus (*associatorNames)
      (CMPIAssociationMI * mi, const CMPIContext * ctx,
       const CMPIResult * rslt, const CMPIObjectPath * op,
       const char *assocClass, const char *resultClass, const char *role,
       const char *resultRole);

       /** Enumerates the association instances that refer to the instance defined by
           &lt;op&gt;.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context
	 @param rslt Result data container.
	 @param op Source ObjectPath containing namespace, classname and key components.
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
	 @param properties If not NULL, the members of the array define one or more Property
	     names. Each returned Object MUST NOT include elements for any Properties
	     missing from this list
	 @return Function return status.
	   The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
     */
      CMPIStatus (*references)
      (CMPIAssociationMI * mi, const CMPIContext * ctx,
       const CMPIResult * rslt, const CMPIObjectPath * op,
       const char *resultClass, const char *role, const char **properties);

      /** Enumerates the association ObjectPaths that refer to the instance defined by
           &lt;op&gt;.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context
	 @param rslt Result data container.
	 @param op Source ObjectPath containing namespace, classname and key components.
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
	 @return Function return status.
	   The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
      */
      CMPIStatus (*referenceNames)
      (CMPIAssociationMI * mi, const CMPIContext * ctx,
       const CMPIResult * rslt, const CMPIObjectPath * op,
       const char *resultClass, const char *role);
  };






  //---------------------------------------------------
  //--
  //   _CMPIMethodMI Method Provider object
  //--
  //---------------------------------------------------


   /** This structure represents an Method provider.
   */
  typedef struct _CMPIMethodMIFT CMPIMethodMIFT;
  typedef struct _CMPIMethodMI
  {

       /** Opaque pointer to Provider specific implementation data.
       */
    void *hdl;

       /** Pointer to the Method Provider Function Table.
       */
    CMPIMethodMIFT *ft;
  } CMPIMethodMI;



  //---------------------------------------------------
  //--
  //   _CMPIMethodMIFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Method
       provider functions. This table must be returend during initialization
       by the provider.
   */
  struct _CMPIMethodMIFT
  {

       /** Function table version
       */
    int ftVersion;

       /** Provider version
       */
    int miVersion;

       /** Provider name
       */
    const char *miName;

       /** The CMPIMethodMIFT.cleanup() function shall perform any necessary cleanup operation
	   prior to the unloading of the library of which this MI group is part. This function is called
	   prior to the unloading of the provider.

	   @param mi The mi argument is a pointer to a CMPIMethodMI structure.
	   @param ctx The ctx argument is a pointer to a CMPIContext structure containing the Invocation
	      Context.
	   @param terminating When true, the terminating argument indicates that the MB is in the process of
	      terminating and that cleanup must be done. When set to false, the MI may respond with
	      CMPI_IRC_DO_NOT_UNLOAD, or CMPI_IRC_NEVER_UNLOAD, indicating that unload will
	      interfere with current MI processing.
	   @return Function return status.
	   The following CMPIrc codes shall be recognized:
	      CMPI_RC_OK Operation successful.
	      CMPI_RC_ERR_FAILED Unspecific error occurred.
	      CMPI_RC_DO_NOT_UNLOAD Operation successful - do not unload now.
	      CMPI_RC_NEVER_UNLOAD Operation successful - never unload.
      */
      CMPIStatus (*cleanup)
      (CMPIMethodMI * mi, const CMPIContext * ctx, CMPIBoolean terminating);

      /** Invoke a named, extrinsic method of an Instance
         defined by the &lt;op&gt; parameter.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace, classname and key components.
	 @param method Method name
	 @param in Input parameters.
	 @param out Output parameters.
	 @return Function return status. The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
 		CMPI_RC_ERR_FAILED Unspecific error occurred.
		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		CMPI_RC_ERR_NOT_FOUND Instance not found.
		CMPI_RC_ERR_METHOD_NOT_AVAILABLE Method not available.
		CMPI_RC_ERR_METHOD_NOT_FOUND Method not found.
      */
      CMPIStatus (*invokeMethod)
      (CMPIMethodMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op, const char *method, const CMPIArgs * in,
       CMPIArgs * out);
  };





  //---------------------------------------------------
  //--
  //   _CMPIPropertyMI Property Provider object
  //--
  //---------------------------------------------------


   /** This structure represents an Property provider.
   */
  typedef struct _CMPIPropertyMIFT CMPIPropertyMIFT;
  typedef struct _CMPIPropertyMI
  {

       /** Opaque pointer to Provider specific implementation data.
       */
    void *hdl;

       /** Pointer to the Property Provider Function Table.
       */
    CMPIPropertyMIFT *ft;
  } CMPIPropertyMI;



  //---------------------------------------------------
  //--
  //   _CMPIPropertyMIFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Property
       provider functions. This table must be returend during initialization
       by the provider.
   */
  struct _CMPIPropertyMIFT
  {

       /** Function table version
       */
    int ftVersion;

       /** Provider version
       */
    int miVersion;

       /** Provider name
       */
    const char *miName;

       /** Cleanup is called prior to unloading of the provider.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context
	   @param terminating When true, the terminating argument indicates that the MB is in the process of
	      terminating and that cleanup must be done. When set to false, the MI may respond with
	      CMPI_IRC_DO_NOT_UNLOAD, or CMPI_IRC_NEVER_UNLOAD, indicating that unload will
	      interfere with current MI processing.

	   @return Function return status.
	   The following CMPIrc codes shall be recognized:
	      CMPI_RC_OK Operation successful.
	      CMPI_RC_ERR_FAILED Unspecific error occurred.
	      CMPI_RC_DO_NOT_UNLOAD Operation successful - do not unload now.
	      CMPI_RC_NEVER_UNLOAD Operation successful  -  never unload.
      */
      CMPIStatus (*cleanup)
      (CMPIPropertyMI * mi, const CMPIContext * ctx, CMPIBoolean terminating);

      /** Set the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace, classname and key components.
	 @param name Property name
	 @param data Property value.
	 @return Function return status. The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
		CMPI_RC_ERR_TYPE_MISATCH type does not correspond to class-defined type.
		CMPI_RC_ERR_INVALID_HANDLE The inst handle is invalid.
      */
      CMPIStatus (*setProperty)
      (CMPIPropertyMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
       const CMPIObjectPath * op, const char *name, const CMPIData data);

      /** Get the named property value of an Instance defined by the &lt;op&gt; parameter.
	 @param mi Provider this pointer.
	 @param ctx Invocation Context
	 @param rslt Result data container.
	 @param op ObjectPath containing namespace, classname and key components.
	 @param name Property name
	 @return Function return status. The following CMPIrc codes shall be recognized:
		CMPI_RC_OK Operation successful.
 		CMPI_RC_ERR_FAILED Unspecific error occurred.
 		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
 		CMPI_RC_ERR_INVALID_NAMESPACE The namespace is invalid.
 		CMPI_RC_ERR_INVALID_PARAMETER The parameter is invalid.
 		CMPI_RC_ERR_INVALID_CLASS The CIM class does not exist in the specified
 			namespace.
 		CMPI_RC_ERR_NOT_FOUND Instance not found.
 		CMPI_RC_ERR_NO_SUCH_PROPERTY Entry not found.
      */
      CMPIStatus (*getProperty)
      (CMPIPropertyMI *, const CMPIContext *, const CMPIResult *,
       const CMPIObjectPath *, const char *name);
#ifdef CMPI_VER_200
	  /** add/replace a named Property value and origin
		  @param mi Provider this pointer.
		  @param ctx Invocation Context
		  @param rslt Result data container.
		  @param op ObjectPath containing namespace, classname and
		  key components
		  @param name Property name
		  @param data Property value.
		  @param origin specifies the instance origin.  If NULL, then
		  no origin is attached to  the property
		  @return Service return status
	  */
	  CMPIStatus (*setPropertyWithOrigin)(CMPIPropertyMI*, const CMPIContext*,
		  const CMPIResult*, CMPIObjectPath * op,
          const char *name, const CMPIData data, const char*);
#endif /* CMPI_VER_200 */
  };





  //---------------------------------------------------
  //--
  //   _CMPIIndicationMI Indication Provider object
  //--
  //---------------------------------------------------


   /** This structure represents an Indication provider.
   */
  typedef struct _CMPIIndicationMIFT CMPIIndicationMIFT;
  typedef struct _CMPIIndicationMI
  {

       /** Opaque pointer to Provider specific implementation data.
       */
    void *hdl;

       /** Pointer to the Property Provider Function Table.
       */
    CMPIIndicationMIFT *ft;
  } CMPIIndicationMI;



  //---------------------------------------------------
  //--
  //   _CMPIIndicationMIFT Function Table
  //--
  //---------------------------------------------------


   /** This structure is a table of pointers providing access to Indication
       provider functions. This table must be returend during initialization
       by the provider.
   */
  struct _CMPIIndicationMIFT
  {

       /** Function table version
       */
    int ftVersion;

       /** Provider version
       */
    int miVersion;

       /** Provider name
       */
    const char *miName;

       /** Cleanup is called prior to unloading of the provider.
	   This function shall perform any necessary cleanup
	   operation prior to the unloading of the library of which this MI group is part.

	   @param mi The mi argument is a pointer to a CMPIIndicationMI structure.
	   @param ctx The ctx argument is a pointer to a CMPIContext structure
	      containing the Invocation Context.
	   @param terminating When true, the terminating argument indicates that
	      the MB is in the process of terminating and that cleanup must be done.
		  When set to false, the MI may respond with
	      CMPI_RC_DO_NOT_UNLOAD, or CMPI_RC_NEVER_UNLOAD, indicating that
		  unload will interfere with current MI processing.
	   @return Function return status.
	   The following CMPIrc codes shall be recognized:
	      CMPI_RC_OK Operation successful.
	      CMPI_RC_ERR_FAILED Unspecific error occurred.
	      CMPI_RC_DO_NOT_UNLOAD Operation successful do not unload now.
	      CMPI_RC_NEVER_UNLOAD Operation successful never unload
      */
      CMPIStatus (*cleanup)
      (CMPIIndicationMI * mi, const CMPIContext * ctx,
       CMPIBoolean terminating);

      /** Ask the provider to verify whether this filter is allowed.
	   @param mi The mi argument is a pointer to a CMPIIndicationMI structure.
	   @param ctx The ctx argument is a pointer to a CMPIContext structure
	      containing the Invocation Context.
	   @param filter Contains the filter that must be authorized.
	   @param className Contains the class name extracted from the filter FROM
	      clause.
	   @param op The name of the class for which monitoring is required. Only
	      the namespace part is set if className is a process indication.
	   @param owner The owner argument is the destination owner.
	   @return This function shall structure containing the service return
	      status.
		 The following CMPIrc codes shall be recognized:
		 CMPI_RC_OK Operation successful.
		 CMPI_RC_ERR_FAILED Unspecific error occurred.
		 CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this
		 CMPI_RC_ERR_ACCESS_DENIED Not authorized.
		 CMPI_RC_ERR_INVALID_QUERY Invalid query or too complex.
  	*/
      CMPIStatus (*authorizeFilter) (CMPIIndicationMI * mi,
                                     const CMPIContext * ctx,
                                     const CMPISelectExp * filter,
                                     const char *className,
                                     const CMPIObjectPath * op,
                                     const char *owner);
        /** Ask the MI whether polling mode should be used.
 		This function enables very simple MIs to support indications without
		   providing a complete indication support implementation. When true
		   is returned, the MB will enumerate the instances of this MI at
		   regular intervals and apply indication filters.
	   @param mi The mi argument is a pointer to a CMPIIndicationMI structure.
	   @param ctx The ctx argument is a pointer to a CMPIContext structure
	      containing the Invocation Context.
 	   @param className The class name extracted from the filter FROM clause.
 	   @param filter  The name of the class for which monitoring is required.
	      Only the namespace part is set if eventType is a process indication.
	   @param classPath The name of the class for which polling would be used.
	      Only the namespace part is set if  className is a process indication.
	   @return This function shall return a CMPIStatus structure containing
	      the service return status.
 		The following CMPIrc codes shall be recognized:
 		CMPI_RC_OK Operation successful.
 		CMPI_RC_ERR_FAILED Unspecific error occurred.
 		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
 		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
 		CMPI_RC_ERR_INVALID_QUERY Invalid query or too complex.
	 */
      CMPIStatus (*mustPoll) (CMPIIndicationMI * mi, const CMPIContext * ctx,
                              const CMPISelectExp * filter,
                              const char *className,
                              const CMPIObjectPath * classPath);

        /** Ask the provider to begin monitoring a resource.
		The function shall begin monitoring the resource according to the
		   filter express only.
	   @param mi The mi argument is a pointer to a CMPIIndicationMI structure.
	   @param ctx The ctx argument is a pointer to a CMPIContext structure
	      containing the Invocation Context.
 	   @param filter The filter argument contains the filter specification for
	      this subscription to become active.
 	   @param className The class name extracted from the filter FROM clause.
 	   @param classPath The name of the class for which monitoring is required.
	      Only the namespace part is set if eventType is a process indication.
 	   @param firstActivation Set to true if this is the first filter for
	      className.
		@return The function shall return a CMPIStatus structure containing
		   the service return status.
 		The following CMPIrc codes shall be recognized:
 			CMPI_RC_OK Operation successful.
 			CMPI_RC_ERR_FAILED Unspecific error occurred.
 			CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
 			CMPI_RC_ERR_ACCESS_DENIED Not authorized.
 			CMPI_RC_ERR_INVALID_QUERY Invalid query or too complex.
		*/
      CMPIStatus (*activateFilter) (CMPIIndicationMI * mi,
                                    const CMPIContext * ctx,
                                    const CMPISelectExp * filter,
                                    const char *className,
                                    const CMPIObjectPath * classPath,
                                    CMPIBoolean firstActivation);

        /** Inform the MI that monitoring using this filter should stop.
 		The function invocation mandates the MI to stop monitoring the resource
		   using this filter.
	   @param mi The mi argument is a pointer to a CMPIIndicationMI structure.
	   @param ctx The ctx argument is a pointer to a CMPIContext structure
	      containing the Invocation Context.
 	   @param filter The filter argument contains the filter specification for
	      this subscription to become active.
 	   @param className The class name extracted from the filter FROM clause.
 	   @param classPath The name of the class for which monitoring is required.
	      Only the namespace part is set if className is a process indication.
 	   @param lastActiviation Set to true if this is the last filter for
	      className.
	@return The function shall return a CMPIStatus structure containing the
	   service return status.
 		The following CMPIrc codes shall be recognized:
 		CMPI_RC_OK Operation successful.
 		CMPI_RC_ERR_FAILED Unspecific error occurred.
 		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
 		CMPI_RC_ERR_ACCESS_DENIED Not authorized.
 		CMPI_RC_ERR_INVALID_QUERY Invalid query or too complex.
	*/
      CMPIStatus (*deActivateFilter) (CMPIIndicationMI * mi,
                                      const CMPIContext * ctx,
                                      const CMPISelectExp * filter,
                                      const char *className,
                                      const CMPIObjectPath * classPath,
                                      CMPIBoolean lastActiviation);

        /** Tell the MI that indications can now be generated.  The MB is now
			prepared
			to process indications. The function is normally called by the MB
			after having done its intialization and processing of persistent
			subscription requests.
			@param mi The mi argument is a pointer to a CMPIIndicationMI
			   structure.
			   @param ctx The ctx argument is a pointer to a CMPIContext
			      structure containing the Invocation Context.
	   @return The function shall return a CMPIStatus structure containing the
	      service return status.
 		The following CMPIrc codes shall be recognized:
 		CMPI_RC_OK Operation successful.
 		CMPI_RC_ERR_FAILED Unspecific error occurred.
 		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
	*/
      CMPIStatus (*enableIndications) (CMPIIndicationMI * mi,
                                       const CMPIContext *);
        /** Tell the MI to stop generating indications.  MB will not accept any
			indications until enabled again. The function is normally called
			when the MB is shutting down indication services either temporarily
			or permanently.
	   @param mi The mi argument is a pointer to a CMPIIndicationMI structure.
	   @param ctx The ctx argument is a pointer to a CMPIContext structure
	      containing the Invocation Context.
	   @return The function shall return a CMPIStatus structure containing the
	      service return status.
 		The following CMPIrc codes shall be recognized:
 		CMPI_RC_OK Operation successful.
 		CMPI_RC_ERR_FAILED Unspecific error occurred.
 		CMPI_RC_ERR_NOT_SUPPORTED Operation not supported by this MI.
	*/
      CMPIStatus (*disableIndications) (CMPIIndicationMI * mi,
                                        const CMPIContext *);

  };

#   include "cmpimacs.h"


#   ifdef __cplusplus
};
#   endif

#endif // _CMPIFT_H_
