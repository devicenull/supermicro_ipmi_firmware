/*****************************************************************************/
/*                                                                           */
/*            Copyright (c) 2002-2003 by ATEN International CO., Ltd.        */
/*                                                                           */
/*  This software is supplied under the terms of a license agreement or      */
/*  nondisclosure agreement with ATEN International CO., Ltd. and do not     */
/*  copy, disseminate or distribute except in accordance with the terms      */
/*  of that agreement.                                                       */
/*                                                                           */
/*  ATEN International CO., Ltd. reserves the right to modify this software  */
/*  without notice.                                                          */
/*                                                                           */
/*  ATEN International CO., Ltd.                                             */
/*  3F, No. 125, Sec. 2, Datung Rd.                                          */
/*  Shi-Chi City                              Taiwan phone: 886-2-8692-6789  */
/*  Taipei Hsien, Taiwan 221, R.O.C.          email: ip_manager@aten.com.tw  */
/*                                                                           */
/*****************************************************************************/

#ifndef _HDDDRV_H_
#define _HDDDRV_H_



extern STATUS SetupHDDDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS PryHDDDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteHDDDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadHDDDRV (DWORD ,DWORD ,void * ,void *);

#ifdef _HDDDRV_C_

/* struct HDD API */
St_SensorAPITag ws_St_HDDAPI=
{
	SetupHDDDRV,
	PryHDDDRV,
	WriteHDDDRV,
	ReadHDDDRV,
};


#else

/* struct HDD API */

extern St_SensorAPITag ws_St_HDDAPI;


#endif //end of #ifdef _HDDDRV_C_

#endif //end of #define _HDDDRV_H_

/* End of file */