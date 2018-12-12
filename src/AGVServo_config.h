/*
 * AGVServo_config.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVSERVO_CONFIG_H_
#define SRC_AGVSERVO_CONFIG_H_

#include "xparameters.h"

XAGVServo_Config XAGVServo_ConfigTable[XPAR_AGVSERVO_NUM_INSTANCES] =
{
	{
		XPAR_AGVSERVO_0_DEVICE_ID,
		XPAR_AGVSERVO_0_S00_AXI_BASEADDR
	}
};

#endif /* SRC_AGVSERVO_CONFIG_H_ */
