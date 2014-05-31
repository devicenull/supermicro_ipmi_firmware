
/*
 * genericList.c
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

#include <stdio.h>
#include <stdlib.h>
#include "genericlist.h"
#include "mlog.h"
#include "support.h"


#ifdef THINK_C
#define malloc NewPtr
#endif

#define NEW(x) ((x *) emalloc(sizeof(x)))


static void initialize_list(Generic_list * list);
static void initialize_sorted_list(Generic_list * list,
                                   int (*lt) (void *a, void *b));
static void destroy_list(Generic_list * list);
static void add_to_beginning(Generic_list list, void *pointer);
static void add_to_end(Generic_list list, void *pointer);
//static void add_to_list(Generic_list list, void *pointer);
static void *remove_from_beginning(Generic_list list);
static void *remove_from_end(Generic_list list);
static void *remove_from_list(Generic_list list, void *pointer);
static void remove_all(Generic_list list);
//static void *peek_at_beginning(Generic_list list);
//static void *peek_at_end(Generic_list list);

static void *first_in_list(Generic_list list);
static void *next_in_list(Generic_list list);
static void *current_in_list(Generic_list list);
static void *remove_current(Generic_list list);
static void *previous_in_list(Generic_list list);
static void *last_in_list(Generic_list list);
static void reset_to_beginning(Generic_list list);
//static void reset_to_end(Generic_list list);

static int num_of_objects(const Generic_list list);
static int is_empty(const Generic_list list);
static int is_in_list(const Generic_list list, const void *pointer);
static Generic_list copy_list(Generic_list list);

/*
static void perform_on_list(Generic_list list, void (*fn) (void *pointer,
    void *args),void *args);
static void *first_that(Generic_list list,
                        int (*fn) (const void *pointer, const void *args),
                        const void *args);
static void *next_that(Generic_list list,
                       int (*fn) (const void *pointer, const void *args),
                       const void *args);
static void *previous_that(Generic_list list,
                           int (*fn) (const void *pointer,
                                      const void *args), const void *args);
static void *last_that(Generic_list list,
                       int (*fn) (const void *pointer, const void *args),
                       const void *args);
static Generic_list all_such_that(Generic_list list,
                                  int (*fn) (const void *pointer,
                                             const void *args),
                                  const void *args);
static void remove_all_such_that(Generic_list list,
                                 int (*fn) (const void *pointer,
                                            const void *args),
                                 const void *args);
*/



static char *module = "generic_list";

static void *emalloc(unsigned int n);

/****************************************************************************/

static void initialize_list(Generic_list * list)
{
   list->info = NEW(Generic_list_info);

   list->info->pre_element.pointer = NULL;
   list->info->pre_element.previous = &list->info->pre_element;
   list->info->pre_element.next = &list->info->post_element;
   list->info->post_element.pointer = NULL;
   list->info->post_element.previous = &list->info->pre_element;
   list->info->post_element.next = &list->info->post_element;

   list->info->current = &list->info->pre_element;
   list->info->deleted_element.pointer = NULL;
   list->info->lt = NULL;
   list->info->num_of_elements = 0;
}

/****************************************************************************/

static void
initialize_sorted_list(Generic_list * list, int (*lt) (void *a, void *b))
{
   initialize_list(list);
   list->info->lt = lt;
}

/****************************************************************************/

static void destroy_list(Generic_list * list)
{
   remove_all(*list);
   free((void *) list->info);
}

/****************************************************************************/

static void add_to_beginning(Generic_list list, void *pointer)
{
   Generic_list_element *element;

   if (!pointer) {
      mlogf(M_ERROR,M_SHOW, "%s: NULL pointer passed 1\n", module);
      return;
      exit(EXIT_FAILURE);
   }

   element = NEW(Generic_list_element);
   element->next = list.info->pre_element.next;
   element->previous = &list.info->pre_element;
   element->pointer = pointer;

   list.info->pre_element.next->previous = element;
   list.info->pre_element.next = element;

   list.info->num_of_elements++;
}

/****************************************************************************/

static void add_to_end(Generic_list list, void *pointer)
{
   Generic_list_element *element;

   if (!pointer) {
      mlogf(M_ERROR,M_SHOW, "%s: NULL pointer passed 2\n", module);
  //    abort();
      return;
      exit(EXIT_FAILURE);
   }

   element = NEW(Generic_list_element);
   element->next = &list.info->post_element;
   element->previous = list.info->post_element.previous;
   element->pointer = pointer;

   list.info->post_element.previous->next = element;
   list.info->post_element.previous = element;

   list.info->num_of_elements++;
}

/****************************************************************************/
/*
static void add_to_list(Generic_list list, void *pointer)
{
    Generic_list_element *element, *new_element;

    if (list.info->lt) {
        if (!pointer) {
            mlogf(M_ERROR,M_SHOW, "%s: NULL pointer passed\n", module);
            exit(EXIT_FAILURE);
        }

        element = list.info->pre_element.next;
        while (element != &list.info->post_element &&
               (*list.info->lt) (element->pointer, pointer))
            element = element->next;

        new_element = NEW(Generic_list_element);
        new_element->next = element;
        new_element->previous = element->previous;
        new_element->pointer = pointer;

        element->previous->next = new_element;
        element->previous = new_element;

        list.info->num_of_elements++;
    } else
        add_to_end(list, pointer);
}
*/
/****************************************************************************/

static void *remove_from_list(Generic_list list, void *pointer)
{
   Generic_list_element *element;

   element = list.info->post_element.previous;

   while (element != &list.info->pre_element && element->pointer != pointer)
      element = element->previous;

   if (element == &list.info->pre_element)
      /* No such element was found. */
      return NULL;

   if (element == list.info->current) {
      list.info->deleted_element.previous = element->previous;
      list.info->deleted_element.next = element->next;
      list.info->current = &list.info->deleted_element;
   }

   element->previous->next = element->next;
   element->next->previous = element->previous;

   free(element);
   list.info->num_of_elements--;

   return pointer;
}

/****************************************************************************/

static void *remove_from_beginning(Generic_list list)
{
   Generic_list_element *element;
   void *pointer;

   if (list.info->num_of_elements == 0)
      return NULL;

   element = list.info->pre_element.next;
   if (element == list.info->current)
      list.info->current = &list.info->pre_element;

   pointer = element->pointer;
   list.info->pre_element.next = element->next;
   element->next->previous = &list.info->pre_element;

   free(element);
   list.info->num_of_elements--;

   return pointer;
}

/****************************************************************************/

static void *remove_from_end(Generic_list list)
{
   Generic_list_element *element;
   void *pointer;

   if (list.info->num_of_elements == 0)
      return NULL;

   element = list.info->post_element.previous;
   if (element == list.info->current)
      list.info->current = &list.info->post_element;

   pointer = element->pointer;
   list.info->post_element.previous = element->previous;
   element->previous->next = &list.info->post_element;

   free(element);
   list.info->num_of_elements--;

   return pointer;
}

/****************************************************************************/

static void *remove_current(Generic_list list)
{
   Generic_list_element *element;
   void *pointer;

   element = list.info->current;
   if (element->pointer == NULL)
      return NULL;

   list.info->deleted_element.previous = element->previous;
   list.info->deleted_element.next = element->next;
   list.info->current = &list.info->deleted_element;

   pointer = element->pointer;
   element->next->previous = element->previous;
   element->previous->next = element->next;

   free(element);
   list.info->num_of_elements--;

   return pointer;
}

/****************************************************************************/

static void remove_all(Generic_list list)
{
   Generic_list_element *element;

   element = list.info->pre_element.next;
   while (element && element != &list.info->post_element) {
      element = element->next;
      if (element) free(element->previous);
   }

   list.info->pre_element.next = &list.info->post_element;
   list.info->post_element.previous = &list.info->pre_element;
   list.info->num_of_elements = 0;
}

/****************************************************************************/
/*
static void *peek_at_beginning(Generic_list list)
{
    return list.info->pre_element.next->pointer;
}
*/
/****************************************************************************/
/*
static void *peek_at_end(Generic_list list)
{
    return list.info->post_element.previous->pointer;
}
*/
/****************************************************************************/

static void *first_in_list(Generic_list list)
{
   list.info->current = list.info->pre_element.next->next->previous;
   return list.info->current->pointer;
}

/****************************************************************************/

static void *current_in_list(Generic_list list)
{
   return list.info->current->pointer;
}

/****************************************************************************/

static void *last_in_list(Generic_list list)
{
   list.info->current = list.info->post_element.previous->previous->next;
   return list.info->current->pointer;
}

/****************************************************************************/

static void *next_in_list(Generic_list list)
{
   list.info->current = list.info->current->next;
   return list.info->current->pointer;
}

/****************************************************************************/

static void *previous_in_list(Generic_list list)
{
   list.info->current = list.info->current->previous;
   return list.info->current->pointer;
}

/****************************************************************************/

static void reset_to_beginning(Generic_list list)
{
   list.info->current = &list.info->pre_element;
}

/****************************************************************************/
/*
static void reset_to_end(Generic_list list)
{
    list.info->current = &list.info->post_element;
}
*/
/****************************************************************************/

static int num_of_objects(const Generic_list list)
{
   return list.info->num_of_elements;
}

/****************************************************************************/

static int is_empty(const Generic_list list)
{
   return (list.info->num_of_elements == 0);
}

/****************************************************************************/

static int is_in_list(const Generic_list list, const void *pointer)
{
   Generic_list_element *element;

   element = list.info->pre_element.next;

   while (element != &list.info->post_element && element->pointer != pointer)
      element = element->next;

   return (element != &list.info->post_element);
}

/****************************************************************************/

static Generic_list copy_list(Generic_list list)
{
   Generic_list list_copy;
   Generic_list_element *element;

   initialize_sorted_list(&list_copy, list.info->lt);
   element = list.info->pre_element.next;
   while (element != &list.info->post_element) {
      add_to_end(list_copy, element->pointer);
      element = element->next;
   }

   return list_copy;
}

/****************************************************************************/
/*
static void
perform_on_list(Generic_list list, void (*fn) (void *pointer, void *args),
                void *args)
{
    Generic_list_element *element;

    element = list.info->pre_element.next;
    while (element != &list.info->post_element) {
        (*fn) (element->pointer, args);
        element = element->next;
    }
}
*/
/****************************************************************************/
/*
static void *first_that(Generic_list list,
                        int (*fn) (const void *pointer, const void *args),
                        const void *args)
{
    Generic_list_element *element;

    element = list.info->pre_element.next;
    while (element != &list.info->post_element &&
           !(*fn) (element->pointer, args)) {
        element = element->next;
    }

    if (element->pointer)
        list.info->current = element;

    return element->pointer;
}
*/
/****************************************************************************/
/*
static void *next_that(Generic_list list,
                       int (*fn) (const void *pointer, const void *args),
                       const void *args)
{
    Generic_list_element *element;

    element = list.info->current->next;
    while (element != &list.info->post_element &&
           !(*fn) (element->pointer, args)) {
        element = element->next;
    }

    if (element->pointer)
        list.info->current = element;

    return element->pointer;
}
*/
/****************************************************************************/
/*
static void *previous_that(Generic_list list,
                           int (*fn) (const void *pointer,
                                      const void *args), const void *args)
{
    Generic_list_element *element;

    element = list.info->current->previous;
    while (element != &list.info->pre_element &&
           !(*fn) (element->pointer, args)) {
        element = element->previous;
    }

    if (element->pointer)
        list.info->current = element;

    return element->pointer;
}
*/
/****************************************************************************/
/*
static void *last_that(Generic_list list,
                       int (*fn) (const void *pointer, const void *args),
                       const void *args)
{
    Generic_list_element *element;

    element = list.info->post_element.previous;
    while (element != &list.info->pre_element &&
           !(*fn) (element->pointer, args)) {
        element = element->previous;
    }

    if (element->pointer)
        list.info->current = element;

    return element->pointer;
}
*/
/****************************************************************************/
/*
static Generic_list
all_such_that(Generic_list list,
              int (*fn) (const void *pointer, const void *args),
              const void *args)
{
    Generic_list list_copy;
    Generic_list_element *element;

    initialize_sorted_list(&list_copy, list.info->lt);
    element = list.info->pre_element.next;
    while (element != &list.info->post_element) {
        if ((*fn) (element->pointer, args))
            add_to_end(list_copy, element->pointer);
        element = element->next;
    }

    return list_copy;
}
*/
/****************************************************************************/
/*
static void
remove_all_such_that(Generic_list list,
                     int (*fn) (const void *pointer, const void *args),
                     const void *args)
{
    void *obj;

    reset_to_beginning(list);
    while ((obj = next_in_list(list)))
        if ((*fn) (obj, args))
            remove_current(list);
}
*/


/****************************************************************************/
/****************************************************************************/
/**                                                                        **/
/**                         Internal functions                             **/
/**                                                                        **/
/****************************************************************************/
/****************************************************************************/

static void *emalloc(unsigned int n)
{
   void *ptr;

   ptr = (void *) malloc(n);
   if (ptr == NULL) {
      mlogf(M_ERROR,M_SHOW, "%s: error allocating memory\n", module);
      exit(EXIT_FAILURE);
   }
   return ptr;
}


static void listRelease(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   destroy_list(&l);
   memUnlinkEncObj(ul->mem_state);
   free(ul);
}

static UtilList *listClone(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   UtilList *nul = NEW(UtilList);
   *nul = *ul;
   nul->hdl = copy_list(l).info;
   return nul;
}

static void listClear(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   remove_all(l);
}

static unsigned long listSize(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return num_of_objects(l);
}

static int listIsEmpty(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return is_empty(l);
}

static int listContains(UtilList * ul, const void *elm)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return is_in_list(l, elm);
}

static void listAppend(UtilList * ul, const void *elm)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   add_to_end(l, (void *) elm);
}

static void listPrepend(UtilList * ul, const void *elm)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   add_to_beginning(l, (void *) elm);
}

static void listAdd(UtilList * ul, const void *elm)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   add_to_beginning(l, (void *) elm);
}

static void *listGetFirst(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   reset_to_beginning(l);
   return first_in_list(l);
}

static void *listGetLast(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return last_in_list(l);
}

static void *listGetNext(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return next_in_list(l);
}

static void *listGetPrevious(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return previous_in_list(l);
}

static void *listGetCurrent(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return current_in_list(l);
}

static void *listRemoveFirst(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return remove_from_beginning(l);
}

static void *listRemoveLast(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return remove_from_end(l);
}

static void *listRemoveCurrent(UtilList * ul)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return remove_current(l);
}

static void *listRemoveThis(UtilList * ul, void *elm)
{
   Generic_list l = *(Generic_list *) & ul->hdl;
   return remove_from_list(l, elm);
}


Util_List_FT UtilList_ft = {
   1,
   listRelease,
   listClone,
   listClear,
   listSize,
   listIsEmpty,
   listContains,
   listAppend,
   listPrepend,
   listAdd,
   listGetFirst,
   listGetLast,
   listGetNext,
   listGetPrevious,
   listGetCurrent,
   listRemoveFirst,
   listRemoveLast,
   listRemoveCurrent,
   listRemoveThis
};

Util_List_FT *UtilListFT = &UtilList_ft;


UtilList *newList()
{
   UtilList ul,*tUl;
   int state;
   
   ul.ft = UtilListFT;
   initialize_list((Generic_list *) & ul.hdl);
   tUl=memAddEncObj(MEM_NOT_TRACKED, &ul, sizeof(ul),&state);
   tUl->mem_state=state;

   return tUl;
}
