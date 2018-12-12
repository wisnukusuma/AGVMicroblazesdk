/*
 * AGVMainMotor_config.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVMAINMOTOR_CONFIG_H_
#define SRC_AGVMAINMOTOR_CONFIG_H_

#include "xparameters.h"

XAGVMainMotor_Config XAGVMainMotor_ConfigTable[XPAR_AGVMAINMOTOR_NUM_INSTANCES] =
{
	{
		XPAR_AGVMAINMOTOR_0_DEVICE_ID,
		XPAR_AGVMAINMOTOR_0_S00_AXI_BASEADDR
	}
};

#endif /* SRC_AGVMAINMOTOR_CONFIG_H_ */
