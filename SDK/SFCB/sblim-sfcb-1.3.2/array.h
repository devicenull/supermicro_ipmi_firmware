
/*
 * array.h
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
 * CMPIArray implementation.
 *
*/


#ifndef array_h
#define array_h

struct native_array_item {
   CMPIValueState state;
   CMPIValue value;
};


struct native_array {
   CMPIArray array;
   int refCount;
   int mem_state;

   CMPICount size,max,dynamic;
   CMPIType type;
   struct native_array_item *data;
};

#endif
