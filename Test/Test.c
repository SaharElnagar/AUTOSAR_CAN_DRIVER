#include "..\tm4c123gh6pm.h"
#include "..\can.h"
#include "..\CanIf_Cbk.h"

int m;

void CanIf_TxConfirmation(PduIdType CanTxPduId){
	
m=CanTxPduId;
}
#define RECEIVE_MODE
//#define TRANSMIT_MODE
#define NUM_Baudrate 5
Can_ConfigType can;
CanControllerBaudrateConfig baud[NUM_Baudrate];
Can_PduType PduType;

 uint8 data1[8];
 uint8 data0[8];
void Baude_rate(uint16 baudrate,uint16 ID,uint8 prop,uint8 phase1,uint8 phase2,uint8 sjw,uint8 i);
void configure_Receive_message_object(uint8 MsgObj_Num,uint32 id,uint32 Mask);
void configure_Transmit_message_object(uint8 MsgObj_Num);
void config_CanWrite_Data(uint16 id_,uint8 length_,uint8*data_,uint16 swpduHandle_);
void Port_Enable(void);
void EnableInterrupts(void);
void PLL_Init(void);
void fill_data(uint8 x){
	uint8 i;
for(i=0;i<8;i++){
		data0[i]=x;
	
}}
int main(){
int j,m=0,k,flag0=0,flag1=0;
  uint8 i=0;

	 PLL_Init();

   EnableInterrupts();
	 Port_Enable();
	
can.CanController[0].CanControllerBaseAddress=BASE0;
Baude_rate(1000,5,2,1,1,1,0);
can.CanController[0].CanControllerDefaultBaudrate=&baud[0];
#ifdef RECEIVE_MODE
  configure_Receive_message_object(17,3,0x07FFF);
	configure_Receive_message_object(18,5,0x07FFF);
#endif
	
#ifdef TRANSMIT_MODE
 configure_Transmit_message_object(2);
 configure_Transmit_message_object(3);
#endif
Can_init(&can);
Can_SetControllerMode(0,CAN_CS_STARTED);
	//HW_WRITE_BIT(BASE0+CAN_CTL_OFFSET, 7 , HIGH); 
	//HW_WRITE_BIT(BASE0+CAN_TST_OFFSET, 4 , HIGH); 


	
	while(1){
	/*	for(k=0;k<5000000;k++){}
	
	
	//fill_data(i++);
	for(j=0;j<8;j++){
		data1[j]=m;
		
	}
	m++;
	if(m==255) m=0;
	config_CanWrite_Data(3,8,data1,11);

Can_Write(2,&PduType);
	
	
	//	config_CanWrite_Data(5,1,data0,11);
	//  Can_Write(5,&PduType);
			
			*/
		#ifdef TRANSMIT_MODE
		while((GPIO_PORTF_DATA_R&0x01)==0||(GPIO_PORTF_DATA_R&0x10)==0){
			if((GPIO_PORTF_DATA_R&0x01)==0){
	    for(j=0;j<8;j++){
	   	data1[j]=1;
			}
		  config_CanWrite_Data(3,8,data1,11);
			Can_Write(2,&PduType);}
			
			if((GPIO_PORTF_DATA_R&0x10)==0){
			fill_data(1);
			config_CanWrite_Data(5,1,data0,11);
	    Can_Write(3,&PduType);
	
			}
		}
		  for(j=0;j<8;j++){
	   	data1[j]=0;
			}
			fill_data(0);
			config_CanWrite_Data(3,8,data1,11);
			Can_Write(2,&PduType);
			
			config_CanWrite_Data(5,1,data0,11);
	    Can_Write(3,&PduType);
		
		#endif
		
	}

}


void configure_Transmit_message_object(uint8 MsgObj_Num){
can.CanHardwareObject[MsgObj_Num].CanObjectId=MsgObj_Num;
can.CanHardwareObject[MsgObj_Num].CanObjectType=TRANSMIT;

}

void configure_Receive_message_object(uint8 MsgObj_Num,uint32 id,uint32 Mask){
can.CanHardwareObject[MsgObj_Num].CanObjectId=MsgObj_Num;
can.CanHardwareObject[MsgObj_Num].CanObjectType=RECEIVE;
can.CanHardwareObject[MsgObj_Num].CanHwFilter.CanHwFilterCode=id;
can.CanHardwareObject[MsgObj_Num].CanHwFilter.CanHwFilterMask=Mask;
}
void Baude_rate(uint16 baudrate,uint16 ID,uint8 prop,uint8 phase1,uint8 phase2,uint8 sjw,uint8 i){
baud[i].CanControllerBaudRate=baudrate;
baud[i].CanControllerBaudRateConfigID=ID;
baud[i].CanControllerPropSeg=prop;
baud[i].CanControllerSeg1=phase1;
baud[i].CanControllerSeg2=phase2;
baud[i].CanControllerSyncJumpWidth=sjw;

}
void config_CanWrite_Data(uint16 id_,uint8 length_,uint8*data_,uint16 swpduHandle_){
PduType.id=id_;
PduType.length=length_;
PduType.sdu=data_;
PduType.swPduHandle=swpduHandle_;


}
void Port_Enable(void){
	int delay;
    SYSCTL_RCGCGPIO_R|=0x22; // port B
	delay=SYSCTL_RCGCGPIO_R;
    SYSCTL_RCGCCAN_R|=0x01;
    delay=SYSCTL_RCGCCAN_R;
	GPIO_PORTB_LOCK_R|=0x4C4F434B ;
	GPIO_PORTB_CR_R|=0x30 ;
    GPIO_PORTB_DIR_R|=0x20 ;     // output transmit
	GPIO_PORTB_DIR_R&=~0x10 ;    // input receive
	GPIO_PORTB_DEN_R|=0x30 ;
	GPIO_PORTB_AMSEL_R&=~0x30 ;
	GPIO_PORTB_AFSEL_R|=0x30 ;
	GPIO_PORTB_PCTL_R&=~0x00FF0000 ;
    GPIO_PORTB_PCTL_R|=0x00880000 ;
	//NVIC Init
	 NVIC_PRI9_R  &=~0XFF000000 ;  
	 NVIC_PRI9_R  |= 0X20000000 ;  // priority 1
	 NVIC_EN1_R|=0x80;      //Enable CAN0 interrupt 
	 //PORTF  
	
	/*GPIO_PORTF_LOCK_R|=0x4C4F434B ;
	GPIO_PORTF_CR_R|=0x17 ;
    GPIO_PORTF_DIR_R|=0x06 ;  //output
	GPIO_PORTF_DIR_R&=~0x11;  //Input
	
	GPIO_PORTF_DEN_R|=0x17 ;
	GPIO_PORTF_PUR_R|=0x11 ;
	GPIO_PORTF_AMSEL_R&=~0x17 ;
	GPIO_PORTF_AFSEL_R&=~0x17 ;
	GPIO_PORTF_PCTL_R&=~0x00F0FFF ;*/
  
}
void PLL_Init(void){
  // 0) Use RCC2
  SYSCTL_RCC2_R |=  0x80000000;  // USERCC2
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |=  0x00000800;  // BYPASS2, PLL bypass
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   // clear XTAL field, bits 10-6
                 + 0x00000540;   // 10101, configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~0x00002000;
  // 4) set the desired system divider
  SYSCTL_RCC2_R |= 0x40000000;   // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clear system clock divider
                  + (4<<22);      // configure for 80 MHz clock
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for PLLRIS bit
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~0x00000800;
}


void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr){
if(PduInfoPtr->SduDataPtr[0]==0x1){
GPIO_PORTF_DATA_R|=0x02;
} 
else GPIO_PORTF_DATA_R&=~0x02;
	
}