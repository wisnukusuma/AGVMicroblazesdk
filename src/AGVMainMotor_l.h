/*
 * AGVMainMotor_l.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVMAINMOTOR_L_H_
#define SRC_AGVMAINMOTOR_L_H_

#include "AGVMainMotor.h"

typedef struct {
	u16 DeviceId;
	UINTPTR BaseAddress;	/* Device base address */
} XAGVMainMotor_Config;

typedef struct {
	UINTPTR BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
} XAGVMainMotor;

typedef struct _typeAGVMainMotor
{
	u16 pwmDrive;
	u8  forward;
	u8  sirine;
	u8  foot;
	u8 	dataOK;
} typeAGVMainMotor;

XAGVMainMotor_Config *XAGVMainMotor_LookupConfig(u16 DeviceId);
int XAGVMainMotor_CfgInitialize(XAGVMainMotor * InstancePtr, XAGVMainMotor_Config * Config,
			UINTPTR EffectiveAddr);
int XAGVMainMotor_Initialize(XAGVMainMotor * InstancePtr, u16 DeviceId);

#endif /* SRC_AGVMAINMOTOR_L_H_ */
