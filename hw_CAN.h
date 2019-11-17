/**
 *  \file     hw_CAN.h
 *
 *  \brief    This file contains the physical addresses of CAN module
 *
 */
 
 #ifndef HW_CAN_H_
 #define HW_CAN_H_
 
 /* ========================================================================== */
 /*                           Macros & Typedefs                                */
 /* ========================================================================== */
                               /********BASE********/
													 
#define BASE0 0x40040000
#define BASE1 0x40041000
 


                               /********OFFSET********/

#define CAN_CTL_OFFSET              0x000
#define CAN_STS_OFFSET              0x004
#define CAN_ERR_OFFSET              0x008
#define CAN_BIT_OFFSET              0x00C
#define CAN_INT_OFFSET              0x010
#define CAN_TST_OFFSET              0x014
#define CAN_BRPE_OFFSET             0x018
#define CAN_IF1CRQ_OFFSET           0x020
#define CAN_IF1CMSK_OFFSET          0x024
#define CAN_IF1MSK1_OFFSET          0x028
#define CAN_IF1MSK2_OFFSET          0x02C
#define CAN_IF1ARB1_OFFSET          0x030
#define CAN_IF1ARB2_OFFSET          0X034
#define CAN_IF1MCTL_OFFSET          0x038
#define CAN_IF1DA1_OFFSET           0x03C
#define CAN_IF1DA2_OFFSET           0x040
#define CAN_IF1DB1_OFFSET           0x044
#define CAN_IF1DB2_OFFSET           0x048
#define CAN_IF2CRQ_OFFSET           0x080
#define CAN_IF2CMSK_OFFSET          0x084
#define CAN_IF2MSK1_OFFSET          0x088
#define CAN_IF2MSK2_OFFSET          0x08C
#define CAN_IF2ARB1_OFFSET          0x090
#define CAN_IF2ARB2_OFFSET          0x094
#define CAN_IF2MCTL_OFFSET          0x098
#define CAN_IF2DA1_OFFSET           0x09C
#define CAN_IF2DA2_OFFSET           0x0A0
#define CAN_IF2DB1_OFFSET           0x0A4
#define CAN_IF2DB2_OFFSET           0x0A8
#define CAN_TXRQ1_OFFSET            0x110
#define CAN_TXRQ2_OFFSET            0x104
#define CAN_NWDA1_OFFSET            0x120
#define CAN_NWDA2_OFFSET            0x124
#define CAN_MSG1INT_OFFSET          0x140
#define CAN_MSG2INT_OFFSET          0x144
#define CAN_MSG1VAL_OFFSET          0x160
#define CAN_MSG2VAL_OFFSET          0x164

               /******************the bit fields values in the CAN_Bit_Timing register *********************/
			/*				 
#define SJW      0x00C0
#define TSEG1    0x0400
#define TSEG2    0x3000*/
                          /*************************the bit fields  **************************/
#define bit15     0x8000
#define bit14     0x4000
#define bit13     0x2000
#define bit12     0x1000
#define bit11     0x0800
#define bit10     0x0400
#define bit9      0x0200
#define bit8     0x0100												
#define bit7     0x0080
#define bit6     0x0040
#define bit5     0x0020
#define bit4     0x0010
#define bit3     0x0008
#define bit2     0x0004
#define bit1     0x0002
#define bit0     0x0001
#define bits0_3     0x000F
#define bits4_7     0x00F0	
#define bits0_3     0x000F
#define bits0_7     0x00FF
#define bits8_11     0x0F00
#define bits12_15    0xF000	
#define bits13_15    0xE000	
#define bits8_15     0xFF00
#define bits0_15     0xFFFF
#define MASK_11_bits_ID     0x5FFC


#endif
