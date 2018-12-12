/*
 * AGVBrake_config.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVBRAKE_CONFIG_H_
#define SRC_AGVBRAKE_CONFIG_H_

#include "xparameters.h"

XAGVBrake_Config XAGVBrake_ConfigTable[XPAR_AGVBRAKE_NUM_INSTANCES] =
{
	{
		XPAR_AGVBRAKE_0_DEVICE_ID,
		XPAR_AGVBRAKE_0_S00_AXI_BASEADDR
	}
};

#endif /* SRC_AGVBRAKE_CONFIG_H_ */
