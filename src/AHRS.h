/*
 * AHRS.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AHRS_H_
#define SRC_AHRS_H_

#include "xuartlite.h"
#include "xuartlite_l.h"

typedef struct _typeAHRS
{
	u8 pos;
	u8 dataStart;
	u8 id;
	u8 indian;
	u32 dataID;
	u32 dataSize;
	u32 dataOffset;
	u32 insStatus;
	u32 hwStatus;
	u32 dataCheck;
	float fYaw;
	u32 checksum;
	u8 checking;
	u8 dataOK;
	u8 dataValid;
} typeAHRS;

#endif /* SRC_AHRS_H_ */
