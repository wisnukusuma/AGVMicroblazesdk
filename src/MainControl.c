/*
 * MainControl.c
 *
 *  Created on: Nov 15, 2018
 *      Author: frtri
 */

#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xintc.h"
#include "AGVButtons_l.h"
#include "AHRS.h"
#include "LEDS.h"
#include "AGVMagnetic_l.h"
#include "RFID.h"
#include "AGVRFID_l.h"
#include "AGVScanSensor_l.h"
#include "Potentio.h"
#include "AGVEncoder_l.h"
#include "AGVMainMotor_l.h"
#include "AGVServo_l.h"
#include "AGVBrake_l.h"

#define PI (3.141592653589793)

static XUartLite AGVAHRS;	/* The Instance of AGVAHARS */
static XUartLite AGVRFID;	/* The Instance of AGVRFID */
static XAGVRFID  AGVRFIDdev;	/* The Instance of AGVRFIDdev */
static XIntc InterruptController; /* The Instance of Interrupt */
static XAGVButtons AGVButtons;	/* The Instance of AGVButtons */
static XAGVMagnetic AGVMagnetic;	/* The Instance of AGVMagnetic */
static XAGVScanSensor AGVScanSensor; /* The Instance of AGVScanSensor */
static XSysMon AGVPotentio;	/* The Instance of AGVPotentio */
static XAGVEncoder AGVEncoder;	/* The Instance of AGVEncoder */
static XAGVMainMotor AGVMainMotor;	/* The Instance of AGVMainMotor */
static XAGVServo AGVServo;	/* The Instance of AGVServo */
static XAGVBrake AGVBrake;	/* The Instance of AGVBrake */

typeAGVButtons dataAGVButtons;
typeAHRS dataAHRS;
typeAGVMagnetic dataAGVMagnetic;
typeRFID dataRFID;
typeAGVScanSensor dataAGVScanSensor;
typePotentio dataPotentio;
typeAGVEncoder dataAGVEncoder;
typeAGVMainMotor dataAGVMainMotor;
typeAGVServo dataAGVServo;
typeAGVBrake dataAGVBrake;

void AHRSSetValue(u8 ch, u8 firstPos, u32 *data, u32 checkValue)
{
	dataAHRS.checksum ^= (((u32)ch)&0xff) << (((dataAHRS.pos-4) % 3) * 8);
	if (dataAHRS.indian == 1)
		*data |= (((u32)ch)&0xff) << ((dataAHRS.pos-firstPos) * 8);
	else
		*data |= (((u32)ch)&0xff) << ((firstPos+3-dataAHRS.pos) * 8);
	if (dataAHRS.pos == firstPos+3)
	{
		if (*data == checkValue)
			dataAHRS.pos++;
		else {
			dataAHRS.pos = 0;
			dataAHRS.dataStart = 0;
		}
	}
	else
		dataAHRS.pos++;
}

void AHRSSetValueN(u8 ch, u8 firstPos, u32 *data)
{
	dataAHRS.checksum ^= (((u32)ch)&0xff) << (((dataAHRS.pos-4) % 3) * 8);
	if (dataAHRS.indian == 1)
		*data |= (((u32)ch)&0xff) << ((dataAHRS.pos-firstPos) * 8);
	else
		*data |= (((u32)ch)&0xff) << ((firstPos+3-dataAHRS.pos) * 8);
	dataAHRS.pos++;
}

u32 *tempFloat;

void AHRSSetValueFloat(u8 ch, u8 firstPos, float *data)
{
	dataAHRS.checksum ^= (((u32)ch)&0xff) << (((dataAHRS.pos-4) % 3) * 8);
	if (dataAHRS.pos == firstPos) tempFloat = (u32 *)data;
	if (dataAHRS.indian == 1)
		*tempFloat |= (((u32)ch)&0xff) << ((dataAHRS.pos-firstPos) * 8);
	else
		*tempFloat |= (((u32)ch)&0xff) << ((firstPos+3-dataAHRS.pos) * 8);
	dataAHRS.pos++;
}

void InitializeAHRS()
{
	u8 configData[] = {0xff, 0x05, 0x00, 0x11, 0x0c, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00,
			0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x2a, 0x2b, 0x03, 0x02, 0x00, 0x00, 0x00,
			0x90, 0xfb, 0x52, 0x01, 0x74, 0xe6, 0xef, 0x00, 0xef, 0x06, 0xf1, 0x72, 0x90, 0xe6, 0xef, 0x00, 0xa4, 0xe6,
			0xef, 0x00, 0xe8, 0xe6, 0xef, 0x00, 0xe4, 0xe6, 0xef, 0x00, 0xb8, 0xe6, 0xef, 0x00, 0xad, 0x06, 0xf1, 0x72,
			0x78, 0xd8, 0x00, 0xfe};
	u8 stopData[] = {0xff, 0x06, 0x00, 0x11, 0xbb, 0xaa, 0xac, 0xfe};
	u8 getData[] = {0xff, 0x03, 0x02, 0x11, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xe8, 0x03, 0x00, 0x00, 0xbb, 0xab, 0x45, 0xfe};

	int i;
    XUartLite_Initialize(&AGVAHRS, XPAR_AXI_UARTLITE_1_DEVICE_ID);


	u16 dataNum = sizeof(configData);
	for (i=0; i<dataNum; i++)
	{
		XUartLite_SendByte(AGVAHRS.RegBaseAddress, configData[i]);
	}
	dataNum = sizeof(stopData);
	for (i=0; i<dataNum; i++)
	{
		XUartLite_SendByte(AGVAHRS.RegBaseAddress, stopData[i]);
	}
	dataNum = sizeof(getData);
	for (i=0; i<dataNum; i++)
	{
		XUartLite_SendByte(AGVAHRS.RegBaseAddress, getData[i]);
	}

	dataAHRS.dataStart = 0;
	dataAHRS.pos = 0;
	dataAHRS.dataOK = 0;
	dataAHRS.dataValid = 0;
}

void AHRSRecvHandler(void *CallBackRef, unsigned int EventData)
{
	XUartLite *ahrs = (XUartLite *)CallBackRef;
	u8 ch = XUartLite_RecvByte(ahrs->RegBaseAddress);

//	if (dataAHRS.dataOK == 1) return;

	if (ch == 0xff && dataAHRS.pos == 0)
	{
		dataAHRS.pos++;
		dataAHRS.dataStart = 1;
		dataAHRS.checking = 0;
		dataAHRS.checksum = 0x00AAAAAA;
		dataAHRS.dataCheck = 0;
		dataAHRS.dataID = 0;
		dataAHRS.dataSize = 0;
		dataAHRS.dataOffset = 0;
		dataAHRS.insStatus = 0;
		dataAHRS.hwStatus = 0;
		dataAHRS.dataCheck = 0;
		dataAHRS.fYaw = 0.0;
	}
	else if (dataAHRS.dataStart == 1)
	{
		if (dataAHRS.pos == 1)
		{
			dataAHRS.checksum ^= ((u32)ch)&0xff;
			dataAHRS.id = ch;
			if (ch == 4) {
				dataAHRS.pos++;
			}
			else {
				dataAHRS.pos = 0;
				dataAHRS.dataStart = 0;
			}
		}
		else if (dataAHRS.pos == 2)
		{
			dataAHRS.checksum ^= (((u32)ch)&0xff) << 8;
			dataAHRS.pos++;
		}
		else if (dataAHRS.pos == 3)
		{
			dataAHRS.checksum ^= (((u32)ch)&0xff) << 16;
			dataAHRS.pos++;
			if ((ch & 0x1) == 0x1) dataAHRS.indian = 1;
			else dataAHRS.indian = 0;
		}
		else if (dataAHRS.pos >= 4 && dataAHRS.pos <= 7)
		{
			AHRSSetValue(ch, 4, &dataAHRS.dataID, 4);
		}
		else if (dataAHRS.pos >= 8 && dataAHRS.pos <= 11)
		{
			AHRSSetValue(ch, 8, &dataAHRS.dataSize, 80);
		}
		else if (dataAHRS.pos >= 12 && dataAHRS.pos <= 15)
		{
			AHRSSetValue(ch, 12, &dataAHRS.dataOffset, 0);
		}
		else if (dataAHRS.pos >= 16 && dataAHRS.pos <= 27)
		{
			dataAHRS.checksum ^= (((u32)ch)&0xff) << (((dataAHRS.pos-4) % 3) * 8);
			dataAHRS.pos++;
		}
		else if (dataAHRS.pos >= 28 && dataAHRS.pos <= 31)
		{
			if (dataAHRS.pos == 28)
				dataAHRS.dataValid = 0;
			AHRSSetValueN(ch, 28, &dataAHRS.insStatus);
		}
		else if (dataAHRS.pos >= 32 && dataAHRS.pos <= 35)
		{
			AHRSSetValueN(ch, 32, &dataAHRS.hwStatus);
		}
		else if (dataAHRS.pos >= 36 && dataAHRS.pos <= 43)
		{
			dataAHRS.checksum ^= (((u32)ch)&0xff) << (((dataAHRS.pos-4) % 3) * 8);
			dataAHRS.pos++;
		}
		else if (dataAHRS.pos >= 44 && dataAHRS.pos <= 47)
		{
			AHRSSetValueFloat(ch, 44, &dataAHRS.fYaw);
		}
		else if (dataAHRS.pos >= 48 && dataAHRS.pos <= 95)
		{
			if (dataAHRS.pos == 48)
				dataAHRS.dataValid = 1;
			dataAHRS.checksum ^= (((u32)ch)&0xff) << (((dataAHRS.pos-4) % 3) * 8);
			if (dataAHRS.pos == 95)
			{
				dataAHRS.dataStart = 0;
				dataAHRS.checking = 1;
			}
			dataAHRS.pos++;
		}
	}
	else if (dataAHRS.checking == 1)
	{
		if (dataAHRS.pos >= 96 && dataAHRS.pos <= 98)
		{
			dataAHRS.dataCheck |= (((u32)ch)&0xff) << ((98-dataAHRS.pos) * 8);
			dataAHRS.pos++;
		}
		else if (dataAHRS.pos >= 99)
		{
			if (ch == 0xfe && dataAHRS.checksum == dataAHRS.dataCheck)
			{
				if (dataAHRS.insStatus & 0x00000011)
				{
					dataAHRS.dataOK = 1;
				}
			}
			dataAHRS.pos = 0;
			dataAHRS.checking = 0;
		}
	}
}

void GetDataAGVRFIDdev()
{
	dataRFID.detect = Xil_In32(AGVRFIDdev.BaseAddress + AGVRFID_S00_AXI_SLV_REG0_OFFSET);
}

void AGVRFIDdevRecvHandler(void *CallBackRef, unsigned int EventData)
{
//	if (dataRFID.dataDevOK == 1) return;
	dataRFID.dataDevValid = 0;
	GetDataAGVRFIDdev();
	dataRFID.dataDevOK = 1;
	dataRFID.dataDevValid = 1;
}

void InitializeRFID()
{
    XUartLite_Initialize(&AGVRFID, XPAR_AXI_UARTLITE_2_DEVICE_ID);

    dataRFID.detect = 0;
	dataRFID.pos = 0;
	dataRFID.dataOK = 0;
	dataRFID.dataValid = 0;
	GetDataAGVRFIDdev();
	dataRFID.dataDevOK = 1;
	dataRFID.dataDevValid = 1;
}

void RFIDRecvHandler(void *CallBackRef, unsigned int EventData)
{
	XUartLite *rfid = (XUartLite *)CallBackRef;
	u8 ch = XUartLite_RecvByte(rfid->RegBaseAddress);

//	if (dataRFID.dataOK == 1) return;

	if (ch == 0xa)
	{
		dataRFID.dataValid = 0;
		dataRFID.buffer[dataRFID.pos] = 0;
		dataRFID.pos = 0;
		dataRFID.dataOK = 1;
		dataRFID.id = atoi((const char*)dataRFID.buffer);
		dataRFID.dataValid = 1;
	}
	else
	{
		if (dataRFID.pos >= 127) dataRFID.pos = 0;
		dataRFID.buffer[dataRFID.pos++] = ch;
	}
}

void AGVButton_InterruptHandler(void *CallBackRef, unsigned int EventData)
{
//	if (dataAGVButtons.dataOK == 1) return;
	dataAGVButtons.dataValid = 0;

	XAGVButtons *button = (XAGVButtons *)CallBackRef;
	u32 buttonData = Xil_In32(button->BaseAddress + AGVBUTTONS_S00_AXI_SLV_REG0_OFFSET);

	dataAGVButtons.btnSafety = ((buttonData&0x1) == 1);
	dataAGVButtons.btnStart = (((buttonData>>1)&0x1) == 1);
	dataAGVButtons.btnStop = (((buttonData>>2)&0x1) == 1);
	dataAGVButtons.btnEmergency = (((buttonData>>3)&0x1) == 1);
	dataAGVButtons.modeAuto = (((buttonData>>4)&0x1) == 1);
	dataAGVButtons.dataOK = 1;

	dataAGVButtons.dataValid = 1;
}

void GetDataAGVButtons()
{
	u32 buttonData = Xil_In32(AGVButtons.BaseAddress + AGVBUTTONS_S00_AXI_SLV_REG0_OFFSET);

	dataAGVButtons.btnSafety = ((buttonData&0x1) == 1);
	dataAGVButtons.btnStart = (((buttonData>>1)&0x1) == 1);
	dataAGVButtons.btnStop = (((buttonData>>2)&0x1) == 1);
	dataAGVButtons.btnEmergency = (((buttonData>>3)&0x1) == 1);
	dataAGVButtons.modeAuto = (((buttonData>>4)&0x1) == 1);
}

void InitializeAGVButtons()
{
    XAGVButtons_Initialize(&AGVButtons, XPAR_AGVBUTTONS_0_DEVICE_ID);
	GetDataAGVButtons();
	dataAGVButtons.dataOK = 1;
	dataAGVButtons.dataValid = 1;
}

void GetDataAGVMagnetic()
{
	u32 magneticData = Xil_In32(AGVMagnetic.BaseAddress + AGVMAGNETIC_S00_AXI_SLV_REG0_OFFSET);

	dataAGVMagnetic.detected = 0;
	dataAGVMagnetic.pos = 0;
	int item = 0;
	for (int i=0; i<16;i++)
	{
		dataAGVMagnetic.swMagnetic[i] = (((magneticData>>i)&0x1) == 1);
		dataAGVMagnetic.detected |= dataAGVMagnetic.swMagnetic[i];
		if (dataAGVMagnetic.swMagnetic[i])
		{
			dataAGVMagnetic.pos += (float)(i-7);
			item++;
		}
	}
	if (dataAGVMagnetic.detected)
		dataAGVMagnetic.pos /= (float)item;
}

void InitializeAGVMagnetic()
{
    XAGVMagnetic_Initialize(&AGVMagnetic, XPAR_AGVMAGNETIC_0_DEVICE_ID);
	GetDataAGVMagnetic();
	dataAGVMagnetic.dataOK = 1;
	dataAGVMagnetic.dataValid = 1;
}

void AGVMagnetic_InterruptHandler(void *CallBackRef, unsigned int EventData)
{
//	if (dataAGVMagnetic.dataOK == 1) return;
	dataAGVMagnetic.dataValid = 0;

	GetDataAGVMagnetic();
	dataAGVMagnetic.dataOK = 1;
	dataAGVMagnetic.dataValid = 1;
}

void GetDataAGVScanSensor()
{
	u32 scansensorData = Xil_In32(AGVScanSensor.BaseAddress + AGVSCANSENSOR_S00_AXI_SLV_REG0_OFFSET);

	dataAGVScanSensor.detected = 0;
	for (int i=0; i<3;i++)
	{
		dataAGVScanSensor.scan[i] = (((scansensorData>>i)&0x1) == 1);
		dataAGVScanSensor.detected += dataAGVScanSensor.scan[i];
	}
	dataAGVScanSensor.trouble = (((scansensorData>>3)&0x1) == 1);
}

void AGVScanSensor_InterruptHandler(void *CallBackRef, unsigned int EventData)
{
//	if (dataAGVScanSensor.dataOK == 1) return;
	dataAGVScanSensor.dataValid = 0;

	GetDataAGVScanSensor();
	dataAGVScanSensor.dataOK = 1;
	dataAGVScanSensor.dataValid = 1;
}

void StopAGVScanSensor()
{
	dataAGVScanSensor.status = 0;
	Xil_Out32(AGVScanSensor.BaseAddress + AGVSCANSENSOR_S00_AXI_SLV_REG1_OFFSET, 0);
}

void InitializeAGVScanSensor()
{
    XAGVScanSensor_Initialize(&AGVScanSensor, XPAR_AGVSCANSENSOR_0_DEVICE_ID);
	StopAGVScanSensor();
	GetDataAGVScanSensor();
	dataAGVScanSensor.dataOK = 0;
	dataAGVScanSensor.dataValid = 0;
}

void LongDistanceSensor()
{
	dataAGVScanSensor.status = 1;
	Xil_Out32(AGVScanSensor.BaseAddress + AGVSCANSENSOR_S00_AXI_SLV_REG1_OFFSET, 2);
}


void MidDistanceSensor()
{
	dataAGVScanSensor.status = 2;
	Xil_Out32(AGVScanSensor.BaseAddress + AGVSCANSENSOR_S00_AXI_SLV_REG1_OFFSET, 4);
}

void Potentio_InterruptHandler(void *CallBackRef, unsigned int EventData)
{
//	if (dataPotentio.dataOK == 1) return;
	dataPotentio.dataValid = 0;

	dataPotentio.value = XSysMon_GetAdcData(&AGVPotentio, XSM_CH_AUX_MIN+4);
	dataPotentio.deg = (float)((int)dataPotentio.value-26780)/243.3;

	dataPotentio.dataOK = 1;
	dataPotentio.dataValid = 1;
}

void InitializePotentio()
{
	XSysMon_Config *xadc_config;
	xadc_config = XSysMon_LookupConfig(XPAR_SYSMON_0_DEVICE_ID);
	XSysMon_CfgInitialize(&AGVPotentio, xadc_config, xadc_config->BaseAddress);
	XSysMon_SetSeqChEnables(&AGVPotentio, XSM_SEQ_CH_AUX04);
	XSysMon_SetSequencerMode(&AGVPotentio, XSM_SEQ_MODE_SIMUL);

	dataPotentio.deg = 0;
	dataPotentio.value = 0;
	dataPotentio.dataOK = 0;
	dataPotentio.dataValid = 0;
}

void GetDataAGVEncoder()
{
	u32 encoderData = Xil_In32(AGVEncoder.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	dataAGVEncoder.value = encoderData;
	dataAGVEncoder.distance = (float)encoderData/3600.0 * 0.95;
}

void AGVEncoder_InterruptHandler(void *CallBackRef, unsigned int EventData)
{
//	if (dataAGVEncoder.dataOK == 1) return;
	dataAGVEncoder.dataValid = 0;

	GetDataAGVEncoder();
	dataAGVEncoder.dataOK = 1;
	dataAGVEncoder.dataValid = 1;
}

void AGVEncoder_reset()
{
	Xil_Out32(AGVEncoder.BaseAddress + AGVENCODER_S00_AXI_SLV_REG1_OFFSET, 1);
	Xil_Out32(AGVEncoder.BaseAddress + AGVENCODER_S00_AXI_SLV_REG1_OFFSET, 0);
}

void InitializeAGVEncoder()
{
    XAGVEncoder_Initialize(&AGVEncoder, XPAR_AGVENCODER_0_DEVICE_ID);

    AGVEncoder_reset();
	GetDataAGVEncoder();
	dataAGVEncoder.dataOK = 1;
	dataAGVEncoder.dataValid = 1;
}

void AGVMainMotor_scanlevel(u8 value)
{
	u32 mainmotor = Xil_In32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	if (((mainmotor >> 16) & 0x3) == value) return;
	mainmotor &= ~(0x30000);
	u8 val;
	if (value == 0) val = 0;
	else if (value == 1) val = 1;
	else if (value == 2) val = 3;
	else val = 0;
	mainmotor |= (val << 16);
}

void AGVMainMotor_drive(u16 value)
{
	u32 mainmotor = Xil_In32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	if ((mainmotor & 0xfff) == value) return;
	mainmotor &= ~(0xfff);
	mainmotor |= (value & 0xfff);
	dataAGVMainMotor.pwmDrive = value;
}

void AGVMainMotor_foot(u8 enable)
{
	u32 mainmotor = Xil_In32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	if (((mainmotor >> 14) & 0x1) == 1) return;
	if (enable)
	{
		mainmotor |= (1 << 14);
		dataAGVMainMotor.foot = 1;
	}
	else
	{
		mainmotor &= ~(1 << 14);
		dataAGVMainMotor.foot = 0;
	}
	Xil_Out32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, mainmotor);
}

void AGVMainMotor_sirine(u8 enable)
{
	u32 mainmotor = Xil_In32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	if (((mainmotor >> 13) & 0x1) == 1) return;
	if (enable)
	{
		mainmotor |= (1 << 13);
		dataAGVMainMotor.sirine = 1;
	}
	else
	{
		mainmotor &= ~(1 << 13);
		dataAGVMainMotor.sirine = 0;
	}
	Xil_Out32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, mainmotor);
}

void AGVMainMotor_forward(u8 enable)
{
	u32 mainmotor = Xil_In32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	if (((mainmotor >> 12) & 0x1) == 1) return;
	if (enable)
	{
		mainmotor |= (1 << 12);
		dataAGVMainMotor.forward = 1;
	}
	else
	{
		mainmotor &= ~(1 << 12);
		dataAGVMainMotor.forward = 0;
	}
	Xil_Out32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, mainmotor);
}

void AGVMainMotor_reset()
{
	Xil_Out32(AGVMainMotor.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, 0);
	dataAGVMainMotor.pwmDrive = 0;
	dataAGVMainMotor.forward = 0;
	dataAGVMainMotor.sirine = 0;
	dataAGVMainMotor.foot = 0;
}

void InitializeAGVMainMotor()
{
    XAGVMainMotor_Initialize(&AGVMainMotor, XPAR_AGVMAINMOTOR_0_DEVICE_ID);
    AGVMainMotor_reset();
    dataAGVMainMotor.dataOK = 0;
}

void GetDataAGVServo()
{
	u32 servo = Xil_In32(AGVServo.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);
	dataAGVServo.hlfb = (u8)(servo & 0x1);
}

void AGVServo_InterruptHandler(void *CallBackRef, unsigned int EventData)
{
	dataAGVServo.dataValid = 0;
	GetDataAGVServo();
	dataAGVServo.dataOK = 1;
	dataAGVServo.dataValid = 1;
}

void AGVServo_pulseout(u16 pulse)
{
	u32 servo = Xil_In32(AGVServo.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	servo &= ~(0xffff0000);
	servo |= (u32)(pulse << 16);

	Xil_Out32(AGVServo.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, servo);
}

void AGVServo_rightleft(u8 right)
{
	u32 servo = Xil_In32(AGVServo.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	if (right)
	{
		servo |= (1 << 9);
		dataAGVServo.rightleft = 1;
	}
	else
	{
		servo &= ~(1 << 9);
		dataAGVServo.rightleft = 0;
	}
	Xil_Out32(AGVServo.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, servo);
}

void AGVServo_enable(u8 enable)
{
	u32 servo = Xil_In32(AGVServo.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);

	if (enable)
	{
		servo |= (1 << 8);
		dataAGVServo.enable = 1;
	}
	else
	{
		servo &= ~(1 << 8);
		dataAGVServo.enable = 0;
	}
	Xil_Out32(AGVServo.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, servo);
}

void AGVServo_reset()
{
	Xil_Out32(AGVServo.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, 0);
	dataAGVServo.pulse = 0;
	dataAGVServo.enable = 0;
	dataAGVServo.rightleft = 0;
}

void InitializeAGVServo()
{
    XAGVServo_Initialize(&AGVServo, XPAR_AGVSERVO_0_DEVICE_ID);
    AGVServo_reset();
    GetDataAGVServo();
    dataAGVServo.dataOK = 1;
    dataAGVServo.dataValid = 1;
}

void GetDataAGVBrake()
{
	u32 brake = Xil_In32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);
	dataAGVBrake.alarm = (u8)(brake & 0x1);
}

void AGVBrake_InterruptHandler(void *CallBackRef, unsigned int EventData)
{
	dataAGVBrake.dataValid = 0;
	GetDataAGVBrake();
	dataAGVBrake.dataOK = 1;
	dataAGVBrake.dataValid = 1;
}

void AGVBrake_svon(u8 svon)
{
	u32 brake = Xil_In32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);
	if (svon)
	{
		brake |= (1 << 8);
		dataAGVBrake.svon = 1;
	}
	else
	{
		brake &= ~(1 << 8);
		dataAGVBrake.svon = 0;
	}
	Xil_Out32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, brake);
}

void AGVBrake_setup(u8 setup)
{
	u32 brake = Xil_In32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);
	if (setup)
	{
		brake |= (1 << 9);
		dataAGVBrake.setup = 1;
	}
	else
	{
		brake &= ~(1 << 9);
		dataAGVBrake.setup = 0;
	}
	Xil_Out32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, brake);
}

void AGVBrake_drive(u8 drive)
{
	u32 brake = Xil_In32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);
	if (drive)
	{
		brake |= (1 << 10);
		dataAGVBrake.drive = 1;
	}
	else
	{
		brake &= ~(1 << 10);
		dataAGVBrake.drive = 0;
	}
	Xil_Out32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, brake);
}

void AGVBrake_reset(u8 reset)
{
	u32 brake = Xil_In32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET);
	if (reset)
	{
		brake |= (1 << 11);
		dataAGVBrake.reset = 1;
	}
	else
	{
		brake &= ~(1 << 11);
		dataAGVBrake.reset = 0;
	}
	Xil_Out32(AGVBrake.BaseAddress + AGVENCODER_S00_AXI_SLV_REG0_OFFSET, brake);
}

void AGVBrake_home()
{
    GetDataAGVBrake();
    if (dataAGVBrake.alarm)
    {
    	AGVBrake_reset(1);
    	for (int i=0; i<10000; i++);
    }
	AGVBrake_reset(0);
	for (int i=0; i<10000; i++);
	AGVBrake_svon(1);
	AGVBrake_setup(1);
	for (int i=0; i<10000; i++);
	AGVBrake_svon(0);
	AGVBrake_setup(0);
}

void AGVBrake_brake()
{
	GetDataAGVBrake();
    if (dataAGVBrake.alarm)
    {
    	AGVBrake_reset(1);
    	for (int i=0; i<10000; i++);
    }
	AGVBrake_reset(0);
	for (int i=0; i<10000; i++);
	AGVBrake_svon(1);
	AGVBrake_drive(1);
	for (int i=0; i<10000; i++);
	AGVBrake_svon(0);
	AGVBrake_drive(0);
}

void InitializeAGVBrake()
{
    XAGVBrake_Initialize(&AGVBrake, XPAR_AGVBRAKE_0_DEVICE_ID);
    AGVBrake_home();
    dataAGVBrake.dataOK = 1;
    dataAGVBrake.dataValid = 1;
}

void SetupInterrupts()
{
    XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_UARTLITE_1_VEC_ID,
    			   (XInterruptHandler)XUartLite_InterruptHandler,
    			   (void *)&AGVAHRS);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID,
    			   (XInterruptHandler)XUartLite_InterruptHandler,
    			   (void *)&AGVRFID);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_AGVBUTTONS_0_VEC_ID,
			   (XInterruptHandler)AGVButton_InterruptHandler,
			   (void *)&AGVButtons);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_AGVMAGNETIC_0_VEC_ID,
			   (XInterruptHandler)AGVMagnetic_InterruptHandler,
			   (void *)&AGVMagnetic);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_AGVSCANSENSOR_0_VEC_ID,
			   (XInterruptHandler)AGVScanSensor_InterruptHandler,
			   (void *)&AGVScanSensor);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_AGVPOTENTIO_0_VEC_ID,
			   (XInterruptHandler)Potentio_InterruptHandler,
			   (void *)&AGVPotentio);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_AGVRFID_0_VEC_ID,
			   (XInterruptHandler)AGVRFIDdevRecvHandler,
			   (void *)&AGVRFID);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_AGVENCODER_0_VEC_ID,
			   (XInterruptHandler)AGVEncoder_InterruptHandler,
			   (void *)&AGVEncoder);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_AGVSERVO_0_VEC_ID,
			   (XInterruptHandler)AGVServo_InterruptHandler,
			   (void *)&AGVServo);
    XIntc_Connect(&InterruptController, XPAR_INTC_0_AGVBRAKE_0_VEC_ID,
			   (XInterruptHandler)AGVBrake_InterruptHandler,
			   (void *)&AGVBrake);

    XIntc_Enable(&InterruptController, XPAR_INTC_0_UARTLITE_1_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_UARTLITE_2_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_AGVBUTTONS_0_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_AGVMAGNETIC_0_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_AGVSCANSENSOR_0_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_AGVPOTENTIO_0_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_AGVRFID_0_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_AGVENCODER_0_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_AGVSERVO_0_VEC_ID);
    XIntc_Enable(&InterruptController, XPAR_INTC_0_AGVBRAKE_0_VEC_ID);
    XIntc_Start(&InterruptController, XIN_REAL_MODE);
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
    			 (Xil_ExceptionHandler)XIntc_InterruptHandler,
    			 &InterruptController);
    Xil_ExceptionEnable();
    XUartLite_SetRecvHandler(&AGVAHRS, AHRSRecvHandler, &AGVAHRS);
    XUartLite_EnableInterrupt(&AGVAHRS);
    XUartLite_SetRecvHandler(&AGVRFID, RFIDRecvHandler, &AGVRFID);
    XUartLite_EnableInterrupt(&AGVRFID);
}

void LaserScandirection()
{

}

void RFIDdirection()
{

}

void MagneticCalc()
{

}

void ServoDirect()
{

}

void AGVMainControl(u8 status)
{
	if ((status & 0x7) == 0x1)
	{
		AGVMainMotor_drive(1024);
	}
	else
	{
		AGVMainMotor_reset();
	}

	if (status & 0x8)
	{
		MagneticCalc();
	}

	if (status & 0x10)
	{
		RFIDdirection();
	}

	if (status & 0x20)
	{
		LaserScandirection();
	}

	if (status & 0x40)
	{
		ServoDirect();
	}
}

int main()
{
    u8 status;

    init_platform();

    print("AGV Main Control Systems\n\r");

    if ((status = InitializeLEDs()) != 0)
    {
    	if (status == 0xff)
    		printf("LEDs failed to initialize.\n\r");
    	else if (status == 0xfe)
    		printf("RGB failed to initialize.\n\r");
    	return -1;
    }
	SetLEDs(0, 0);
	SetLEDRGB(0, 0, 0);

	InitializeAHRS();
	InitializeAGVButtons();
	InitializeAGVMagnetic();
	InitializeRFID();
	InitializeAGVScanSensor();
	InitializePotentio();
	InitializeAGVEncoder();
	InitializeAGVMainMotor();
	InitializeAGVServo();
	InitializeAGVBrake();

	SetupInterrupts();

	int scanDistanceLevel = 1; // (0: Medium distance, 1: Long distance)
	int scanLevel = 0;	// (0: long,  1: middle, 2: short)
	int cntWait = 0;

	/*
	 * status variable for driving
	 * bit 0:	(0: steady,	1: run)
	 * bit 1:	(0: no, 1: emergency)
	 * bit 2:   (0: no, 1: safety bumper)
	 * bit 3:   (0: no, 1: magnetic detection)
	 * bit 4:   (0: no, 1: RFID detected)
	 * bit 5:	(0: no, 1: Laser detected)
	 * bit 6:	(0: no, 1: hlfb)
	 */
	status = 0;

	while(1)
	{
		if (dataAGVButtons.modeAuto && !dataAGVButtons.btnSafety &&
			!dataAGVButtons.btnEmergency &&
			dataAGVScanSensor.detected < (1 << scanLevel) &&
			dataAGVButtons.dataValid &&
			dataAGVScanSensor.dataValid)
		{
			if (!dataAGVMainMotor.dataOK)
				dataAGVMainMotor.dataOK = 1;

			AGVMainMotor_forward(1);
			AGVMainMotor_sirine(1);
			AGVMainMotor_scanlevel(scanLevel);

			if (cntWait > 20000)
				AGVMainMotor_foot(1);

			if (cntWait > 40000)
				AGVMainControl(status);

			if (cntWait < 100000)
				cntWait++;
		}
		else
		{
			if (dataAGVMainMotor.dataOK)
			{
				AGVMainMotor_reset();
				AGVServo_reset();
				dataAGVMainMotor.dataOK = 0;
			}
			cntWait = 0;
		}
		if (dataAGVServo.dataOK)
		{
			while(!dataAGVServo.dataValid);
			if (dataAGVServo.hlfb)
				status |= 0x40;
			else
				status &= ~0x40;
			dataAGVServo.dataOK = 0;
		}
		if (dataAGVButtons.dataOK)
		{
//			printf("Bumper Safety: %s, Start: %s, Stop: %s, Emergency: %s, Auto: %s\n\r",
//					(dataAGVButtons.btnSafety)?"On":"Off",
//					(dataAGVButtons.btnStart)?"On":"Off",
//					(dataAGVButtons.btnStop)?"On":"Off",
//					(dataAGVButtons.btnEmergency)?"On":"Off",
//					(dataAGVButtons.modeAuto)?"On":"Off"
//				);
			while(!dataAGVButtons.dataValid);
			if (dataAGVButtons.btnStart)
				status |= 0x1;
			if (dataAGVButtons.btnStop)
				status &= ~0x1;
			if (dataAGVButtons.btnEmergency)
			{
				status |= 0x2;
				AGVBrake_brake();
			}
			else
			{
				status &= ~0x2;
				AGVBrake_home();
			}
			if (dataAGVButtons.btnSafety)
				status |= 0x4;
			else
				status &= ~0x4;
			if (dataAGVButtons.modeAuto)
			{
				if (scanDistanceLevel == 1)
					LongDistanceSensor();
				else
					MidDistanceSensor();
				dataAGVMainMotor.dataOK = 1;
				status = 0;
			}
			else
			{
				StopAGVScanSensor();
				AGVMainMotor_reset();
			}
			dataAGVButtons.dataOK = 0;
		}
		if (dataAGVMagnetic.dataOK)
		{
			while(!dataAGVMagnetic.dataValid);
			if (dataAGVMagnetic.detected)
			{
//				printf("Magnetic pos : %f cm\n\r", dataAGVMagnetic.pos);
				status |= 0x8;
			}
			else
			{
				status &= ~0x8;
			}
			dataAGVMagnetic.dataOK = 0;
		}
		if (dataRFID.dataDevOK)
		{
//			printf("RFID: %s\n\r", (dataRFID.detect)?"detected":"not detected");
			while(!dataRFID.dataDevValid);
			if (dataRFID.detect)
				status |= 0x10;
			else
				status &= ~0x10;
			dataRFID.dataDevOK = 0;
		}
		if (dataRFID.dataOK)
		{
//			printf("RFID id: %d\n\r", dataRFID.id);
			while(!dataRFID.dataValid);
			dataRFID.dataOK = 0;
		}
		if (dataAGVScanSensor.dataOK)
		{
			while(!dataAGVScanSensor.dataValid);
/*			if (dataAGVScanSensor.status == 0)
			{
				printf("Scan sensor 0ff\n\r");
			}
			else if (dataAGVScanSensor.status == 1)
			{
				printf("Scan sensor: ");
				switch(dataAGVScanSensor.detected)
				{
				case 1:
					printf("< 1.9 m");
					break;
				case 2:
					printf("< 1.6 m");
					break;
				case 3:
					printf("< 1.3 m");
					break;
				default:
					printf("No detection");
					break;
				}
				printf("\n\r");
			}
			else if (dataAGVScanSensor.status == 2)
			{
				printf("Scan sensor: ");
				switch(dataAGVScanSensor.detected)
				{
				case 1:
					printf("< 1.2 m");
					break;
				case 2:
					printf("< 0.9 m");
					break;
				case 3:
					printf("< 0.4 m");
					break;
				default:
					printf("No detection");
					break;
				}
				printf("\n\r");
			}
			*/
			if (dataAGVScanSensor.detected)
				status |= 0x20;
			else
				status &= ~0x20;
			dataAGVScanSensor.dataOK = 0;
		}
		if (dataPotentio.dataOK)
		{
			while(!dataPotentio.dataValid);
//			printf("Potentio: %f deg\n\r", dataPotentio.deg);
			dataPotentio.dataOK = 0;
		}
		if (dataAGVEncoder.dataOK)
		{
			while(!dataAGVEncoder.dataValid);
			printf("Distance: %f m\n\r", dataAGVEncoder.distance);
			printf("Distance: %d m\n\r", dataAGVEncoder.value );
			dataAGVEncoder.dataOK = 0;
		}
		if (dataAHRS.dataOK)
		{
			while(!dataAHRS.dataValid);
//			printf("INS Status = %08lx; HW Status = %08lx; Yaw: %8.3f\r",
//				dataAHRS.insStatus, dataAHRS.hwStatus,
//				dataAHRS.fYaw / PI * 180.0);
			dataAHRS.dataOK = 0;
		}
	}

    cleanup_platform();
    return 0;
}
