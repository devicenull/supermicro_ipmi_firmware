#ifndef __vusbhs_h__
#define __vusbhs_h__ 1
/*******************************************************************************
** File          : $HeadURL$ 
** Author        : $Author: achechik $
** Project       : HSCTRL 
** Instances     : 
** Creation date : 
********************************************************************************
********************************************************************************
** ChipIdea Microelectronica - IPCS
** TECMAIA, Rua Eng. Frederico Ulrich, n 2650
** 4470-920 MOREIRA MAIA
** Portugal
** Tel: +351 229471010
** Fax: +351 229471011
** e_mail: chipidea.com
********************************************************************************
** ISO 9001:2000 - Certified Company
** (C) 2005 Copyright Chipidea(R)
** Chipidea(R) - Microelectronica, S.A. reserves the right to make changes to
** the information contained herein without notice. No liability shall be
** incurred as a result of its use or application.
********************************************************************************
** Modification history:
** $Date: 2008/02/14 10:37:26Z $
** $Revision: 1.3 $
*******************************************************************************
*** Description:      
***  This file contains the defines, externs and
***  data structure definitions required by the VUSB_HS Device
***  driver.
***                                                               
**************************************************************************
**END*********************************************************/
#include <linux/types.h>

/* VUSBHS specific defines */
#define  VUSBHS_MAX_PORTS                          (8)
#define  EHCI_CAP_LEN_MASK                         (0x000000FF)
#define  EHCI_DATA_STRUCTURE_BASE_ADDRESS          (0)

/* Command Register Bit Masks */
#define  EHCI_CMD_RUN_STOP                         (0x00000001)
#define  EHCI_CMD_CTRL_RESET                       (0x00000002)
#define  EHCI_CMD_SETUP_TRIPWIRE_SET               (0x00002000)
#define  EHCI_CMD_SETUP_TRIPWIRE_CLEAR             ~EHCI_CMD_SETUP_TRIPWIRE_SET

/* Bit 14 is the tripwire bit not bit 12 as some versions of the docs state. */
#define  EHCI_CMD_ATDTW_TRIPWIRE_SET               (0x00004000)
#define  EHCI_CMD_ATDTW_TRIPWIRE_CLEAR             ~EHCI_CMD_ATDTW_TRIPWIRE_SET

/*bit 15,3,2 are for frame list size */
#define  EHCI_CMD_FRAME_SIZE_1024                  (0x00000000) /* 000 */
#define  EHCI_CMD_FRAME_SIZE_512                   (0x00000004) /* 001 */
#define  EHCI_CMD_FRAME_SIZE_256                   (0x00000008) /* 010 */
#define  EHCI_CMD_FRAME_SIZE_128                   (0x0000000C) /* 011 */
#define  EHCI_CMD_FRAME_SIZE_64                    (0x00008000) /* 100 */
#define  EHCI_CMD_FRAME_SIZE_32                    (0x00008004) /* 101 */
#define  EHCI_CMD_FRAME_SIZE_16                    (0x00008008) /* 110 */
#define  EHCI_CMD_FRAME_SIZE_8                     (0x0000800C) /* 111 */


/* Hardware Rev 4.0 related change */                                 
#ifdef STREAM_ENABLE

/* Mode Register Bit Masks */
#define  VUSBHS_MODE_CTRL_MODE_IDLE                (0x00000010)
#define  VUSBHS_MODE_CTRL_MODE_DEV                 (0x00000012)
#define  VUSBHS_MODE_CTRL_MODE_HOST                (0x00000013)
#define  VUSBHS_MODE_BIG_ENDIAN                    (0x00000014)
#define  VUSBHS_MODE_SETUP_LOCK_DISABLE            (0x00000008)

#else

/* Mode Register Bit Masks */
#define  VUSBHS_MODE_CTRL_MODE_IDLE                (0x00000000)
#define  VUSBHS_MODE_CTRL_MODE_DEV                 (0x00000002)
#define  VUSBHS_MODE_CTRL_MODE_HOST                (0x00000003)
#define  VUSBHS_MODE_BIG_ENDIAN                    (0x00000004)
#define  VUSBHS_MODE_SETUP_LOCK_DISABLE            (0x00000008)

#endif
                                                
/* Interrupt Enable Register Bit Masks */
#define  EHCI_INTR_INT_EN                          (0x00000001)
#define  EHCI_INTR_ERR_INT_EN                      (0x00000002)
#define  EHCI_INTR_PORT_CHANGE_DETECT_EN           (0x00000004)

#define  EHCI_INTR_ASYNC_ADV_AAE                   (0x00000020)
#define  EHCI_INTR_ASYNC_ADV_AAE_ENABLE            (0x00000020) /* | with this to enable */
#define  EHCI_INTR_ASYNC_ADV_AAE_DISABLE           (0xFFFFFFDF) /* & with this to disable */

#define  EHCI_INTR_RESET_EN                        (0x00000040)
#define  EHCI_INTR_SOF_UFRAME_EN                   (0x00000080)
#define  EHCI_INTR_DEVICE_SUSPEND                  (0x00000100)

/* Interrupt Status Register Masks */
#define  EHCI_STS_SOF                              (0x00000080)
#define  EHCI_STS_RESET                            (0x00000040)
#define  EHCI_STS_PORT_CHANGE                      (0x00000004)
#define  EHCI_STS_ERR                              (0x00000002)
#define  EHCI_STS_INT                              (0x00000001)
#define  EHCI_STS_SUSPEND                          (0x00000100)
#define  EHCI_STS_HC_HALTED                        (0x00001000)

/* Endpoint Queue Head Bit Masks */
#define  VUSB_EP_QUEUE_HEAD_IOS                    (0x00008000)
#define  VUSB_EP_QUEUE_HEAD_IOC                    (0x00008000)
#define  VUSB_EP_QUEUE_HEAD_INT                    (0x00000100)
#define  VUSB_EP_QUEUE_HEAD_NEXT_TERMINATE         (0x00000001)
#define  VUSB_EP_QUEUE_HEAD_MAX_PKT_LEN_POS        (16)
#define  VUSB_EP_QUEUE_HEAD_ZERO_LEN_TER_SEL       (0x20000000)
#define  VUSB_EP_QUEUE_HEAD_MULT_POS               (30)
#define  VUSB_EP_MAX_LENGTH_TRANSFER               (0x4000)

#define  VUSB_EP_QUEUE_HEAD_STATUS_ACTIVE          (0x00000080)

#define  VUSBHS_TD_NEXT_TERMINATE                  (0x00000001)
#define  VUSBHS_TD_IOC                             (0x00008000)
#define  VUSBHS_TD_STATUS_ACTIVE                   (0x00000080)
#define  VUSBHS_TD_STATUS_HALTED                   (0x00000040)
#define  VUSBHS_TD_RESERVED_FIELDS                 (0x00007F00)
#define  VUSBHS_TD_ERROR_MASK                      (0x68)
#define  VUSBHS_TD_ADDR_MASK                       (0xFFFFFFE0)
#define  VUSBHS_TD_LENGTH_BIT_POS                  (16)

#define  EHCI_EPCTRL_TX_DATA_TOGGLE_RST            (0x00400000)
#define  EHCI_EPCTRL_TX_EP_STALL                   (0x00010000)
#define  EHCI_EPCTRL_RX_EP_STALL                   (0x00000001)
#define  EHCI_EPCTRL_RX_DATA_TOGGLE_RST            (0x00000040)
#define  EHCI_EPCTRL_RX_ENABLE                     (0x00000080)
#define  EHCI_EPCTRL_TX_ENABLE                     (0x00800000)
#define  EHCI_EPCTRL_CONTROL                       (0x00000000)
#define  EHCI_EPCTRL_ISOCHRONOUS                   (0x00040000)
#define  EHCI_EPCTRL_BULK                          (0x00080000)
#define  EHCI_EPCTRL_INT                           (0x000C0000)

#define  EHCI_EPCTRL_TX_TYPE                       (0x000C0000)
#define  EHCI_EPCTRL_RX_TYPE                       (0x0000000C)

#define  EHCI_EPCTRL_DATA_TOGGLE_INHIBIT           (0x00000020)
#define  EHCI_EPCTRL_TX_EP_TYPE_SHIFT              (18)
#define  EHCI_EPCTRL_RX_EP_TYPE_SHIFT              (2)

#define  EHCI_PORTSCX_FORCE_FULL_SPEED_CONNECT     (0x01000000) // set bit 24 (PFSC) in PORTSCX register
#define  EHCI_PORTSCX_PHY_CLOCK_DISABLE            (0x00800000) // set bit 23 (PHCD) in PORTSCX register
#define  EHCI_PORTSCX_PORT_TEST_MODE_DISABLE_MASK  (0x000F0000)
#define  EHCI_PORTSCX_PORT_TEST_CONTROL_J_STATE    (0x00010000)
#define  EHCI_PORTSCX_PORT_TEST_CONTROL_K_STATE    (0x00020000)
#define  EHCI_PORTSCX_PORT_TEST_CONTROL_SE0_NAK    (0x00030000)
#define  EHCI_PORTSCX_PORT_TEST_CONTROL_PACKET     (0x00040000)
#define  EHCI_PORTSCX_PORT_TEST_CONTROL_FORCE_HS   (0x00050000)
#define  EHCI_PORTSCX_PORT_TEST_CONTROL_FORCE_FS   (0x00060000)
#define  EHCI_PORTSCX_PORT_TEST_CONTROL_FORCE_LS   (0x00070000)
#define  EHCI_PORTSCX_PORT_POWER                   (0x00001000)
#define  EHCI_PORTSCX_LINE_STATUS_BITS             (0x00000C00)
#define  EHCI_PORTSCX_LINE_STATUS_SE0              (0x00000000)
#define  EHCI_PORTSCX_LINE_STATUS_KSTATE           (0x00000400)
#define  EHCI_PORTSCX_LINE_STATUS_JSTATE           (0x00000800)
#define  EHCI_PORTSCX_PORT_HIGH_SPEED              (0x00000200)
#define  EHCI_PORTSCX_PORT_RESET                   (0x00000100)
#define  EHCI_PORTSCX_PORT_SUSPEND                 (0x00000080)
#define  EHCI_PORTSCX_PORT_FORCE_RESUME            (0x00000040)
#define  EHCI_PORTSCX_PORT_EN_DIS_CHANGE           (0x00000008)
#define  EHCI_PORTSCX_PORT_ENABLE                  (0x00000004)
#define  EHCI_PORTSCX_CONNECT_STATUS_CHANGE        (0x00000002)
#define  EHCI_PORTSCX_CURRENT_CONNECT_STATUS       (0x00000001)

#define  VUSBHS_PORTSCX_PORT_SPEED_FULL            (0x00000000)
#define  VUSBHS_PORTSCX_PORT_SPEED_LOW             (0x04000000)
#define  VUSBHS_PORTSCX_PORT_SPEED_HIGH            (0x08000000)
#define  VUSBHS_SPEED_MASK                         (0x0C000000)
#define  VUSBHS_SPEED_BIT_POS                      (26)

#define  EHCI_PORTSCX_W1C_BITS                     (0x2A)
#define  VUSB_EP_QH_PACKET_SIZE                    (0x3FFF0000)
#define  VUSB_EP_TR_PACKET_SIZE                    (0x7FFF0000)

#define  VUSBHS_FRINDEX_MS_MASK                    (0xFFFFFFF8)
#define  VUSBHS_ADDRESS_BIT_SHIFT                  (25)

#define  VUSB20_MAX_ENDPTS_SUPPORTED               (0x1F)
#define  EHCI_HCC_PARAMS_64_BIT_ADDR_CAP           (0x01)
#define  EHCI_HCC_PARAMS_PGM_FRM_LIST_FLAG         (0x02)
#define  EHCI_HCC_PARAMS_ASYNC_PARK_CAP            (0x04)
#define  EHCI_HCC_PARAMS_ISOCH_SCHED_THRESHOLD     (0xF0)
#define  EHCI_HCC_PARAMS_ISOCH_FRAME_CACHED        (0x80)

#define  VUSB20_HCS_PARAMS_PORT_POWER_CONTROL_FLAG (0x10)

#define  VUSB20_HOST_INTR_EN_BITS                  (0x37)

#define  VUSB20_DEFAULT_PERIODIC_FRAME_LIST_SIZE   (1024)
#define  VUSB20_NEW_PERIODIC_FRAME_LIST_BITS       (2)
#define  EHCI_FRAME_LIST_ELEMENT_POINTER_T_BIT     (0x01)
#define  EHCI_ITD_T_BIT                            (0x01)
#define  EHCI_SITD_T_BIT                           (0x01)
#define  EHCI_QUEUE_HEAD_POINTER_T_BIT             (0x01)


/************************************************************
Split transatcions specific defines
************************************************************/
#define  EHCI_START_SPLIT_MAX_BUDGET               188

#define  EHCI_ELEMENT_TYPE_ITD                     (0x00)
#define  EHCI_ELEMENT_TYPE_QH                      (0x02)
#define  EHCI_ELEMENT_TYPE_SITD                    (0x04)
#define  EHCI_ELEMENT_TYPE_FSTN                    (0x06)
#define  EHCI_ELEMENT_TYPE_MASK                    (0x06)

#define  EHCI_FRAME_LIST_ELEMENT_TYPE_ITD          (0x00)
#define  EHCI_FRAME_LIST_ELEMENT_TYPE_QH           (0x01)
#define  EHCI_FRAME_LIST_ELEMENT_TYPE_SITD         (0x02)
#define  EHCI_FRAME_LIST_ELEMENT_TYPE_FSTN         (0x03)
#define  EHCI_FRAME_LIST_ELEMENT_TYPE_BIT_POS      (1)


#define  EHCI_QH_ELEMENT_TYPE_ITD                  (0x00)
#define  EHCI_QH_ELEMENT_TYPE_QH                   (0x01)
#define  EHCI_QH_ELEMENT_TYPE_SITD                 (0x02)
#define  EHCI_QH_ELEMENT_TYPE_FSTN                 (0x03)

#define  EHCI_QH_ELEMENT_TYPE_BIT_POS              (1)

#define  EHCI_QTD_PID_OUT_TOKEN                    (0x000)
#define  EHCI_QTD_PID_IN_TOKEN                     (0x100)
#define  EHCI_QTD_PID_SETUP_TOKEN                  (0x200)
#define  EHCI_QTD_IOC                              (0x8000)
#define  EHCI_QTD_STATUS_ACTIVE                    (0x0080)
#define  EHCI_QTD_STATUS_HALTED                    (0x0040)
#define  EHCI_QTD_PID_SETUP                        (0x0200)
#define  EHCI_QTD_PID_IN                           (0x0100)
#define  EHCI_QTD_PID_OUT                          (0x0000)
#define  EHCI_QTD_LENGTH_BIT_POS                   (16)
#define  EHCI_QTD_DATA_TOGGLE                      (0x80000000)
#define  EHCI_QTD_DATA_TOGGLE_BIT_POS              (31)
#define  EHCI_QTD_LENGTH_BIT_MASK                  (0x7FFF0000)
#define  EHCI_QTD_ERROR_BITS_MASK                  (0x0000003E)
#define  EHCI_QTD_DEFAULT_CERR_VALUE               (0xC00)

#define  EHCI_SETUP_TOKEN                          (2)
#define  EHCI_OUT_TOKEN                            (0)
#define  EHCI_IN_TOKEN                             (1)

#define  EHCI_QTD_T_BIT                            (0x01)

#define  EHCI_QH_ENDPOINT_SPEED_FULL               (0x00)
#define  EHCI_QH_ENDPOINT_SPEED_LOW                (0x01)
#define  EHCI_QH_ENDPOINT_SPEED_HIGH               (0x02)
#define  EHCI_QH_ENDPOINT_SPEED_RESERVED           (0x03)

#define  EHCI_ITD_LENGTH_BIT_POS                   (16)
#define  EHCI_ITD_IOC_BIT                          (0x00008000)
#define  EHCI_ITD_ACTIVE_BIT                       (0x80000000)
#define  EHCI_ITD_PG_SELECT_BIT_POS                (12)
#define  EHCI_ITD_DIRECTION_BIT_POS                (11)
#define  EHCI_ITD_EP_BIT_POS                       (8)
#define  EHCI_ITD_STATUS                           (0xF0000000)
#define  EHCI_ITD_STATUS_ACTIVE                    (0x80000000) /*bit 4 = 1000*/
#define  EHCI_ITD_STATUS_DATA_BUFFER_ERR           (0x40000000) /*bit 3 = 0100*/
#define  EHCI_ITD_STATUS_BABBLE_ERROR              (0x20000000) /*bit 2 = 0010*/
#define  EHCI_ITD_STATUS_TRANSACTION_ERR           (0x10000000) /*bit 4 = 0001*/

#define  EHCI_ITD_LENGTH_TRANSMITTED               (0x0FFF0000)
#define  EHCI_ITD_BUFFER_OFFSET                    (0x00000FFF)
#define  EHCI_ITD_PAGE_NUMBER                      (0x00007000)
#define  EHCI_ITD_BUFFER_POINTER                   (0xFFFFF000)
#define  EHCI_ITD_MULTI_TRANSACTION_BITS           (0x00000003)

          

/* SITD position bits */
#define  EHCI_SITD_DIRECTION_BIT_POS               (31)
#define  EHCI_SITD_PORT_NUMBER_BIT_POS             (24)
#define  EHCI_SITD_HUB_ADDR_BIT_POS                (16)
#define  EHCI_SITD_EP_ADDR_BIT_POS                 (8)

#define  EHCI_SITD_COMPLETE_SPLIT_MASK_BIT_POS     (8)

#define  EHCI_SITD_IOC_BIT_SET                     (0x80000000)
#define  EHCI_SITD_PAGE_SELECT_BIT_POS             (30)
#define  EHCI_SITD_TRANSFER_LENGTH_BIT_POS         (16)
#define  EHCI_SITD_STATUS_ACTIVE                   (0x80)

#define  EHCI_SITD_STATUS                          (0xFF)
#define  EHCI_SITD_LENGTH_TRANSMITTED              (0x03FF0000)
#define  EHCI_SITD_BUFFER_OFFSET                   (0x00000FFF)
#define  EHCI_SITD_PAGE_NUMBER                     (0x40000000)
#define  EHCI_SITD_BUFFER_POINTER                  (0xFFFFF000)

#define  EHCI_SITD_BUFFER_PTR_BIT_POS              (12)
#define  EHCI_SITD_TP_BIT_POS                      (3)
#define  EHCI_SITD_TP_ALL                          (0)
#define  EHCI_SITD_TP_BEGIN                        (1)
#define  EHCI_SITD_TP_MID                          (2)
#define  EHCI_SITD_TP_END                          (3)

/* Interrupt enable bit masks */
#define  EHCI_IER_ASYNCH_ADVANCE                   (0x00000020)
#define  EHCI_IER_HOST_SYS_ERROR                   (0x00000010)
#define  EHCI_IER_FRAME_LIST_ROLLOVER              (0x00000008)
#define  EHCI_IER_PORT_CHANGE                      (0x00000004)
#define  EHCI_IER_USB_ERROR                        (0x00000002)
#define  EHCI_IER_USB_INTERRUPT                    (0x00000001)

/* Interrupt status bit masks */
#define  EHCI_STS_RECLAIMATION                     (0x00002000)
#define  EHCI_STS_SOF_COUNT                        (0x00000080)
#define  EHCI_STS_ASYNCH_ADVANCE                   (0x00000020)
#define  EHCI_STS_HOST_SYS_ERROR                   (0x00000010)
#define  EHCI_STS_FRAME_LIST_ROLLOVER              (0x00000008)
#define  EHCI_STS_PORT_CHANGE                      (0x00000004)
#define  EHCI_STS_USB_ERROR                        (0x00000002)
#define  EHCI_STS_USB_INTERRUPT                    (0x00000001)

/* Status bit masks */
#define  EHCI_STS_ASYNCH_SCHEDULE                  (0x00008000)
#define  EHCI_STS_PERIODIC_SCHEDULE                (0x00004000)
#define  EHCI_STS_RECLAMATION                      (0x00002000)
#define  EHCI_STS_HC_HALTED                        (0x00001000)

/* USB command bit masks */
#define  EHCI_USBCMD_ASYNC_SCHED_ENABLE            (0x00000020)
#define  EHCI_USBCMD_PERIODIC_SCHED_ENABLE         (0x00000010)

#define  EHCI_HCS_PARAMS_N_PORTS                   (0x0F)

#define  VUSB_HS_DELAY                             (3500)

#define  EHCI_QH_EP_NUM_MASK                       (0x0F00)
#define  EHCI_QH_EP_NUM_BITS_POS                   (8)
#define  EHCI_QH_DEVICE_ADDRESS_MASK               (0x7F)
#define  EHCI_QH_SPEED_BITS_POS                    (12)
#define  EHCI_QH_MAX_PKT_SIZE_BITS_POS             (16)
#define  EHCI_QH_NAK_COUNT_RL_BITS_POS             (28)
#define  EHCI_QH_EP_CTRL_FLAG_BIT_POS              (27)
#define  EHCI_QH_HEAD_RECLAMATION_BIT_POS          (15)
#define  EHCI_QH_DTC_BIT_POS                       (14)
#define  EHCI_QH_HIGH_BW_MULT_BIT_POS              (30)
#define  EHCI_QH_HUB_PORT_NUM_BITS_POS             (23)
#define  EHCI_QH_HUB_ADDR_BITS_POS                 (16)
#define  EHCI_QH_SPLIT_COMPLETION_MASK_BITS_POS    (8)
#define  EHCI_QH_SPLIT_COMPLETION_MASK             (0xFF00)
#define  EHCI_QH_INTR_SCHED_MASK                   (0xFF)
#define  EHCI_QH_INACTIVATE_NEXT_TR_BIT_POS        (7)
#define  EHCI_QH_HORIZ_PHY_ADDRESS_MASK            (0xFFFFFFE0)
#define  EHCI_QH_TR_OVERLAY_DT_BIT                 (0x80000000)

#define  EHCI_SITD_SPLIT_COMPLETION_MASK_BITS_POS  (8)

#define  EHCI_INTR_NO_THRESHOLD_IMMEDIATE          (0x00010000)
#define  EHCI_NEW_PERIODIC_FRAME_LIST_SIZE         (1024)
#define  EHCI_FRAME_LIST_SIZE_BITS_POS             (2)
#define  EHCI_HORIZ_PHY_ADDRESS_MASK               (0xFFFFFFE0)

#define  DEFAULT_MAX_NAK_COUNT                     (15)

/* OTG Status and control register bit masks */

/* OTG interrupt enable bit masks */
#define  VUSBHS_OTGSC_INTERRUPT_ENABLE_BITS_MASK   (0x5F000000)
#define  VUSBHS_OTGSC_DPIE                         (0x40000000)   /* Data-line pulsing IE */
#define  VUSBHS_OTGSC_1MSIE                        (0x20000000)
#define  VUSBHS_OTGSC_BSEIE                        (0x10000000)   /* B-session end IE */
#define  VUSBHS_OTGSC_BSVIE                        (0x08000000)   /* B-session valid IE */
#define  VUSBHS_OTGSC_ASVIE                        (0x04000000)   /* A-session valid IE */
#define  VUSBHS_OTGSC_AVVIE                        (0x02000000)   /* A-V-bus valid IE */
#define  VUSBHS_OTGSC_IDIE                         (0x01000000)   /* OTG ID IE */

/* OTG interrupt status bit masks */
#define  VUSBHS_OTGSC_INTERRUPT_STATUS_BITS_MASK   (0x005F0000)
#define  VUSBHS_OTGSC_DPIS                         (0x00400000)   /* Data-line pulsing IS */
#define  VUSBHS_OTGSC_1MSIS                        (0x00200000)
#define  VUSBHS_OTGSC_BSEIS                        (0x00100000)   /* B-session end IS */
#define  VUSBHS_OTGSC_BSVIS                        (0x00080000)   /* B-session valid IS */
#define  VUSBHS_OTGSC_ASVIS                        (0x00040000)   /* A-session valid IS */
#define  VUSBHS_OTGSC_AVVIS                        (0x00020000)   /* A-Vbus valid IS */
#define  VUSBHS_OTGSC_IDIS                         (0x00010000)   /* OTG ID IS */

/* OTG status bit masks */
#define  VUSBHS_OTGSC_DPS                          (0x00004000)
#define  VUSBHS_OTGSC_BSE                          (0x00001000)   /* B-session end */
#define  VUSBHS_OTGSC_BSV                          (0x00000800)   /* B-session valid */
#define  VUSBHS_OTGSC_ASV                          (0x00000400)   /* A-session valid */
#define  VUSBHS_OTGSC_AVV                          (0x00000200)   /* A-Vbus Valid */
#define  VUSBHS_OTGSC_ID                           (0x00000100)   /* OTG ID */

/* OTG control bit masks */
#define  VUSBHS_OTGSC_CTL_BITS                     (0x2F)
#define  VUSBHS_OTGSC_HABA                         (0x00000080)   /* hardware assisted data pulse bits*/
#define  VUSBHS_OTGSC_HADP                         (0x00000040)   /* hardware assisted data pulse bits*/

#ifdef PATCH_3

/*
 * the following change is to be compatable with 4.0 revision of
 * hardware. Enable the following switch in config.mk to enable the
 * changes.
 */

/*
 * WEB20040409 below line changed from VUSBHS_OTGSC_B_HOST_EN to VUSBHS_OTGSC_IDPU 
 * to reflect change in usbhs4.0  B_HOST_EN has not been used quite some time 
 */
 
#define  VUSBHS_OTGSC_IDPU                         (0x00000020)   /* ID pull enable */

#else

#define  VUSBHS_OTGSC_B_HOST_EN                    (0x00000020)   /* B_host_enable */

#endif

#define  VUSBHS_OTGSC_DP                           (0x00000010)   /* Data-pulsing */
#define  VUSBHS_OTGSC_OT                           (0x00000008)   /* OTG termination */


#define  VUSBHS_OTGSC_HAAR                         (0x00000004)   /* Auto reset bit*/

#define  VUSBHS_OTGSC_VC                           (0x00000002)   /* Vbus charge */
#define  VUSBHS_OTGSC_VD                           (0x00000001)   /* Vbus discharge */

typedef  __u32  USB_REGISTER, * USB_REGISTER_PTR;

typedef  volatile struct {
  
   __u32 dwMaxPktLength;       /* Bits 16..26 Bit 15 is Interrupt On Setup */
   __u32 dwCurrDtdPtr;         /* Current dTD Pointer */
   __u32 dwNextDtdPtr;         /* Next dTD Pointer */
   __u32 dwSizeIocIntSts;      /* Total bytes (16..30), IOC (15), INT (8), STS (0-7) */
   __u32 dwBuffPtr0;           /* Buffer void* Page 0 (12-31) */
   __u32 dwBuffPtr1;           /* Buffer void* Page 1 (12-31) */
   __u32 dwBuffPtr2;           /* Buffer void* Page 2 (12-31) */
   __u32 dwBuffPtr3;           /* Buffer void* Page 3 (12-31) */
   __u32 dwBuffPtr4;           /* Buffer void* Page 4 (12-31) */
   __u32 dwReserved1;
   __u8  dwSetupBuffer[8];     /* 8 bytes of setup data that follows the Setup PID */
   __u32 dwReserved2[4];
   
} EpQueueHeadStruct_t;

typedef volatile EpQueueHeadStruct_t* EpQueueHeadStructPtr_t;

typedef struct {
  
   void* pPrivate;
   void  (*pFreeCallback)(void*);
   void* pXdForThisDtd;
   
} ScratchStruct_t;

typedef ScratchStruct_t* ScratchStructPtr_t;

typedef  volatile struct ep_tr_struct {
   __u32 dwNextTrElemPtr; /* Memory address of next dTD to be processed (5-31) and the T (bit 0) indicating void* validity */
   __u32 dwSizeIocSts;    /* total bytes (16-30), IOC (15), Status (0-7) */
   __u32 dwBuffPtr0;      /* Buffer void* Page 0 */
   __u32 dwBuffPtr1;      /* Buffer void* Page 1 */
   __u32 dwBuffPtr2;      /* Buffer void* Page 2 */
   __u32 dwBuffPtr3;      /* Buffer void* Page 3 */
   __u32 dwBuffPtr4;      /* Buffer void* Page 4 */
   ScratchStructPtr_t   wScratchPtr;
} EpTrStruct_t;

typedef volatile EpTrStruct_t* EpTrStructPtr_t;


#endif
/* EOF */


