/*
 * AGVBrake_l.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVBRAKE_L_H_
#define SRC_AGVBRAKE_L_H_

#include "AGVBrake.h"

typedef struct {
	u16 DeviceId;
	UINTPTR BaseAddress;	/* Device base address */
} XAGVBrake_Config;

typedef struct {
	UINTPTR BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
} XAGVBrake;

typedef struct _typeAGVBrake
{
	u8	reset;
	u8	svon;
	u8	setup;
	u8	drive;
	u8	alarm;
	u8 	dataOK;
	u8  dataValid;
} typeAGVBrake;

XAGVBrake_Config *XAGVBrake_LookupConfig(u16 DeviceId);
int XAGVBrake_CfgInitialize(XAGVBrake * InstancePtr, XAGVBrake_Config * Config,
			UINTPTR EffectiveAddr);
int XAGVBrake_Initialize(XAGVBrake * InstancePtr, u16 DeviceId);

#endif /* SRC_AGVBRAKE_L_H_ */
