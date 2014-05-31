
/*
 * $Id: classSchemaMem.c,v 1.3 2006/10/25 16:59:43 mihajlov Exp $
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
 * Author:        Viktor Mihajlovski <mihajlov@de.ibm.com>
 *
 * Description:
 *
 * In Memory Class Repository Namespace Listing
 * Instruction: add  extern ClassSchema declarations for all
 *              in-memory namespaces.
 *
*/

#include <stddef.h>
#include "classSchemaMem.h"

extern ClassSchema interop_classes;
/* extern ClassSchema cimv2_classes; */

NamespaceDir sfcb_mem_namespaces[] = {
  {"root/interop", &interop_classes},
  /* {"root/cimv2", &cimv2_classes}, */
  {NULL, NULL} /* this is the end marker, don't remove */
};
