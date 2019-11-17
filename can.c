


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "can.h"
#include "CanIf_Cbk.h"
 
 
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
const Can_ConfigType* object ;
static uint8 PDU_L[Controller_MsgObj_NUM];
static uint32 ControllerBaseAddress[]={BASE0,BASE1}	;	
static uint8 Received_Data[8];
Can_ControllerStateType controller_state ;
 Can_HwType mailbox ;
PduInfoType pduInfoType;



/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Can_init(const Can_ConfigType* obj){
	
	object=obj;
uint8 i,j;
	                    /**************Enable_Clock****************/

	   #if (Controllers_NUM>0 )        	           
	for( i=0;i<Controllers_NUM ;i++){
	HW_WRITE_BIT(BASE_SYSTEM_CONTROL+CAN_RUN_MODE_CLOCK_GATING_CONTROL_OFFSET,i,HIGH); //clock

  HW_WRITE_BIT(object->CanController[i].CanControllerBaseAddress+CAN_CTL_OFFSET, 0 , HIGH);    // bit Init high
	HW_WRITE_BIT(object->CanController[i].CanControllerBaseAddress+CAN_CTL_OFFSET, 6 , HIGH);    // to enable write in BIT register
	HW_WRITE_BIT(object->CanController[i].CanControllerBaseAddress+CAN_STS_OFFSET, 3 , LOW);     // must be cleared
	HW_WRITE_BIT(object->CanController[i].CanControllerBaseAddress+CAN_STS_OFFSET, 4 , LOW);     // must be cleared
	                     /**************Set_Baudrate****************/ 
		Can_SetBaudrate(i,object->CanController[i].CanControllerDefaultBaudrate->CanControllerBaudRateConfigID);
		
		                  /**************inti_Msgobjects****************/
		//Transmit
		
		#if (Controller_MsgObj_NUM>0 ) 
		
		     for(j=0;j<Controller_MsgObj_NUM;j++){
				if(object->CanHardwareObject[j].CanObjectId>0){	 
		if(object->CanHardwareObject[j].CanObjectType==TRANSMIT){
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1CMSK_OFFSET,bits4_7); // write in a message object
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1MCTL_OFFSET  ,bit12+bit11); // allow masking
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1MSK1_OFFSET  ,LOW);    
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1MSK2_OFFSET  ,LOW); 
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1MSK2_OFFSET  ,MASK_11_bits_ID ); 
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1ARB2_OFFSET  ,LOW);//bit 15 must be cleared before modifing in ARB2 Register
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1ARB2_OFFSET  ,(bit13)); //transmit
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1ARB2_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[i]+CAN_IF1ARB2_OFFSET))|bit15);
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF1CRQ_OFFSET   ,object->CanHardwareObject[j].CanObjectId);
		
				 
		}
		//Receive		 
		if(object->CanHardwareObject[j].CanObjectType==RECEIVE){
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2CMSK_OFFSET,bits4_7); // write in a message object
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2MCTL_OFFSET  ,bit12+bit10+bit7); // allow masking
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2MSK1_OFFSET  ,LOW);
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2MSK2_OFFSET  ,LOW); 
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2MSK2_OFFSET  ,bit14);
	    HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2MSK2_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[i]+CAN_IF2MSK2_OFFSET)) +(object->CanHardwareObject[j].CanHwFilter.CanHwFilterMask<<2) ); //mask 
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2ARB2_OFFSET  ,LOW); //bit 15 must be cleared before modifing in ARB2 Register
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2ARB2_OFFSET  ,(object->CanHardwareObject[j].CanHwFilter.CanHwFilterCode<<2)); //receive
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2ARB2_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[i]+CAN_IF2ARB2_OFFSET ))+bit15);//Valid MsgObj
		HW_WRITE_REG_32BIT(object->CanController[i].CanControllerBaseAddress+CAN_IF2CRQ_OFFSET   ,object->CanHardwareObject[j].CanObjectId);
		
}}
	#endif
	}
	Can_EnableControllerInterrupts(i);
	}
	#endif
}



/******Can_SetBaudrate*******\
      * \brief CANBIT Register Values
      * BRP          [1 .. 64]      Defines the length of the time quantum tq
      *TSEG2 = Phase2 - 1
      *TSEG1 = Prop + Phase1 - 1 
      *SJW   = SJW - 1 
      *BRP   =BRP -1
      *TQ_NUM =(clockSystem in Hz)/5
      * The bit time may consist of 4 to 25 tq
      */

/*Std_ReturnType*/ void Can_SetBaudrate(  uint8 Controller,  uint16 BaudRateConfigID ){
	uint8 i,SJW,TSEG1,TSEG2, BRP,Prop,phase1,phase2;
	uint16 BaudRate,value;
	   	
for (int i=0;i<BaudRate_Config_NUM;i++){
	
 if(object->CanController[Controller].CanControllerDefaultBaudrate[i].CanControllerBaudRateConfigID==BaudRateConfigID){
	 
   BaudRate=object->CanController[Controller].CanControllerDefaultBaudrate[i].CanControllerBaudRate;
   Prop    =object->CanController[Controller].CanControllerDefaultBaudrate[i].CanControllerPropSeg;
   phase1  =object->CanController[Controller].CanControllerDefaultBaudrate[i].CanControllerSeg1;
   phase2  =object->CanController[Controller].CanControllerDefaultBaudrate[i].CanControllerSeg2;
   SJW     =object->CanController[Controller].CanControllerDefaultBaudrate[i].CanControllerSyncJumpWidth;
 }}
   BRP=SYSTEM_CLOCK_FREQ/(BaudRate*TQ_NUM)-1;
   TSEG1=Prop+phase1-1;
   TSEG2 = phase2 - 1;
   SJW=SJW - 1 ;
   value=BRP+(SJW<<6)+(TSEG1<<8)+(TSEG2<<12);
   HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_BIT_OFFSET,value);
   HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 6 , LOW);    // to disable write in BIT register
}




/*Can_ReturnType*/void Can_SetControllerMode(   uint8 Controller,  Can_ControllerStateType Transition ){

	 if(Transition==CAN_CS_STARTED){
			 HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 0 , LOW);    // particpate on the bus
		   controller_state= CAN_CS_STARTED;
			 }
	else if(Transition==CAN_CS_STOPPED){
       HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 0 , HIGH);    // bit Init high
	     HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 6 , HIGH);    // to enable write in BIT register;  
		   controller_state=CAN_CS_STOPPED;
	}
	else if(Transition==CAN_CS_SLEEP){}
	else if(Transition==CAN_CS_UNINIT){}
		
}
void Can_DisableControllerInterrupts(uint8 Controller )
{
 HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 1 , LOW); //Interrupts disabled
 HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 2 , LOW); //Status Interrupt disable
 HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 3 , LOW); //Error Interrupt disable
}

void Can_EnableControllerInterrupts( uint8 Controller ) {

 HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 1 , HIGH); //Interrupts Enabled
 HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 2 , HIGH); //Status Interrupt Enable
 HW_WRITE_BIT(object->CanController[Controller].CanControllerBaseAddress+CAN_CTL_OFFSET, 3 , HIGH); //Error Interrupt Enable
}
 uint32 k;
//*********Can_Write***********//
//checks if the hardware transmit object that is identified by the HTH is free
//The function Can_Write shall perform no actions if the hardware transmit object is busy with another transmit request for an L-PDU
//PduInfo :Pointer to SDU user memory, Data Length and Identifier. 
Can_ReturnType Can_Write(  Can_HwHandleType Hth,  const Can_PduType* PduInfo ) {
	k=ControllerBaseAddress[Hth/33];
	PDU_L[Hth-1]=PduInfo->swPduHandle;
	//the message object is busy with another transmit
	
if( HW_READ_BIT(ControllerBaseAddress[Hth/33]+CAN_TXRQ1_OFFSET,Hth-1)==1) return CAN_BUSY;

else{
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1CMSK_OFFSET  ,(bit7+bit6+bit5+bit4));
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1ARB2_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1ARB2_OFFSET))&(~bit15));// bit15 must be cleared before modifing ARB1
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1ARB2_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1ARB2_OFFSET)&bits13_15)+(PduInfo->id<<2));
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1MCTL_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1MCTL_OFFSET))| (PduInfo->length&bits0_3));//data legnth
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1MCTL_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1MCTL_OFFSET))| (bit7));
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1ARB2_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1ARB2_OFFSET))|(bit15));

	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1CRQ_OFFSET   ,Hth);
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1CMSK_OFFSET  ,(bit7+bit4+bit1+bit0));
        if(PduInfo->length==1){ 
	
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA1_OFFSET   ,PduInfo->sdu[0]+(0x00<<8));//data bytes 1&2
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA2_OFFSET   ,0);//data bytes 3&4
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB1_OFFSET   ,0);//data bytes 5&6
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB2_OFFSET   ,0);//data bytes 7&8
	}
	      if(PduInfo->length==2){ 
	
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA1_OFFSET   ,PduInfo->sdu[0]+(PduInfo->sdu[1]<<8));//data bytes 1&2
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA2_OFFSET   ,0);//data bytes 3&4
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB1_OFFSET   ,0);//data bytes 5&6
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB2_OFFSET   ,0);//data bytes 7&8
	}
	      if(PduInfo->length==3){ 
	
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA1_OFFSET   ,PduInfo->sdu[0]+(PduInfo->sdu[1]<<8));//data bytes 1&2
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA2_OFFSET   ,PduInfo->sdu[2]+(0x00<<8));//data bytes 3&4
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB1_OFFSET   ,0);//data bytes 5&6
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB2_OFFSET   ,0);//data bytes 7&8
	}
		    if(PduInfo->length==4){ 
	
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA1_OFFSET   ,PduInfo->sdu[0]+(PduInfo->sdu[1]<<8));//data bytes 1&2
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA2_OFFSET   ,PduInfo->sdu[2]+(PduInfo->sdu[3]<<8));//data bytes 3&4
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB1_OFFSET   ,0x0000);//data bytes 5&6
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB2_OFFSET   ,0x0);//data bytes 7&8
	}
				if(PduInfo->length==5){ 
	
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA1_OFFSET   ,PduInfo->sdu[0]+(PduInfo->sdu[1]<<8));//data bytes 1&2
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA2_OFFSET   ,PduInfo->sdu[2]+(PduInfo->sdu[3]<<8));//data bytes 3&4
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB1_OFFSET   ,PduInfo->sdu[4]+(0x00<<8));//data bytes 5&6
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB2_OFFSET   ,0);//data bytes 7&8
	}
				if(PduInfo->length==6){ 
	
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA1_OFFSET   ,PduInfo->sdu[0]+(PduInfo->sdu[1]<<8));//data bytes 1&2
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA2_OFFSET   ,PduInfo->sdu[2]+(PduInfo->sdu[3]<<8));//data bytes 3&4
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB1_OFFSET   ,PduInfo->sdu[4]+(PduInfo->sdu[5]<<8));//data bytes 5&6
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB2_OFFSET   ,0);//data bytes 7&8
	}
				if(PduInfo->length==7){ 
	
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA1_OFFSET   ,PduInfo->sdu[0]+(PduInfo->sdu[1]<<8));//data bytes 1&2
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA2_OFFSET   ,PduInfo->sdu[2]+(PduInfo->sdu[3]<<8));//data bytes 3&4
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB1_OFFSET   ,PduInfo->sdu[4]+(PduInfo->sdu[5]<<8));//data bytes 5&6
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB2_OFFSET   ,PduInfo->sdu[6]+(0x00<<8));//data bytes 7&8
	}
				if(PduInfo->length==8){ 
	
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA1_OFFSET   ,PduInfo->sdu[0]+(PduInfo->sdu[1]<<8));//data bytes 1&2
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DA2_OFFSET   ,PduInfo->sdu[2]+(PduInfo->sdu[3]<<8));//data bytes 3&4
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB1_OFFSET   ,PduInfo->sdu[4]+(PduInfo->sdu[5]<<8));//data bytes 5&6
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1DB2_OFFSET   ,PduInfo->sdu[6]+(PduInfo->sdu[7]<<8));//data bytes 7&8
	}

	

	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1CRQ_OFFSET   ,Hth);
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1MCTL_OFFSET  ,(HW_READ_REG_32BIT(ControllerBaseAddress[Hth/33]+CAN_IF1MCTL_OFFSET))| (bit8+bit15));
	
return CAN_OK;
}


}



void Interrupt(uint8 Controller){
uint8 hoh,data_legnth ;


	 
                            /************Transmit OK*************/
	 hoh=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_INT_OFFSET);
   if(object->CanHardwareObject[hoh].CanObjectType==TRANSMIT) 
	{
   HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_STS_OFFSET,HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_STS_OFFSET)&(~bit3));		
   HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF1CMSK_OFFSET,0x18); //Clear Interrupt
   HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF1CRQ_OFFSET  ,HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_INT_OFFSET));
	 CanIf_TxConfirmation(PDU_L[hoh-1]);
	}
	                           /***********Receive OK*************/
	
		 hoh=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_INT_OFFSET);
     if(object->CanHardwareObject[hoh].CanObjectType==RECEIVE) 
		{
	HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_STS_OFFSET,HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_STS_OFFSET)&(~bit4));		
  HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2CMSK_OFFSET,0x73); // ReadData
  hoh=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_INT_OFFSET);
			
  HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2CRQ_OFFSET   ,hoh);
	data_legnth=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2MCTL_OFFSET)&bits0_3 ;
	Received_Data[0]=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2DA1_OFFSET) & bits0_7;
	Received_Data[1]=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2DA1_OFFSET) & bits8_15;		
	Received_Data[2]=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2DA2_OFFSET) & bits0_7;
	Received_Data[3]=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2DA2_OFFSET) & bits8_15;
	Received_Data[4]=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2DB1_OFFSET) & bits0_7;
	Received_Data[5]=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2DB1_OFFSET) & bits8_15;
	Received_Data[6]=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2DB2_OFFSET) & bits0_7;
	Received_Data[7]=HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2DB2_OFFSET) & bits8_15;
  mailbox.CanId=(HW_READ_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2ARB2_OFFSET)&0x1FFC)>>2;
	mailbox.ControllerId=object->CanController[Controller].CanControllerId;
	mailbox.Hoh=hoh;
	pduInfoType.SduDataPtr=Received_Data;
	pduInfoType.SduLength=data_legnth;
	CanIf_RxIndication(&mailbox,&pduInfoType);
  HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2CMSK_OFFSET,0x18); // Clear Interrupt
  HW_WRITE_REG_32BIT(ControllerBaseAddress[Controller]+CAN_IF2CRQ_OFFSET   ,hoh);			
   


}
}

void CAN0_Handler(void){
Interrupt(CAN0);
	
}

void CAN1_Handler(void){
Interrupt(CAN1);
}
 #define SCB_CPAC (*((volatile unsigned int*)0xE000ED88))




void SystemInit (void){
	 SCB_CPAC |= 0x00F00000;
 }
