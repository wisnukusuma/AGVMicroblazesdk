/*
 * AGVEncoder_config.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVENCODER_CONFIG_H_
#define SRC_AGVENCODER_CONFIG_H_

#include "xparameters.h"

XAGVEncoder_Config XAGVEncoder_ConfigTable[XPAR_AGVENCODER_NUM_INSTANCES] =
{
	{
		XPAR_AGVENCODER_0_DEVICE_ID,
		XPAR_AGVENCODER_0_S00_AXI_BASEADDR
	}
};

#endif /* SRC_AGVENCODER_CONFIG_H_ */
