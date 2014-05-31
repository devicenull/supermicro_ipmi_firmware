
/*
 * hashtable.h
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
 * Author:        Keith Pomakis <pomaki@pobox.xom>
 * Contributions: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * hashtable implementation.
 *
*/

/*--------------------------------------------------------------------------*\
 *                   -----===== HashTable =====-----
 *
 * Author: Keith Pomakis (pomakis@pobox.com)
 * Date:   August, 1998
 *
\*--------------------------------------------------------------------------*/


#ifndef _HASHTABLE_H
#define _HASHTABLE_H

/* These structs should not be accessed directly from user code.
 * All access should be via the public functions declared below. */

typedef struct KeyValuePair_struct {
   const void *key;
   void *value;
   struct KeyValuePair_struct *next;
} KeyValuePair;

typedef struct {
   long numOfBuckets;
   long numOfElements;
   KeyValuePair **bucketArray;
   float idealRatio, lowerRehashThreshold, upperRehashThreshold;
   int (*keycmp) (const void *key1, const void *key2);
   int (*valuecmp) (const void *value1, const void *value2);
   unsigned long (*hashFunction) (const void *key);
   void (*keyDeallocator) (void *key);
   void (*valueDeallocator) (void *value);
} HashTable;

struct _HashTableIterator {
   int bucket;
   KeyValuePair *pair;
};
typedef struct _HashTableIterator HashTableIterator;


#endif                          /* _HASHTABLE_H */
