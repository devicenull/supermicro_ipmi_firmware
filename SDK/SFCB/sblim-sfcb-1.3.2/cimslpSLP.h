/*
 * cimslpSLP.h
 *
 * (C) Copyright IBM Corp. 2006
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Sven Schuetz <sven@de.ibm.com>
 * Contributions:
 *
 * Description:
 *
 * Functions for slp regs/deregs
 *
*/

#ifndef _cimslpSLP_h
#define _cimslpSLP_h

char* buildAttrString(char * name, char * value, char * attrstring);
char* buildAttrStringFromArray(char * name, char ** value, char * attrstring);
int registerCIMService(cimSLPService css, int slpLifeTime);
void deregisterCIMService();

#endif
