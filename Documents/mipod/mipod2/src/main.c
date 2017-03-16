#include <bma055.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include <stdio.h>
#include "debug.h"

#include "bus.h"
#include "stc3105.h"
//#include "rv3049.h"
#include "adxl375.h"
#include "bmx055.h"
#include "nrf51422.h"
#define LED_PIN 13
#define LED_ON   GPIO->P[gpioPortC].DOUTSET = (1 << LED_PIN)
#define LED_OFF  GPIO->P[gpioPortC].DOUTCLR = (1 << LED_PIN)
//#include "interfaces.h"

int main(void)
{
	uint8_t txData[12] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A, 0x0B};
	uint8_t rxData[12];
	uint8_t i = 0;
	uint8_t *buff;
	uint8_t dataAcc[6];
	uint8_t dataGyro[6];
	uint8_t dataAcc2[6];
	CHIP_Init();
	DEBUG_Init();
	//BMX055_Acc_GetData(dataAcc);
	//BMX055_Gyro_GetData(dataGyro);
	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1) ;
		  /* Initialize LED driver */
		  /* Infinite loop */
		CMU_ClockEnable(cmuClock_HFPER, true);
		CMU_ClockEnable(cmuClock_GPIO, true);
		GPIO_PinModeSet(gpioPortC, 13, gpioModePushPullDrive, 0);
		GPIO_PinModeSet(gpioPortA,14, gpioModeInput, 0);
		//BUS_Init(BMA055, true);
		LED_OFF;
		BUS_Init(NRF51422, true);
		BUS_Init(ADXL375,true);
		BUS_Init(BMX055_ACC,true);
		BUS_Init(BMX055_GYRO,true);
		BUS_Init(BMX055_MAG,true);
		ADXL375_Init(6,8);
		BMX055_Init(bmx055AccXAccYAccZ,
					bmx055GyroXGyroYGyroZ,
					bmx055MagXMagYMagZ,
					bmx055Acc125Hz,
					bmx055Gyro1000Hz116Hz,
					bmx055Mag30Hz,
					bmx055Acc_16G,
					bmx055Gyro_1000DPS);
		while(1)
		{
			//getDataAccXYZ(data);
			//getDataGyroXYZ();

			if(GPIO_PinInGet(gpioPortA,14))
			{
				LED_ON;
				txData[0] = i++;
				//ADXL375_GetData(dataAcc2);
				BMX055_Acc_GetData(dataAcc);
				printf("AX = %i",		(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)dataAcc[1] << 8) | dataAcc[0])));
				printf(" AY = %i",		(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)dataAcc[3] << 8) | dataAcc[2])));
				printf(" AZ = %i\n",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)dataAcc[5] << 8) | dataAcc[4])));
				for (uint8_t i = 0; i < 6 ; i++)
				{
					txData[i] = dataAcc[i];
					//buff[i]=0;
				}
				BMX055_Gyro_GetData(dataGyro);
				printf("GX = %i",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)dataGyro[1] << 8) | dataGyro[0])));
				printf(" GY = %i",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)dataGyro[3] << 8) | dataGyro[2])));
				printf(" GZ = %i\n",(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)dataGyro[5] << 8) | dataGyro[4])));

				for (uint8_t i = 0; i < 6 ; i++)
				{
					txData[i+6] = dataGyro[i];
					//buff[i]=0;
				}
			    BUS_SPI_TransferNBytes(NRF51422, txData, rxData, sizeof(txData));
			    //produce some delay for testing
			}

			else
			{
				LED_OFF;
			}
			for(volatile uint32_t i = 0; i < 10000; i++);
		}
}

