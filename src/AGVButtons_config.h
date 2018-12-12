/*
 * AGVButtons_config.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVBUTTONS_CONFIG_H_
#define SRC_AGVBUTTONS_CONFIG_H_

#include "xparameters.h"

XAGVButtons_Config XAGVButtons_ConfigTable[XPAR_AGVBUTTONS_NUM_INSTANCES] =
{
	{
		XPAR_AGVBUTTONS_0_DEVICE_ID,
		XPAR_AGVBUTTONS_0_S00_AXI_BASEADDR
	}
};

#endif /* SRC_AGVBUTTONS_CONFIG_H_ */
