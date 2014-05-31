
/*
 * utilStringBuffer.c
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
 * Contributions: Sven Schuetz <sven@de.ibm.com>
 *
 * Description:
 *
 * string buffer utility implementation.
 *
*/



#include "utilft.h"
//#include "native.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static void sbft_release(UtilStringBuffer * sb)
{
   if (sb->hdl)
      free(sb->hdl);
   free(sb);
}

static UtilStringBuffer *sbft_clone(UtilStringBuffer * sb)
{
   UtilStringBuffer *nsb =
       (UtilStringBuffer *) malloc(sizeof(UtilStringBuffer));
   *nsb = *sb;
   if (sb->hdl)
      nsb->hdl = strdup(sb->hdl);
   nsb->max = nsb->len = sb->len;
   return nsb;
}

static const char *sbft_getCharPtr(UtilStringBuffer * sb)
{
   return (const char *) sb->hdl;
}

static unsigned int sbft_getSize(UtilStringBuffer * sb)
{
   return (unsigned int) sb->len;
}

static void sbft_appendChars(UtilStringBuffer * sb, const char *chars)
{
   int sl;
   char *ns;

   if (chars == NULL)
      return;
   if ((sl = strlen(chars)) + sb->len + 1 >= sb->max) {
      if (sb->max == 0)
         sb->max = 8;
      while (sl + sb->len + 1 >= sb->max)
         sb->max *= 2;
      ns = (char *) realloc(sb->hdl, sb->max + 2);
      sb->hdl = ns;
   }
   memcpy(((char *) sb->hdl) + sb->len, chars, sl + 1);
   sb->len += sl;
}

static void sbft_appendBlock(UtilStringBuffer * sb, void *data, unsigned int size)
{
   int sl;
   char *ns;

   if (data == NULL) return;
   if ((sl = size) + sb->len + 1 >= sb->max) {
      if (sb->max == 0) sb->max = 8;
      while (sl + sb->len + 1 >= sb->max) sb->max *= 2;
      ns = (char *) realloc(sb->hdl, sb->max + 2);
      sb->hdl = ns;
   }
   memcpy(((char *) sb->hdl) + sb->len, data, sl);
   sb->len += sl;
   ((char*)sb->hdl)[sb->len]=0;
}

static void sbft_append6Chars(UtilStringBuffer * sb, const char *chars1, const char *chars2, 
   const char *chars3, const char *chars4, const char *chars5, const char *chars6)
{
   sbft_appendChars(sb,chars1);  
   sbft_appendChars(sb,chars2);  
   sbft_appendChars(sb,chars3);  
   sbft_appendChars(sb,chars4);  
   sbft_appendChars(sb,chars5);  
   sbft_appendChars(sb,chars6);  
}
         
static void sbft_append5Chars(UtilStringBuffer * sb, const char *chars1, const char *chars2, 
   const char *chars3, const char *chars4, const char *chars5)
{
   sbft_appendChars(sb,chars1);  
   sbft_appendChars(sb,chars2);  
   sbft_appendChars(sb,chars3);  
   sbft_appendChars(sb,chars4);  
   sbft_appendChars(sb,chars5);  
}
         
static void sbft_append3Chars(UtilStringBuffer * sb, const char *chars1, const char *chars2, const char *chars3)
{
   sbft_appendChars(sb,chars1);  
   sbft_appendChars(sb,chars2);  
   sbft_appendChars(sb,chars3);  
}


/*
 * 
 * removed to create a common util library which can be used by
 * sfcc as well.
 * Prereq are no external headers, which CMPIString would require ...
 * as appendString is apparantly not used - removed
 * 
 * static void sbft_appendString(UtilStringBuffer * sb, CMPIString * string)
{
   sbft_appendChars(sb, (const char *) string->hdl);
}
*/


static void sbft_reset(UtilStringBuffer * sb)
{
   if (sb->max)
      *((char *) sb->hdl) = 0;
   sb->len = 0;
}


UtilStringBuffer *newStringBuffer(int s)
{
   static Util_StringBuffer_FT sbft = {
      UTIL_FT_VERSION,
      sbft_release,
      sbft_clone,
      sbft_getCharPtr,
      sbft_getSize,
      sbft_appendChars,
//      sbft_appendString,
      sbft_reset,
      sbft_appendBlock,
      sbft_append3Chars,
      sbft_append5Chars,
      sbft_append6Chars      
   };

   UtilStringBuffer *sb = (UtilStringBuffer *) malloc(sizeof(UtilStringBuffer));

   if (s == 0)
      s = 32;
   sb->hdl = malloc(s);
   *((char *) sb->hdl) = 0;
   sb->ft = &sbft;
   sb->max = s;
   sb->len = 0;

   return sb;
}
