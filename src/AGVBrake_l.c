/*
 * AGVBrake_l.c
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#include "AGVBrake_l.h"
#include "AGVBrake_config.h"
#include "xil_io.h"

XAGVBrake_Config *XAGVBrake_LookupConfig(u16 DeviceId)
{
	XAGVBrake_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_AGVBRAKE_NUM_INSTANCES; Index++) {
		if (XAGVBrake_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAGVBrake_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

int XAGVBrake_CfgInitialize(XAGVBrake * InstancePtr, XAGVBrake_Config * Config,
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

int XAGVBrake_Initialize(XAGVBrake * InstancePtr, u16 DeviceId)
{
	XAGVBrake_Config *ConfigPtr;

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */
	ConfigPtr = XAGVBrake_LookupConfig(DeviceId);
	if (ConfigPtr == (XAGVBrake_Config *) NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAGVBrake_CfgInitialize(InstancePtr, ConfigPtr,
				   ConfigPtr->BaseAddress);
}

