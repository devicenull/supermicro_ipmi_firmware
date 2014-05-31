
/*
 * $Id: sfcBasicPAMAuthentication.c,v 1.1 2007/02/15 14:07:22 mihajlov Exp $
 *
 * © Copyright IBM Corp. 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Viktor Mihajlovski <mihajlov@de.ibm.com>
 *
 * Description:
 *
 * Basic Authentication exit implemented via PAM.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>


#define SFCB_PAM_APP "sfcb"

static int sfcBasicConv(int num_msg, const struct pam_message **msg,
			struct pam_response **resp, void *credentials)
{
  if (num_msg == 1 && msg && resp && credentials) {
    /* we just understand one message */
    (*resp) = calloc(num_msg,sizeof(struct pam_response));
    (*resp)->resp_retcode = 0;
    (*resp)->resp = NULL;
    switch (msg[0]->msg_style) {
    case PAM_PROMPT_ECHO_OFF:
    case PAM_PROMPT_ECHO_ON:
      (*resp)->resp = strdup((char*) credentials);
      break;
    default:
      break;
    }
    return PAM_SUCCESS;
  }
  return PAM_CONV_ERR;
}


static int _sfcBasicAuthenticateRemote(char *user, char *pw, char *rhost)
{
  struct pam_conv sfcConvStruct = {
    sfcBasicConv,
    pw
  };
  pam_handle_t *pamh = NULL;
  int rc, retval;
   
  
  rc = pam_start(SFCB_PAM_APP, user, &sfcConvStruct, & pamh);

  if (rc == PAM_SUCCESS) {
    rc = pam_authenticate(pamh, PAM_SILENT);
  }

  if (rc == PAM_SUCCESS) {
    /* host based authentication not yet supported 
       - needs client IP address extraction in httpAdapter
    */
    if (rhost) {
      pam_set_item(pamh, PAM_RHOST, rhost);
    }
    rc = pam_acct_mgmt(pamh, PAM_SILENT);
  }
  
  if (rc == PAM_SUCCESS) {
    retval = 1;
  } else {
    retval = 0;
  }
  
  pam_end(pamh, rc);

  return retval;
}

int _sfcBasicAuthenticate(char *user, char *pw)
{
  return _sfcBasicAuthenticateRemote(user, pw, NULL);
}
