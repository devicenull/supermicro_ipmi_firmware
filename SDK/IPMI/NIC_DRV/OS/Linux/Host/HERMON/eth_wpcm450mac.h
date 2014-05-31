



#ifndef _p450MAC_H_
#define _p450MAC_H_

typedef struct 
{
	volatile unsigned long	SL;
	volatile unsigned long	buffer;
	volatile unsigned long	reserved;
	volatile unsigned long	next;
}RXBD;


typedef struct 
{
	volatile unsigned long mode;
	volatile unsigned long buffer;
	volatile unsigned long SL;
	volatile unsigned long next;
}TXBD;

#define WPCM450_IRQREG(x)   ((x) + WPCM450_VA_IRQ)
#define WPCM450_AIC_IMR  	WPCM450_IRQREG(0x114)
#define WPCM450_AIC_MECR	WPCM450_IRQREG(0x120)
#define WPCM450_AIC_MDCR	WPCM450_IRQREG(0x124)
#define WPCM450_AIC_SCR15	WPCM450_IRQREG(0x03C)
#define WPCM450_AIC_SCR16	WPCM450_IRQREG(0x040)
#define WPCM450_AIC_SCR17	WPCM450_IRQREG(0x044)
#define WPCM450_AIC_SCR18	WPCM450_IRQREG(0x048)


#define Enable_IRQ(n)     				outl(1 << (n),WPCM450_AIC_MECR)
#define Disable_IRQ(n)    				outl(1 << (n),WPCM450_AIC_MDCR)

#define _NOMMU ((unsigned long) 0xC0000000) 
#define NOMMU (~_NOMMU)

#define  MAC_OFFSET  	0x0
#define  MAC_0_OFFSET  	MAC_OFFSET


#define AIC_SCR_EMCRX0 WPCM450_AIC_SCR15
#define AIC_SCR_EMCTX0 WPCM450_AIC_SCR16 
#define AIC_SCR_EMCRX1 WPCM450_AIC_SCR17
#define AIC_SCR_EMCTX1 WPCM450_AIC_SCR18

#define  CAMCMR			    (MAC_OFFSET)     
#define  CAMEN			    (MAC_OFFSET+0x4) 
#define  CAM0M			    (MAC_OFFSET+0x8) 
#define  CAM0L		    	(MAC_OFFSET+0xc) 
#define  CAM_ENTRY_SIZE	(0x8)     		   
#define  CAM_ENTRIES	  (0x16)    		   

#define MIEN			      (MAC_OFFSET+0xac) 
#define MCMDR			      (MAC_OFFSET+0x90) 
#define MIID			      (MAC_OFFSET+0x94) 
#define MIIDA			      (MAC_OFFSET+0x98) 
#define MPCNT			      (MAC_OFFSET+0xb8) 

#define TXDLSA			    (MAC_OFFSET+0x88) 
#define RXDLSA			    (MAC_OFFSET+0x8c) 
#define DMARFC			    (MAC_OFFSET+0xa8) 
#define TSDR			      (MAC_OFFSET+0xa0) 
#define RSDR			      (MAC_OFFSET+0xa4) 
#define FFTCR			      (MAC_OFFSET+0x9c) 

#define MISTA			      (MAC_OFFSET+0xb0) 
#define MGSTA			      (MAC_OFFSET+0xb4) 
#define MRPC			      (MAC_OFFSET+0xbc) 
#define MRPCC			      (MAC_OFFSET+0xc0) 
#define MREPC			      (MAC_OFFSET+0xc4) 

#define DMARFS			    (MAC_OFFSET+0xc8) 
#define CTXDSA			    (MAC_OFFSET+0xcc) 
#define CTXBSA			    (MAC_OFFSET+0xd0) 
#define CRXDSA			    (MAC_OFFSET+0xd4) 
#define CRXBSA			    (MAC_OFFSET+0xd8) 

#define RXFSM			      (MAC_OFFSET+0x200)
#define TXFSM			      (MAC_OFFSET+0x204)
#define FSM0			      (MAC_OFFSET+0x208)
#define FSM1			      (MAC_OFFSET+0x20c)


#define CAM_AUP  0x0001  
#define CAM_AMP  0x0002  
#define CAM_ABP  0x0004  
#define CAM_CCAM 0x0008  
#define CAM_ECMP 0x0010  

#define RXfOwnership_DMA 0x80000000  
#define RXfOwnership_CPU 0x3fffffff  

#define TXfOwnership_DMA 0x80000000  
#define TXfOwnership_CPU 0x7fffffff  

#define MACTxIntEn    0x04
#define CRCMode       0x02
#define NoCRCMode     0x00
#define PaddingMode   0x01
#define NoPaddingMode 0x00

#define RXDS_RXINTR     (1<<16) 
#define RXDS_CRCE		(1<<17) 
#define RXDS_PTLE		(1<<19) 
#define RXDS_RXGD		(1<<20) 
#define RXDS_ALIE		(1<<21) 
#define RXDS_RP			(1<<22) 
#define RXDS_Inverse	(1<<26) 
#define RXDS_PortHit	(1<<27) 
#define RXDS_IPHit		(1<<28)	
#define RXDS_Hit		(1<<29) 

#define TX_OWNERSHIP_CPU  (0x0<<31) 
#define TX_OWNERSHIP_DMA  (0x1<<31)
#define  TX_MODE_PAD	 0x1	  
#define  TX_MODE_CRC	(0x1<<1)  
#define  TX_MODE_IE		(0x1<<2)  

#define TXDS_TXINTR 	(1<<16)		
#define TXDS_DEF		(1<<17)		
#define TXDS_TXCP		(1<<19)		
#define TXDS_EXDEF		(1<<20)		
#define TXDS_NCS		(1<<21)		
#define TXDS_TXABT		(1<<22)		
#define TXDS_LC			(1<<23)		
#define TXDS_TXHA		(1<<24)		
#define TXDS_PAU		(1<<25)		
#define TXDS_SQE		(1<<26)		
#define TXDS_CCNT		(0xf<<27)	
#define CAMCMR_AUP		0x1 
#define CAMCMR_AMP		(0x1<<1) 
#define CAMCMR_ABP		(0x1<<2) 
#define CAMCMR_CCAM		(0x1<<3) 
#define CAMCMR_ECMP		(0x1<<4) 


#define MDCCR    0x00f00000  
#define PHYAD    0x00001f00  
#define PHYWR    0x00010000  
#define PHYBUSY  0x00020000  
#define PHYPreSP 0x00040000  





#define EnRXINTR 0x00000001  
#define EnCRCE   0x00000002  
#define EnRXOV   0x00000004  
#define EnPTLE   0x00000008  
#define EnRXGD   0x00000010  
#define EnALIE   0x00000020  
#define EnRP     0x00000040  
#define EnMMP    0x00000080  
#define EnDFO    0x00000100  
#define EnDEN    0x00000200  
#define EnRDU    0x00000400  
#define EnRxBErr 0x00000800  
#define EnCFR    0x00004000  
#define EnTXINTR 0x00010000  
#define EnTXEMP  0x00020000  
#define EnTXCP   0x00040000  
#define EnEXDEF  0x00080000  
#define EnNCS    0x00100000  
#define EnTXABT  0x00200000  
#define EnLC     0x00400000  
#define EnTDU    0x00800000  
#define EnTxBErr 0x01000000  


#define MIEN_EnRXINTR	1
#define MIEN_EnCRCE		(1<<1)
#define MIEN_EnRXOV		(1<<2)
#define MIEN_EnPTLE		(1<<3)
#define MIEN_EnRXGD		(1<<4)
#define MIEN_EnALIE		(1<<5)
#define MIEN_EnRP		  (1<<6)
#define MIEN_EnMMP		(1<<7)
#define MIEN_EnDFO		(1<<8)
#define MIEN_EnDEN		(1<<9)
#define MIEN_EnRDU		(1<<10)
#define MIEN_EnRXBErr	(1<<11)
#define MIEN_EnCFR		(1<<14)
#define MIEN_EnTXINTR	(1<<16)
#define MIEN_EnTXEMP	(1<<17)
#define MIEN_EnTXCP		(1<<18)
#define MIEN_EnEXDEF	(1<<19)
#define MIEN_EnNCS		(1<<20)
#define MIEN_EnTXABT	(1<<21)
#define MIEN_EnLC		  (1<<22)
#define MIEN_EnTDU		(1<<23)
#define MIEN_EnTxBErr	(1<<24)
#define MCMDR_RXON		1
#define MCMDR_ALP		(1<<1)
#define MCMDR_ARP		(1<<2)
#define MCMDR_ACP		(1<<3)
#define MCMDR_AEP		(1<<4)
#define MCMDR_SPCRC		(1<<5)
#define MCMDR_TXON		(1<<8)
#define MCMDR_NDEF		(1<<9)
#define MCMDR_SDPZ		(1<<16)
#define MCMDR_EnSQE		(1<<17)
#define MCMDR_FDUP		(1<<18)
#define MCMDR_EnMDC		(1<<19)
#define MCMDR_OPMOD		(1<<20)
#define MCMDR_LBK		(1<<21)
#define SWR				(1<<24)

#define  MIIDA_PHYRAD	1
#define  MIIDA_PHYAD	(1<<8)
#define  MIIDA_WR		(1<<16)
#define  MIIDA_BUSY		(1<<17)
#define  MIIDA_PreSP	(1<<18)
#define  MIIDA_MDCON	(1<<19)
#define  MIIDA_MDCCR	(1<<20)

#define  FIFOTHD_RxTHD	 1
#define  FIFOTHD_TxTHD	(1<<8)
#define  FIFOTHD_SWR	(1<<16)
#define  FIFOTHD_Blength (1<<20)
#define MISTA_RXINTR	1
#define MISTA_CRCE		(1<<1)
#define MISTA_RXOV		(1<<2)
#define MISTA_PTLE		(1<<3)
#define MISTA_RXGD		(1<<4)
#define MISTA_ALIE		(1<<5)
#define MISTA_RP		(1<<6)
#define MISTA_MMP		(1<<7)
#define MISTA_DFOI		(1<<8)
#define MISTA_DENI		(1<<9)
#define MISTA_RDU		(1<<10)
#define MISTA_RxBErr	(1<<11)
#define MISTA_CFR		(1<<14)
#define MISTA_TXINTR	(1<<16)
#define MISTA_TXEMP		(1<<17)
#define MISTA_TXCP		(1<<18)
#define MISTA_EXDEF		(1<<19)
#define MISTA_NCS		(1<<20)
#define MISTA_TXABT		(1<<21)
#define MISTA_LC		(1<<22)
#define MISTA_TDU		(1<<23)
#define MISTA_TxBErr	(1<<24)

#define MGSTA_CFR		1
#define MGSTA_RXHA		(1<<1)
#define MGSTA_RFFull	(1<<2) 
#define MGSTA_DEF		(1<<4)
#define MGSTA_PAU		(1<<5)
#define MGSTA_SQE		(1<<6)
#define MGSTA_TXHA		(1<<7)


#define wpcm450_WriteReg(reg,val,which)      outl(val,(WPCM450_VA_EMC+reg+(which)*0x1000))
#define wpcm450_ReadReg(reg,which)           inl(WPCM450_VA_EMC+reg+(which)*0x1000)

#define wpcm450_WriteCam0(which,x,lsw,msw) \
		wpcm450_WriteReg(CAM0L+(x)*CAM_ENTRY_SIZE,lsw,which);\
		wpcm450_WriteReg(CAM0M+(x)*CAM_ENTRY_SIZE,msw,which);\
		

#define MDCCR1   0x00f00000  


#define WPCM450_MFSEL1 (WPCM450_VA_GCR+0x00c)
#endif 

