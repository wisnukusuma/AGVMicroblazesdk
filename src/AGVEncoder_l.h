/*
 * AGVEncoder_l.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVEncoder_L_H_
#define SRC_AGVEncoder_L_H_

#include "AGVEncoder.h"

typedef struct {
	u16 DeviceId;
	UINTPTR BaseAddress;	/* Device base address */
} XAGVEncoder_Config;

typedef struct {
	UINTPTR BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
} XAGVEncoder;

typedef struct _typeAGVEncoder
{
	u32 value;
	float distance;
	u8 dataOK;
	u8 dataValid;
} typeAGVEncoder;

XAGVEncoder_Config *XAGVEncoder_LookupConfig(u16 DeviceId);
int XAGVEncoder_CfgInitialize(XAGVEncoder * InstancePtr, XAGVEncoder_Config * Config,
			UINTPTR EffectiveAddr);
int XAGVEncoder_Initialize(XAGVEncoder * InstancePtr, u16 DeviceId);

#endif /* SRC_AGVEncoder_L_H_ */
