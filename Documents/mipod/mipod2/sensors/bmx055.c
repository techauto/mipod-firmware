/****************************************************************************************************************************************************
 ***   INCLUDES   ***********************************************************************************************************************************
 ***************************************************************************************************************************************************/

#include "BMX055.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

/* Default BMX055 parameter structure */
#define BMX055_PARAMS_DEFAULT				\
{											\
	false,									\
	false,									\
	false,									\
	bmx055AccXAccYAccZ,						\
	bmx055GyroXGyroYGyroZ,					\
	bmx055MagXMagYMagZ,						\
	bmx055Acc1000Hz,						\
	bmx055Gyro100Hz32Hz,					\
	bmx055Mag10Hz,							\
	bmx055Acc_16G,							\
	bmx055Gyro_125DPS,						\
	0,										\
	0,										\
	0,										\
}

SL_ALIGN(4)
SL_PACK_START(1)
static BMX055_Params __attribute__ ((aligned(4))) bmx055Params = BMX055_PARAMS_DEFAULT;
SL_PACK_END()

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

__STATIC_INLINE void spiWriteAcc(uint8_t ctrlReg, uint8_t *writeData, uint8_t len);
__STATIC_INLINE void spiWriteGyro(uint8_t ctrlReg, uint8_t *writeData, uint8_t len);
__STATIC_INLINE void spiWriteMag(uint8_t ctrlReg, uint8_t *writeData, uint8_t len);

__STATIC_INLINE void spiReadAcc(uint8_t ctrlReg, uint8_t *readData, uint8_t len);
__STATIC_INLINE void spiReadGyro(uint8_t ctrlReg, uint8_t *readData, uint8_t len);
__STATIC_INLINE void spiReadMag(uint8_t ctrlReg, uint8_t *readData, uint8_t len);

__STATIC_INLINE void getDeviceIdAcc(uint8_t *data);
__STATIC_INLINE void getDeviceIdGyro(uint8_t *data);
__STATIC_INLINE void getDeviceIdMag(uint8_t *data);

__STATIC_INLINE void setAccRange(BMX055_Acc_Range_TypeDef 	AccRange);
__STATIC_INLINE void setGyroRange(BMX055_Gyro_Range_TypeDef GyroRange);

__STATIC_INLINE void setAccFreq(BMX055_Acc_Frequency_TypeDef AccFreq);
__STATIC_INLINE void setGyroFreq(BMX055_Gyro_Frequency_TypeDef GyroFreq);
__STATIC_INLINE void setMagFreq(BMX055_Mag_Frequency_TypeDef MagFreq);

//__STATIC_INLINE void setPowerControl(BMX055_LinkActivities_TypeDef link, BMX055_Sleep_TypeDef sleep, BMX055_Standby_TypeDef standby);
//__STATIC_INLINE void setFrequency(BMX055_Frequency_TypeDef freq);
//__STATIC_INLINE void setDataFormat(BMX055_SelfTest_TypeDef st, BMX055_Spi_TypeDef spi, BMX055_IrqLevel_TypeDef irq, BMX055_Align_TypeDef align);
//__STATIC_INLINE void setFifoControl(BMX055_FifoMode_TypeDef mode, BMX055_FifoTriggerEvent_TypeDef trigger, uint8_t samples);

/***************************************************************************************************************************************************/
/****************************************************************************************************************************************************
 ***   FUNCTIONS   **********************************************************************************************************************************
 ***************************************************************************************************************************************************/
static volatile uint32_t msTicks; /* counts 1ms timeTicks */

static void Delay(uint32_t dlyTicks);

/**************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/

static void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

BMX055_Return_TypeDef BMX055_Init(BMX055_Acc_Sensors_TypeDef 		AccSensors,
								  BMX055_Gyro_Sensors_TypeDef 		GyroSensors,
								  BMX055_Mag_Sensors_TypeDef 		MagSensors,
								  BMX055_Acc_Frequency_TypeDef 		Accfreq,
								  BMX055_Gyro_Frequency_TypeDef 	Gyrofreq,
								  BMX055_Mag_Frequency_TypeDef 		Magfreq,
								  BMX055_Acc_Range_TypeDef			AccRange,
								  BMX055_Gyro_Range_TypeDef			GyroRange)
{

		uint8_t BMX_Power;
		BMX_Power=0;
		ResetBMX055();												//Reset BMX055
		Delay(1000);												// Wait for all registers to reset
		BMX055_Return_TypeDef retStatus;
		/* Set sensor selection */
		bmx055Params.AccSensors		=	AccSensors;
		bmx055Params.GyroSensors	=	GyroSensors;
		bmx055Params.MagSensors		=	MagSensors;

		/* Set sampling frequency */
		bmx055Params.Accfreq 		= 	Accfreq;
		bmx055Params.Gyrofreq 		= 	Gyrofreq;
		bmx055Params.Magfreq 		=	Magfreq;

		setAccFreq(Accfreq);							// Set accelerometer bandwidth
		setGyroFreq(bmx055Gyro1000Hz116Hz);				// Set Gyro bandwidth
		setMagFreq(Magfreq);							// Set Mag bandwidth

		/* Set bandwith */
		bmx055Params.AccRange		=	AccRange;
		bmx055Params.GyroRange		= 	GyroRange;
		setAccRange(AccRange);							// Set accelerometer range
		setGyroRange(GyroRange);				// Set Gyro range
		spiWriteAcc(BMX055_ACC_PMU_LPW,&BMX_Power,1);	// Use filtered data
		/* Get sensor ID */
		getDeviceIdAcc(&bmx055Params.AccID);
		/* Set initialization flag */
		if(bmx055Params.AccID == BMX055_ACC_ID)
		{
			retStatus = bmx055Ok;
			bmx055Params.initAcc = true;
		}
		else
		{
			retStatus = bmx055InitError;
			bmx055Params.initAcc = false;
		}
		/* Get sensor ID */
		getDeviceIdGyro(&bmx055Params.GyroID);
		/* Set initialization flag */
		if(bmx055Params.GyroID == BMX055_GYRO_ID)
		{
			retStatus = bmx055Ok;
			bmx055Params.initGyro = true;
		}
		else
		{
			retStatus = bmx055InitError;
			bmx055Params.initGyro = false;
		}
		/* Get sensor ID */
		getDeviceIdMag(&bmx055Params.MagID);
		/* Set initialization flag */
		if(bmx055Params.MagID == BMX055_MAG_ID)
		{
			retStatus = bmx055Ok;
			bmx055Params.initMag = true;
		}
		else
		{
			retStatus = bmx055InitError;
			bmx055Params.initMag = false;
		}
		/* Print debug information */
	#ifdef BMX055_DEBUGGING
		printf("BMX055_Init(): params.AccSensors 	= %i\n", (int)bmx055Params.AccSensors);
		printf("BMX055_Init(): params.Accfreq    	= %i\n", (int)bmx055Params.Accfreq);
		printf("BMX055_Init(): params.initAcc    	= %i\n", (int)bmx055Params.initAcc);
		printf("BMX055_Init(): params.AccID     	= %i\n", (int)bmx055Params.AccID);

		printf("BMX055_Init(): params.GyroSensors 	= %i\n", (int)bmx055Params.GyroSensors);
		printf("BMX055_Init(): params.Gyrofreq    	= %i\n", (int)bmx055Params.Gyrofreq);
		printf("BMX055_Init(): params.initGyro    	= %i\n", (int)bmx055Params.initGyro);
		printf("BMX055_Init(): params.GyroID     	= %i\n", (int)bmx055Params.GyroID);

		printf("BMX055_Init(): params.Magsensors 	= %i\n", (int)bmx055Params.MagSensors);
		printf("BMX055_Init(): params.Magfreq    	= %i\n", (int)bmx055Params.Magfreq);
		printf("BMX055_Init(): params.initMag    	= %i\n", (int)bmx055Params.initMag);
		printf("BMX055_Init(): params.MagID     	= %i\n", (int)bmx055Params.MagID);
	#endif

		/* Return status */
		return retStatus;
}
/***************************************************************************************************************************************************/
void ResetBMX055(void)
{
	spiReadAcc(BMX055_SOFTRESET, BMX055_RESET_VALUE, 1);

}
/***************************************************************************************************************************************************/
__STATIC_INLINE void getDeviceIdAcc(uint8_t *data)
{
	spiReadAcc(BMX055_ACC_WHOAMI, data, 1);

}
/***************************************************************************************************************************************************/
__STATIC_INLINE void getDeviceIdGyro(uint8_t *data)
{
	spiReadGyro(BMX055_GYRO_WHOAMI, data, 1);

}
/***************************************************************************************************************************************************/
__STATIC_INLINE void getDeviceIdMag(uint8_t *data)
{
	spiReadMag(BMX055_MAG_WHOAMI, data, 1);

}
/***************************************************************************************************************************************************/
__STATIC_INLINE void setAccFreq(BMX055_Acc_Frequency_TypeDef AccFreq)
{
	spiWriteAcc(BMX055_ACC_PMU_BW, &AccFreq, 1);

}
/***************************************************************************************************************************************************/
__STATIC_INLINE void setGyroFreq(BMX055_Gyro_Frequency_TypeDef GyroFreq)
{
	spiWriteMag(BMX055_GYRO_BW, &GyroFreq, 1);

}
/***************************************************************************************************************************************************/
__STATIC_INLINE void setMagFreq(BMX055_Mag_Frequency_TypeDef MagFreq)
{
	spiWriteMag(BMX055_MAG_PWR_CNTL2 , &MagFreq, 1);

}
/***************************************************************************************************************************************************/
__STATIC_INLINE void setAccRange(BMX055_Acc_Range_TypeDef AccRange)
{
	spiWriteAcc(BMX055_ACC_PMU_RANGE, &AccRange, 1);
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void setGyroRange(BMX055_Gyro_Range_TypeDef GyroRange)
{
	spiWriteGyro(BMX055_GYRO_RANGE, &GyroRange, 1);
}
/***************************************************************************************************************************************************/
__STATIC_INLINE void spiWriteAcc(uint8_t ctrlReg, uint8_t *writeData, uint8_t len)
{
	uint8_t txBuffer[len + 1];
	uint8_t rxBuffer[len + 1];

	/* Clear write indication bit */
	ctrlReg &= ~(1 << 7);

	/* Set multiple byte write bit */
	if(len > 1)
	{
		ctrlReg |= (1 << 6);
	}

	txBuffer[0] = ctrlReg;

	/* Move data buffers and transfer data */
	memcpy((uint8_t *)(txBuffer + 1), writeData, len);
	BUS_SPI_TransferNBytes(BMX055_ACC, txBuffer, rxBuffer, (len + 1));
}
/***************************************************************************************************************************************************/

__STATIC_INLINE void spiWriteGyro(uint8_t ctrlReg, uint8_t *writeData, uint8_t len)
{
	uint8_t txBuffer[len + 1];
	uint8_t rxBuffer[len + 1];

	/* Clear write indication bit */
	ctrlReg &= ~(1 << 7);

	/* Set multiple byte write bit */
	if(len > 1)
	{
		ctrlReg |= (1 << 6);
	}

	txBuffer[0] = ctrlReg;

	/* Move data buffers and transfer data */
	memcpy((uint8_t *)(txBuffer + 1), writeData, len);
	BUS_SPI_TransferNBytes(BMX055_GYRO, txBuffer, rxBuffer, (len + 1));
}
/***************************************************************************************************************************************************/

__STATIC_INLINE void spiWriteMag(uint8_t ctrlReg, uint8_t *writeData, uint8_t len)
{
	uint8_t txBuffer[len + 1];
	uint8_t rxBuffer[len + 1];

	/* Clear write indication bit */
	ctrlReg &= ~(1 << 7);

	/* Set multiple byte write bit */
	if(len > 1)
	{
		ctrlReg |= (1 << 6);
	}

	txBuffer[0] = ctrlReg;

	/* Move data buffers and transfer data */
	memcpy((uint8_t *)(txBuffer + 1), writeData, len);
	BUS_SPI_TransferNBytes(BMX055_MAG, txBuffer, rxBuffer, (len + 1));
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void spiReadAcc(uint8_t ctrlReg, uint8_t *readData, uint8_t len)
{
	uint8_t txBuffer[len + 1];
	uint8_t rxBuffer[len + 1];

	/* Set read indication bit */
	ctrlReg |= (1 << 7);

	/* Set multiple byte read bit */
	if(len > 1)
	{
		ctrlReg |= (1 << 6);
	}

	txBuffer[0] = ctrlReg;

	/* Move data buffers and transfer data */
	BUS_SPI_TransferNBytes(BMX055_ACC, txBuffer, rxBuffer, (len + 1));
	memcpy(readData, (uint8_t *)(rxBuffer + 1), len);
}
/***************************************************************************************************************************************************/
__STATIC_INLINE void spiReadGyro(uint8_t ctrlReg, uint8_t *readData, uint8_t len)
{
	uint8_t txBuffer[len + 1];
	uint8_t rxBuffer[len + 1];

	/* Set read indication bit */
	ctrlReg |= (1 << 7);

	/* Set multiple byte read bit */
	if(len > 1)
	{
		ctrlReg |= (1 << 6);
	}

	txBuffer[0] = ctrlReg;

	/* Move data buffers and transfer data */
	BUS_SPI_TransferNBytes(BMX055_GYRO, txBuffer, rxBuffer, (len + 1));
	memcpy(readData, (uint8_t *)(rxBuffer + 1), len);
}
/***************************************************************************************************************************************************/
__STATIC_INLINE void spiReadMag(uint8_t ctrlReg, uint8_t *readData, uint8_t len)
{
	uint8_t txBuffer[len + 1];
	uint8_t rxBuffer[len + 1];

	/* Set read indication bit */
	ctrlReg |= (1 << 7);

	/* Set multiple byte read bit */
	if(len > 1)
	{
		ctrlReg |= (1 << 6);
	}

	txBuffer[0] = ctrlReg;

	/* Move data buffers and transfer data */
	BUS_SPI_TransferNBytes(BMX055_MAG, txBuffer, rxBuffer, (len + 1));
	memcpy(readData, (uint8_t *)(rxBuffer + 1), len);
}
/***************************************************************************************************************************************************/
BMX055_Return_TypeDef BMX055_Acc_GetData(uint8_t *data)
{
	/* Data is coded in twos-complementary format  */
	/* ------------------------------------------- */
	/* |--AXL-|--AXH-|--AYL-|--AYH-|--AZL-|--AZH-| */
	/* |----0-|----1-|----2-|----3-|----4-|----5-| */

	uint8_t temp[BMX055_ACC_FRAME];
	/* Read appropriate data registers */
	switch(bmx055Params.AccSensors) {
		case(bmx055AccX):
			spiReadAcc(BMX055_ACC_D_X_LSB, temp, 2);
			memcpy(data, temp, 2);
			break;

		case(bmx055AccY):
			spiReadAcc(BMX055_ACC_D_Y_LSB, temp, 2);
			memcpy((uint8_t *)(data + 2), temp, 2);
			break;

		case(bmx055AccZ):
			spiReadAcc(BMX055_ACC_D_Z_LSB, temp, 2);
			memcpy((uint8_t *)(data + 4), temp, 2);
			break;

		case(bmx055AccXAccY):
			spiReadAcc(BMX055_ACC_D_X_LSB, temp, 4);
			memcpy(data, temp, 4);
			break;

		case(bmx055AccXAccZ):
			spiReadAcc(BMX055_ACC_D_X_LSB, temp, 6);
			memcpy(data, temp, 2);
			memcpy((uint8_t *)(data + 4), (uint8_t *)(temp + 4), 6);
			break;

		case(bmx055AccYAccZ):
			spiReadAcc(BMX055_ACC_D_Y_LSB, temp, 4);
			memcpy((uint8_t *)(data + 2), temp, 4);
			break;

		case(bmx055AccXAccYAccZ):
			//spiReadAcc(BMX055_ACC_D_X_LSB,&data[0], 6);
			spiReadAcc(BMX055_ACC_D_X_LSB,&data[0] , 1);
			spiReadAcc(BMX055_ACC_D_X_MSB,&data[1] , 1);
			spiReadAcc(BMX055_ACC_D_Y_LSB,&data[2] , 1);
			spiReadAcc(BMX055_ACC_D_Y_MSB,&data[3] , 1);
			spiReadAcc(BMX055_ACC_D_Z_LSB,&data[4] , 1);
			spiReadAcc(BMX055_ACC_D_Z_MSB,&data[5] , 1);
			//memcpy(data, temp, 6);
			printf("BMX055_ACC_GetData()");
			//GPIO_PinOutSet(gpioPortC, 13);
			break;

		case(bmx055AccNone):
			break;

		default:
			break;
	}

	/* Print debug information */
#ifdef BMX055_DEBUGGING
	int32_t x, y, z;
	x = (COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)data[1] << 8) | data[0]));
	y = (COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)data[3] << 8) | data[2]));
	z = (COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)data[5] << 8) | data[4]));
	printf("BMX055_ACC_GetData(): x = %i\n", (int)x);
	printf("BMX055_ACC_GetData(): y = %i\n", (int)y);
	printf("BMX055_ACC_GetData(): z = %i\n", (int)z);
#endif

	/* Return status */
	return bmx055Ok;
}
/***************************************************************************************************************************************************/
BMX055_Return_TypeDef BMX055_Gyro_GetData(uint8_t *data)
{
	/* Data is coded in twos-complementary format  */
	/* ------------------------------------------- */
	/* |--GXL-|--GXH-|--GYL-|--GYH-|--GZL-|--GZH-| */
	/* |----0-|----1-|----2-|----3-|----4-|----5-| */

	uint8_t temp[BMX055_GYRO_FRAME];

	/* Read appropriate data registers */
	switch(bmx055Params.GyroSensors) {
		case(bmx055GyroX):
			spiReadGyro(BMX055_GYRO_RATE_X_LSB, temp, 2);
			memcpy(data, temp, 2);
			break;

		case(bmx055GyroY):
			spiReadGyro(BMX055_GYRO_RATE_Y_LSB, temp, 2);
			memcpy((uint8_t *)(data + 2), temp, 2);
			break;

		case(bmx055GyroZ):
			spiReadGyro(BMX055_GYRO_RATE_Z_LSB, temp, 2);
			memcpy((uint8_t *)(data + 4), temp, 2);
			break;

		case(bmx055GyroXGyroY):
			spiReadGyro(BMX055_GYRO_RATE_X_LSB, temp, 4);
			memcpy(data, temp, 4);
			break;

		case(bmx055GyroXGyroZ):
			spiReadGyro(BMX055_GYRO_RATE_X_LSB, temp, 6);
			memcpy(data, temp, 2);
			memcpy((uint8_t *)(data + 4), (uint8_t *)(temp + 4), 6);
			break;

		case(bmx055GyroYGyroZ):
			spiReadGyro(BMX055_GYRO_RATE_Y_LSB, temp, 4);
			memcpy((uint8_t *)(data + 2), temp, 4);
			break;

		case(bmx055GyroXGyroYGyroZ):
//			spiReadGyro(BMX055_GYRO_RATE_X_LSB, temp, 6);
//			memcpy(data, temp, 6);
//			GPIO_PinOutSet(gpioPortC, 13);
			spiReadGyro(BMX055_GYRO_RATE_X_LSB, &data[0], 1);
			spiReadGyro(BMX055_GYRO_RATE_X_MSB, &data[1], 1);
			spiReadGyro(BMX055_GYRO_RATE_Y_LSB, &data[2], 1);
			spiReadGyro(BMX055_GYRO_RATE_Y_MSB, &data[3], 1);
			spiReadGyro(BMX055_GYRO_RATE_Z_LSB, &data[4], 1);
			spiReadGyro(BMX055_GYRO_RATE_Z_MSB, &data[5], 1);

			break;

		case(bmx055GyroNone):
			break;

		default:
			break;
	}

	/* Print debug information */
//#ifdef BMX055_DEBUGGING
//	int32_t x, y, z;
//	x = (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[1] << 8) | data[0]));
//	y = (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[3] << 8) | data[2]));
//	z = (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[5] << 8) | data[4]));
//	printf("BMX055_Gyro_GetData(): x = %i\n", (int)x);
//	printf("BMX055_Gyro_GetData(): y = %i\n", (int)y);
//	printf("BMX055_Gyro_GetData(): z = %i\n", (int)z);
//#endif

	/* Return status */
	return bmx055Ok;
}
/***************************************************************************************************************************************************/
BMX055_Return_TypeDef BMX055_Mag_GetData(uint8_t *data)
{
	/* Data is coded in twos-complementary format  */
	/* ------------------------------------------- */
	/* |--MXL-|--MXH-|--MYL-|--MYH-|--MZL-|--MZH-| */
	/* |----0-|----1-|----2-|----3-|----4-|----5-| */

	//uint8_t temp[BMX055_MAG_FRAME];

//	/* Read appropriate data registers */
//	switch(bmx055Params.MagSensors) {
//		case(bmx055MagX):
//			spiReadMag(BMX055_MAG_XOUT_LSB, temp, 2);
//			memcpy(data, temp, 2);
//			break;
//
//		case(bmx055MagY):
//			spiReadMag(BMX055_MAG_YOUT_LSB, temp, 2);
//			memcpy((uint8_t *)(data + 2), temp, 2);
//			break;
//
//		case(bmx055MagZ):
//			spiReadMag(BMX055_MAG_ZOUT_LSB, temp, 2);
//			memcpy((uint8_t *)(data + 4), temp, 2);
//			break;
//
//		case(bmx055MagXMagY):
//			spiReadMag(BMX055_MAG_XOUT_LSB, temp, 4);
//			memcpy(data, temp, 4);
//			break;
//
//		case(bmx055MagXMagZ):
//			spiReadMag(BMX055_MAG_XOUT_LSB, temp, 6);
//			memcpy(data, temp, 2);
//			memcpy((uint8_t *)(data + 4), (uint8_t *)(temp + 4), 6);
//			break;
//
//		case(bmx055MagYMagZ):
//			spiReadMag(BMX055_MAG_YOUT_LSB, temp, 4);
//			memcpy((uint8_t *)(data + 2), temp, 4);
//			break;
//
//		case(bmx055MagXMagYMagZ):
//			spiReadMag(BMX055_MAG_XOUT_LSB, temp, 6);
//			memcpy(data, temp, 6);
//			GPIO_PinOutSet(gpioPortC, 13);
//			break;
//
//		case(bmx055MagNone):
//			break;
//
//		default:
//			break;
//	}

	/* Print debug information */
//#ifdef BMX055_DEBUGGING
//	int32_t x, y, z;
//	x = (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[1] << 8) | data[0]));
//	y = (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[3] << 8) | data[2]));
//	z = (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[5] << 8) | data[4]));
//	printf("BMX055_Mag_GetData(): x = %i\n", (int)x);
//	printf("BMX055_Mag_GetData(): y = %i\n", (int)y);
//	printf("BMX055_Mag_GetData(): z = %i\n", (int)z);
//#endif

	/* Return status */
	return bmx055Ok;
}
