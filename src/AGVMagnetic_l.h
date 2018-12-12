/*
 * AGVMagnetic_l.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVMAGNETIC_L_H_
#define SRC_AGVMAGNETIC_L_H_

#include "AGVMagnetic.h"

typedef struct {
	u16 DeviceId;
	UINTPTR BaseAddress;	/* Device base address */
} XAGVMagnetic_Config;

typedef struct {
	UINTPTR BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
} XAGVMagnetic;

typedef struct _typeAGVMagnetic
{
	u8 swMagnetic[15];
	float pos;
	u8 detected;
	u8 dataOK;
	u8 dataValid;
} typeAGVMagnetic;

XAGVMagnetic_Config *XAGVMagnetic_LookupConfig(u16 DeviceId);
int XAGVMagnetic_CfgInitialize(XAGVMagnetic * InstancePtr, XAGVMagnetic_Config * Config,
			UINTPTR EffectiveAddr);
int XAGVMagnetic_Initialize(XAGVMagnetic * InstancePtr, u16 DeviceId);

#endif /* SRC_AGVMAGNETIC_L_H_ */
