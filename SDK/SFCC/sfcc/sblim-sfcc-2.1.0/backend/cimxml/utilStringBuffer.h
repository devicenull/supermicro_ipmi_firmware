   

/*
 * utilStringBuffer.h
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
 *
*/

#ifndef UTILStringBuffer 
#define UTILStringBuffer 
 
   
   typedef struct _Util_StringBuffer_FT Util_StringBuffer_FT;
   struct _UtilStringBuffer {
      void *hdl;
      Util_StringBuffer_FT *ft;
      int max, len;
   };
   typedef struct _UtilStringBuffer UtilStringBuffer;

   struct _Util_StringBuffer_FT {
      int version;
      void (*release) (UtilStringBuffer * sb);
      UtilStringBuffer *(*clone) (UtilStringBuffer * sb);
      const char *(*getCharPtr) (UtilStringBuffer * sb);
      unsigned int (*getSize) (UtilStringBuffer * sb);
      void (*appendChars) (UtilStringBuffer * sb, const char *chars);
      void (*appendString) (UtilStringBuffer * sb, CMPIString * string);
      void (*reset) (UtilStringBuffer * sb);
      void (*appendBlock) (UtilStringBuffer * sb, void *, unsigned int size);
      void (*append3Chars) (UtilStringBuffer * sb, const char *chars1, const char *chars2,
         const char *chars3);
      void (*append5Chars) (UtilStringBuffer * sb, const char *chars1, const char *chars2,
         const char *chars3, const char *chars4, const char *chars5);
      void (*append6Chars) (UtilStringBuffer * sb, const char *chars1, const char *chars2,
         const char *chars3, const char *chars4, const char *chars5, const char *chars6);
   };

UtilStringBuffer *newStringBuffer(int s);

#endif
