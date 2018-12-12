/*
 * AGVRFID_l.c
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#include "AGVRFID_l.h"
#include "AGVRFID_config.h"
#include "xil_io.h"

XAGVRFID_Config *XAGVRFID_LookupConfig(u16 DeviceId)
{
	XAGVRFID_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_AGVMAGNETIC_NUM_INSTANCES; Index++) {
		if (XAGVRFID_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XAGVRFID_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

int XAGVRFID_CfgInitialize(XAGVRFID * InstancePtr, XAGVRFID_Config * Config,
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

int XAGVRFID_Initialize(XAGVRFID * InstancePtr, u16 DeviceId)
{
	XAGVRFID_Config *ConfigPtr;

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver.
	 */
	ConfigPtr = XAGVRFID_LookupConfig(DeviceId);
	if (ConfigPtr == (XAGVRFID_Config *) NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAGVRFID_CfgInitialize(InstancePtr, ConfigPtr,
				   ConfigPtr->BaseAddress);
}


