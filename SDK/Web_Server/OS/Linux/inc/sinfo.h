#ifndef _SINFO_H_
#define _SINFO_H_

#include <time.h>

#include "STypeDef.h"




typedef struct _baseinfo{
	  UWORD t_out;
	  UBYTE u_index;
	  UBYTE no_sess_refresh; 
	  UBYTE no_cgi_output; 
	  char  cgi_name[64]; 
	  char  sess_sid[32];
	  char  ab_hash_key[20];
}BaseInfo;

#endif




