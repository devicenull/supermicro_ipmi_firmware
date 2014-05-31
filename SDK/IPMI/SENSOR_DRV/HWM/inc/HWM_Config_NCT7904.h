#ifndef _HWM_NCT7904_H
#define _HWM_NCT7904_H

extern const HW_MONITOR_CONFIG HW_monitor_config_table_NCT7904[];
extern const unsigned int at_dw_HWMonitorConfigSize_NCT7904;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DR6Rev110[];
extern const unsigned int at_dw_HWMonitorConfigSize_X9DR6Rev110;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRG[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRG;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRG_OF[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRG_OF;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRL[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRL;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRL_7F[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRL_7F;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9SRG[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9SRG;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRX[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRX;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRW_PLUS[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRW_PLUS;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRW_CF[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRW_CF;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRW_7TPF_PLUS[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRW_7TPF_PLUS;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRT[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRT;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRT_SG[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRT_SG;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRD[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRD;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DBU[];
extern const unsigned int  at_dw_HWMonitorConfigSizeX9DBU;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DBL[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DBL;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9QR7[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9QR7;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRH[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRH;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRF[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRF ;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRT_HFPLUS[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRT_HFPLUS;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRT_P[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRT_P;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DAX[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DAX;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRT_F[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRT_F;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DR7_LN4[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DR7_LN4;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DR7_TFPLUS[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DR7TFPLUS;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9QR7_TF[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9QR7TF;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9SRE_DS108[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9SRE_DS108;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRFF[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRFF;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9SCFF[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9SCFF;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRFF7P[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRFF7P;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DRFF7GP[];
extern const unsigned int at_dw_HWMonitorConfigSizeX9DRFF7GP;
extern const HW_MONITOR_CONFIG HW_monitor_config_table_X9DB3[];	// joe added for x9db3
extern const unsigned int  at_dw_HWMonitorConfigSizeX9DB3;		// joe added for x9db3


unsigned char 	  *FanFullSpeed;
unsigned char     at_b_FanFullSpeed_Size;
unsigned char	  *FanNormalSpeed;
unsigned char     at_b_FanNormalSpeed_Size;	
unsigned char	  *FanOptimalSpeed;
unsigned char     at_b_FanOptimalSpeed_Size;

unsigned char 	*FanHeavyIOSpeed;
unsigned char     at_b_FanHeavyIOSpeed_Size;

//linda begin --- Fan PUE Optimal mode
unsigned char	  *FanPUEOptimalSpeed;
unsigned char     at_b_FanPUEOptimalSpeed_Size;
//linda end --- Fan PUE Optimal mode

extern unsigned char FanFullSpeed_Def[];
extern unsigned char at_b_FanFullSpeed_Def_Size;
extern unsigned char FanNormalSpeed_Def[];
extern unsigned char at_b_FanNormalSpeed_Def_Size;
extern unsigned char FanOptimalSpeed_Def[];
extern unsigned char at_b_FanOptimalSpeed_Def_Size;

extern unsigned char FanHeavyIOSpeed_Def[];
extern unsigned char at_b_FanHeavyIOSpeed_Def_Size;
extern unsigned char FanHeavyIOSpeed_SZone_Def[];
extern unsigned char at_b_FanHeavyIOSpeed_SZone_Def_Size;


//linda begin --- Fan PUE Optimal mode
extern unsigned char FanPUEOptimalSpeed_Def[];
extern unsigned char at_b_FanPUEOptimalSpeed_Def_Size;
extern unsigned char FanPUEStandardSpeed_Def[];
extern unsigned char at_b_FanPUEStandardSpeed_Def_Size;

extern unsigned char FanPUEOptimalSpeed_Def_X9DRF[];
extern unsigned char at_b_FanPUEOptimalSpeed_Def_Size_X9DRF;
extern unsigned char FanPUEStandardSpeed_Def_X9DRF[];
extern unsigned char at_b_FanPUEStandardSpeed_Def_Size_X9DRF;
extern unsigned char FanHeavyIOSpeed_Def_X9DRF[];
extern unsigned char at_b_FanHeavyIOSpeed_Def_Size_X9DRF;


//linda end --- Fan PUE Optimal mode

extern unsigned char FanNormalSpeed_X9DRG[];
extern unsigned char at_b_FanNormalSpeed_X9DRG_Size;
extern unsigned char FanOptimalSpeed_X9DRG[];
extern unsigned char at_b_FanOptimalSpeed_X9DRG_Size;
extern unsigned char FanOptimalSpeed_X9DRG_OF[];
extern unsigned char at_b_FanOptimalSpeed_X9DRG_OF_Size;
extern unsigned char FanOptimalSpeed_X9SRG[];
extern unsigned char at_b_FanOptimalSpeed_X9SRG_Size;

extern unsigned char FanNormalSpeed_X9QR7_TF[];
extern unsigned char at_b_FanNormalSpeed_X9QR7_TF_Size;
extern unsigned char FanOptimalSpeed_X9QR7_TF[];
extern unsigned char at_b_FanOptimalSpeed_X9QR7_TF_Size;
extern unsigned char FanFullSpeed_X9QR7_TF[];
extern unsigned char at_b_FanFullSpeed_X9QR7_TF_Size;

extern unsigned char FanPUEOptimalSpeed_X9DRFF_7GP[];
extern unsigned char at_b_FanPUEOptimalSpeed_X9DRFF_7GP_Size;
extern unsigned char FanPUEStandardSpeed_X9DRFF_7GP[];
extern unsigned char at_b_FanPUEStandardSpeed_X9DRFF_7GP_Size;

/*
*
* X9DRF
*/
/*
extern unsigned char FanNormalSpeed_X9DRF[];
extern unsigned char at_b_FanNormalSpeed_X9DRF_Size;
extern unsigned char FanOptimalSpeed_X9DRF[];
extern unsigned char at_b_FanOptimalSpeed_X9DRF_Size;
*/
#endif
