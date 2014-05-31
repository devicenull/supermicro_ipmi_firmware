
/*
 * sfcVersion.h
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
 * Author:        Frank Scheffler
 * Contributions: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * Version definition.
 *
*/

#ifndef SFCVERSION_H
#define SFCVERSION_H

#ifdef HAVE_CONFIG_H
#include "config.h"

#define sfcBrokerVersion PACKAGE_VERSION
#define sfcHttpDaemonVersion PACKAGE_VERSION
#define sfcdbpDaemonVersion "0.1.0"

#else
/* this should never be used - but who knows */
#define sfcBrokerVersion "0.8.1"
#define sfcHttpDaemonVersion "0.8.1"
#define sfcdbpDaemonVersion "0.1.0"

#endif

#endif
