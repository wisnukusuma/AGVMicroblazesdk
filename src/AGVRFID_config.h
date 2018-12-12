/*
 * AGVRFID_config.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_AGVRFID_CONFIG_H_
#define SRC_AGVRFID_CONFIG_H_

#include "xparameters.h"

XAGVRFID_Config XAGVRFID_ConfigTable[XPAR_AGVRFID_NUM_INSTANCES] =
{
	{
		XPAR_AGVRFID_0_DEVICE_ID,
		XPAR_AGVRFID_0_S00_AXI_BASEADDR
	}
};

#endif /* SRC_AGVRFID_CONFIG_H_ */
