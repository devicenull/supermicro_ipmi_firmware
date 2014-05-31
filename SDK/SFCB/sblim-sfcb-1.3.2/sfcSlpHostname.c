
/*
 * sfcSlpHostname.c
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
 * Author:        Tyrel Datwyler <tyreld@us.ibm.com>
 *
 * Description:
 *
 * Obtain custom hostname string to register with SLP DA
 * This is only a sample of how to write the custom routine
 * used to provide the hostname. You will need to replace this
 * with a routine that uses the desired method to obtain the 
 * proper value. 
 *
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int _sfcGetSlpHostname(char **hostname)
{
   char *sn;
   sn = (char *) malloc((strlen("mycimom.com") + 1) * sizeof(char));
   sn = strncpy(sn, "mycimom.com", strlen("mycimom.com") + 1);
   if (sn == NULL)
      return 0;
   
   printf("-#- Request for custom SLP service hostname: (hostname = %s)\n", sn);
   *hostname = sn;

   /* Return value of 1 for successs and 0 for failure. */
   return 1;
}

