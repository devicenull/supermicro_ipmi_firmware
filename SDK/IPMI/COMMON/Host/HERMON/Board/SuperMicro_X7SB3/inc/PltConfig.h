

#ifndef _CONFIG_H_
#define _CONFIG_H_
#define BOARD_X7SB3				0xa980
#define BOARD_X8SIL_SIOM		0x062d //rev2.21
#define BOARD_X8SIL				0x0605
#define BOARD_X8DTL_iF      	0x0006
#define BOARD_X8DTL_6F      	0x040a	
#define BOARD_X8STI_3F      	0xf680
#define BOARD_X8DTU         	0x0600
#define BOARD_X8DTUplus     	0x060c
#define BOARD_X8DTU_6TF     	0x060d
#define BOARD_X8DTU_F       	0x0614
#define BOARD_X8DTT        		0x0007
#define BOARD_X8SII       		0x040d
// [Jack] Correct the MB Name
// #define BOARD_X9DRT_HF    	0x0706	 //1.46
#define BOARD_X9SBAA         	0x0651	 //1.90
#define BOARD_X9DRT_HFPLUS		0x0706	 //1.46
#define BOARD_X9DRT       		0x062f	 //1.18
#define BOARD_X9DRT_F      		0x0708	 
#define BOARD_X9DB3       		0x0722	//v1.88
#define BOARD_X9DBU       		0x0637	//v1.13
#define BOARD_X9DBL       		0x0703	//v1.44
#define BOARD_X9SRG       		0x0661	//v1.30
#define BOARD_X9DRG      		0x0662	//v1.19
#define BOARD_X9DRG_HTFPLUS		0x0711
#define BOARD_X9DRG_OF	       	0x0732
#define BOARD_X9SRW       		0x0663	//v1.19
#define BOARD_X9DRH       		0x0664	//v1.30
#define BOARD_X9DRL       		0x0667	//v1.30
#define BOARD_X9DR6       		0x0628	//v1.12
#define BOARD_X9DR6_PLUS  		0x0626	//v1.23
#define BOARD_X9DRD       		0x0660	//v1.21
#define BOARD_X9DA6       		0x062c	//v1.08 or later
#define BOARD_X9SRA       		0x062a	//v1.05
#define BOARD_X9SRI       		0x062b	//v1.02
#define BOARD_X9SCA       		0x0631	//v1.02
#define BOARD_X9SCE      		0x0647	//v1.79
#define BOARD_X9SCD      		0x0634	//v1.09
#define BOARD_X9SCM       		0x0624	//v2.13
#define BOARD_X9SCL       		0x061e	//v2.12
#define BOARD_X9QR7			0x0707    // 
#define BOARD_X9DRF	      		0x070a    // V1.49
#define BOARD_X9SPU			0x0643
#define BOARD_X9DR7_LN4   		0x0705
#define BOARD_X9DAX			0x070E		
#define BOARD_X9DR7_TFPLUS		0x0704
#define BOARD_X9QR7_TF			0x070F
#define BOARD_X9SRE_DS108		0x0710
#define BOARD_X9DRFF			0x0713
#define BOARD_X9SRH7			0x0714
#define BOARD_X9DRW_7TPF		0x0715
#define BOARD_X9DRFF7P			0x0717
#define BOARD_X9DRFF7GP			0x0718
#define BOARD_X9DRW_7TPF_PLUS		0x0720
#define BOARD_X9DRL_7F			0x0723
#define BOARD_X9DRH_7TF     0x0724
#define BOARD_X9DRW_CF      0x0727
#define BOARD_X9DRT_P       0x0731

// Whitney Board
#define BOARD_X9DRG_HTFPLUS_II  0x0725

#define BOARD_X9SCFF			0x0646

//linda begin --- 12-21-11 for acer x9
#define BOARD_X9DR6_LN4_AI034	0x066e
#define BOARD_X9DRW_LN4_AI034	0x066f
#define BOARD_X9DRT_H_AI034		0x0700
#define BOARD_X9DRT_HIBQ_AI034	0x0701
//linda end --- 12-21-11 for acer x9


//linda begin 11-30-11	rev 1.38
#define BOARD_X9SRD       		0x0668
#define BOARD_X9SRD_CF     		0x070c

// Jack: Note that X9DRD-ED-UC014 is sharing this board ID but different MB Revision.

#define BOARD_X9DRD_LN4       	0x0702
//linda end 11-30-11


//linda begin 11-08-11	rev 1.32
#define BOARD_X9SCV       		0x0641
#define BOARD_X9SRL       		0x066B
#define BOARD_X9SRL_CF       	0x070B
#define BOARD_X9SCM_II       	0x0640	//func similar to x9scd, x9sca

#define BOARD_X9DRX      		0x0669
#define BOARD_X9DRT_SG      	0x0666
#define BOARD_X9DRW_PLUS		0x0630
//linda end 11-08-11


#define BOARD_X7SPA       		0x0a06
#define BOARD_X7SPT       		0x0627	//v2.16
#define BOARD_X8SIA       		0x060b
#define BOARD_X8DTNplus    		0x060f
#define BOARD_X8DTN        		0x0610 
#define BOARD_X8SIT  	 		0x060e
#define BOARD_X8SIU		 		0x061a
#define BOARD_X8STE				0x062e		//v2.27
#define BOARD_H8SGL         	0xa711
#define BOARD_H8DGU            	0xa811
#define BOARD_H8DGG            	0xa911
#define BOARD_H8DGT            	0xaa11
#define BOARD_H8DCL    			0xCD11	//v2.21 similar to H8DCT 
#define BOARD_H8DCT    			0xbe11	//v2.08 same as H8DCT_HIBQF 
#define BOARD_H8DCT_LN4			0xce11 //v2.34
#define BOARD_H8QG6            	0xab11
#define BOARD_H8DGU_LN4    		0xac11
#define BOARD_H8SCM				0xba11
#define BOARD_H8DG6				0xbc11
#define BOARD_H8DGT_HF_AI034 	0x0435
#define BOARD_H8DGT_HIBQF_AI034 0x0436
#define BOARD_H8DGU_LN4_AI034	0x0443
#define BOARD_H8QGI_AI034		0x046e

//Platform capability
#define DISABLE_OOB_BIOS_UPDATE 0x80000
#define DISABLE_PWRCONSUMP_DISPLAY 0x40000
#define LSI_HDD_Support   		0x20000
#define HIGH_SPEED_FAN   		0x10000
#define FAN_MODE_RESERVE4   		0x8000
#define FAN_MODE_RESERVE3		0x4000
#define FAN_MODE_RESERVE2		0x2000
// #define FAN_MODE_RESERVE1		0x1000
#define FAN_MODE_HEAVY_IO			0x1000
#define FAN_MODE_PUE_OPTIMAL   		0x0800
#define FAN_MODE_OPTIMAL		0x0400
#define FAN_MODE_FULL_SPEED		0x0200
#define FAN_MODE_STANDARD    		0x0100
#define PS_STATUS_GPIO			0x20
#define CPU_DEG_DISPLAY			0x10
#define FAN_CONTROL	   		0x8
#define HWM_7904_CHASSIS_INTR		0x4
#define POPWER_MONITOR 			0x2
#define UID_CONTROL    			0x1

#define MAX_SENSOR_DATA_NUM	100

#define MAX_USER_NUM   10

#ifndef _PLT_CONFIG_C_
#define MAX_LAN_NUM                     1
#define	MAX_SER_CHNL                    3
#define TOTAL_IPMB_CHANNEL  1

#define PAYLOAD_SIZE_OF_PACKETPOOL      1536
#define SIZE_OF_UDP_RECV_BUF            1536

#define MAX_PLUGIN_NUM                 2	
#define MAX_MAGIC_NUM				   2

extern unsigned char at_b_Max_Sess_Num;

extern unsigned long int ws_b_UART_Pool_Size;

extern unsigned short int ws_b_SOL_Pkt_Size;

#endif
#endif  









