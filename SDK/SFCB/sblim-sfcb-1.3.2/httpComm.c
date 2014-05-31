
/*
 * htppCommon.c
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
 * http common (http(https) support
 *
*/


#include "cmpidt.h"
#include "httpComm.h"

#if defined USE_SSL 
void handleSSLerror(const char *file, int lineno, const char *msg)
{
    mlogf(M_ERROR,M_SHOW,"\n*** %s:%i %s -- exiting\n", file, lineno, msg);
#ifdef SFCB_DEBUG
    ERR_print_errors_fp(stderr);
#endif
    exit(-1);
}
#endif


extern int sfcbSSLMode;

void commInit()
{
   _SFCB_ENTER(TRACE_HTTPDAEMON, "commInit");

#if defined USE_SSL
   if (sfcbSSLMode) {
      _SFCB_TRACE(1,("--- SSL mode"));
      if (!SSL_library_init()) {
	mlogf(M_ERROR,M_SHOW,"** OpenSSL initialization failed!\n");
	exit(-1);
      }
      SSL_load_error_strings();
      RAND_load_file("/dev/urandom", 1024);
   }
#endif

   _SFCB_EXIT();
}

int commWrite(CommHndl to, void *data, size_t count)
{
   int rc=0;

   _SFCB_ENTER(TRACE_HTTPDAEMON | TRACE_XMLOUT, "commWrite");

#ifdef SFCB_DEBUG
  if ((_sfcb_trace_mask & TRACE_XMLOUT) ) {
     char *mp,*m=alloca(count*2),*d=(char*)data;
     int i;
     _SFCB_TRACE(1,("->> xmlOut %d bytes:\n",count));
     for (mp=m,i=0; i<count; i++)
        switch (d[i]) {
        case '\r': *mp++='\\'; *mp++='r'; break;
        case '\n': *mp++='\\'; *mp++='n'; break;
        case ' ' : *mp++='~'; break;
        default:   *mp++=d[i];
     } 
     *mp=0;  
     _SFCB_TRACE(1,("%s\n",m));
     _SFCB_TRACE(1,("-<< xmlOut end\n"));
  }
#endif 
  
#if defined USE_SSL
  if (to.bio) {
    rc = BIO_write(to.bio, data, count);
  } else if (to.ssl) {
      rc = SSL_write(to.ssl, data, count);
  } else
#endif
     if (to.file == NULL) { 
       rc = write(to.socket,data,count);
     } else {
       rc = fwrite(data,count,1,to.file);
       if (rc == 1) {
	 /* return number of bytes written */
	 rc = count;
       }
     }

   _SFCB_RETURN(rc);
}

int commRead(CommHndl from, void *data, size_t count)
{
   int rc=0;

   _SFCB_ENTER(TRACE_HTTPDAEMON, "commRead");

#if defined USE_SSL
   if (from.ssl) {
     rc = SSL_read(from.ssl, data, count);
   } else
#endif
      rc = read(from.socket,data,count);

   _SFCB_RETURN(rc);
}

void commFlush(CommHndl hndl)
{
  _SFCB_ENTER(TRACE_HTTPDAEMON, "commFlush");

#if defined USE_SSL
   if (hndl.bio) {
     BIO_flush(hndl.bio);
   } else
#endif     
     if (hndl.file) {
       fflush(hndl.file);
     }
  _SFCB_EXIT();
}

void commClose(CommHndl hndl)
{
  
  _SFCB_ENTER(TRACE_HTTPDAEMON, "commClose");
#if defined USE_SSL
  if (hndl.ssl) {
    if ((SSL_get_shutdown(hndl.ssl) & SSL_RECEIVED_SHUTDOWN))
      SSL_shutdown(hndl.ssl);
    else SSL_clear(hndl.ssl);
    SSL_free(hndl.ssl);
  } else 
#endif
    if (hndl.file == NULL) {
      close(hndl.socket);
    } else {
      fclose(hndl.file);
      if (hndl.buf) {
	free(hndl.buf);
      }
    }
  _SFCB_EXIT();
}
