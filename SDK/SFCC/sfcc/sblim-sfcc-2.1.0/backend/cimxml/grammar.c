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
 * CIM XML parser for sfcc to be used in connection with lexer cimXmlParser.c
 *
*/

#include "grammar.h"
#include "cimXmlParser.h"
#include "sfcUtil/utilft.h"
#include "parserUtil.h"


static int ct = 0;
static int dontLex = 0;


static void parseError(char* tokExp, int tokFound, ParserControl *parm)
{
	printf("Parse error. Expected token(s) %s, found tag number %d (see cimXmlParser.h) and following xml: %.255s...\nAborting.\n", tokExp, tokFound, parm->xmb->cur+1);
	exit(0);
}


static int localLex(parseUnion *lvalp, ParserControl *parm)
{
    if(! dontLex) {
        ct = sfccLex(lvalp, parm);
    }
    else {
        dontLex = 0;
        return ct;
    }
    return ct;
}


void startParsing(ParserControl *parm)
{
    parseUnion stateUnion;
    start(parm, &stateUnion);
}

static void start(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex(stateUnion, parm);
	if(ct == XTOK_XML) {
		ct = localLex(stateUnion, parm);
		if(ct == ZTOK_XML) {
			cim(parm, stateUnion);
		}
		else {
			parseError("ZTOK_XML", ct, parm);
		}
	}
	else {
		parseError("XTOK_XML", ct, parm);
	}
}

static void cim(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex(stateUnion, parm);
	if(ct == XTOK_CIM) {
		message(parm, (parseUnion*)&stateUnion->xtokMessage);
		ct = localLex(stateUnion, parm);
		if(ct == ZTOK_CIM) {
		}
		else {
			parseError("ZTOK_CIM", ct, parm);
		}
	}
	else {
		parseError("XTOK_CIM", ct, parm);
	}
}

static void message(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokMessage, parm);
	if(ct == XTOK_MESSAGE) {
		messageContent(parm, stateUnion);
		ct = localLex((parseUnion*)&stateUnion->xtokMessage, parm);
		if(ct == ZTOK_MESSAGE) {
		}
		else {
			parseError("ZTOK_MESSAGE", ct, parm);
		}
	}
	else {
		parseError("XTOK_MESSAGE", ct, parm);
	}
}

static void messageContent(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex(stateUnion, parm);
	if(ct == XTOK_SIMPLERSP) {
		simpleRspContent(parm, (parseUnion*)&stateUnion->xtokSimpleRespContent);
		ct = localLex(stateUnion, parm);
		if(ct == ZTOK_SIMPLERSP) {
		}
		else {
			parseError("ZTOK_SIMPLERSP", ct, parm);
		}
	}
	else if(ct == XTOK_SIMPLEEXPREQ) {
		exportIndication(parm, stateUnion);
		ct = localLex(stateUnion, parm);
		if(ct == ZTOK_SIMPLEEXPREQ) {
		}
		else {
			parseError("ZTOK_SIMPLEEXPREQ", ct, parm);
		}
	}
	else {
		parseError("XTOK_SIMPLERSP", ct, parm);
	}
}

static void simpleRspContent(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokSimpleRespContent, parm);
	if(ct == XTOK_METHODRESP) {
		methodRespContent(parm, (parseUnion*)&stateUnion->xtokSimpleRespContent.resp);
		setReturnArgs(parm, &stateUnion->xtokSimpleRespContent.resp.values);
		ct = localLex((parseUnion*)&stateUnion->xtokSimpleRespContent, parm);
		if(ct == ZTOK_METHODRESP) {
		}
		else {
			parseError("ZTOK_METHODRESP", ct, parm);
		}
	}
	else if(ct == XTOK_IMETHODRESP) {
		iMethodRespContent(parm, stateUnion);
		ct = localLex((parseUnion*)&stateUnion->xtokSimpleRespContent, parm);
		if(ct == ZTOK_IMETHODRESP) {
		}
		else {
			parseError("ZTOK_IMETHODRESP", ct, parm);
		}
	}
	else {
		parseError("XTOK_METHODRESP", ct, parm);
	}
}

static void exportIndication(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex(stateUnion, parm);
	if(ct == XTOK_EXPORTINDICATION) {
		exParamValue(parm, stateUnion);
		ct = localLex(stateUnion, parm);
		if(ct == ZTOK_EXPMETHODCALL) {
		}
		else {
			parseError("ZTOK_EXPMETHODCALL", ct, parm);
		}
	}
	else {
		parseError("XTOK_EXPORTINDICATION", ct, parm);
	}
}

static void exParamValue(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	CMPIInstance *inst;
	ct = localLex(stateUnion, parm);
	if(ct == XTOK_EP_INSTANCE) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_INSTANCE) {
			dontLex = 1;
			instance(parm, (parseUnion*)&lvalp.xtokInstance);
			inst = native_new_CMPIInstance(NULL,NULL);
			setInstNsAndCn(inst,parm->da_nameSpace,lvalp.xtokInstance.className);
			setInstProperties(inst, &lvalp.xtokInstance.properties);
			simpleArrayAdd(parm->respHdr.rvArray,(CMPIValue*)&inst,CMPI_instance);
		}
		ct = localLex(stateUnion, parm);
		if(ct == ZTOK_EXPPARAMVALUE) {
		}
		else {
			parseError("ZTOK_EXPPARAMVALUE or XTOK_INSTANCE", ct, parm);
		}
	}
	else {
		parseError("XTOK_EP_INSTANCE", ct, parm);
	}
}

static void methodRespContent(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex(&lvalp, parm);
	if(ct == XTOK_ERROR) {
		dontLex = 1;
		error(parm, (parseUnion*)&lvalp.xtokErrorResp);
	}
	else if(ct == XTOK_RETVALUE || ct == XTOK_PARAMVALUE) {
		dontLex = 1;
		if(ct == XTOK_RETVALUE) {
			dontLex = 1;
			returnValue(parm, (parseUnion*)&lvalp.xtokReturnValue);
		}
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_PARAMVALUE) {
			do {
				dontLex = 1;
				paramValue(parm, (parseUnion*)&lvalp.xtokParamValue);
				addParamValue(parm, &stateUnion->xtokMethodRespContent.values, &lvalp.xtokParamValue);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_PARAMVALUE);
			dontLex = 1;
		}
	}
	else if(ct == ZTOK_METHODRESP) {
		dontLex = 1;
	}
	else {
		parseError("XTOK_ERROR or XTOK_RETVALUE or XTOK_PARAMVALUE or ZTOK_METHODRESP", ct, parm);
	}
}

static void iMethodRespContent(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex(&lvalp, parm);
	if(ct == XTOK_ERROR) {
		dontLex = 1;
		error(parm, (parseUnion*)&lvalp.xtokErrorResp);
	}
	else if(ct == XTOK_IRETVALUE) {
		dontLex = 1;
		iReturnValue(parm, stateUnion);
	}
	else {
		parseError("XTOK_ERROR or XTOK_IRETVALUE", ct, parm);
	}
}

static void error(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokErrorResp, parm);
	if(ct == XTOK_ERROR) {
		setError(parm, &stateUnion->xtokErrorResp);
		ct = localLex((parseUnion*)&stateUnion->xtokErrorResp, parm);
		if(ct == ZTOK_ERROR) {
		}
		else {
			parseError("ZTOK_ERROR", ct, parm);
		}
	}
	else {
		parseError("XTOK_ERROR", ct, parm);
	}
}

static void returnValue(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	CMPIType  t;
	CMPIValue val;
	CMPIInstance *inst;
	ct = localLex((parseUnion*)&stateUnion->xtokReturnValue, parm);
	if(ct == XTOK_RETVALUE) {
		returnValueData(parm, (parseUnion*)&stateUnion->xtokReturnValue.data);
		if(stateUnion->xtokReturnValue.data.type == CMPI_ref) {
			t = CMPI_ref;
			val = str2CMPIValue(t, NULL, &stateUnion->xtokReturnValue.data.ref);
		}
		else if(stateUnion->xtokReturnValue.data.value.type == typeValue_Instance) {
			t = CMPI_instance;
			inst = native_new_CMPIInstance(NULL,NULL);
			setInstNsAndCn(inst,parm->da_nameSpace,stateUnion->xtokReturnValue.data.value.data.inst->className);
			setInstProperties(inst, &stateUnion->xtokReturnValue.data.value.data.inst->properties);
			val.inst = inst;
		}
		else {
			t = stateUnion->xtokReturnValue.type;
			if (t == CMPI_null) {
				t = guessType(stateUnion->xtokReturnValue.data.value.data.value);
			}
			val = str2CMPIValue(t, stateUnion->xtokReturnValue.data.value.data.value, NULL);
		}
		simpleArrayAdd(parm->respHdr.rvArray, (CMPIValue*)&val, t);
		ct = localLex((parseUnion*)&stateUnion->xtokReturnValue, parm);
		if(ct == ZTOK_RETVALUE) {
		}
		else {
			parseError("ZTOK_RETVALUE", ct, parm);
		}
	}
	else {
		parseError("XTOK_RETVALUE", ct, parm);
	}
}

static void returnValueData(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokReturnValueData, parm);
	if(ct == XTOK_VALUE) {
		dontLex = 1;
		value(parm, (parseUnion*)&stateUnion->xtokReturnValueData.value);
	}
	else if(ct == XTOK_VALUEREFERENCE) {
		dontLex = 1;
		valueReference(parm, (parseUnion*)&stateUnion->xtokReturnValueData.ref);
		stateUnion->xtokReturnValueData.type = CMPI_ref;
	}
	else {
		parseError("XTOK_VALUE or XTOK_VALUEREFERENCE", ct, parm);
	}
}

static void paramValue(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokParamValue, parm);
	if(ct == XTOK_PARAMVALUE) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_VALUE || ct == XTOK_VALUEREFERENCE || ct == XTOK_VALUEARRAY || ct == XTOK_VALUEREFARRAY) {
			dontLex = 1;
			paramValueData(parm, (parseUnion*)&lvalp.xtokParamValueData);
			stateUnion->xtokParamValue.data = lvalp.xtokParamValueData;
			if(lvalp.xtokParamValueData.type == CMPI_instance) {
				stateUnion->xtokParamValue.type = CMPI_instance;
			}
			else {
				stateUnion->xtokParamValue.type |= lvalp.xtokParamValueData.type;
			}
		}
		ct = localLex((parseUnion*)&stateUnion->xtokParamValue, parm);
		if(ct == ZTOK_PARAMVALUE) {
		}
		else {
			parseError("ZTOK_PARAMVALUE or XTOK_VALUE or XTOK_VALUEREFERENCE or XTOK_VALUEARRAY or XTOK_VALUEREFARRAY", ct, parm);
		}
	}
	else {
		parseError("XTOK_PARAMVALUE", ct, parm);
	}
}

static void paramValueData(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokParamValueData, parm);
	if(ct == XTOK_VALUE) {
		dontLex = 1;
		value(parm, (parseUnion*)&stateUnion->xtokParamValueData.value);
		if(stateUnion->xtokParamValueData.value.type == typeValue_Instance) {
			stateUnion->xtokParamValueData.type = CMPI_instance;
		}
	}
	else if(ct == XTOK_VALUEREFERENCE) {
		dontLex = 1;
		valueReference(parm, (parseUnion*)&stateUnion->xtokParamValueData.valueRef);
		stateUnion->xtokParamValueData.type = CMPI_ref;
	}
	else if(ct == XTOK_VALUEARRAY) {
		dontLex = 1;
		valueArray(parm, (parseUnion*)&stateUnion->xtokParamValueData.valueArray);
		stateUnion->xtokParamValueData.type |= CMPI_ARRAY;
	}
	else if(ct == XTOK_VALUEREFARRAY) {
		dontLex = 1;
		valueRefArray(parm, (parseUnion*)&stateUnion->xtokParamValueData.valueRefArray);
		stateUnion->xtokParamValueData.type = CMPI_refA;
	}
	else {
		parseError("XTOK_VALUE or XTOK_VALUEREFERENCE or XTOK_VALUEARRAY or XTOK_VALUEREFARRAY", ct, parm);
	}
}

static void iReturnValue(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex(stateUnion, parm);
	if(ct == XTOK_IRETVALUE) {
		iReturnValueContent(parm, stateUnion);
		ct = localLex(stateUnion, parm);
		if(ct == ZTOK_IRETVALUE) {
		}
		else {
			parseError("ZTOK_IRETVALUE", ct, parm);
		}
	}
	else {
		parseError("XTOK_IRETVALUE", ct, parm);
	}
}

static void iReturnValueContent(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	CMPIObjectPath *op;
	CMPIInstance *inst;
	CMPIConstClass *cls;
	ct = localLex(&lvalp, parm);
	dontLex = 1;
	if(ct == XTOK_CLASS) {
		do {
			dontLex = 1;
			class(parm, (parseUnion*)&lvalp.xtokClass);
			cls = native_new_CMPIConstClass(lvalp.xtokClass.className,NULL);
			setClassProperties(cls, &lvalp.xtokClass.properties);
			simpleArrayAdd(parm->respHdr.rvArray,(CMPIValue*)&cls,CMPI_class);
			ct = localLex(&lvalp, parm);
		}
		while(ct == XTOK_CLASS);
		dontLex = 1;
	}
	else if(ct == XTOK_CLASSNAME) {
		do {
			dontLex = 1;
			className(parm, (parseUnion*)&lvalp.xtokClassName);
			op = newCMPIObjectPath(NULL, lvalp.xtokClassName.value, NULL);
			simpleArrayAdd(parm->respHdr.rvArray,(CMPIValue*)&op,CMPI_ref);
			ct = localLex(&lvalp, parm);
		}
		while(ct == XTOK_CLASSNAME);
		dontLex = 1;
	}
	else if(ct == XTOK_INSTANCE) {
		do {
			dontLex = 1;
			instance(parm, (parseUnion*)&lvalp.xtokInstance);
			inst = native_new_CMPIInstance(NULL,NULL);
			setInstNsAndCn(inst,parm->da_nameSpace,lvalp.xtokInstance.className);
			setInstProperties(inst, &lvalp.xtokInstance.properties);
			simpleArrayAdd(parm->respHdr.rvArray,(CMPIValue*)&inst,CMPI_instance);
			ct = localLex(&lvalp, parm);
		}
		while(ct == XTOK_INSTANCE);
		dontLex = 1;
	}
	else if(ct == XTOK_INSTANCENAME) {
		do {
			dontLex = 1;
			instanceName(parm, (parseUnion*)&lvalp.xtokInstanceName);
			createPath(&op, &lvalp.xtokInstanceName);
			simpleArrayAdd(parm->respHdr.rvArray,(CMPIValue*)&op,CMPI_ref);
			ct = localLex(&lvalp, parm);
		}
		while(ct == XTOK_INSTANCENAME);
		dontLex = 1;
	}
	else if(ct == XTOK_VALUENAMEDINSTANCE) {
		do {
			dontLex = 1;
			valueNamedInstance(parm, (parseUnion*)&lvalp.xtokNamedInstance);
			createPath(&op,&(lvalp.xtokNamedInstance.path));
			inst = native_new_CMPIInstance(op,NULL);
			//setInstQualifiers(inst, &lvalp.xtokNamedInstance.instance.qualifiers);
			setInstProperties(inst, &lvalp.xtokNamedInstance.instance.properties);
			simpleArrayAdd(parm->respHdr.rvArray,(CMPIValue*)&inst,CMPI_instance);
			ct = localLex(&lvalp, parm);
		}
		while(ct == XTOK_VALUENAMEDINSTANCE);
		dontLex = 1;
	}
	else if(ct == XTOK_OBJECTPATH) {
		do {
			dontLex = 1;
			objectPath(parm, (parseUnion*)&lvalp.xtokObjectPath);
			createPath(&op, &lvalp.xtokObjectPath.path.instanceName);
			CMSetNameSpace(op, lvalp.xtokObjectPath.path.path.nameSpacePath.value);
			CMSetHostname(op, lvalp.xtokObjectPath.path.path.host.host);
			simpleArrayAdd(parm->respHdr.rvArray,(CMPIValue*)&op,CMPI_ref);
			ct = localLex(&lvalp, parm);
		}
		while(ct == XTOK_OBJECTPATH);
		dontLex = 1;
	}
	else if(ct == XTOK_VALUEOBJECTWITHPATH) {
		do {
			dontLex = 1;
			valueObjectWithPath(parm, (parseUnion*)&lvalp.xtokObjectWithPath);
			ct = localLex(&lvalp, parm);
		}
		while(ct == XTOK_VALUEOBJECTWITHPATH);
		dontLex = 1;
	}
	else if(ct == XTOK_VALUE || ct == XTOK_VALUEARRAY || ct == XTOK_VALUEREFERENCE) {
		dontLex = 1;
		if(ct == XTOK_VALUE || ct == XTOK_VALUEARRAY || ct == XTOK_VALUEREFERENCE) {
			dontLex = 1;
			getPropertyRetValue(parm, (parseUnion*)&lvalp.xtokGetPropRetContent);
		}
	}
	else if(ct == ZTOK_IRETVALUE) {
		dontLex = 1;
	}
	else {
		parseError("XTOK_CLASS or XTOK_CLASSNAME or XTOK_INSTANCE or XTOK_INSTANCENAME or XTOK_VALUENAMEDINSTANCE or XTOK_OBJECTPATH or XTOK_VALUEOBJECTWITHPATH or XTOK_VALUE or ZTOK_IRETVALUE", ct, parm);
	}
}

static void getPropertyRetValue(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	CMPIType  t;
	CMPIValue val;
	ct = localLex((parseUnion*)&stateUnion->xtokGetPropRetContent, parm);
	if(ct == XTOK_VALUE) {
		dontLex = 1;
		value(parm, (parseUnion*)&stateUnion->xtokGetPropRetContent.value);
		t   = guessType(stateUnion->xtokGetPropRetContent.value.data.value);
		val = str2CMPIValue(t, stateUnion->xtokGetPropRetContent.value.data.value, NULL);
		simpleArrayAdd(parm->respHdr.rvArray, (CMPIValue*)&val, t);
	}
	else if(ct == XTOK_VALUEARRAY) {
		dontLex = 1;
		valueArray(parm, (parseUnion*)&stateUnion->xtokGetPropRetContent.arr);
	}
	else if(ct == XTOK_VALUEREFERENCE) {
		dontLex = 1;
		valueReference(parm, (parseUnion*)&stateUnion->xtokGetPropRetContent.ref);
	}
	else {
		parseError("XTOK_VALUE or XTOK_VALUEARRAY or XTOK_VALUEREFERENCE", ct, parm);
	}
}

static void valueObjectWithPath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokObjectWithPath, parm);
	if(ct == XTOK_VALUEOBJECTWITHPATH) {
		valueObjectWithPathData(parm, (parseUnion*)&stateUnion->xtokObjectWithPath.object);
		stateUnion->xtokObjectWithPath.type = stateUnion->xtokObjectWithPath.object.type;
		ct = localLex((parseUnion*)&stateUnion->xtokObjectWithPath, parm);
		if(ct == ZTOK_VALUEOBJECTWITHPATH) {
		}
		else {
			parseError("ZTOK_VALUEOBJECTWITHPATH", ct, parm);
		}
	}
	else {
		parseError("XTOK_VALUEOBJECTWITHPATH", ct, parm);
	}
}

static void valueObjectWithPathData(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	CMPIObjectPath *op;
	CMPIInstance *inst;
	ct = localLex((parseUnion*)&stateUnion->xtokObjectWithPathData, parm);
	if(ct == XTOK_CLASSPATH) {
		dontLex = 1;
		classWithPath(parm, (parseUnion*)&stateUnion->xtokObjectWithPathData.cls);
		stateUnion->xtokObjectWithPathData.type = 1;
	}
	else if(ct == XTOK_INSTANCEPATH) {
		dontLex = 1;
		instanceWithPath(parm, (parseUnion*)&stateUnion->xtokObjectWithPathData.inst);
		stateUnion->xtokObjectWithPathData.type = 0;
		createPath(&op, &stateUnion->xtokObjectWithPathData.inst.path.instanceName);
		CMSetNameSpace(op, stateUnion->xtokObjectWithPathData.inst.path.path.nameSpacePath.value);
		CMSetHostname(op, stateUnion->xtokObjectWithPathData.inst.path.path.host.host);
		inst = native_new_CMPIInstance(op,NULL);
		setInstQualifiers(inst, &stateUnion->xtokObjectWithPathData.inst.inst.qualifiers);
		setInstProperties(inst, &stateUnion->xtokObjectWithPathData.inst.inst.properties);
		simpleArrayAdd(parm->respHdr.rvArray,(CMPIValue*)&inst,CMPI_instance);
	}
	else {
		parseError("XTOK_CLASSPATH or XTOK_INSTANCEPATH", ct, parm);
	}
}

static void classWithPath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokClassWithPath, parm);
	if(ct == XTOK_CLASSPATH) {
		dontLex = 1;
		classPath(parm, (parseUnion*)&stateUnion->xtokClassWithPath.path);
		class(parm, (parseUnion*)&stateUnion->xtokClassWithPath.cls);
	}
	else {
		parseError("XTOK_CLASSPATH or XTOK_CLASS", ct, parm);
	}
}

static void instanceWithPath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokInstanceWithPath, parm);
	if(ct == XTOK_INSTANCEPATH) {
		dontLex = 1;
		instancePath(parm, (parseUnion*)&stateUnion->xtokInstanceWithPath.path);
		instance(parm, (parseUnion*)&stateUnion->xtokInstanceWithPath.inst);
	}
	else {
		parseError("XTOK_INSTANCEPATH or XTOK_INSTANCE", ct, parm);
	}
}

static void class(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokClass, parm);
	if(ct == XTOK_CLASS) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				addQualifier(parm,&stateUnion->xtokClass.qualifiers,&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_PROPERTY || ct == XTOK_PROPERTYARRAY || ct == XTOK_PROPERTYREFERENCE) {
			do {
				dontLex = 1;
				genProperty(parm, (parseUnion*)&lvalp.xtokProperty);
				addProperty(parm,&stateUnion->xtokClass.properties,&lvalp.xtokProperty);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_PROPERTY || ct == XTOK_PROPERTYARRAY || ct == XTOK_PROPERTYREFERENCE);
			dontLex = 1;
		}
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_METHOD) {
			do {
				dontLex = 1;
				method(parm, (parseUnion*)&lvalp.xtokMethod);
				addMethod(parm,&stateUnion->xtokClass.methods,&lvalp.xtokMethod);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_METHOD);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokClass, parm);
		if(ct == ZTOK_CLASS) {
		}
		else {
			parseError("ZTOK_CLASS or XTOK_METHOD or XTOK_PROPERTY or XTOK_PROPERTYARRAY or XTOK_PROPERTYREFERENCE or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_CLASS", ct, parm);
	}
}

static void method(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokMethod, parm);
	if(ct == XTOK_METHOD) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				addQualifier(parm,&stateUnion->xtokMethod.qualifiers,&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_PARAM || ct == XTOK_PARAMREF || ct == XTOK_PARAMARRAY || ct == XTOK_PARAMREFARRAY) {
			do {
				dontLex = 1;
				methodData(parm, (parseUnion*)&lvalp.xtokMethodData);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_PARAM || ct == XTOK_PARAMREF || ct == XTOK_PARAMARRAY || ct == XTOK_PARAMREFARRAY);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokMethod, parm);
		if(ct == ZTOK_METHOD) {
		}
		else {
			parseError("ZTOK_METHOD or XTOK_PARAM or XTOK_PARAMREF or XTOK_PARAMARRAY or XTOK_PARAMREFARRAY or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_METHOD", ct, parm);
	}
}

static void methodData(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	if(ct == XTOK_PARAM) {
		dontLex = 1;
		if(ct == XTOK_PARAM) {
			dontLex = 1;
			parameter(parm, (parseUnion*)&lvalp.xtokParam);
		}
	}
	else if(ct == XTOK_PARAMREF) {
		dontLex = 1;
		if(ct == XTOK_PARAMREF) {
			dontLex = 1;
			parameterReference(parm, (parseUnion*)&lvalp.xtokParam);
		}
	}
	else if(ct == XTOK_PARAMARRAY) {
		dontLex = 1;
		if(ct == XTOK_PARAMARRAY) {
			dontLex = 1;
			parameterArray(parm, (parseUnion*)&lvalp.xtokParam);
		}
	}
	else if(ct == XTOK_PARAMREFARRAY) {
		dontLex = 1;
		if(ct == XTOK_PARAMREFARRAY) {
			dontLex = 1;
			parameterRefArray(parm, (parseUnion*)&lvalp.xtokParam);
		}
	}
	else {
		parseError("XTOK_PARAM or XTOK_PARAMREF or XTOK_PARAMARRAY or XTOK_PARAMREFARRAY", ct, parm);
	}
}

static void parameter(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokParam, parm);
	if(ct == XTOK_PARAM) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokParam, parm);
		if(ct == ZTOK_PARAM) {
		}
		else {
			parseError("ZTOK_PARAM or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_PARAM", ct, parm);
	}
}

static void parameterReference(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokParam, parm);
	if(ct == XTOK_PARAMREF) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokParam, parm);
		if(ct == ZTOK_PARAMREF) {
		}
		else {
			parseError("ZTOK_PARAMREF or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_PARAMREF", ct, parm);
	}
}

static void parameterRefArray(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokParam, parm);
	if(ct == XTOK_PARAMREFARRAY) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokParam, parm);
		if(ct == ZTOK_PARAMREFARRAY) {
		}
		else {
			parseError("ZTOK_PARAMREFARRAY or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_PARAMREFARRAY", ct, parm);
	}
}

static void parameterArray(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokParam, parm);
	if(ct == XTOK_PARAMARRAY) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokParam, parm);
		if(ct == ZTOK_PARAMARRAY) {
		}
		else {
			parseError("ZTOK_PARAMARRAY or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_PARAMARRAY", ct, parm);
	}
}

static void objectPath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokObjectPath, parm);
	if(ct == XTOK_OBJECTPATH) {
		instancePath(parm, (parseUnion*)&stateUnion->xtokObjectPath.path);
		ct = localLex((parseUnion*)&stateUnion->xtokObjectPath, parm);
		if(ct == ZTOK_OBJECTPATH) {
		}
		else {
			parseError("ZTOK_OBJECTPATH", ct, parm);
		}
	}
	else {
		parseError("XTOK_OBJECTPATH", ct, parm);
	}
}

static void classPath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokClassPath, parm);
	if(ct == XTOK_CLASSPATH) {
		nameSpacePath(parm, (parseUnion*)&stateUnion->xtokClassPath.name);
		className(parm, (parseUnion*)&stateUnion->xtokClassPath.className);
		ct = localLex((parseUnion*)&stateUnion->xtokClassPath, parm);
		if(ct == ZTOK_CLASSPATH) {
		}
		else {
			parseError("ZTOK_CLASSPATH", ct, parm);
		}
	}
	else {
		parseError("XTOK_CLASSPATH", ct, parm);
	}
}

static void className(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokClassName, parm);
	if(ct == XTOK_CLASSNAME) {
		ct = localLex((parseUnion*)&stateUnion->xtokClassName, parm);
		if(ct == ZTOK_CLASSNAME) {
		}
		else {
			parseError("ZTOK_CLASSNAME", ct, parm);
		}
	}
	else {
		parseError("XTOK_CLASSNAME", ct, parm);
	}
}

static void instancePath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokInstancePath, parm);
	if(ct == XTOK_INSTANCEPATH) {
		nameSpacePath(parm, (parseUnion*)&stateUnion->xtokInstancePath.path);
		instanceName(parm, (parseUnion*)&stateUnion->xtokInstancePath.instanceName);
		ct = localLex((parseUnion*)&stateUnion->xtokInstancePath, parm);
		if(ct == ZTOK_INSTANCEPATH) {
		}
		else {
			parseError("ZTOK_INSTANCEPATH", ct, parm);
		}
	}
	else {
		parseError("XTOK_INSTANCEPATH", ct, parm);
	}
}

static void localInstancePath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokLocalInstancePath, parm);
	if(ct == XTOK_LOCALINSTANCEPATH) {
		localNameSpacePath(parm, (parseUnion*)&stateUnion->xtokLocalInstancePath.path);
		instanceName(parm, (parseUnion*)&stateUnion->xtokLocalInstancePath.instanceName);
		ct = localLex((parseUnion*)&stateUnion->xtokLocalInstancePath, parm);
		if(ct == ZTOK_LOCALINSTANCEPATH) {
		}
		else {
			parseError("ZTOK_LOCALINSTANCEPATH", ct, parm);
		}
	}
	else {
		parseError("XTOK_LOCALINSTANCEPATH", ct, parm);
	}
}

static void nameSpacePath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokNameSpacePath, parm);
	if(ct == XTOK_NAMESPACEPATH) {
		host(parm, (parseUnion*)&stateUnion->xtokNameSpacePath.host);
		localNameSpacePath(parm, (parseUnion*)&stateUnion->xtokNameSpacePath.nameSpacePath);
		ct = localLex((parseUnion*)&stateUnion->xtokNameSpacePath, parm);
		if(ct == ZTOK_NAMESPACEPATH) {
		}
		else {
			parseError("ZTOK_NAMESPACEPATH", ct, parm);
		}
	}
	else {
		parseError("XTOK_NAMESPACEPATH", ct, parm);
	}
}

static void host(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokHost, parm);
	if(ct == XTOK_HOST) {
		ct = localLex((parseUnion*)&stateUnion->xtokHost, parm);
		if(ct == ZTOK_HOST) {
		}
		else {
			parseError("ZTOK_HOST", ct, parm);
		}
	}
	else {
		parseError("XTOK_HOST", ct, parm);
	}
}

static void localNameSpacePath(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokLocalNameSpacePath, parm);
	if(ct == XTOK_LOCALNAMESPACEPATH) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_NAMESPACE) {
			do {
				dontLex = 1;
				nameSpace(parm, (parseUnion*)&lvalp.xtokNameSpace);
				if(stateUnion->xtokLocalNameSpacePath.value) {
					stateUnion->xtokLocalNameSpacePath.value = parser_realloc(parm->heap, stateUnion->xtokLocalNameSpacePath.value, strlen(stateUnion->xtokLocalNameSpacePath.value) + strlen(lvalp.xtokNameSpace.ns) + 2);
					strcat(stateUnion->xtokLocalNameSpacePath.value, "/");
					strcat(stateUnion->xtokLocalNameSpacePath.value, lvalp.xtokNameSpace.ns);
				}
				else {
					stateUnion->xtokLocalNameSpacePath.value = parser_malloc(parm->heap, strlen(lvalp.xtokNameSpace.ns) + 1);
					strcpy(stateUnion->xtokLocalNameSpacePath.value, lvalp.xtokNameSpace.ns);
				}
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_NAMESPACE);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokLocalNameSpacePath, parm);
		if(ct == ZTOK_LOCALNAMESPACEPATH) {
		}
		else {
			parseError("ZTOK_LOCALNAMESPACEPATH or XTOK_NAMESPACE", ct, parm);
		}
	}
	else {
		parseError("XTOK_LOCALNAMESPACEPATH", ct, parm);
	}
}

static void nameSpace(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokNameSpace, parm);
	if(ct == XTOK_NAMESPACE) {
		ct = localLex((parseUnion*)&stateUnion->xtokNameSpace, parm);
		if(ct == ZTOK_NAMESPACE) {
		}
		else {
			parseError("ZTOK_NAMESPACE", ct, parm);
		}
	}
	else {
		parseError("XTOK_NAMESPACE", ct, parm);
	}
}

static void valueNamedInstance(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokNamedInstance, parm);
	if(ct == XTOK_VALUENAMEDINSTANCE) {
		instanceName(parm, (parseUnion*)&stateUnion->xtokNamedInstance.path);
		instance(parm, (parseUnion*)&stateUnion->xtokNamedInstance.instance);
		ct = localLex((parseUnion*)&stateUnion->xtokNamedInstance, parm);
		if(ct == ZTOK_VALUENAMEDINSTANCE) {
		}
		else {
			parseError("ZTOK_VALUENAMEDINSTANCE", ct, parm);
		}
	}
	else {
		parseError("XTOK_VALUENAMEDINSTANCE", ct, parm);
	}
}

static void instance(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokInstance, parm);
	if(ct == XTOK_INSTANCE) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				addQualifier(parm,&stateUnion->xtokInstance.qualifiers,&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_PROPERTY || ct == XTOK_PROPERTYARRAY || ct == XTOK_PROPERTYREFERENCE) {
			do {
				dontLex = 1;
				genProperty(parm, (parseUnion*)&lvalp.xtokProperty);
				addProperty(parm,&stateUnion->xtokInstance.properties,&lvalp.xtokProperty);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_PROPERTY || ct == XTOK_PROPERTYARRAY || ct == XTOK_PROPERTYREFERENCE);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokInstance, parm);
		if(ct == ZTOK_INSTANCE) {
		}
		else {
			parseError("ZTOK_INSTANCE or XTOK_PROPERTY or XTOK_PROPERTYARRAY or XTOK_PROPERTYREFERENCE or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_INSTANCE", ct, parm);
	}
}

static void genProperty(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokProperty, parm);
	if(ct == XTOK_PROPERTY) {
		dontLex = 1;
		property(parm, (parseUnion*)&stateUnion->xtokProperty.val);
		if(stateUnion->xtokProperty.val.value.type == typeValue_Instance) {
			stateUnion->xtokProperty.valueType = CMPI_instance;
		}
	}
	else if(ct == XTOK_PROPERTYARRAY) {
		dontLex = 1;
		propertyArray(parm, (parseUnion*)&stateUnion->xtokProperty.val);
	}
	else if(ct == XTOK_PROPERTYREFERENCE) {
		dontLex = 1;
		propertyReference(parm, (parseUnion*)&stateUnion->xtokProperty.val);
	}
	else {
		parseError("XTOK_PROPERTY or XTOK_PROPERTYARRAY or XTOK_PROPERTYREFERENCE", ct, parm);
	}
}

static void qualifier(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokQualifier, parm);
	if(ct == XTOK_QUALIFIER) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_VALUE || ct == XTOK_VALUEARRAY) {
			dontLex = 1;
			qualifierData(parm, (parseUnion*)&lvalp.xtokQualifierData);
			stateUnion->xtokQualifier.data = lvalp.xtokQualifierData;
			if(lvalp.xtokQualifierData.isArray) {
				stateUnion->xtokQualifier.type |= CMPI_ARRAY;
			}
		}
		ct = localLex((parseUnion*)&stateUnion->xtokQualifier, parm);
		if(ct == ZTOK_QUALIFIER) {
		}
		else {
			parseError("ZTOK_QUALIFIER or XTOK_VALUE or XTOK_VALUEARRAY", ct, parm);
		}
	}
	else {
		parseError("XTOK_QUALIFIER", ct, parm);
	}
}

static void qualifierData(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokQualifierData, parm);
	if(ct == XTOK_VALUE) {
		dontLex = 1;
		value(parm, (parseUnion*)&stateUnion->xtokQualifierData.value);
		stateUnion->xtokQualifierData.isArray = 0;
	}
	else if(ct == XTOK_VALUEARRAY) {
		dontLex = 1;
		valueArray(parm, (parseUnion*)&stateUnion->xtokQualifierData.array);
		stateUnion->xtokQualifierData.isArray = 1;
	}
	else {
		parseError("XTOK_VALUE or XTOK_VALUEARRAY", ct, parm);
	}
}

static void property(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokPropertyData, parm);
	if(ct == XTOK_PROPERTY) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				addQualifier(parm,&stateUnion->xtokPropertyData.qualifiers,&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_VALUE) {
			dontLex = 1;
			value(parm, (parseUnion*)&lvalp.xtokValue);
			stateUnion->xtokPropertyData.value = lvalp.xtokValue;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokPropertyData, parm);
		if(ct == ZTOK_PROPERTY) {
		}
		else {
			parseError("ZTOK_PROPERTY or XTOK_VALUE or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_PROPERTY", ct, parm);
	}
}

static void propertyArray(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokPropertyData, parm);
	if(ct == XTOK_PROPERTYARRAY) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				addQualifier(parm,&stateUnion->xtokPropertyData.qualifiers,&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_VALUEARRAY) {
			dontLex = 1;
			valueArray(parm, (parseUnion*)&lvalp.xtokValueArray);
			stateUnion->xtokPropertyData.array = lvalp.xtokValueArray;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokPropertyData, parm);
		if(ct == ZTOK_PROPERTYARRAY) {
		}
		else {
			parseError("ZTOK_PROPERTYARRAY or XTOK_VALUEARRAY or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_PROPERTYARRAY", ct, parm);
	}
}

static void propertyReference(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokPropertyData, parm);
	if(ct == XTOK_PROPERTYREFERENCE) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_QUALIFIER) {
			do {
				dontLex = 1;
				qualifier(parm, (parseUnion*)&lvalp.xtokQualifier);
				addQualifier(parm,&stateUnion->xtokPropertyData.qualifiers,&lvalp.xtokQualifier);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_QUALIFIER);
			dontLex = 1;
		}
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_VALUEREFERENCE) {
			dontLex = 1;
			valueReference(parm, (parseUnion*)&lvalp.xtokValueReference);
			stateUnion->xtokPropertyData.ref = lvalp.xtokValueReference;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokPropertyData, parm);
		if(ct == ZTOK_PROPERTYREFERENCE) {
		}
		else {
			parseError("ZTOK_PROPERTYREFERENCE or XTOK_VALUEREFERENCE or XTOK_QUALIFIER", ct, parm);
		}
	}
	else {
		parseError("XTOK_PROPERTYREFERENCE", ct, parm);
	}
}

static void instanceName(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokInstanceName, parm);
	if(ct == XTOK_INSTANCENAME) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_KEYBINDING) {
			do {
				dontLex = 1;
				keyBinding(parm, (parseUnion*)&lvalp.xtokKeyBinding);
				addKeyBinding(parm, &stateUnion->xtokInstanceName.bindings, &lvalp.xtokKeyBinding);
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_KEYBINDING);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokInstanceName, parm);
		if(ct == ZTOK_INSTANCENAME) {
		}
		else {
			parseError("ZTOK_INSTANCENAME or XTOK_KEYBINDING", ct, parm);
		}
	}
	else {
		parseError("XTOK_INSTANCENAME", ct, parm);
	}
}

static void keyBinding(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokKeyBinding, parm);
	if(ct == XTOK_KEYBINDING) {
		keyBindingContent(parm, (parseUnion*)&stateUnion->xtokKeyBinding.val);
		stateUnion->xtokKeyBinding.type = stateUnion->xtokKeyBinding.val.type;
		ct = localLex((parseUnion*)&stateUnion->xtokKeyBinding, parm);
		if(ct == ZTOK_KEYBINDING) {
		}
		else {
			parseError("ZTOK_KEYBINDING", ct, parm);
		}
	}
	else {
		parseError("XTOK_KEYBINDING", ct, parm);
	}
}

static void keyBindingContent(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokKeyBindingContent, parm);
	if(ct == XTOK_KEYVALUE) {
		dontLex = 1;
		keyValue(parm, (parseUnion*)&stateUnion->xtokKeyBindingContent.keyValue);
		stateUnion->xtokKeyBindingContent.type = stateUnion->xtokKeyBindingContent.keyValue.valueType;
	}
	else if(ct == XTOK_VALUEREFERENCE) {
		dontLex = 1;
		valueReference(parm, (parseUnion*)&stateUnion->xtokKeyBindingContent.ref);
		stateUnion->xtokKeyBindingContent.type = "ref";
	}
	else {
		parseError("XTOK_KEYVALUE or XTOK_VALUEREFERENCE", ct, parm);
	}
}

static void keyValue(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokKeyValue, parm);
	if(ct == XTOK_KEYVALUE) {
		ct = localLex((parseUnion*)&stateUnion->xtokKeyValue, parm);
		if(ct == ZTOK_KEYVALUE) {
		}
		else {
			parseError("ZTOK_KEYVALUE", ct, parm);
		}
	}
	else {
		parseError("XTOK_KEYVALUE", ct, parm);
	}
}

static void value(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokValue, parm);
	if(ct == XTOK_VALUE) {
		valueData(parm, (parseUnion*)&stateUnion->xtokValue.data);
		stateUnion->xtokValue.type = stateUnion->xtokValue.data.type;
		ct = localLex((parseUnion*)&stateUnion->xtokValue, parm);
		if(ct == ZTOK_VALUE) {
		}
		else {
			parseError("ZTOK_VALUE", ct, parm);
		}
	}
	else {
		parseError("XTOK_VALUE", ct, parm);
	}
}

static void valueData(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokValueData, parm);
	if(ct == ZTOK_VALUE) {
		stateUnion->xtokValueData.type=typeValue_charP;
		dontLex = 1;
	}
	else if(ct == XTOK_CDATA) {
		stateUnion->xtokValueData.inst = parser_malloc(parm->heap, sizeof(XtokInstance));
		instance(parm, (parseUnion*)stateUnion->xtokValueData.inst);
		stateUnion->xtokValueData.type=typeValue_Instance;
		ct = localLex((parseUnion*)&stateUnion->xtokValueData, parm);
		if(ct == ZTOK_CDATA) {
		}
		else {
			parseError("ZTOK_CDATA", ct, parm);
		}
	}
	else {
		parseError("ZTOK_VALUE", ct, parm);
	}
}

static void valueArray(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokValueArray, parm);
	if(ct == XTOK_VALUEARRAY) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_VALUE) {
			do {
				dontLex = 1;
				value(parm, (parseUnion*)&lvalp.xtokValue);
				if(stateUnion->xtokValueArray.next >= stateUnion->xtokValueArray.max) {
					stateUnion->xtokValueArray.max *= 2;
					stateUnion->xtokValueArray.values = (char**)parser_realloc(parm->heap, stateUnion->xtokValueArray.values, sizeof(char*) * stateUnion->xtokValueArray.max);
				}
				stateUnion->xtokValueArray.values[stateUnion->xtokValueArray.next++] = lvalp.xtokValue.data.value;
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_VALUE);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokValueArray, parm);
		if(ct == ZTOK_VALUEARRAY) {
		}
		else {
			parseError("ZTOK_VALUEARRAY or XTOK_VALUE", ct, parm);
		}
	}
	else {
		parseError("XTOK_VALUEARRAY", ct, parm);
	}
}

static void valueRefArray(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokValueRefArray, parm);
	if(ct == XTOK_VALUEREFARRAY) {
		ct = localLex(&lvalp, parm);
		dontLex = 1;
		if(ct == XTOK_VALUEREFERENCE) {
			do {
				dontLex = 1;
				valueReference(parm, (parseUnion*)&lvalp.xtokValueReference);
				if(stateUnion->xtokValueRefArray.next >= stateUnion->xtokValueRefArray.max) {
					stateUnion->xtokValueRefArray.max *= 2;
					stateUnion->xtokValueRefArray.values = (XtokValueReference*)parser_realloc(parm->heap, stateUnion->xtokValueRefArray.values, sizeof(XtokValueReference) * stateUnion->xtokValueRefArray.max);
				}
				stateUnion->xtokValueRefArray.values[stateUnion->xtokValueRefArray.next++] = lvalp.xtokValueReference;
				ct = localLex(&lvalp, parm);
			}
			while(ct == XTOK_VALUEREFERENCE);
			dontLex = 1;
		}
		ct = localLex((parseUnion*)&stateUnion->xtokValueRefArray, parm);
		if(ct == ZTOK_VALUEREFARRAY) {
		}
		else {
			parseError("ZTOK_VALUEREFARRAY or XTOK_VALUEREFERENCE", ct, parm);
		}
	}
	else {
		parseError("XTOK_VALUEREFARRAY", ct, parm);
	}
}

static void valueReference(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokValueReference, parm);
	if(ct == XTOK_VALUEREFERENCE) {
		valueReferenceData(parm, (parseUnion*)&stateUnion->xtokValueReference.data);
		stateUnion->xtokValueReference.type = stateUnion->xtokValueReference.data.type;
		ct = localLex((parseUnion*)&stateUnion->xtokValueReference, parm);
		if(ct == ZTOK_VALUEREFERENCE) {
		}
		else {
			parseError("ZTOK_VALUEREFERENCE", ct, parm);
		}
	}
	else {
		parseError("XTOK_VALUEREFERENCE", ct, parm);
	}
}

static void valueReferenceData(ParserControl *parm, parseUnion *stateUnion)
{
	parseUnion lvalp;
	ct = localLex((parseUnion*)&stateUnion->xtokValueReferenceData, parm);
	if(ct == XTOK_INSTANCEPATH) {
		dontLex = 1;
		instancePath(parm, (parseUnion*)&stateUnion->xtokValueReferenceData.instancePath);
		stateUnion->xtokValueReferenceData.type = typeValRef_InstancePath;
	}
	else if(ct == XTOK_LOCALINSTANCEPATH) {
		dontLex = 1;
		localInstancePath(parm, (parseUnion*)&stateUnion->xtokValueReferenceData.localInstancePath);
		stateUnion->xtokValueReferenceData.type = typeValRef_LocalInstancePath;
	}
	else if(ct == XTOK_INSTANCENAME) {
		dontLex = 1;
		instanceName(parm, (parseUnion*)&stateUnion->xtokValueReferenceData.instanceName);
		stateUnion->xtokValueReferenceData.type = typeValRef_InstanceName;
	}
	else {
		parseError("XTOK_INSTANCEPATH or XTOK_LOCALINSTANCEPATH or XTOK_INSTANCENAME", ct, parm);
	}
}

