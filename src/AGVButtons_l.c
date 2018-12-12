/*
 * AGVButtons_l.c
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#include "AGVButtons_l.h"
#include "AGVButtons_config.h"
#include "xil_io.h"

XAGVButtons_Config *XAGVButtons_LookupConfig(u16 DeviceId)
{
	XAGVButtons_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_AGVBUTTONS_NUM_INSTANCES; Index++) {
		if (XAGVButtons_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAGVButtons_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

int XAGVButtons_CfgInitialize(XAGVButtons * InstancePtr, XAGVButtons_Config * Config,
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

int XAGVButtons_Initialize(XAGVButtons * InstancePtr, u16 DeviceId)
{
	XAGVButtons_Config *ConfigPtr;

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */
	ConfigPtr = XAGVButtons_LookupConfig(DeviceId);
	if (ConfigPtr == (XAGVButtons_Config *) NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAGVButtons_CfgInitialize(InstancePtr, ConfigPtr,
				   ConfigPtr->BaseAddress);
}

