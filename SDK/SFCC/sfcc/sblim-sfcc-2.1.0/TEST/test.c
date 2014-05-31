/*
 * test.c
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
 *  General test for library APIs, original from sfcc project. To pass 
 *  all tests requires additional support to be added to your CIMOM.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <string.h>

static char * _HOSTNAME;

extern char *value2Chars(CMPIType type, CMPIValue * value);

static void showObjectPath( CMPIObjectPath * objectpath )
{
   CMPIString * namespace = objectpath->ft->getNameSpace(objectpath, NULL);
   CMPIString * classname = objectpath->ft->getClassName(objectpath, NULL);
   int numkeys = objectpath->ft->getKeyCount(objectpath, NULL);
   int i;

   if (namespace && namespace->hdl) printf("namespace=%s\n", (char *)namespace->hdl);
   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numkeys) {
      printf("keys:\n");
      for (i=0; i<numkeys; i++) {
         CMPIString * keyname;
	 char       * str=NULL;
         CMPIData data = objectpath->ft->getKeyAt(objectpath, i, &keyname, NULL);
         printf("\t%s=%s\n", (char *)keyname->hdl, str = value2Chars(data.type, &data.value)); 
	 if (keyname) CMRelease(keyname);
	 if (str) free(str);
      }
   }

   if (classname) CMRelease(classname); 
   if (namespace) CMRelease(namespace);
}


static void showInstance( CMPIInstance *instance )
{
   CMPIObjectPath * objectpath = instance->ft->getObjectPath(instance, NULL);
   CMPIString * objectpathname = objectpath->ft->toString(objectpath, NULL);
   CMPIString * namespace = objectpath->ft->getNameSpace(objectpath, NULL);
   CMPIString * classname = objectpath->ft->getClassName(objectpath, NULL);
   int numproperties = instance->ft->getPropertyCount(instance, NULL);
   int i;

   if (objectpathname && objectpathname->hdl) printf("objectpath=%s\n", (char *)objectpathname->hdl);
   if (namespace && namespace->hdl) printf("namespace=%s\n", (char *)namespace->hdl);
   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numproperties) {
      printf("properties:\n");
      for (i=0; i<numproperties; i++) {
         CMPIString * propertyname;
	 char * str = NULL;
         CMPIData data = instance->ft->getPropertyAt(instance, i, &propertyname, NULL);
         printf("\t%s=%s\n", (char *)propertyname->hdl, str=value2Chars(data.type, &data.value));
	 if (propertyname) CMRelease(propertyname);
	 if (str) free(str);
      }
   }

   if (classname) CMRelease(classname);
   if (namespace) CMRelease(namespace);
   if (objectpathname) CMRelease(objectpathname);
   if (objectpath) CMRelease(objectpath);
}


static void showClass( CMPIConstClass * class )
{
   CMPIString * classname = class->ft->getClassName(class, NULL);
   int numproperties = class->ft->getPropertyCount(class, NULL);
   int i;

   if (classname && classname->hdl) printf("classname=%s\n", (char *)classname->hdl);
   if (numproperties) {
      printf("properties:\n");
      for (i=0; i<numproperties; i++) {
         CMPIString * propertyname;
	 char * str=NULL;
         CMPIData data = class->ft->getPropertyAt(class, i, &propertyname, NULL);
         if (data.state==0)
            printf("\t%s=%s\n", (char *)propertyname->hdl, str=value2Chars(data.type, &data.value));
         else printf("\t%s=NIL\n", (char *)propertyname->hdl);
	 if (propertyname) CMRelease(propertyname);
	 if (str) free(str);
      }
   }

   if (classname) CMRelease(classname);
}


int main( int argc, char * argv[] )
{
   CMCIClient *cc;
   CMPIObjectPath * objectpath, * objectpath2;
   CMPIInstance * instance;
   CMPIEnumeration * enumeration;
   CMPIConstClass * class;
   CMPIStatus status;
   CMPIValue yesValue;
   char hostName[512];

   /* Setup a conncetion to the CIMOM */   
   cc = cmciConnect("localhost", NULL, "5988", "clp", NULL, NULL);
   
   gethostname(hostName,511);
   _HOSTNAME=strdup(hostName);

   if (1) {
      /* Test enumClassNames() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing enumClassNames() ...\n");   
      objectpath = newCMPIObjectPath("root/cimv2", NULL, NULL);
      enumeration = cc->ft->enumClassNames(cc, objectpath, 0, &status);

      /* Print the results */
      printf("enumClassNames() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test enumClasses() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing enumClasses() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", NULL, NULL);
      enumeration = cc->ft->enumClasses(cc, objectpath, 0, &status);

      /* Print the results */
      printf("enumClasses() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showClass(data.value.cls);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test getClass() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing getClass() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
      class = cc->ft->getClass(cc, objectpath, 0, NULL, &status);

      /* Print the results */
      printf("getClass() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result:\n");
         showClass(class);
      }
      if (class) CMRelease(class);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test enumInstanceNames() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing enumInstanceNames() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
      enumeration = cc->ft->enumInstanceNames(cc, objectpath, &status);

      /* Print the results */
      printf("enumInstanceNames() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
   }

   if (1) {
      /* Test enumInstances() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing enumInstances() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
      enumeration = cc->ft->enumInstances(cc, objectpath, 0, NULL, &status);

      /* Print the results */
      printf("enumInstances() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showInstance(data.value.inst);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test getInstance() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing getInstance() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
      CMAddKey(objectpath, "CreationClassName", "CIM_ComputerSystem", CMPI_chars);
      CMAddKey(objectpath, "Name", _HOSTNAME, CMPI_chars);
      instance = cc->ft->getInstance(cc, objectpath, 0, NULL, &status);   

      /* Print the results */
      printf("getInstance() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result:\n");
         showInstance(instance);
      }
      if (instance) CMRelease(instance);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test createInstance() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing createInstance() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
      instance = newCMPIInstance(objectpath, NULL);
      CMSetProperty(instance, "Username", "bestorga", CMPI_chars);
      CMSetProperty(instance, "Classname", "foobar", CMPI_chars);

      objectpath2 = cc->ft->createInstance(cc, objectpath, instance, &status);
      CMRelease(objectpath);
      objectpath = objectpath2;
                                                                                                                
      /* Print the results */
      printf("createInstance() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result:\n");
         showObjectPath(objectpath);
      }
      if (instance) CMRelease(instance);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test setInstance() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing setInstance() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
      CMAddKey(objectpath, "Username", "bestorga", CMPI_chars);
      CMAddKey(objectpath, "Classname", "foobar", CMPI_chars);
      instance = newCMPIInstance(objectpath, NULL);
      CMSetProperty(instance, "Username", "bestorga", CMPI_chars);
      CMSetProperty(instance, "Classname", "foobar", CMPI_chars);
      yesValue.boolean = 1;
      CMSetProperty(instance, "Query", &yesValue, CMPI_boolean);
                                                                                                                
      status = cc->ft->setInstance(cc, objectpath, instance, 0, NULL);
                                                                                                                
      /* Print the results */
      printf("setInstance() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (instance) CMRelease(instance);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test invokeMethod() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing invokeMethod() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
      CMAddKey(objectpath, "Username", "bestorga", CMPI_chars);
      CMAddKey(objectpath, "Classname", "foobar", CMPI_chars);
      CMPIArgs * argsin = newCMPIArgs(NULL);
      CMPIValue value;
      value.string = newCMPIString("Query", NULL);
      argsin->ft->addArg(argsin, "operation", &value, CMPI_string);
      CMPIData data = cc->ft->invokeMethod(cc, objectpath, "IsAuthorized", argsin, NULL, &status);
                                                                                                                
      /* Print the results */
      printf("invokeMethod() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         printf("User 'bestorga' IsAuthorized? %s\n", value2Chars(data.type, &data.value));
      }
      if (argsin) CMRelease(argsin);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
      if (value.string) CMRelease(value.string);
   }

   if (1) {
      /* Test deleteInstance() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing deleteInstance() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
      CMAddKey(objectpath, "Username", "bestorga", CMPI_chars);
      CMAddKey(objectpath, "Classname", "foobar", CMPI_chars);

      status = cc->ft->deleteInstance(cc, objectpath);
                                                                                                                
      /* Print the results */
      printf("deleteInstance() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test associators() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing associators() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      CMAddKey(objectpath, "CreationClassName", "Linux_ComputerSystem", CMPI_chars);
      CMAddKey(objectpath, "Name", _HOSTNAME, CMPI_chars);
      enumeration = cc->ft->associators(cc, objectpath, "Linux_RunningOS", NULL, NULL, NULL, 0, NULL, &status);

      /* Print the results */
      printf("associators() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showInstance(data.value.inst);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test associatorNames() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing associatorNames() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      CMAddKey(objectpath, "CreationClassName", "Linux_ComputerSystem", CMPI_chars);
      CMAddKey(objectpath, "Name", _HOSTNAME, CMPI_chars);
      enumeration = cc->ft->associatorNames(cc, objectpath, "Linux_RunningOS", NULL, NULL, NULL, &status);
                                                                                                                
      /* Print the results */
      printf("associatorNames() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test references() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing references() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      CMAddKey(objectpath, "CreationClassName", "Linux_ComputerSystem", CMPI_chars);
      CMAddKey(objectpath, "Name", _HOSTNAME, CMPI_chars);
      enumeration = cc->ft->references(cc, objectpath, NULL, NULL, 0, NULL, &status);
                                                                                                                
      /* Print the results */
      printf("references() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showInstance(data.value.inst);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test referenceNames() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing referenceNames() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      CMAddKey(objectpath, "CreationClassName", "Linux_ComputerSystem", CMPI_chars);
      CMAddKey(objectpath, "Name", _HOSTNAME, CMPI_chars);
      enumeration = cc->ft->referenceNames(cc, objectpath, NULL, NULL, &status);
                                                                                                                
      /* Print the results */
      printf("referenceNames() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test getProperty() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing getProperty() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      CMAddKey(objectpath, "CreationClassName", "Linux_ComputerSystem", CMPI_chars);
      CMAddKey(objectpath, "Name", _HOSTNAME, CMPI_chars);
      CMPIData data = cc->ft->getProperty(cc, objectpath, "PrimaryOwnerName", &status);

      /* Print the results */
      printf("getProperty() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         printf("PrimaryOwnerName=%s\n", (char*)(data.value.string)->hdl);
      }
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (1) {
      /* Test setProperty() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing setProperty() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      CMAddKey(objectpath, "CreationClassName", "Linux_ComputerSystem", CMPI_chars);
      CMAddKey(objectpath, "Name", _HOSTNAME, CMPI_chars);
      CMPIValue value;
      value.string = newCMPIString("Gareth Bestor", NULL);
      status = cc->ft->setProperty(cc, objectpath, "PrimaryOwnerName", &value, CMPI_string);

      /* Print the results */
      printf("setProperty() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
      if (value.string) CMRelease(value.string);
   }

   if (1) {
      /* Test execQuery() */
      printf("\n----------------------------------------------------------\n");
      printf("Testing execQuery() ...\n");
      objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
      char * query = "select * from Linux_ComputerSystem where PrimaryOwnerName=\"root\"";
      enumeration = cc->ft->execQuery(cc, objectpath, query, "WQL", &status);
                                                                                                                
      /* Print the results */
      printf("execQuery() rc=%d, msg=%s\n", status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
      if (!status.rc) {
         printf("result(s):\n");
         while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showInstance(data.value.inst);
         }
      }
      if (enumeration) CMRelease(enumeration);
      if (objectpath) CMRelease(objectpath);
      if (status.msg) CMRelease(status.msg);
   }

   if (cc) CMRelease(cc);
   if (_HOSTNAME) free(_HOSTNAME);
   
   return 0;
}
