
#define GPIO_ADDR					0xB8003000
#define SYSTEM_GLOBAL_CONTROL_ADDR	0xB0000000

#define REG(x, shift_num)	((*(volatile unsigned long int *)(x + shift_num)))

#define PORT0_SHIFT_NUM				0x14
#define PORT1_SHIFT_NUM 			0x24
#define PORT2_SHIFT_NUM				0x3c
#define PORT3_SHIFT_NUM				0x50
#define PORT4_SHIFT_NUM				0x64
#define PORT5_SHIFT_NUM				0x78
#define PORT7_SHIFT_NUM				0x90

#define PORT0_OUT_DATA_SHIFT_NUM	0x1c
#define PORT1_OUT_DATA_SHIFT_NUM	0x34
#define PORT2_OUT_DATA_SHIFT_NUM	0x48	
#define PORT3_OUT_DATA_SHIFT_NUM	0x5c
#define PORT4_OUT_DATA_SHIFT_NUM	0x70
#define PORT5_OUT_DATA_SHIFT_NUM	0x84
#define PORT7_OUT_DATA_SHIFT_NUM	0x9c

#define PORT0_IN_DATA_SHIFT_NUM		0x20
#define PORT1_IN_DATA_SHIFT_NUM		0x38
#define PORT2_IN_DATA_SHIFT_NUM		0x4c
#define PORT3_IN_DATA_SHIFT_NUM		0x60
#define PORT4_IN_DATA_SHIFT_NUM		0x74
#define PORT5_IN_DATA_SHIFT_NUM		0x88
#define PORT6_IN_DATA_SHIFT_NUM		0x8c
#define PORT7_IN_DATA_SHIFT_NUM		0xa0

#define PORT0_OUT_CFG_1				0x18
#define PORT1_OUT_CFG_1				0x28
#define PORT2_OUT_CFG_1				0x40
#define PORT3_OUT_CFG_1				0x54
#define PORT4_OUT_CFG_1				0x68
#define PORT5_OUT_CFG_1				0x7C
#define PORT7_OUT_CFG_1				0x94

#define GPIO_INT0					0x02
#define GPIO_INT1					0x03
#define GPIO_INT2					0x04
#define GPIO_INT3					0x05

#define GPEVTYPE					0x00
#define GPEVPOL						0x04
#define GPEVDBNC					0x08
#define GPEVEN						0x0C
#define GPEVST						0x10

#define GP1BLINK					0x30


#define PORT0						0x00
#define PORT1						0x01
#define PORT2						0x02
#define PORT3						0x03
#define PORT4						0x04
#define PORT5						0x05
#define PORT6						0x06
#define PORT7						0x07

#define PIN0						0x00
#define PIN1						0x01
#define PIN2						0x02
#define PIN3						0x03
#define PIN4						0x04
#define PIN5						0x05
#define PIN6						0x06
#define PIN7						0x07
#define PIN8						0x08
#define PIN9						0x09
#define PIN10						0x0A
#define PIN11						0x0B
#define PIN12						0x0C
#define PIN13						0x0D
#define PIN14						0x0E
#define PIN15						0x0F
#define PIN16						0x10
#define PIN17						0x11


#define MFSEL1						0x0C
#define MFSEL2						0x10




#define SSPISEL						(1<<31)
#define SDIOSEL						(1<<30)
#define XCS1SEL						(1<<29)
#define XCS2SEL						(1<<28)
#define GSPISEL						(1<<24)
#define SMISEL						(1<<22)
#define CLKOSEL						(1<<21)
#define DVOSEL2						(1<<20)
#define DVOSEL1						(1<<19)
#define DVOSEL0						(1<<18)
#define KBCSEL						(1<<17)
#define R2MDSEL						(1<<16)
#define R2ERRSEL					(1<<15)
#define RMIISEL						(1<<14)
#define R1MDSEL						(1<<13)
#define R1ERRSEL					(1<<12)
#define HSP2SEL						(1<<11)
#define HSP1SEL						(1<<10)
#define BSPSEL						(1<<9)
#define SMB2SEL						(1<<8)
#define SMB1SEL						(1<<7)
#define SBM0SEL						(1<<6)
#define SCS3SEL						(1<<5)
#define SCS2SEL						(1<<4)
#define SCS1SEL						(1<<3)
#define SMB5SEL						(1<<2)
#define SMB4SEL						(1<<1)
#define SMB3SEL						1


#define HG7SEL						(1<<31)
#define HG6SEL						(1<<30)
#define HG5SEL						(1<<29)
#define HG4SEL						(1<<28)
#define PWM3SEL						(1<<19)
#define PWM2SEL						(1<<18)
#define PWM1SEL						(1<<17)
#define PWM0SEL						(1<<16)
#define FI15SEL						(1<<15)
#define FI14SEL						(1<<14)
#define FI13SEL						(1<<13)
#define FI12SEL						(1<<12)
#define FI11SEL						(1<<11)
#define FI10SEL						(1<<10)
#define FI9SEL						(1<<9)
#define FI8SEL						(1<<8)
#define FI7SEL						(1<<7)
#define FI6SEL						(1<<6)
#define FI5SEL						(1<<5)
#define FI4SEL						(1<<4)
#define FI3SEL						(1<<3)
#define FI2SEL						(1<<2)
#define FI1SEL						(1<<1)
#define FI0SEL						1









