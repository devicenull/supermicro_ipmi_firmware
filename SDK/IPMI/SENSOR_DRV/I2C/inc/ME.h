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
/***************************************************************************** 
 *  $RCSfile: ME.h,v $
 *  $Revision: 1.0 $
 *  $Date: 2008/07/08 12:54:21 $
 *  $Author: dean $
 *  $Source: /home/cvsroot/SM_SRC/IPMI/SENSOR_DRV/I2C/inc/ME.h,v $
 *****************************************************************************/

#ifndef _ME_H_
#define _ME_H_


extern STATUS SetupME(DWORD , DWORD, void *, void *);
extern STATUS PryME(DWORD , DWORD, void *, void *);
extern STATUS WriteME(DWORD , DWORD, void *, void *);
extern STATUS ReadME(DWORD , DWORD, void *, void *);


/* struct ME API */
#ifdef _ME_C_
St_SensorAPITag at_St_ME_API =
{
    SetupME,
    PryME,
    WriteME,
    ReadME,
};

#else
extern int GetSELTimeFromME(unsigned long int *);
extern St_SensorAPITag at_St_ME_AP;

#endif 


#endif //End of "_ME_H_"
