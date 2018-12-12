/*
 * AGVEncoder_l.c
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#include "AGVEncoder_l.h"
#include "AGVEncoder_config.h"
#include "xil_io.h"

XAGVEncoder_Config *XAGVEncoder_LookupConfig(u16 DeviceId)
{
	XAGVEncoder_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_AGVENCODER_NUM_INSTANCES; Index++) {
		if (XAGVEncoder_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAGVEncoder_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

int XAGVEncoder_CfgInitialize(XAGVEncoder * InstancePtr, XAGVEncoder_Config * Config,
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

int XAGVEncoder_Initialize(XAGVEncoder * InstancePtr, u16 DeviceId)
{
	XAGVEncoder_Config *ConfigPtr;

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */
	ConfigPtr = XAGVEncoder_LookupConfig(DeviceId);
	if (ConfigPtr == (XAGVEncoder_Config *) NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAGVEncoder_CfgInitialize(InstancePtr, ConfigPtr,
				   ConfigPtr->BaseAddress);
}

