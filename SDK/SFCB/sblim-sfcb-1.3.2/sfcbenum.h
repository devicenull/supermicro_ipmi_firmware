/*
 * sfcbenum.h
 *
 * (C) Copyright IBM Corp. 2008
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Gary Readler
 * Contributions: 
 *
 * Description:
 *
 * Header file for native enumeration 
 *
*/


#ifndef LARGE_VOL_SUPPORT 
struct native_enum {
   CMPIEnumeration enumeration;
   int refCount;
   int mem_state;
   CMPICount current;
   CMPIArray *data;
};
#else
/*
 * enum_states
 */
#define ENUM_INIT       0x80    /* initialized                                */
#define ENUM_STARTED    0x85    /* some data has been added to the data array */
#define ENUM_COMPLETE   0x90    /* finished adding data to the array          */ 

struct native_enum {
   CMPIEnumeration enumeration;
   int refCount;
   int mem_state;
   CMPICount current;
   CMPIArray *data;
   int enum_state ;             
   CMPICount last_valid ;       /* the count of last added array element */
   pthread_mutex_t enumlock ;
};
#endif
