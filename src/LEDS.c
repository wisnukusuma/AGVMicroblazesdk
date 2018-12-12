/*
 * LEDS.c
 *
 *  Created on: Nov 17, 2018
 *      Author: frtri
 */
#include "xgpio.h"

static XGpio Gpio0; /* The Instance of the GPIO Driver 0 */
static XGpio Gpio1; /* The Instance of the GPIO Driver 1 */

u8 InitializeLEDs()
{
	int Status;

	Status = XGpio_Initialize(&Gpio0, XPAR_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return 0xff;
	}
	Status = XGpio_Initialize(&Gpio1, XPAR_GPIO_1_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return 0xfe;
	}
	XGpio_SetDataDirection(&Gpio0, 1, ~(0x03));
	XGpio_SetDataDirection(&Gpio1, 1, ~(0x07));

	return 0;
}

void SetLEDs(u8 led1, u8 led2)
{
	u32 mask = (led1&0x1) | (led2&0x1)<<1;
	XGpio_DiscreteWrite(&Gpio0, 1, mask);
}

void SetLEDRGB(u8 red, u8 green, u8 blue)
{
	u32 mask = (~red&0x1)<<2 | (~green&0x1)<<1 | (~blue&0x1);
	XGpio_DiscreteWrite(&Gpio1, 1, mask);
}


