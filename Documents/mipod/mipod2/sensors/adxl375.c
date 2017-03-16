/****************************************************************************************************************************************************
 ***   INCLUDES   ***********************************************************************************************************************************
 ***************************************************************************************************************************************************/

#include "adxl375.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

/* Default ADXL375 parameter structure */
#define ADXL375_PARAMS_DEFAULT				\
{											\
	false,									\
	adxl375AccXAccYAccZ,					\
	adxl375LowPower200Hz,					\
	0,										\
}

SL_ALIGN(4)
SL_PACK_START(1)
static ADXL375_Params __attribute__ ((aligned(4))) adxl375Params = ADXL375_PARAMS_DEFAULT;
SL_PACK_END()

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

__STATIC_INLINE void spiWrite(uint8_t ctrlReg, uint8_t *writeData, uint8_t len);
__STATIC_INLINE void spiRead(uint8_t ctrlReg, uint8_t *readData, uint8_t len);

__STATIC_INLINE void getDeviceId(uint8_t *data);

__STATIC_INLINE void setPowerControl(ADXL375_LinkActivities_TypeDef link, ADXL375_Sleep_TypeDef sleep, ADXL375_Standby_TypeDef standby);
__STATIC_INLINE void setFrequency(ADXL375_Frequency_TypeDef freq);
__STATIC_INLINE void setDataFormat(ADXL375_SelfTest_TypeDef st, ADXL375_Spi_TypeDef spi, ADXL375_IrqLevel_TypeDef irq, ADXL375_Align_TypeDef align);
__STATIC_INLINE void setFifoControl(ADXL375_FifoMode_TypeDef mode, ADXL375_FifoTriggerEvent_TypeDef trigger, uint8_t samples);

/****************************************************************************************************************************************************
 ***   FUNCTIONS   **********************************************************************************************************************************
 ***************************************************************************************************************************************************/

ADXL375_Return_TypeDef ADXL375_Init(ADXL375_Sensors_TypeDef sensors, ADXL375_Frequency_TypeDef freq)
{
	ADXL375_Return_TypeDef retStatus;

	/* Set sensor selection */
	adxl375Params.sensors = sensors;
	/* Set sampling frequency */
	adxl375Params.freq = freq;

	/* Get sensor ID */
	getDeviceId(&adxl375Params.id);

	/* Set sensor parameter */
	setFrequency(freq);
	ADXL375_SetCalibrationOffsets(0, 0, 0);
	setDataFormat(adxl375SelfTestDisabled, adxl375Spi4Wires, adxl375IrqActiveHigh, adxl375RightAligend);
	setFifoControl(adxl375FifoBypass, adxl375TriggerEventInt0, 0);
	setPowerControl(adxl375LinkAvtivitiesConcurrent, adxl375SleepDisabled, adxl375Active);

	/* Set initialization flag */
	if(adxl375Params.id == ADXL375_DEVID_DEFAULT)
	{
		retStatus = adxl375Ok;
		adxl375Params.init = true;
	}
	else
	{
		retStatus = adxl375InitError;
		adxl375Params.init = false;
	}

	/* Print debug information */
#ifdef ADXL375_DEBUGGING
	printf("ADXL375_Init(): params.sensors = %i\n", (int)adxl375Params.sensors);
	printf("ADXL375_Init(): params.freq    = %i\n", (int)adxl375Params.freq);
	printf("ADXL375_Init(): params.init    = %i\n", (int)adxl375Params.init);
	printf("ADXL375_Init(): params.id      = %i\n", (int)adxl375Params.id);
#endif

	/* Return status */
	return retStatus;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_GetData(uint8_t *data)
{
	/* Data is coded in twos-complementary format  */
	/* ------------------------------------------- */
	/* |--AXL-|--AXH-|--AYL-|--AYH-|--AZL-|--AZH-| */
	/* |----0-|----1-|----2-|----3-|----4-|----5-| */

	uint8_t temp[ADXL375_FRAME];

	/* Read appropriate data registers */
	switch(adxl375Params.sensors) {
		case(adxl375AccX):
			spiRead(ADXL375_DATAX0_REG, temp, 2);
			memcpy(data, temp, 2);
			break;

		case(adxl375AccY):
			spiRead(ADXL375_DATAY0_REG, temp, 2);
			memcpy((uint8_t *)(data + 2), temp, 2);
			break;

		case(adxl375AccZ):
			spiRead(ADXL375_DATAZ0_REG, temp, 2);
			memcpy((uint8_t *)(data + 4), temp, 2);
			break;

		case(adxl375AccXAccY):
			spiRead(ADXL375_DATAX0_REG, temp, 4);
			memcpy(data, temp, 4);
			break;

		case(adxl375AccXAccZ):
			spiRead(ADXL375_DATAX0_REG, temp, 6);
			memcpy(data, temp, 2);
			memcpy((uint8_t *)(data + 4), (uint8_t *)(temp + 4), 6);
			break;

		case(adxl375AccYAccZ):
			spiRead(ADXL375_DATAY0_REG, temp, 4);
			memcpy((uint8_t *)(data + 2), temp, 4);
			break;

		case(adxl375AccXAccYAccZ):
			spiRead(ADXL375_DATAX0_REG, temp, 6);
			memcpy(data, temp, 6);
			GPIO_PinOutSet(gpioPortC, 13);
			break;

		case(adxl375None):
			break;
		default:
			break;
	}

	/* Print debug information */
#ifdef ADXL375_DEBUGGING
	int32_t x, y, z;
	x = (COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)data[1] << 8) | data[0]) * ADXL375_DATA_SCALE_FACTOR);
	y = (COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)data[3] << 8) | data[2]) * ADXL375_DATA_SCALE_FACTOR);
	z = (COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)data[5] << 8) | data[4]) * ADXL375_DATA_SCALE_FACTOR);
	printf("ADXL375_GetData(): x = %i\n", (int)x);
	printf("ADXL375_GetData(): y = %i\n", (int)y);
	printf("ADXL375_GetData(): z = %i\n", (int)z);
#endif

	/* Return status */
	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_SetCalibrationOffsets(int32_t offX, int32_t offY, int32_t offZ)
{
	/* Input format offX, offY, offZ in [mg] */

	uint8_t regOffX = (uint8_t)(COMMON_devisionToNearestInteger(offX, ADXL375_OFFSET_CONST));
	uint8_t regOffY = (uint8_t)(COMMON_devisionToNearestInteger(offY, ADXL375_OFFSET_CONST));
	uint8_t regOffZ = (uint8_t)(COMMON_devisionToNearestInteger(offZ, ADXL375_OFFSET_CONST));

	if((regOffX > 255) || (regOffY > 255) || (regOffZ > 255))
	{
		return adxl375OffsetError;
	}

	/* Print debug information */
#ifdef ADXL375_DEBUGGING
	printf("ADXL375_SetCalibrationOffsets(): xOff = %i\n", (int)regOffX);
	printf("ADXL375_SetCalibrationOffsets(): yOff = %i\n", (int)regOffY);
	printf("ADXL375_SetCalibrationOffsets(): zOff = %i\n", (int)regOffZ);
#endif

	spiWrite(ADXL375_OFSX_REG, &regOffX, 1);
	spiWrite(ADXL375_OFSY_REG, &regOffY, 1);
	spiWrite(ADXL375_OFSZ_REG, &regOffZ, 1);

	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_AutoCalibrate(void)
{
	uint32_t i;
	uint8_t data[ADXL375_FRAME];
	int32_t x = 0;
	int32_t y = 0;
	int32_t z = 0;

	/* Set sensor into standby */
	setPowerControl(adxl375LinkAvtivitiesConcurrent, adxl375SleepDisabled, adxl375Standby);
	/* Set data output rate to 100 Hz */
	setFrequency(adxl375Normal100Hz);
	/* Clear last calibration */
	ADXL375_SetCalibrationOffsets(0, 0, 0);
	/* Start measurement */
	setPowerControl(adxl375LinkAvtivitiesConcurrent, adxl375SleepDisabled, adxl375Active);

	/* Capture ADXL375_CALIBRATION_SAMPLES samples */
	for(i = 0; i < ADXL375_CALIBRATION_SAMPLES; i++)
	{
		ADXL375_GetData(data);
		x += (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[1] << 8) | data[0]) * ADXL375_DATA_SCALE_FACTOR);
		y += (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[3] << 8) | data[2]) * ADXL375_DATA_SCALE_FACTOR);
		z += (convert16bitComplementaryInto32bitSignedInt(((uint16_t)data[5] << 8) | data[4]) * ADXL375_DATA_SCALE_FACTOR);
	}
	x = devisionToNearestInteger(x, ADXL375_CALIBRATION_SAMPLES);
	y = devisionToNearestInteger(y, ADXL375_CALIBRATION_SAMPLES);
	z = devisionToNearestInteger(z, ADXL375_CALIBRATION_SAMPLES);

	/* Print debug information */
#ifdef ADXL375_DEBUGGING
	printf("ADXL375_AutoCalibrate(): xMean = %i\n", (int)x);
	printf("ADXL375_AutoCalibrate(): yMean = %i\n", (int)y);
	printf("ADXL375_AutoCalibrate(): zMean = %i\n", (int)z);
#endif

	/* Set sensor into standby */
	setPowerControl(adxl375LinkAvtivitiesConcurrent, adxl375SleepDisabled, adxl375Standby);
	/* Set data output rate back again */
	setFrequency(adxl375Params.freq);
	/* Clear last calibration */
	ADXL375_SetCalibrationOffsets(0 - x, 0 - y, 1000 - z);
	/* Start measurement */
	setPowerControl(adxl375LinkAvtivitiesConcurrent, adxl375SleepDisabled, adxl375Active);

	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_SetActivityDetection(
		ADXL375_ActivityAxes_TypeDef axesAct,
		ADXL375_InactivityAxes_TypeDef axesInact,
		int32_t actTh,
		int32_t inactTh,
		uint8_t inactTime,
		ADXL375_LinkActivities_TypeDef link,
		ADXL375_Sleep_TypeDef sleep,
		ADXL375_Standby_TypeDef standby)
{
	/* Input format actTh, inactTh in [mg] and inactTime in [s] */

	uint8_t regActTh = (uint8_t)(devisionToNearestInteger(actTh, ADXL375_ACT_INACT_TH_CONST));
	uint8_t regInactTh = (uint8_t)(devisionToNearestInteger(inactTh, ADXL375_ACT_INACT_TH_CONST));

	if((regActTh > 255) || (regInactTh > 255) || (inactTime > 255))
	{
		return adxl375ActivityThresholdError;
	}

	spiWrite(ADXL375_THRESH_ACT_REG, &regActTh, 1);
	spiWrite(ADXL375_THRESH_INACT_REG, &regInactTh, 1);
	spiWrite(ADXL375_TIME_INCAT_REG, &inactTime, 1);

	uint8_t data = axesAct | axesInact;
	spiWrite(ADXL375_ACT_INACT_CTL_REG, &data, 1);

	setPowerControl(link, sleep, standby);

	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_SetShockDetection(
		ADXL375_ShockDetectionMode_TypeDef mode,
		ADXL375_ShockAxes_TypeDef axes,
		int32_t shockTh,
		uint32_t duration,
		uint32_t latency,
		uint32_t window)
{
	/* Input format shockTh in [mg] and duration, latency, window in [ms] */

	uint8_t regShockTh = (uint8_t)(devisionToNearestInteger(shockTh, ADXL375_SHOCK_TH_CONST));
	uint8_t regDuration = (uint8_t)(devisionToNearestInteger(duration, ADXL375_SHOCK_DURATION_CONST));
	uint8_t regLatency = (uint8_t)(devisionToNearestInteger(latency, ADXL375_SHOCK_LATENCY_CONST));
	uint8_t regWindow = (uint8_t)(devisionToNearestInteger(window, ADXL375_SHOCK_WINDOW_CONST));

	if((regShockTh > 255) || (regDuration > 255) || (regLatency > 255) || (regWindow > 255))
	{
		return adxl375ShockThresholdError;
	}

	spiWrite(ADXL375_THRESH_SHOCK_REG, &regShockTh, 1);
	spiWrite(ADXL375_DUR_REG, &regDuration, 1);
	spiWrite(ADXL375_LATENT_REG, &regLatency, 1);
	spiWrite(ADXL375_WINDOW_REG, &regWindow, 1);

	uint8_t data = mode | axes;
	spiWrite(ADXL375_SHOCK_AXES_REG, &data, 1);

	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375SetInterruptMap(ADXL375_IrqMap_TypeDef irq, uint8_t pin)
{
	uint8_t data;

	if(pin == 1)
	{
		spiRead(ADXL375_INT_MAP_REG, &data, 1);
		data |= irq;
		spiWrite(ADXL375_INT_MAP_REG, &data, 1);
		return adxl375Ok;
	}
	else if(pin == 0)
	{
		spiRead(ADXL375_INT_MAP_REG, &data, 1);
		data &= ~irq;
		spiWrite(ADXL375_INT_MAP_REG, &data, 1);
		return adxl375Ok;
	}
	else
	{
		return adxl375IrqMapError;
	}
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_SetInterruptEnable(ADXL375_IrqEnable_TypeDef irq, bool en)
{
	uint8_t data;
	spiRead(ADXL375_INT_ENABLE_REG, &data, 1);

	if(en == true)
	{
		data |= irq;
		spiWrite(ADXL375_INT_ENABLE_REG, &data, 1);
	}
	else
	{
		data &= ~irq;
		spiWrite(ADXL375_INT_ENABLE_REG, &data, 1);
	}

	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_GetActivityShockSource(uint8_t *data)
{
	spiRead(ADXL375_ACT_SHOCK_STATUS_REG, data, 1);
	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_GetInterruptSource(uint8_t *data)
{
	spiRead(ADXL375_INT_SOURCE_REG, data, 1);
	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_GetFifoTriggerEvent(uint8_t *data)
{
	spiRead(ADXL375_FIFO_STATUS_REG, data, 1);
	data[0] |= __ADXL375_FIFO_TRIGGER_EVENT;
	return adxl375Ok;
}

/***************************************************************************************************************************************************/
ADXL375_Return_TypeDef ADXL375_GetFifoBytes(uint8_t *data)
{
	spiRead(ADXL375_FIFO_STATUS_REG, data, 1);
	data[0] |= __ADXL375_FIFO_SAMPLES;
	return adxl375Ok;
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void getDeviceId(uint8_t *data)
{
	spiRead(ADXL375_DEVID_REG, data, 1);
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void setFrequency(ADXL375_Frequency_TypeDef freq)
{
	spiWrite(ADXL375_BW_RATE_REG, &freq, 1);
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void setPowerControl(ADXL375_LinkActivities_TypeDef link, ADXL375_Sleep_TypeDef sleep, ADXL375_Standby_TypeDef standby)
{
	uint8_t data;

	spiRead(ADXL375_POWER_CTL_REG, &data, 1);
	data &= ~(1 << adxl375Standby);
	spiWrite(ADXL375_POWER_CTL_REG, &data, 1);

	data = link | sleep | standby;
	spiWrite(ADXL375_POWER_CTL_REG, &data, 1);
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void setDataFormat(ADXL375_SelfTest_TypeDef st, ADXL375_Spi_TypeDef spi, ADXL375_IrqLevel_TypeDef irq, ADXL375_Align_TypeDef align)
{
	uint8_t data = ADXL375_DATA_FORMAT_DEFAULT | st | spi | irq | align;
	spiWrite(ADXL375_DATA_FORMAT_REG, &data, 1);
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void setFifoControl(ADXL375_FifoMode_TypeDef mode, ADXL375_FifoTriggerEvent_TypeDef trigger, uint8_t samples)
{
	if(samples > 31)
	{
		return;
	}

	uint8_t data = mode | trigger | samples;
	spiWrite(ADXL375_FIFO_CTL_REG, &data, 1);
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void spiWrite(uint8_t ctrlReg, uint8_t *writeData, uint8_t len)
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
	BUS_SPI_TransferNBytes(ADXL375, txBuffer, rxBuffer, (len + 1));
}

/***************************************************************************************************************************************************/
__STATIC_INLINE void spiRead(uint8_t ctrlReg, uint8_t *readData, uint8_t len)
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
	BUS_SPI_TransferNBytes(ADXL375, txBuffer, rxBuffer, (len + 1));
	memcpy(readData, (uint8_t *)(rxBuffer + 1), len);
}
