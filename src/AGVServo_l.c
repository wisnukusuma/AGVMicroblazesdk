/*
 * AGVServo_l.c
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#include "AGVServo_l.h"
#include "AGVServo_config.h"
#include "xil_io.h"

XAGVServo_Config *XAGVServo_LookupConfig(u16 DeviceId)
{
	XAGVServo_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_AGVSERVO_NUM_INSTANCES; Index++) {
		if (XAGVServo_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAGVServo_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

int XAGVServo_CfgInitialize(XAGVServo * InstancePtr, XAGVServo_Config * Config,
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

int XAGVServo_Initialize(XAGVServo * InstancePtr, u16 DeviceId)
{
	XAGVServo_Config *ConfigPtr;

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */
	ConfigPtr = XAGVServo_LookupConfig(DeviceId);
	if (ConfigPtr == (XAGVServo_Config *) NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAGVServo_CfgInitialize(InstancePtr, ConfigPtr,
				   ConfigPtr->BaseAddress);
}

