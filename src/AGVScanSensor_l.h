/*
 * AGVScanSensor_l.h
 *
 *  Created on: Nov 18, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVSCANSENSOR_L_H_
#define SRC_AGVSCANSENSOR_L_H_

#include "AGVScanSensor.h"

typedef struct {
	u16 DeviceId;
	UINTPTR BaseAddress;	/* Device base address */
} XAGVScanSensor_Config;

typedef struct {
	UINTPTR BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
} XAGVScanSensor;

typedef struct _typeAGVScanSensor
{
	u8 scan[3];
	u8 trouble;
	u8 detected;
	u8 status;
	u8 dataOK;
	u8 dataValid;
} typeAGVScanSensor;

XAGVScanSensor_Config *XAGVScanSensor_LookupConfig(u16 DeviceId);
int XAGVScanSensor_CfgInitialize(XAGVScanSensor * InstancePtr, XAGVScanSensor_Config * Config,
			UINTPTR EffectiveAddr);
int XAGVScanSensor_Initialize(XAGVScanSensor * InstancePtr, u16 DeviceId);

#endif /* SRC_AGVSCANSENSOR_L_H_ */
