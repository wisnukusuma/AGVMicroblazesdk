/*
 * RFID.h
 *
 *  Created on: Nov 18, 2018
 *      Author: frtri
 */

#ifndef SRC_RFID_H_
#define SRC_RFID_H_

#include "xuartlite.h"
#include "xuartlite_l.h"

typedef struct _typeRFID
{
	u8 pos;
	int id;
	u8 buffer[128];
	u8 detect;
	u8 dataOK;
	u8 dataDevOK;
	u8 dataValid;
	u8 dataDevValid;
} typeRFID;

#endif /* SRC_RFID_H_ */
