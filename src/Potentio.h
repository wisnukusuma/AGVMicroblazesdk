/*
 * Potentio.h
 *
 *  Created on: Nov 19, 2018
 *      Author: frtri
 */

#ifndef SRC_POTENTIO_H_
#define SRC_POTENTIO_H_

#include "xsysmon.h"

typedef struct _typePotentio
{
	u32 value;
	float deg;
	u8 dataOK;
	u8 dataValid;
} typePotentio;

#endif /* SRC_POTENTIO_H_ */
