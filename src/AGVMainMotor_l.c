/*
 * AGVMainMotor_l.c
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#include "AGVMainMotor_l.h"
#include "AGVMainMotor_config.h"
#include "xil_io.h"

XAGVMainMotor_Config *XAGVMainMotor_LookupConfig(u16 DeviceId)
{
	XAGVMainMotor_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_AGVMAINMOTOR_NUM_INSTANCES; Index++) {
		if (XAGVMainMotor_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAGVMainMotor_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

int XAGVMainMotor_CfgInitialize(XAGVMainMotor * InstancePtr, XAGVMainMotor_Config * Config,
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

int XAGVMainMotor_Initialize(XAGVMainMotor * InstancePtr, u16 DeviceId)
{
	XAGVMainMotor_Config *ConfigPtr;

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */
	ConfigPtr = XAGVMainMotor_LookupConfig(DeviceId);
	if (ConfigPtr == (XAGVMainMotor_Config *) NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAGVMainMotor_CfgInitialize(InstancePtr, ConfigPtr,
				   ConfigPtr->BaseAddress);
}

