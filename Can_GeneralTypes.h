#ifndef _CAN_GENERALTYPES_
#define _CAN_GENERALTYPES_
 #include "types.h"
typedef uint16 Can_IdType;
typedef uint8 Can_HwHandleType;

/** \brief States that are used by the several ControllerMode functions. */
typedef enum {
	CAN_CS_UNINIT ,
	CAN_CS_STARTED ,
	CAN_CS_STOPPED ,
	CAN_CS_SLEEP 
}Can_ControllerStateType;

/** \brief Error states of a CAN controller*/
typedef enum {
CAN_ERRORSTATE_ACTIVE,    // The CAN controller takes fully part in communication.
CAN_ERRORSTATE_PASSIVE,   //- The CAN controller takes part in communication, but does not send active error frames.
CAN_ERRORSTATE_BUSOFF     // The CAN controller does not take part in communication
}Can_ErrorStateType ;

typedef struct {

	Can_IdType  CanId ;
	Can_HwHandleType  Hoh;
	uint8  ControllerId ;

}Can_HwType;

#endif
