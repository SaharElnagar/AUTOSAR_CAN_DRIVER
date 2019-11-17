

#ifndef CAN_H_
#define CAN_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "types.h"
#include "Can_GeneralTypes.h"
#include "hw_types.h"
#include "hw_CAN.h"
#include "hw_sysctrl.h"
#include "ComStack_Types.h"

 
/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */



#define SYSTEM_CLOCK_FREQ        80000 //in KHz
#define TQ_NUM                   16
#define Controllers_NUM          1
#define BaudRate_Config_NUM      4
#define HTH_Num                  16
#define HRH_Num                  16
#define Controller_MsgObj_NUM    32
#define CAN0                     0
#define CAN1                     1


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */





typedef enum{
CAN_OK ,CAN_NOT_OK ,CAN_BUSY 
}Can_ReturnType;

/** \brief structure holding the  initialization 
data for the CAN driver and SFR settings affecting all controllers.*/


/** \brief Specifies the type (Full-CAN or Basic-CAN) of a hardware object.*/
typedef enum {
	  BASIC     //For several L-PDUs are hadled by the hardware object
	 ,FULL      //For only one L-PDU (identifier) is handled by the hardware object
} canHandleType ;



/** \brief Specifies whether the IdValue is of type 
  * standard identifier 
  * extended identifier 
  * mixed mode
  **/
typedef enum {
	 EXTENDED,MIXED,STANDARD 
}canIdType ;
typedef enum {
	 RECEIVE ,TRANSMIT
}canObjectType ;
typedef enum {
	CAN_UNINIT ,
	CAN_READY 
}CanDriverStateType;


typedef enum {
INTERRUPT ,
POLLING
}gCanRxProcessing_t ;



                                           /******************************/


typedef struct canControllerBaudrateConfig{
	uint16 CanControllerBaudRate;         //Specifies the baudrate of the controller in kbps from 0 to 2000
	uint16 CanControllerBaudRateConfigID; //Uniquely identifies a specific baud rate configuration
  uint8  CanControllerPropSeg;           //Specifies propagation delay in time quantas
  uint8  CanControllerSeg1 ;             //Specifies phase segment 1 in time quantas.
  uint8  CanControllerSeg2   ;           //Specifies phase segment 2 in time quantas. 
  uint8  CanControllerSyncJumpWidth   ;  //Specifies the synchronization jump width for the controller in time quantas 
	
} CanControllerBaudrateConfig;


                                     /******************************/



/** \brief This container contains the configuration parameters of the CAN controller(s). */
typedef struct canController{

uint32	           CanControllerBaseAddress;
uint8              CanControllerId  ;
gCanRxProcessing_t CanRxProcessing;
gCanRxProcessing_t CanTxProcessing;
	
CanControllerBaudrateConfig *CanControllerDefaultBaudrate;
	
} gCanController_t;
                                /******************************/
typedef struct can_pduType{
PduIdType    swPduHandle;
uint8          length     ;
uint16         id         ;
uint8*         sdu        ;

}Can_PduType;
                            	        /******************************/
      typedef struct{
         uint32 CanHwFilterCode;
         uint32 CanHwFilterMask;
       } canHwFilter;
                             /******************************/


  /**\briefThis container contains the configuration (parameters) of CAN Hardware Objects.*/
   typedef struct canHardwareObject{
     canHandleType      CanHandleType ;      //Specifies the type (Full-CAN or Basic-CAN) of a hardware object.
     uint16             CanHwObjectCount ;   //Number of hardware objects used to implement one HOH
	   canIdType          CanIdType;           //Specifies whether the IdValue is of type 
	   uint16             CanObjectId;         //Holds the handle ID of HRH or HTH
     canObjectType      CanObjectType;       //Specifies if the HardwareObject is used as Transmit or as Receive object 
	   bool               CanTriggerTransmitEnable;
	   gCanController_t*  CanControllerRef; 
		 canHwFilter        CanHwFilter ;
  }gCanHardwareObject_t;
                              



                                     /******************************/



typedef struct can_ConfigType {
  gCanController_t     CanController[Controllers_NUM] 	  ;
	gCanHardwareObject_t CanHardwareObject[Controller_MsgObj_NUM]; 
}Can_ConfigType;
	
                                  /******************************/

/** \brief structure holding the CAN PDU_Types object.*/












	
/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/** \brief This function initializes the module.*/
void Can_init(const Can_ConfigType* obj);

/** \brief This function sets Baudrate the module.*/
/*Can_ReturnType*/void Can_SetBaudrate(  uint8 Controller, uint16 BaudRateConfigID );

Can_ReturnType Can_Write(  Can_HwHandleType Hth,  const Can_PduType* PduInfo ) ;

void Can_SetControllerMode( uint8 Controller,  Can_ControllerStateType Transition);

void Can_DisableControllerInterrupts(uint8 Controller );


void Can_EnableControllerInterrupts( uint8 Controller );

#endif /* APP_CAN_H_ */

