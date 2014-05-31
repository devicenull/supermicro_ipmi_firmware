struct MDInfo_IPMI{
	unsigned int     STARTED_ADDRESS;
	unsigned int     END_ADDRESS;
	unsigned short    CHECKSUM_16BIT;
	
	unsigned char     MANUFACTORY_ID[16];
	unsigned char     MOT_VER[2];
	unsigned char     DEVICE_ID;
	unsigned char     DEVICE_REVISION;
	unsigned char     FIRMWARE_REVISON[2];
	unsigned char     IPMI_VERSION;
	unsigned char     ADDITIONAL_DEVICE_SUPPORT;
	unsigned char     MANUFACTER_ID[3];
	unsigned char     PRODUCT_ID[2];
	unsigned char     AUX_FW_REVISION_INFO[4];
	};

extern short bbCRCCal(unsigned int dw_Startaddr, unsigned int dw_Endaddr);

