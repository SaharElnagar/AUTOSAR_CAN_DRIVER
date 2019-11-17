#ifndef CANIF_CBK_H_
#define CANIF_CBK_H_
#include "can.h"

void CanIf_TxConfirmation(PduIdType CanTxPduId); // L-PDU id
void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);
void CanIf_ControllerBusOff(uint8 Controller);
void CanIf_ControllerModeIndication(uint8 Controller, Can_ControllerStateType ControllerMode);

//void CanIf_TrcvModeIndication(uint8 Transceiver, CanTrcv_TrcvModeType TransceiverMode);

#endif //CANIF_CBK_H_
