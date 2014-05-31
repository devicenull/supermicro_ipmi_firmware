/**
 * $Id: backend.h,v 1.3 2006/10/27 13:14:21 sschuetz Exp $
 *
 * (C) Copyright IBM Corp. 2004
 * 
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Viktor Mihajlovski <mihajlov@de.ibm.com>
 * Contributors: 
 *
 * Description: Common MOF Compiler Backend Declarations
 *
 */

#ifndef BACKEND_H
#define BACKEND_H

#include "symtab.h"

#define BACKEND_DEFAULT   0x0000
#define BACKEND_VERBOSE   0x0001

typedef int (backend_type) (class_chain * cls_chain, class_chain * inst_chain, qual_chain * qual_chain,
							const char * outfile, const char * outdir, const char * ns,
							unsigned options, const char * extraopts);

#endif
