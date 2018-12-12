/*
 * AGVScanSensor_l.c
 *
 *  Created on: Nov 18, 2018
 *      Author: frtri
 */

#include "AGVScanSensor_l.h"
#include "AGVScanSensor_config.h"
#include "xil_io.h"

XAGVScanSensor_Config *XAGVScanSensor_LookupConfig(u16 DeviceId)
{
	XAGVScanSensor_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_AGVSCANSENSOR_NUM_INSTANCES; Index++) {
		if (XAGVScanSensor_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAGVScanSensor_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

int XAGVScanSensor_CfgInitialize(XAGVScanSensor * InstancePtr, XAGVScanSensor_Config * Config,
			UINTPTR EffectiveAddr)
{
	/* Assert arguments */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/* Set some default values. */
	InstancePtr->BaseAddress = EffectiveAddr;

	/*
	 * Indicate the instance is now ready to use, initialized without error
	 */
	InstancePtr->IsReady = XIL_COMPONENT_IS_READY;
	return (XST_SUCCESS);
}

int XAGVScanSensor_Initialize(XAGVScanSensor * InstancePtr, u16 DeviceId)
{
	XAGVScanSensor_Config *ConfigPtr;

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */
	ConfigPtr = XAGVScanSensor_LookupConfig(DeviceId);
	if (ConfigPtr == (XAGVScanSensor_Config *) NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAGVScanSensor_CfgInitialize(InstancePtr, ConfigPtr,
				   ConfigPtr->BaseAddress);
}

