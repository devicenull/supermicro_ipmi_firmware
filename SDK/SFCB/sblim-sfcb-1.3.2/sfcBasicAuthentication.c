
/*
 * sfcBasicAuthentication.c
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
 * Basic Authentication exit.
 *
*/


#include <stdio.h>
#include <string.h>

extern int _sfcBasicAuthenticate(char *user, char *pw)
{
   printf("-#- Authentication request for %s\n",user);
   return 0;
   /* - for security reasons always return false -
   if (strcmp(user,"REJECT")==0) return 0;
   return 1;
   */
}
