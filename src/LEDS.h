/*
 * LEDS.h
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */

#ifndef SRC_LEDS_H_
#define SRC_LEDS_H_

u8 InitializeLEDs();
void SetLEDs(u8 led1, u8 led2);
void SetLEDRGB(u8 red, u8 green, u8 blue);

#endif /* SRC_LEDS_H_ */
