/*
 * AGVRFID_l.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVRFID_L_H_
#define SRC_AGVRFID_L_H_

#include "AGVRFID.h"

typedef struct {
	u16 DeviceId;
	UINTPTR BaseAddress;	/* Device base address */
} XAGVRFID_Config;

typedef struct {
	UINTPTR BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
} XAGVRFID;

typedef struct _typeAGVRFID
{
	u8 detected;
	u8 dataOK;
	u8 dataValid;
} typeAGVRFID;

XAGVRFID_Config *XAGVRFID_LookupConfig(u16 DeviceId);
int XAGVRFID_CfgInitialize(XAGVRFID * InstancePtr, XAGVRFID_Config * Config,
			UINTPTR EffectiveAddr);
int XAGVRFID_Initialize(XAGVRFID * InstancePtr, u16 DeviceId);

#endif /* SRC_AGVRFID_L_H_ */
