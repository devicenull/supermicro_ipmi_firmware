/*
 * Dropbear - a SSH2 server
 * 
 * Copyright (c) 2002,2003 Matt Johnston
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

/* Validates a user password */

#include "includes.h"
#include "session.h"
#include "buffer.h"
#include "dbutil.h"
#include "auth.h"
#include "runopts.h"
#include "PltConfig.h"
#include "status.h"
#include "type.h"
#include "OS_api.h"
#include "gparam.h"
#include "msgbuf.h"
#include "ps.h"

#if defined(ENABLE_OWN_AST_LDAP) || defined(ENABLE_ACTIVE_DIRECTORY)
extern unsigned char AuthLdapProcess(char *usr_name, char *usr_pwd);
#endif

extern BYTE UtilAuthUser(BYTE * a_b_Name, BYTE * a_b_Password);


#ifdef ENABLE_SVR_PASSWORD_AUTH

static int constant_time_strcmp(const char* a, const char* b) {
	size_t la = strlen(a);
	size_t lb = strlen(b);

	if (la != lb) {
		return 1;
	}

	return constant_time_memcmp(a, b, la);
}

/* Process a password auth request, sending success or failure messages as
 * appropriate */
void svr_auth_password() {
	
	char * passwdcrypt = NULL; /* the crypt from /etc/passwd or /etc/shadow */
	char * testcrypt = NULL; /* crypt generated from the user's password sent */
	unsigned char * password;
	unsigned int passwordlen;

	unsigned int changepw;
	int 		 priv = 0x0F;
	int 		 user_idx =	MAX_USER_NUM;

	passwdcrypt = ses.authstate.pw_passwd;

#ifdef DEBUG_HACKCRYPT
	/* debugging crypt for non-root testing with shadows */
	passwdcrypt = DEBUG_HACKCRYPT;
#endif

	/* check if client wants to change password */
	changepw = buf_getbool(ses.payload);
	if (changepw) {
		/* not implemented by this server */
		send_msg_userauth_failure(0, 1);
		return;
	}

	password = buf_getstring(ses.payload, &passwordlen);

	/* the first bytes of passwdcrypt are the salt */
//	testcrypt = crypt((char*)password, passwdcrypt);
//	m_burn(password, passwordlen);
//	m_free(password);
	user_idx = UtilAuthUser(ses.authstate.username, password);
	if ( MAX_USER_NUM != user_idx ) {

	ses.authstate.user_priv = UtilGetLANPriv(user_idx);
		dropbear_log(LOG_NOTICE, 
				"password auth succeeded for '%s' from %s",
				ses.authstate.pw_name,
				svr_ses.addrstring);
		send_msg_userauth_success();
	} else {
		
#ifdef ENABLE_OWN_AST_LDAP
		if (at_St_PS.a_St_LdapInfo.b_LdapEnable)
    		priv = AuthLdapProcess(ses.authstate.username, password);
#endif
#ifdef ENABLE_ACTIVE_DIRECTORY
    	if (at_St_PS.a_St_ActiveDirectoryInfo.b_ActiveDirectoryEnable)
    		priv = AuthLdapProcess(ses.authstate.username, password);
#endif	
#ifdef ENABLE_AUTH_RADIUS
    if (at_St_PS.a_St_RadiusInfo.b_RadiusEnable && 0x0F == priv)
    	priv = AuthRadiusProcess(ses.authstate.username, password);
#endif
		if (priv >= 1 && priv <= 4)
		{
			dropbear_log(LOG_NOTICE, 
				"password auth succeeded for '%s' from %s",
				ses.authstate.pw_name,
				svr_ses.addrstring);
			send_msg_userauth_success(); 
			ses.authstate.user_priv = priv;
		}
		else 
		{
			dropbear_log(LOG_WARNING,
				"bad password attempt for '%s' from %s",
				ses.authstate.pw_name,
				svr_ses.addrstring);
			send_msg_userauth_failure(0, 1);
		}
	}
//	if (testcrypt == NULL) {
//		/* crypt() with an invalid salt like "!!" */
//		dropbear_log(LOG_WARNING, "User account '%s' is locked",
//				ses.authstate.pw_name);
//		send_msg_userauth_failure(0, 1);
//		return;
//	}

	/* check for empty password */
//	if (passwdcrypt[0] == '\0') {
//		dropbear_log(LOG_WARNING, "User '%s' has blank password, rejected",
//		send_msg_userauth_failure(0, 1);
//		return;
//	}

//	if (constant_time_strcmp(testcrypt, passwdcrypt) == 0) {
//		/* successful authentication */
//		dropbear_log(LOG_NOTICE, 
//				"Password auth succeeded for '%s' from %s",
//				ses.authstate.pw_name,
//				svr_ses.addrstring);
//		send_msg_userauth_success();
//	} else {
//		dropbear_log(LOG_WARNING,
//				ses.authstate.pw_name,
//				svr_ses.addrstring);
//		send_msg_userauth_failure(0, 1);
//	}
}

#endif
