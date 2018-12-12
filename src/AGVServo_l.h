/*
 * AGVServo_l.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVSERVO_L_H_
#define SRC_AGVSERVO_L_H_

#include "AGVServo.h"

typedef struct {
	u16 DeviceId;
	UINTPTR BaseAddress;	/* Device base address */
} XAGVServo_Config;

typedef struct {
	UINTPTR BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
} XAGVServo;

typedef struct _typeAGVServo
{
	u16 pulse;
	u8  enable;
	u8  rightleft;
	u8  hlfb;
	u8 	dataOK;
	u8  dataValid;
} typeAGVServo;

XAGVServo_Config *XAGVServo_LookupConfig(u16 DeviceId);
int XAGVServo_CfgInitialize(XAGVServo * InstancePtr, XAGVServo_Config * Config,
			UINTPTR EffectiveAddr);
int XAGVServo_Initialize(XAGVServo * InstancePtr, u16 DeviceId);

#endif /* SRC_AGVSERVO_L_H_ */
