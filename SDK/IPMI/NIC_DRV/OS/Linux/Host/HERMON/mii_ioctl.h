
typedef struct St_mii
{
	unsigned int  u32PhyID;
	unsigned char u8Op;
	unsigned char u8Reg;
	unsigned int  u32value;
	
	
}St_mii_data;

#define SIOCwpcm450_SHOWRXDESC		SIOCDEVPRIVATE+1
#define SIOCwpcm450_SHOWTXDESC		SIOCDEVPRIVATE+2
#define SIOCwpcm450_SNDNCSIMSG		SIOCDEVPRIVATE+3
#define SIOCwpcm450_QUERYFIFO		SIOCDEVPRIVATE+4
#define SIOCwpcm450_ENNCSICHNL		SIOCDEVPRIVATE+5
#define SIOCwpcm450_DISNCSICHNL		SIOCDEVPRIVATE+6
#define SIOCwpcm450_FORCERESETPHY		SIOCDEVPRIVATE+7
#define SIOCwpcm450_LINKINFO		SIOCDEVPRIVATE+8
#define SIOCwpcm450_GetNONAUTONEG_LAN_INFO	SIOCDEVPRIVATE+9	// joe --- 1-4-2013
#define SIOCwpcm450_SetNONAUTONEG_LAN_INFO	SIOCDEVPRIVATE+10	// joe --- 1-4-2013

#define PHY_CNTL_REG    	0x00
#define PHY_STATUS_REG  	0x01
#define PHY_ID1_REG     	0x02
#define PHY_ID2_REG     	0x03
#define PHY_ANA_REG     	0x04
#define PHY_ANLPA_REG   	0x05
#define PHY_ANE_REG     	0x06

#define PHY_DSC_REG     	0x10
#define PHY_DSCS_REG    	0x11
#define PHY_10BTCS_REG  	0x12
#define PHY_SINT_REG    	0x15
#define PHY_SREC_REG    	0x16
#define PHY_DISC_REG    	0x17

#define PHY_LED_Setting    	0x11	//linda
#define PHY_LED_REG   	 	0x13	//linda
#define PHY_PGE_SELCT    	0x1f	//linda
#define PHY_RMII_SETTING   	0x10	//linda
#define RESET_PHY       	(1 << 15)
#define ENABLE_LOOPBACK 	(1 << 14)
#define DR_100MB        	(1 << 13)
#define ENABLE_AN       	(1 << 12)
#define PHY_MAC_ISOLATE 	(1 << 10)
#define RESTART_AN      	(1 << 9)
#define PHY_FULLDUPLEX  	(1 << 8)
#define PHY_COL_TEST    	(1 << 7)

#define BASIC_AN_COMPLETE  	(1 << 5)

#define LINK_STATUS_VALID  	(1 << 0)
#define SPEED_STATUS_10    	(1 << 1)
#define DUPLEX_STATUS_FULL 	(1 << 2)
#define PHY_AN_COMPLETE    	(1 << 4)
#define PHY_REMOTE_FAULT   	(1 << 6)


#define MODE_DR100_FULL   	3
#define MODE_DR100_HALF   	2
#define MODE_DR10_FULL    	1
#define MODE_DR10_HALF    	0


#define DR100_TX_FULL   	(1 << 8)
#define DR100_TX_HALF   	(1 << 7)
#define DR10_TX_FULL   		(1 << 6)
#define DR10_TX_HALF    	(1 << 5)
#define IEEE_802_3_CSMA_CD   1



#define RTL8201FN_PHY_ID1 	0x001C
#define RTL8201FN_PHY_ID2	0xC816

#define RTL8201N_PHY_ID1 	0x0000
#define RTL8201N_PHY_ID2	0x8201

#define DP83848C_PHY_ID1	0x2000
#define DP83848C_PHY_ID2	0x5C90

#define BCM5221_PHY_ID1 	0x0040
#define BCM5221_PHY_ID2 	0x61E4

#define DM9161_PHY_ID1      0x0181
#define DM9161_PHY_ID2      0xB8A0


typedef struct{
	unsigned char auto_negotiation;
	unsigned char duplex;
	unsigned char speed;
	unsigned char operstate;
}st_lan_mii_info;

typedef struct{
	unsigned char auto_negotiation;	
	unsigned char speed_and_duplex;
	unsigned char operstate;
}st_lan_ncsi_info;

typedef struct{
	st_lan_mii_info	mii_info;	
	st_lan_ncsi_info ncsi_info;	
	unsigned char bond0_port;
	unsigned char dev_if_mode;
}st_lan_link_info;// joe --- begin 1-4-2013
typedef struct{
	unsigned char enable;	// 0-disable, 1-enable
	unsigned char speed;	// 0-10, 1-100, 2-1000
	unsigned char duplex;	// 0-half, 1-full
}st_NonAutoNeg_lan_info;
// joe --- end 1-4-2013

