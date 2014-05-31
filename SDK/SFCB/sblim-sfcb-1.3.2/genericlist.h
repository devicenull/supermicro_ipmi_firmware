
/*
 * genericList.h
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
 * Author:        Keith Pomakis <pomaki@pobox.com>
 * Contributions: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * list implementation.
 *
*/

   /************************************************************************
   *************************************************************************
   **                                                                     **
   **                        Generic List Library                         **
   **                                                                     **
   **                          by Keith Pomakis                           **
   **                     kppomaki@jeeves.uwaterloo.ca                    **
   **                                                                     **
   **                            Spring, 1994                             **
   **                                                                     **
   *************************************************************************
   ************************************************************************/

#ifndef GENERIC_LIST_DEFINED
#define GENERIC_LIST_DEFINED

#include "utilft.h"

typedef struct GLE_struct {
   void *pointer;
   struct GLE_struct *previous;
   struct GLE_struct *next;
} Generic_list_element;

typedef struct {
   Generic_list_element *current;
   Generic_list_element pre_element, post_element, deleted_element;
   int (*lt) (void *a, void *b);
   unsigned int num_of_elements;
} Generic_list_info;

typedef struct {
   Generic_list_info *info;
} Generic_list;

#define Generic_stack Generic_list
#define Generic_queue Generic_list


/****************************************************************************/

/* Stack operations */

#define initialize_stack initialize_list
#define destroy_stack destroy_list
#define push add_to_beginning
#define pop remove_from_beginning
#define peek_at_top peek_at_beginning
#define copy_stack copy_list

/* Queue operations */

#define initialize_queue initialize_list
#define destroy_queue destroy_list
#define enqueue add_to_end
#define dequeue remove_from_beginning
#define dequeue_all remove_all
#define peek_at_head peek_at_beginning
#define peek_at_tail peek_at_end
#define copy_queue copy_list

#endif                          /* GENERIC_LIST_DEFINED */
