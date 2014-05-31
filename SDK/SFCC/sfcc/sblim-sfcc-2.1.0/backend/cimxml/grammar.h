/*
 * $id$
 *
 *  © Copyright IBM Corp. 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Sven Schuetz <sven@de.ibm.com>
 *
 * Description:
 *
 * CIM XML parser header file
 * 
 *
*/

#include "cimXmlParser.h"



static void start(ParserControl *parm, parseUnion *stateUnion);
static void cim(ParserControl *parm, parseUnion *stateUnion);
static void message(ParserControl *parm, parseUnion *stateUnion);
static void messageContent(ParserControl *parm, parseUnion *stateUnion);
static void simpleRspContent(ParserControl *parm, parseUnion *stateUnion);
static void exportIndication(ParserControl *parm, parseUnion *stateUnion);
static void exParamValue(ParserControl *parm, parseUnion *stateUnion);
static void methodRespContent(ParserControl *parm, parseUnion *stateUnion);
static void iMethodRespContent(ParserControl *parm, parseUnion *stateUnion);
static void error(ParserControl *parm, parseUnion *stateUnion);
static void returnValue(ParserControl *parm, parseUnion *stateUnion);
static void returnValueData(ParserControl *parm, parseUnion *stateUnion);
static void paramValue(ParserControl *parm, parseUnion *stateUnion);
static void paramValueData(ParserControl *parm, parseUnion *stateUnion);
static void iReturnValue(ParserControl *parm, parseUnion *stateUnion);
static void iReturnValueContent(ParserControl *parm, parseUnion *stateUnion);
static void getPropertyRetValue(ParserControl *parm, parseUnion *stateUnion);
static void valueObjectWithPath(ParserControl *parm, parseUnion *stateUnion);
static void valueObjectWithPathData(ParserControl *parm, parseUnion *stateUnion);
static void classWithPath(ParserControl *parm, parseUnion *stateUnion);
static void instanceWithPath(ParserControl *parm, parseUnion *stateUnion);
static void class(ParserControl *parm, parseUnion *stateUnion);
static void method(ParserControl *parm, parseUnion *stateUnion);
static void methodData(ParserControl *parm, parseUnion *stateUnion);
static void parameter(ParserControl *parm, parseUnion *stateUnion);
static void parameterReference(ParserControl *parm, parseUnion *stateUnion);
static void parameterRefArray(ParserControl *parm, parseUnion *stateUnion);
static void parameterArray(ParserControl *parm, parseUnion *stateUnion);
static void objectPath(ParserControl *parm, parseUnion *stateUnion);
static void classPath(ParserControl *parm, parseUnion *stateUnion);
static void className(ParserControl *parm, parseUnion *stateUnion);
static void instancePath(ParserControl *parm, parseUnion *stateUnion);
static void localInstancePath(ParserControl *parm, parseUnion *stateUnion);
static void nameSpacePath(ParserControl *parm, parseUnion *stateUnion);
static void host(ParserControl *parm, parseUnion *stateUnion);
static void localNameSpacePath(ParserControl *parm, parseUnion *stateUnion);
static void nameSpace(ParserControl *parm, parseUnion *stateUnion);
static void valueNamedInstance(ParserControl *parm, parseUnion *stateUnion);
static void instance(ParserControl *parm, parseUnion *stateUnion);
static void genProperty(ParserControl *parm, parseUnion *stateUnion);
static void qualifier(ParserControl *parm, parseUnion *stateUnion);
static void qualifierData(ParserControl *parm, parseUnion *stateUnion);
static void property(ParserControl *parm, parseUnion *stateUnion);
static void propertyArray(ParserControl *parm, parseUnion *stateUnion);
static void propertyReference(ParserControl *parm, parseUnion *stateUnion);
static void instanceName(ParserControl *parm, parseUnion *stateUnion);
static void keyBinding(ParserControl *parm, parseUnion *stateUnion);
static void keyBindingContent(ParserControl *parm, parseUnion *stateUnion);
static void keyValue(ParserControl *parm, parseUnion *stateUnion);
static void value(ParserControl *parm, parseUnion *stateUnion);
static void valueData(ParserControl *parm, parseUnion *stateUnion);
static void valueArray(ParserControl *parm, parseUnion *stateUnion);
static void valueRefArray(ParserControl *parm, parseUnion *stateUnion);
static void valueReference(ParserControl *parm, parseUnion *stateUnion);
static void valueReferenceData(ParserControl *parm, parseUnion *stateUnion);

