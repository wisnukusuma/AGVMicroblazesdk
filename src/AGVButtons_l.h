/*
 * AGVButtons_l.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVBUTTONS_L_H_
#define SRC_AGVBUTTONS_L_H_

#include "AGVButtons.h"

typedef struct {
	u16 DeviceId;
	UINTPTR BaseAddress;	/* Device base address */
} XAGVButtons_Config;

typedef struct {
	UINTPTR BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
} XAGVButtons;

typedef struct _typeAGVButtons
{
	u8 btnSafety;
	u8 btnStart;
	u8 btnStop;
	u8 btnEmergency;
	u8 modeAuto;
	u8 dataValid;
	u8 dataOK;
} typeAGVButtons;

XAGVButtons_Config *XAGVButtons_LookupConfig(u16 DeviceId);
int XAGVButtons_CfgInitialize(XAGVButtons * InstancePtr, XAGVButtons_Config * Config,
			UINTPTR EffectiveAddr);
int XAGVButtons_Initialize(XAGVButtons * InstancePtr, u16 DeviceId);

#endif /* SRC_AGVBUTTONS_L_H_ */
