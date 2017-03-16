/************************************************************************************************************************************************//**
 * @file stc3105.c
 * @brief Hardware driver for the power measurement chip STC2105.
 * @version 1.0
 * @author Peter Blank
 ****************************************************************************************************************************************************
 * @section License
 * <b>TODO license need to be added here...</b>
 ***************************************************************************************************************************************************/

#include "stc3105.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

/**
 * @name Register configuration.
 *
 * @{
 */
/* Register addresses */
#define STC3105_MODE_REG					0x00		/**< Mode register. */
#define STC3105_CTRL_REG					0x01		/**< Control and status register. */
#define STC3105_SOC_LOW_REG					0x02		/**< Lower SOC register. */
#define STC3105_SOC_HI_REG					0x03		/**< Higher SOC register. */
#define STC3105_COUNTER_LOW_REG				0x04		/**< Lower number of conversions register. */
#define STC3105_COUNTER_HI_REG				0x05		/**< Higher number of conversions register. */
#define STC3105_CURRENT_LOW_REG				0x06		/**< Lower battery current value register. */
#define STC3105_CURRENT_HI_REG				0x07		/**< Higher battery current value register. */
#define STC3105_VOLTAGE_LOW_REG				0x08		/**< Lower battery voltage value register. */
#define STC3105_VOLTAGE_HI_REG				0x09		/**< Higher battery voltage value register. */
#define STC3105_SOC_BASE_LOW_REG			0x0A		/**< Lower SOC base value register. */
#define STC3105_SOC_BASE_HI_REG				0x0B		/**< Higher SOC base value register. */
#define STC3105_ALARM_SOC_LOW_REG			0x0C		/**< Lower SOC alarm level register. */
#define STC3105_ALARM_SOC_HI_REG			0x0D		/**< Higher SOC alarm level register. */
#define STC3105_ALARM_VOLTAGE_REG			0x0E		/**< Battery low voltage alarm level register. */
#define STC3105_CURRENT_THRES				0x0F		/**< Current threshold for the voltage relaxation counter register. */
#define STC3105_RELAX_COUNT_REG				0x10		/**< Voltage relaxation counter register. */
#define STC3105_DEVICE_ID_REG				0x18		/**< Part type ID register. */
/* Default register values */
#define STC3105_MODE_REG_DEFAULT			0b11000001	/**< Default mode register value 0xC1. */
#define STC3105_CTRL_REG_DEFAULT			0b10000000	/**< Default control register value 0x80. */
/* CTRL register bit masks */
#define _STC3105_ALM_PIN_STATUS_BIT			(0x01 << 0)	/**< Alarm pin status bit mask. */
#define _STC3105_GG_EOC_BIT					(0x01 << 2)	/**< End of battery current conversion bit mask. */
#define _STC3105_VM_EOC_BIT					(0x01 << 3)	/**< End of battery voltage conversion bit mask. */
#define _STC3105_ALM_SOC_BIT				(0x01 << 5)	/**< Low SOC alarm detection bit mask. */
#define _STC3105_ALM_VOLT_BIT				(0x01 << 6)	/**< Low voltage alarm detection bit mask. */
/**
 * @}
 */

/**
 * @brief STC3105 calculation constants.
 *
 * @{
 */
#define STC3105_CURRENT_CONSTANT			(11770 / STC3105_SHUNT)					/**< Current constant in [uA]. */
#define STC3105_VOLTAGE_CONSTANT			2440									/**< Voltage constant in [uV]. */
#define STC3105_SOC_CONSTANT				(6700 / STC3105_SHUNT)					/**< SOC constant in [uAh]. */
#define STC3105_SOC_BASE_CONSTANT			(STC3105_SOC_CONSTANT / STC3105_SHUNT)	/**< SOC base constant in [uAh]. */
#define STC3105_RELAXATION_TIME_CONSTANT	8										/**< Relaxation constant in [s]. */
/**
 * @}
 */

/**
 * @brief STC3105 data frame size.
 *
 * @{
 */
#define STC3105_RAW_FRAME					8	/**< Number of bytes for raw data acquisition. */
#define STC3105_CONV_FRAME					12	/**< Number of bytes for converted data acquisition. */
/**
 * @}
 */

/**
 * @brief STC3105 parameters.
 *
 * This structure indicates the global parameters of the STC3105 chip.
 */
SL_PACK_START(1)
typedef struct
{
	bool 								init;
	uint8_t								id;
	STC3105_Sensors_TypeDef				sensors;
	STC3105_PowerMode_TypeDef			powerMode;
	STC3105_RunMode_TypeDef				runMode;
	STC3105_Alarm_TypeDef				alarm;
	STC3105_AlarmPinOutput_TypeDef		alarmPinOutput;
} __attribute__ ((packed)) STC3105_Params;
SL_PACK_END()

/**
 * @brief STC3105 default parameters.
 *
 * This macro indicates the default global parameters of the STC3105 chip.
 */
#define STC3105_PARAMS_DEFAULT \
{ \
	false,								/**< Disabled at the beginning of initialization. */ \
	0x00,								/**< No ID set at the beginning of initialization. */ \
	stc3105SocCurrVolt,					/**< Set to record all internal sensors. */ \
	stc3105ActiceMode,					/**< Set to active measurement mode. */ \
	stc3105OperatingMode,				/**< Set to operating mode. */ \
	stc3105AlarmDisabled,				/**< Alarm mode disabled. */ \
	stc3105AlarmPinOuputByConditions,	/**< Alarm pin is controlled by alarm conditions. */ \
}

SL_ALIGN(4)
SL_PACK_START(1)
static STC3105_Params __attribute__ ((aligned(4))) stc3105Params = STC3105_PARAMS_DEFAULT;
SL_PACK_END()

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

static inline void convert(uint8_t *data);

/****************************************************************************************************************************************************
 ***   FUNCTIONS   **********************************************************************************************************************************
 ***************************************************************************************************************************************************/

/**
 * @brief Initialization.
 *
 * This function initializes the STC3105 power measurement chip by setting the internal sensors and different modes.
 *
 * @param STC3105_Sensors_TypeDef sensors		internal sensors
 * @param STC3105_PowerMode_TypeDef powerMode	power mode
 * @param STC3105_Mode_TypeDef mode				operating mode
 * @param STC3105_Alarm_TypeDef alarm			alarm type
 * @return STC3105_Return_TypeDef				return status
 */
STC3105_Return_TypeDef STC3105_InitDefault(void)
{
	/* Read ID */
	if(BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_DEVICE_ID_REG, &stc3105Params.id, 1) != i2cTransferDone)
	{
		return stc3105InitError;
	}

	/* Write control register */
	uint8_t ctrlReg = STC3105_CTRL_REG_DEFAULT | stc3105Params.alarmPinOutput;
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_CTRL_REG, &ctrlReg, 1) != i2cTransferDone)
	{
		return stc3105InitError;
	}

	/* Write mode register */
	uint8_t modeReg = STC3105_MODE_REG_DEFAULT | stc3105Params.powerMode | stc3105Params.alarm | stc3105Params.runMode;
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_MODE_REG, &modeReg, 1) != i2cTransferDone)
	{
		return stc3105InitError;
	}

	/* Initialization done */
	stc3105Params.init = true;

	/* Print debug information */
#ifdef STC3105_DEBUG
	printf("STC3105_Init(): params.sensors        = %i\n",(int)stc3105Params.sensors);
	printf("STC3105_Init(): params.alarm          = %i\n",(int)stc3105Params.alarm);
	printf("STC3105_Init(): params.powerMode      = %i\n",(int)stc3105Params.powerMode);
	printf("STC3105_Init(): params.runMode        = %i\n",(int)stc3105Params.runMode);
	printf("STC3105_Init(): params.alarmPinOutput = %i\n",(int)stc3105Params.alarmPinOutput);
	printf("STC3105_Init(): params.init           = %i\n",(int)stc3105Params.init);
	printf("STC3105_Init(): params.id             = 0x%X\n",(int)stc3105Params.id);
#endif

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Control operating mode.
 *
 * This function controls the operating mode of the STC3105 chip.
 *
 * @param STC3105_Mode_TypeDef mode		selected mode type
 * @return STC3105_Return_TypeDef		return status
 */
STC3105_Return_TypeDef STC3105_SetOperatingMode(STC3105_RunMode_TypeDef runMode)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	/* Set operating mode */
	stc3105Params.runMode = runMode;

	/* Write mode register */
	uint8_t modeReg = STC3105_MODE_REG_DEFAULT | stc3105Params.powerMode | stc3105Params.alarm | stc3105Params.runMode;
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_MODE_REG, &modeReg, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Control of reset.
 *
 * This function controls the reset mode of the STC3105 chip.
 *
 * @param STC3105_Reset_TypeDef reset	selected reset type
 * @return STC3105_Return_TypeDef		return status
 */
STC3105_Return_TypeDef STC3105_Reset(STC3105_Reset_TypeDef reset)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	/* Write control register */
	uint8_t ctrlReg = STC3105_CTRL_REG_DEFAULT | stc3105Params.alarmPinOutput | reset;
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_CTRL_REG, &ctrlReg, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Sensor settings.
 *
 * This function sets the internal sensors of the STC3105 chip.
 *
 * @param STC3105_Sensors_TypeDef sensors	selected sensors type
 * @return STC3105_Return_TypeDef			return status
 */
STC3105_Return_TypeDef STC3105_SetSensors(STC3105_Sensors_TypeDef sensors)
{
	/* Set sensors */
	stc3105Params.sensors = sensors;

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Power mode settings.
 *
 * This function sets the power mode of the STC3105 chip.
 *
 * @param STC3105_PowerMode_TypeDef powerMode	selected power mode type
 * @return STC3105_Return_TypeDef				return status
 */
STC3105_Return_TypeDef STC3105_SetPowerMode(STC3105_PowerMode_TypeDef powerMode)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	/* Set power mode */
	stc3105Params.powerMode = powerMode;

	/* Write mode register */
	uint8_t modeReg = STC3105_MODE_REG_DEFAULT | stc3105Params.powerMode | stc3105Params.alarm | stc3105Params.runMode;
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_MODE_REG, &modeReg, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Alarm settings.
 *
 * This function sets the settings for the alarm and the alarm output pin.
 *
 * @param STC3105_Alarm_TypeDef alarm			selected alarm type
 * @param STC3105_AlarmPinOutput_TypeDef pin	selected pin output type
 * @return STC3105_Return_TypeDef				return status
 */
STC3105_Return_TypeDef STC3105_SetAlarm(STC3105_Alarm_TypeDef alarm, STC3105_AlarmPinOutput_TypeDef pin)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	/* Set alarm mode and alarm output pin */
	stc3105Params.alarm = alarm;
	stc3105Params.alarmPinOutput = pin;

	/* Write mode register */
	uint8_t modeReg = STC3105_MODE_REG_DEFAULT | stc3105Params.powerMode | stc3105Params.alarm | stc3105Params.runMode;
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_MODE_REG, &modeReg, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Write control register */
	uint8_t ctrlReg = STC3105_CTRL_REG_DEFAULT | stc3105Params.alarmPinOutput;
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_CTRL_REG, &ctrlReg, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief SOC base level settings.
 *
 * This function sets the base level of the SOC of the STC3105 chip.
 * This value is automatically added to the accumulated SOC data registers.
 *
 * @param uint16_t data				SOC base level in [?]
 * @return STC3105_Return_TypeDef	return status
 */
STC3105_Return_TypeDef STC3105_SetSOCBaseLevel(uint16_t data)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	/* Set SOC base level */
	uint8_t SOCBase[2];
	SOCBase[0] = (uint8_t)((uint8_t)(data) & 0xFF);
	SOCBase[1] = (uint8_t)((uint8_t)(data >> 8) & 0xFF);

	/* Write SOC base registers */
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_SOC_BASE_LOW_REG, SOCBase, 2) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Low SOC alarm level settings.
 *
 * This function sets the SOC alarm level of the STC3105 chip.
 * If the SOC level falls below this threshold an alarm interrupt is generated.
 *
 * @param uint16_t data				SOC alarm level in [?]
 * @return STC3105_Return_TypeDef	return status
 */
STC3105_Return_TypeDef STC3105_SetLowSOCAlarmLevel(uint16_t data)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	/* Set SOC alarm level */
	uint8_t SOCAlarm[2];
	SOCAlarm[0] = (uint8_t)((uint8_t)(data) & 0xFF);
	SOCAlarm[1] = (uint8_t)((uint8_t)(data >> 8) & 0xFF);

	/* Write SOC alarm registers */
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_ALARM_SOC_LOW_REG, SOCAlarm, 2) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Low voltage alarm level settings.
 *
 * This function sets the voltage alarm level of the STC3105 chip.
 * If the voltage level falls below this threshold an alarm interrupt is generated.
 *
 * @param uint8_t data				voltage alarm level in [?]
 * @return STC3105_Return_TypeDef	return status
 */
STC3105_Return_TypeDef STC3105_SetLowVoltageAlarmLevel(uint8_t data)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	/* Write voltage alarm register */
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_ALARM_VOLTAGE_REG, &data, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Current threshold for relaxation.
 *
 * This function sets the current threshold for the voltage relaxation.
 *
 * @param uint8_t data				current threshold in [?]
 * @return STC3105_Return_TypeDef	return status
 */
STC3105_Return_TypeDef STC3105_SetCurrentThresholdforRelaxation(uint8_t data)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	/* Write current threshold register */
	if(BUS_I2C_WriteNBytes(STC3105, STC3105_ADDR, STC3105_CURRENT_THRES, &data, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Relaxation counter.
 *
 * This function reads the relaxation counter.
 * The timer counts since the current is below zero (charging) or above the current threshold (high discharging).
 *
 * @param uint16_t *sec				relaxation time in [s]
 * @return STC3105_Return_TypeDef	return status
 */
STC3105_Return_TypeDef STC3105_GetRelaxationTime(uint16_t *sec)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	uint8_t temp[1];

	/* Read relaxation register */
	if(BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_RELAX_COUNT_REG, temp, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	*sec = (uint16_t)temp[0] * STC3105_RELAXATION_TIME_CONSTANT;

	/* Print debug information */
#ifdef STC3105_DEBUG
	printf("STC3105_GetRelaxationTime(): relaxation time = %i s\n", *sec);
#endif

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Conversion counter.
 *
 * This function reads the conversion counter.
 *
 * @param uint16_t *n				number of performed conversions
 * @return STC3105_Return_TypeDef	return status
 */
STC3105_Return_TypeDef STC3105_GetConversions(uint16_t *n)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	uint8_t temp[2];

	/* Read conversion counter registers */
	if(BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_COUNTER_LOW_REG, temp, 2) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	*n = ((uint16_t)temp[1] << 8) | temp[0];

	/* Print debug information */
#ifdef STC3105_DEBUG
	printf("STC3105_GetConversions(): number of conversions = %i\n", *n);
#endif

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Sensor data.
 *
 * This function reads the internal sensor data in following byte order:
 *
 * RAW DATA:
 *
 * |--SOCL-|--SOCH-|-CURRL-|-CURRH-|-VOLTL-|-VOLTH-|
 *
 * |-----0-|-----1-|-----2-|-----3-|-----4-|-----5-|
 *
 * CONVERTED DATA:
 *
 * |--SOCH-|---SOC-|---SOC-|--SOCL-|-CURRH-|--CURR-|--CURR-|-CURRL-|-VOLTH-|--VOLT-|--VOLT-|-VOLTL-|
 *
 * |-----0-|-----1-|-----2-|-----3-|-----4-|-----5-|-----6-|-----7-|-----8-|-----9-|----10-|----11-|
 *
 * @param uint8_t *data				internal sensor data
 * @return STC3105_Return_TypeDef	return status
 */
STC3105_Return_TypeDef STC3105_GetSensorData(STC3105_DataType_TypeDef type, uint8_t *data)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	uint8_t temp[8];
	I2C_TransferReturn_TypeDef i2cStatus;

	/* Read data registers */
	switch(stc3105Params.sensors)
	{
		case(stc3105Soc):
			i2cStatus = BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_SOC_LOW_REG, temp, 2);
			memcpy(data, temp, 2);
			break;

		case(stc3105Curr):
			i2cStatus = BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_CURRENT_LOW_REG, temp, 2);
			memcpy((uint8_t *)(data + 2), temp, 2);
			break;

		case(stc3105Volt):
			i2cStatus = BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_VOLTAGE_LOW_REG, temp, 2);
			memcpy((uint8_t *)(data + 4), temp, 2);
			break;

		case(stc3105SocCurr):
			i2cStatus = BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_SOC_LOW_REG, temp, 6);
			memcpy(data, temp, 2);
			memcpy((uint8_t *)(data + 2), (uint8_t *)(temp + 4), 2);
			break;

		case(stc3105SocVolt):
			i2cStatus = BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_SOC_LOW_REG, temp, 8);
			memcpy(data, temp, 2);
			memcpy((uint8_t *)(data + 4), (uint8_t *)(temp + 6), 2);
			break;

		case(stc3105CurrVolt):
			i2cStatus = BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_CURRENT_LOW_REG, temp, 4);
			memcpy((uint8_t *)(data + 2), temp, 4);
			break;

		case(stc3105SocCurrVolt):
			i2cStatus = BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_SOC_LOW_REG, temp, 8);
			memcpy(data, temp, 2);
			memcpy((uint8_t *)(data + 2), (uint8_t *)(temp + 4), 4);
			break;

		case(stc3105None):
			i2cStatus = i2cTransferDone;
			break;

		default:
			i2cStatus = i2cTransferDone;
			break;
	}

	if(i2cStatus != i2cTransferDone)
	{
		return stc3105ReadError;
	}

	/* Convert data into physical values */
	if(type == stc3105ConvertedData)
	{
		convert(data);
		/* Print debug information */
#ifdef STC3105_DEBUG
		int32_t soc = (int32_t)(((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | data[3]);
		int32_t cur = (int32_t)(((uint32_t)data[4] << 24) | ((uint32_t)data[5] << 16) | ((uint32_t)data[6] << 8) | data[7]);
		int32_t vol = (int32_t)(((uint32_t)data[8] << 24) | ((uint32_t)data[9] << 16) | ((uint32_t)data[10] << 8) | data[11]);
		printf("STC3105_GetSensorData(): converted SOC     = %3.2f mAh\n", (float)soc / 1000);
		printf("STC3105_GetSensorData(): converted current = %3.2f mA\n", (float)cur / 1000);
		printf("STC3105_GetSensorData(): converted voltage = %3.2f V\n", (float)vol / 1000000);
#endif
	}
	else
	{
		/* Print debug information */
#ifdef STC3105_DEBUG
		uint16_t soc = ((uint16_t)data[1] << 8) | data[0];
		uint16_t cur = ((uint16_t)data[3] << 8) | data[2];
		uint16_t vol = ((uint16_t)data[5] << 8) | data[4];
		printf("STC3105_GetSensorData(): raw SOC     = %i\n", COMMON_convert16bitComplementaryIntoSignedInt(soc));
		printf("STC3105_GetSensorData(): raw current = %i\n", COMMON_convert16bitComplementaryIntoSignedInt(cur));
		printf("STC3105_GetSensorData(): raw voltage = %i\n", (int)vol);
#endif
	}

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Alarm type.
 *
 * This function checks the type of the occurred alarm.
 *
 * @param STC3105_AlarmType_TypeDef *alarmType	alarm type
 * @return STC3105_Return_TypeDef				return status
 */
STC3105_Return_TypeDef STC3105_GetAlarmType(STC3105_AlarmType_TypeDef *alarmType)
{
	/* Check if sensor is initialized */
	if(stc3105Params.init == false)
	{
		return stc3105InitError;
	}

	uint8_t temp[1];

	/* Read conversion counter registers */
	if(BUS_I2C_ReadNBytes(STC3105, STC3105_ADDR, STC3105_COUNTER_LOW_REG, temp, 1) != i2cTransferDone)
	{
		return stc3105WriteError;
	}

	/* Set alarm type */
	if(((temp[0] & _STC3105_ALM_SOC_BIT) != 1) && ((temp[0] & _STC3105_ALM_VOLT_BIT) != 1))
	{
		*alarmType = stc3105NoAlarm;
	}
	if(((temp[0] & _STC3105_ALM_SOC_BIT) == 1) && ((temp[0] & _STC3105_ALM_VOLT_BIT) != 1))
	{
		*alarmType = stc3105LowSocAlarm;
	}
	if(((temp[0] & _STC3105_ALM_SOC_BIT) != 1) && ((temp[0] & _STC3105_ALM_VOLT_BIT) == 1))
	{
		*alarmType = stc3105LowVoltageAlarm;
	}
	if(((temp[0] & _STC3105_ALM_SOC_BIT) == 1) && ((temp[0] & _STC3105_ALM_VOLT_BIT) == 1))
	{
		*alarmType = stc3105LowSocLowVoltageAlarm;
	}

	/* Print debug information */
#ifdef STC3105_DEBUG
	printf("STC3105_GetAlarmType(): alarm type = %i\n", *alarmType);
#endif

	/* Return status */
	return stc3105Ok;
}

/**
 * @brief Convert data.
 *
 * This function converts the SOC data into [uAh], current data into [uA] and voltage data into [uV].
 *
 * @param uint8_t *data		input data
 */
static inline void convert(uint8_t *data)
{
	int32_t soc = (int32_t)COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)data[1] << 8) | data[0]) * STC3105_SOC_CONSTANT;
	int32_t cur = (int32_t)COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)data[3] << 8) | data[2]) * STC3105_CURRENT_CONSTANT;
	int32_t vol = (int32_t)(((uint32_t)data[5] << 8) | data[4]) * STC3105_VOLTAGE_CONSTANT;
	data[0]  = (uint8_t)(soc >> 24);
	data[1]  = (uint8_t)(soc >> 16);
	data[2]  = (uint8_t)(soc >>  8);
	data[3]  = (uint8_t)(soc);
	data[4]  = (uint8_t)(cur >> 24);
	data[5]  = (uint8_t)(cur >> 16);
	data[6]  = (uint8_t)(cur >> 8);
	data[7]  = (uint8_t)(cur);
	data[8]  = (uint8_t)(vol >> 24);
	data[9]  = (uint8_t)(vol >> 16);
	data[10] = (uint8_t)(vol >> 8);
	data[11] = (uint8_t)(vol);
}
