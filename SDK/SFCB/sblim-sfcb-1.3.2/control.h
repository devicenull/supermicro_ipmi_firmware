/*
 * $Id: control.h,v 1.3 2007/01/16 12:45:31 mihajlov Exp $
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
 * sfcb.cfg config parser
 *
 */

#ifndef _CONTROL_
#define _CONTROL_

int setupControl(char *fn);
void sunsetControl();
int getControlChars(char *id, char **val);
int getControlNum(char *id, long *val);
int getControlBool(char *id, int *val);

#endif
