






#ifndef COMSTACK_TYPES_H_
#define COMSTACK_TYPES_H_

#define ECUC_SW_MAJOR_VERSION   1
#define ECUC_SW_MINOR_VERSION   0
#define ECUC_SW_PATCH_VERSION   0

#include "types.h"




typedef uint8 PduIdType;

typedef uint8 PduLengthType;

typedef struct {
	uint8 *SduDataPtr;			// payload
	PduLengthType SduLength;	// length of SDU
} PduInfoType;



#endif /*COMSTACK_TYPES_H_*/
