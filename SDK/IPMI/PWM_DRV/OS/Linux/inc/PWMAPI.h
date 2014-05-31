

#define CMD_SUCCESS		0
#define INVALID_MODULE_NUM	-1
#define INVALID_CHANNEL_NUM	-2
#define DEV_OPEN_FAIL		-3

#define SET_MODE                0
#define SET_FREQUENCY           1
#define GET_FREQUENCY           2
#define SET_DUTY_CYCLE          3
#define GET_DUTY_CYCLE          4
#define CHANNEL_ENABLE          5
#define CHANNEL_DISABLE         6
#define PWM_DISABLE             7
#define SET_INVERTOR            8
#define GET_BASE_FREQ		9

#define ONE_SHOT_MODE           0
#define TOGGLE_MODE             1

#define GET_BIT(Reg, bit)	(Reg & (0x01 << bit)) >> (0x01 << bit)		

typedef struct PWM_Info
{
    unsigned char b_Cmd;
    unsigned short int w_CounterValue;
    unsigned short int w_ComparatorValue;
    unsigned char b_PreScaler;
    unsigned char b_ClockDivider;
    unsigned char b_ModuleAndChannelNum;
    unsigned char b_Mode;
    unsigned char b_InvertFlag;
    unsigned long int dw_BaseFreq;
}St_PWM_InfoTag;

#ifdef _PWMAPI_C_

typedef unsigned char		BYTE;
typedef unsigned short int	WORD;
typedef unsigned long int	DWORD;

int PWM_Init(void);
int SET_DutyCycle(BYTE, DWORD, BYTE, BYTE, BYTE, BYTE);
int SET_ChannelEnable(BYTE, BYTE, BYTE);
int SET_PWMEnable(BYTE, BYTE);
int SET_PWMFrequency(BYTE, BYTE, BYTE, BYTE);
int PWM_Exit(void);

#else

extern int PWM_Init(void);
extern int SET_DutyCycle(BYTE, DWORD, BYTE, BYTE, BYTE, BYTE);
extern int SET_ChannelEnable(BYTE, BYTE, BYTE);
extern int SET_PWMEnable(BYTE, BYTE);
extern int SET_PWMFrequency(BYTE, BYTE, BYTE, BYTE);
extern int PWM_Exit(void);

#endif


