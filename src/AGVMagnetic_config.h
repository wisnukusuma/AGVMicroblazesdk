/*
 * AGVMagnetic_config.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVMAGNETIC_CONFIG_H_
#define SRC_AGVMAGNETIC_CONFIG_H_

#include "xparameters.h"

XAGVMagnetic_Config XAGVMagnetic_ConfigTable[XPAR_AGVMAGNETIC_NUM_INSTANCES] =
{
	{
		XPAR_AGVMAGNETIC_0_DEVICE_ID,
		XPAR_AGVMAGNETIC_0_S00_AXI_BASEADDR
	}
};

#endif /* SRC_AGVMAGNETIC_CONFIG_H_ */
