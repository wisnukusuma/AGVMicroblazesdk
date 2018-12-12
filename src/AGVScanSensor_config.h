/*
 * AGVScanSensor_config.h
 *
 *  Created on: Nov 18, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVSCANSENSOR_CONFIG_H_
#define SRC_AGVSCANSENSOR_CONFIG_H_

#include "xparameters.h"

XAGVScanSensor_Config XAGVScanSensor_ConfigTable[XPAR_AGVSCANSENSOR_NUM_INSTANCES] =
{
	{
		XPAR_AGVSCANSENSOR_0_DEVICE_ID,
		XPAR_AGVSCANSENSOR_0_S00_AXI_BASEADDR
	}
};

#endif /* SRC_AGVSCANSENSOR_CONFIG_H_ */
