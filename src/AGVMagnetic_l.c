/*
 * AGVMagnetic_l.c
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#include "AGVMagnetic_l.h"
#include "AGVMagnetic_config.h"
#include "xil_io.h"

XAGVMagnetic_Config *XAGVMagnetic_LookupConfig(u16 DeviceId)
{
	XAGVMagnetic_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_AGVMAGNETIC_NUM_INSTANCES; Index++) {
		if (XAGVMagnetic_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAGVMagnetic_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

int XAGVMagnetic_CfgInitialize(XAGVMagnetic * InstancePtr, XAGVMagnetic_Config * Config,
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

int XAGVMagnetic_Initialize(XAGVMagnetic * InstancePtr, u16 DeviceId)
{
	XAGVMagnetic_Config *ConfigPtr;

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */
	ConfigPtr = XAGVMagnetic_LookupConfig(DeviceId);
	if (ConfigPtr == (XAGVMagnetic_Config *) NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAGVMagnetic_CfgInitialize(InstancePtr, ConfigPtr,
				   ConfigPtr->BaseAddress);
}


