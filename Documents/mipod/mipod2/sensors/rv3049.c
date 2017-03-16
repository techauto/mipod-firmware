///************************************************************************************************************************************************//**
// * @file rv3049.c
// * @brief Hardware driver for the real time clock chip RV3049.
// * @version 1.0
// * @author Peter Blank
// ****************************************************************************************************************************************************
// * @section License
// * <b>TODO license need to be added here...</b>
// ***************************************************************************************************************************************************/
//
//#include "rv3049.h"
//
///****************************************************************************************************************************************************
// ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
// ***************************************************************************************************************************************************/
//
//#define RV3049_BCD_CONVERSION
//#define RV3049_TEMP_CONVERSION
//
//
//
///**
// * @name Register configuration.
// *
// * @{
// */
///* Register addresses */
//#define RV3049_CONTROL_1_REG				0x00		/**< <b>@TODO</b>. */
//#define RV3049_CONTROL_INT_REG				0x01		/**< <b>@TODO</b>. */
//#define RV3049_CONTROL_INT_FLAG_REG			0x02		/**< <b>@TODO</b>. */
//#define RV3049_CONTROL_STATUS_REG			0x03		/**< <b>@TODO</b>. */
//#define RV3049_CONTROL_RESET_REG			0x04		/**< <b>@TODO</b>. */
//#define RV3049_SECONDS_REG					0x08		/**< <b>@TODO</b>. */
//#define RV3049_MINUTES_REG					0x09		/**< <b>@TODO</b>. */
//#define RV3049_HOURS_REG					0x0A		/**< <b>@TODO</b>. */
//#define RV3049_DAYS_REG						0x0B		/**< <b>@TODO</b>. */
//#define RV3049_WEEKDAYS_REG					0x0C		/**< <b>@TODO</b>. */
//#define RV3049_MONTHS_REG					0x0D		/**< <b>@TODO</b>. */
//#define RV3049_YEARS_REG					0x0E		/**< <b>@TODO</b>. */
//#define RV3049_SECOND_ALARM_REG				0x10		/**< <b>@TODO</b>. */
//#define RV3049_MINUTE_ALARM_REG				0x11		/**< <b>@TODO</b>. */
//#define RV3049_HOUR_ALARM_REG				0x12		/**< <b>@TODO</b>. */
//#define RV3049_DAY_ALARM_REG				0x13		/**< <b>@TODO</b>. */
//#define RV3049_WEEKDAY_ALARM_REG			0x14		/**< <b>@TODO</b>. */
//#define RV3049_MONTHS_ALARM_REG				0x14		/**< <b>@TODO</b>. */
//#define RV3049_YEAR_ALARM_REG				0x15		/**< <b>@TODO</b>. */
//#define RV3049_TIMER_LOW_REG				0x18		/**< <b>@TODO</b>. */
//#define RV3049_TIMER_HIGH_REG				0x19		/**< <b>@TODO</b>. */
//#define RV3049_TEMPERATURE_REG				0x20		/**< <b>@TODO</b>. */
//#define RV3049_EEPROM_USER_1_REG			0x28		/**< <b>@TODO</b>. */
//#define RV3049_EEPROM_USER_2_REG			0x29		/**< <b>@TODO</b>. */
//#define RV3049_EEPROM_CONTROL_REG			0x30		/**< <b>@TODO</b>. */
//#define RV3049_XTAL_OFFSET_REG				0x31		/**< <b>@TODO</b>. */
//#define RV3049_XTAL_COEFF_REG				0x32		/**< <b>@TODO</b>. */
//#define RV3049_XTAL_T0_REG					0x33		/**< <b>@TODO</b>. */
///* Default register values */
//#define RV3049_CONTROL_1_REG_DEFAULT		0b00000000	/**< <b>@TODO</b>. */
//#define RV3049_CONTROL_INT_REG_DEFAULT		0x11000000	/**< <b>@TODO</b>. */
///* CONTROL_INT_FLAG register bit masks */
//#define __RV3049_SR_FLAG_MASK				(0x01 << 4)	/**< <b>@TODO</b>. */
//#define __RV3049_VLOW2INT_FLAG_MASK			(0x01 << 3)	/**< <b>@TODO</b>. */
//#define __RV3049_VLOW1INT_FLAG_MASK			(0x01 << 2)	/**< <b>@TODO</b>. */
//#define __RV3049_TINT_FLAG_MASK				(0x01 << 1)	/**< <b>@TODO</b>. */
//#define __RV3049_AINT_FLAG_MASK				(0x01 << 0)	/**< <b>@TODO</b>. */
///* CONTROL_STATUS register bit masks */
//#define __RV3049_EE_BUSY_MASK				(0x01 << 7)	/**< <b>@TODO</b>. */
//#define __RV3049_PON_MASK					(0x01 << 5)	/**< <b>@TODO</b>. */
//#define __RV3049_SR_MASK					(0x01 << 4)	/**< <b>@TODO</b>. */
//#define __RV3049_V2F_MASK					(0x01 << 3)	/**< <b>@TODO</b>. */
//#define __RV3049_V1F_MASK					(0x01 << 2)	/**< <b>@TODO</b>. */
///**
// * @}
// */
//
///**
// * @brief RV3049 calculation constants.
// *
// * @{
// */
//#define RV3049_TEMP_CONSTANT				60			/**< Temperature constant in [°C]. */
//
///**
// * @brief RV3049 parameters.
// *
// * This structure indicates the global parameters of the RV3049 chip.
// */
//EFM32_PACK_START(1)
//typedef struct {
//	bool 									init;
//	RV3049_Sensors_TypeDef					sensors;
//	RV3049_CLKOUTPinFunction_TypeDef		clkoutPinFunction;
//	RV3049_TimerClockSource_TypeDef			timerClockSource;
//	RV3049_SelfRecovery_TypeDef				selfRecovery;
//	RV3049_EEPROMRefresh_TypeDef			eepromRefresh;
//	RV3049_CountdownTimerAutoReload_TypeDef	countdownTimerAutoReload;
//	RV3049_CountdownTimer_TypeDef			countdownTimer;
//	RV3049_WatchClockSource_TypeDef			watchClockSource;
//	RV3049_SelfRecoveryInterrupt_TypeDef	selfRecoveryInterupt;
//	RV3049_VLOW2Interrupt_TypeDef			vlow2Interrupt;
//	RV3049_VLOW1Interrupt_TypeDef			vlow1Interrupt;
//	RV3049_CountdownTimerInterrupt_TypeDef	countdownTimerInterrupt;
//	RV3049_AlarmInterrupt_TypeDef			alarmInterrupt;
//	RV3049_SecondAlarm_TypeDef				secondAlarm;
//	RV3049_MinuteAlarm_TypeDef				minuteAlarm;
//	RV3049_HourAlarm_TypeDef				hourAlarm;
//	RV3049_DayAlarm_TypeDef					dayAlarm;
//	RV3049_WeekdayAlarm_TypeDef				weekdayAlarm;
//	RV3049_MonthAlarm_TypeDef				monthAlarm;
//	RV3049_YearAlarm_TypeDef				yearAlarm;
//	RV3049_TrickleChargeResistor_TypeDef	trickleChargeResistor;
//	RV3049_CLKOUTSource_TypeDef				clkoutSource;
//	RV3049_Thermometer_TypeDef				thermometer;
//	RV3049_ThermometerInterval_TypeDef		thermometerInterval;
//} __attribute__ ((packed)) RV3049_Params;
//EFM32_PACK_END()
//
///**
// * @brief RV3049 default parameters.
// *
// * This macro indicates the default global parameters of the RV3049 chip.
// */
//#define RV3049_PARAMS_DEFAULT { \
//	false,									/**< <b>@TODO</b>. */ \
//	rv3049TimeDateTemp,						/**< <b>@TODO</b>. */\
//	rv3049ClkoutToClkoutPin,				/**< <b>@TODO</b>. */ \
//	rv3049TimerClock32Hz,					/**< <b>@TODO</b>. */ \
//	rv3049EnableSelfRecovery,				/**< <b>@TODO</b>. */ \
//	rv3049EnableEEPROMRefresh,				/**< <b>@TODO</b>. */ \
//	rv3049DisableCountdownTimerAutoReload,	/**< <b>@TODO</b>. */ \
//	rv3049DisableCountdownTimer,			/**< <b>@TODO</b>. */ \
//	rv3049Disable1HzWatchClockSource,		/**< <b>@TODO</b>. */ \
//	rv3049DisableSelfRecoveryInterrupt,		/**< <b>@TODO</b>. */ \
//	rv3049DisableVLOW2Interrupt,			/**< <b>@TODO</b>. */ \
//	rv3049DisableVLOW1Interrupt,			/**< <b>@TODO</b>. */ \
//	rv3049DisableCountdownTimerInterrupt,	/**< <b>@TODO</b>. */ \
//	rv3049DisableAlarmInterrupt,			/**< <b>@TODO</b>. */ \
//	rv3049NoReset,							/**< <b>@TODO</b>. */ \
//	rv3049DisableSecondAlarm,				/**< <b>@TODO</b>. */ \
//	rv3049DisableMinuteAlarm,				/**< <b>@TODO</b>. */ \
//	rv3049DisableHourAlarm,					/**< <b>@TODO</b>. */ \
//	rv3049DisableDayAlarm,					/**< <b>@TODO</b>. */ \
//	rv3049DisableWeekdayAlarm,				/**< <b>@TODO</b>. */ \
//	rv3049DisableMonthAlarm,				/**< <b>@TODO</b>. */ \
//	rv3049DisableYearAlarm,					/**< <b>@TODO</b>. */ \
//	rv3049RNone,							/**< <b>@TODO</b>. */ \
//	rv3049Clkout32Hz,						/**< <b>@TODO</b>. */ \
//	rv3049EnableThermometer,				/**< <b>@TODO</b>. */ \
//	rv3049ThermometerInterval1s,			/**< <b>@TODO</b>. */ \
//}
//
//EFM32_ALIGN(4)
//EFM32_PACK_START(1)
//static RV3049_Params __attribute__ ((aligned(4))) rv3049Params = RV3049_PARAMS_DEFAULT;
//EFM32_PACK_END()
//
///****************************************************************************************************************************************************
// ***   PROTOTYPES   *********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
//__STATIC_INLINE void spiWrite(uint8_t ctrlReg, uint8_t *writeData, uint8_t len);
//__STATIC_INLINE void spiRead(uint8_t ctrlReg, uint8_t *readData, uint8_t len);
//
///****************************************************************************************************************************************************
// ***   FUNCTIONS   **********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
///**
// * @brief Initialization.
// *
// * This function initializes the STC3105 power measurement chip by setting the internal sensors and different modes.
// *
// * @param STC3105_Sensors_TypeDef sensors		internal sensors
// * @param STC3105_PowerMode_TypeDef powerMode	power mode
// * @param STC3105_Mode_TypeDef mode				operating mode
// * @param STC3105_Alarm_TypeDef alarm			alarm type
// * @return STC3105_Return_TypeDef				return status
// */
//RV3049_Return_TypeDef RV3049_InitDefault(void)
//{
//	uint8_t byte;
//
//	/* Apply CLKOUT to CLKOUT and 1 Hz timer clock */
//	/* Enable self recovery, EEPROM refresh and watch */
//	/* Disable timer interrupt */
//	byte = _RV3049_CLKOUT_TO_CLKOUT | _RV3049_TIMER_CLOCK_1_HZ;
//	byte |= _RV3049_SR_ENABLE |_RV3049_EEREH_ENABLE | _RV3049_1_HZ_WATCH_ENABLE;
//	byte |= _RV3049_TARM_DISABLE | _RV3049_CT_DISABLE;
//	spiWrite(RV3049_CONTROL_1_REG, &byte, 1);
//
//	/* Disable interrupt, timer interrupt, voltage and self recovery alarms */
//	byte = _RV3049_TINT_DISABLE | _RV3049_AINT_DISABLE | _RV3049_VLOW1INT_DISABLE |_RV3049_VLOW2INT_DISABLE | _RV3049_SRINT_DISABLE;
//	spiWrite(RV3049_CONTROL_INT_REG, &byte, 1);
//
//	/* Clear all flags */
//	byte = 0x00;
//	spiWrite(RV3049_CONTROL_INT_REG, &byte, 1);
//	spiWrite(RV3049_CONTROL_STATUS_REG, &byte, 1);
//
//	/* Disable single alarm functions */
//	byte = _RV3049_SECONDS_DISABLE;
//	spiWrite(RV3049_SECOND_ALARM_REG, &byte, 1);
//	byte = _RV3049_MINUTES_DISABLE;
//	spiWrite(RV3049_MINUTE_ALARM_REG, &byte, 1);
//	byte = _RV3049_HOURS_DISABLE;
//	spiWrite(RV3049_HOUR_ALARM_REG, &byte, 1);
//	byte = _RV3049_DAYS_DISABLE;
//	spiWrite(RV3049_DAY_ALARM_REG, &byte, 1);
//	byte = _RV3049_WEEKDAYS_DISABLE;
//	spiWrite(RV3049_WEEKDAY_ALARM_REG, &byte, 1);
//	byte = _RV3049_MONTHS_DISABLE;
//	spiWrite(RV3049_MONTHS_ALARM_REG, &byte, 1);
//	byte = _RV3049_YEARS_DISABLE;
//	spiWrite(RV3049_YEAR_ALARM_REG, &byte, 1);
//
//	/* Set initialization flag */
//	rv3049Params.init = true;
//
//	/* Print debug information */
//#ifdef RV3049_DEBUGGING
//	printf("RV3049_Init(): params.sensors  = %i\n", (int)rv3049Params.sensors);
//	printf("RV3049_Init(): params.acqTime  = %i\n", (int)rv3049Params.acqTime);
//	printf("RV3049_Init(): params.tempMode = %i\n", (int)rv3049Params.tempMode);
//	printf("RV3049_Init(): params.init     = %i\n", (int)rv3049Params.init);
//#endif
//
//	/* Return status */
//	return rv3049Ok;
//}
//
///***************************************************************************************************************************************************/
//RV3049_Return_TypeDef RV3049_GetData(uint8_t *data)
//{
//	/* |--SEC-|--MIN-|-HOUR-|--DAY-|-WDAY-|--MON-|-YEAR-|-TEMP-| */
//	/* |--------TIME--------|------------DATE-----------|------| */
//	/* |----0-|----1-|----2-|----3-|----4-|----5-|----6-|----7-| */
//
//	uint8_t temp[RV3049_FRAME];
//
//	/* Read appropriate data registers */
//	switch(rv3049Params.sensors) {
//		case(rv3049Time):
//			spiRead(RV3049_SECONDS_REG, temp, 3);
//			memcpy(data, temp, 3);
//#ifdef RV3049_BCD_CONVERSION
//			data[0] = (((data[0] & 0x70) >> 4) * 10) + (data[0] & 0x0F);
//			data[1] = (((data[1] & 0x70) >> 4) * 10) + (data[1] & 0x0F);
//			data[2] = (((data[2] & 0x30) >> 4) * 10) + (data[2] & 0x0F);
//#endif
//			break;
//
//		case(rv3049Date):
//			spiRead(RV3049_DAYS_REG, temp, 4);
//			memcpy((uint8_t *)(data + 3), temp, 4);
//#ifdef RV3049_BCD_CONVERSION
//			data[3] = (((data[0] & 0x30) >> 4) * 10) + (data[0] & 0x0F);
//			data[4] = (((data[1] & 0x00) >> 4) * 10) + (data[1] & 0x07);
//			data[5] = (((data[2] & 0x10) >> 4) * 10) + (data[2] & 0x0F);
//			data[6] = (((data[3] & 0x70) >> 4) * 10) + (data[3] & 0x0F);
//#endif
//			break;
//
//		case(rv3049Temp):
//			spiRead(RV3049_TEMPERATURE_REG, temp, 1);
//			memcpy((uint8_t *)(data + 7), temp, 1);
//#ifdef RV3049_TEMP_CONVERSION
//			data[7] = (uint8_t)((int8_t)data[7] - RV3049_TEMP_CONSTANT);
//#endif
//			break;
//
//		case(rv3049TimeDate):
//			spiRead(RV3049_SECONDS_REG, temp, 7);
//			memcpy(data, temp, 7);
//#ifdef RV3049_BCD_CONVERSION
//			data[0] = (((data[0] & 0x70) >> 4) * 10) + (data[0] & 0x0F);
//			data[1] = (((data[1] & 0x70) >> 4) * 10) + (data[1] & 0x0F);
//			data[2] = (((data[2] & 0x30) >> 4) * 10) + (data[2] & 0x0F);
//			data[3] = (((data[3] & 0x30) >> 4) * 10) + (data[0] & 0x0F);
//			data[4] = (((data[4] & 0x00) >> 4) * 10) + (data[1] & 0x07);
//			data[5] = (((data[5] & 0x10) >> 4) * 10) + (data[2] & 0x0F);
//			data[6] = (((data[6] & 0x70) >> 4) * 10) + (data[2] & 0x0F);
//#endif
//			break;
//
//		case(rv3049TimeTemp):
//			spiRead(RV3049_SECONDS_REG, temp, 3);
//			memcpy(data, temp, 3);
//#ifdef RV3049_BCD_CONVERSION
//			data[0] = (((data[0] & 0x70) >> 4) * 10) + (data[0] & 0x0F);
//			data[1] = (((data[1] & 0x70) >> 4) * 10) + (data[1] & 0x0F);
//			data[2] = (((data[2] & 0x30) >> 4) * 10) + (data[2] & 0x0F);
//#endif
//			spiRead(RV3049_TEMPERATURE_REG, temp, 1);
//			memcpy((uint8_t *)(data + 7), temp, 1);
//#ifdef RV3049_TEMP_CONVERSION
//			data[7] = (uint8_t)((int8_t)data[7] - RV3049_TEMP_CONSTANT);
//#endif
//			break;
//
//		case(rv3049DateTemp):
//			spiRead(RV3049_DAYS_REG, temp, 4);
//			memcpy((uint8_t *)(data + 3), temp, 4);
//#ifdef RV3049_BCD_CONVERSION
//			data[3] = (((data[0] & 0x30) >> 4) * 10) + (data[0] & 0x0F);
//			data[4] = (((data[1] & 0x00) >> 4) * 10) + (data[1] & 0x07);
//			data[5] = (((data[2] & 0x10) >> 4) * 10) + (data[2] & 0x0F);
//			data[6] = (((data[3] & 0x70) >> 4) * 10) + (data[3] & 0x0F);
//#endif
//			spiRead(RV3049_TEMPERATURE_REG, temp, 1);
//			memcpy((uint8_t *)(data + 7), temp, 1);
//#ifdef RV3049_TEMP_CONVERSION
//			data[7] = (uint8_t)((int8_t)data[7] - RV3049_TEMP_CONSTANT);
//#endif
//			break;
//
//		case(rv3049TimeDateTemp):
//			spiRead(RV3049_SECONDS_REG, temp, 7);
//			memcpy(data, temp, 7);
//#ifdef RV3049_BCD_CONVERSION
//			data[0] = (((data[0] & 0x70) >> 4) * 10) + (data[0] & 0x0F);
//			data[1] = (((data[1] & 0x70) >> 4) * 10) + (data[1] & 0x0F);
//			data[2] = (((data[2] & 0x30) >> 4) * 10) + (data[2] & 0x0F);
//			data[3] = (((data[3] & 0x30) >> 4) * 10) + (data[3] & 0x0F);
//			data[4] = (((data[4] & 0x00) >> 4) * 10) + (data[4] & 0x07);
//			data[5] = (((data[5] & 0x10) >> 4) * 10) + (data[5] & 0x0F);
//			data[6] = (((data[6] & 0x70) >> 4) * 10) + (data[6] & 0x0F);
//#endif
//			spiRead(RV3049_TEMPERATURE_REG, temp, 1);
//			memcpy((uint8_t *)(data + 7), temp, 1);
//#ifdef RV3049_TEMP_CONVERSION
//			data[7] = (uint8_t)((int8_t)data[7] - RV3049_TEMP_CONSTANT);
//#endif
//			break;
//
//		case(rv3049None):
//			break;
//
//		default:
//			break;
//	}
//
//	/* Print debug information */
//#ifdef RV3049_DEBUGGING
//	printf("RV3049_GetData(): seconds     = %i\n", (int)data[0]);
//	printf("RV3049_GetData(): minutes     = %i\n", (int)data[1]);
//	printf("RV3049_GetData(): hours       = %i\n", (int)data[2]);
//	printf("RV3049_GetData(): days        = %i\n", (int)data[3]);
//	printf("RV3049_GetData(): weekdays    = %i\n", (int)data[4]);
//	printf("RV3049_GetData(): months      = %i\n", (int)data[5]);
//	printf("RV3049_GetData(): years       = %i\n", (int)data[6]);
//	printf("RV3049_GetData(): temperature = %i\n", (int)data[7]);
//#endif
//
//	/* Return status */
//	return rv3049Ok;
//}
//
///***************************************************************************************************************************************************/
//RV3049_Return_TypeDef RV3049_SetData(uint8_t *data)
//{
//	/* |--SEC-|--MIN-|-HOUR-|--DAY-|-WDAY-|--MON-|-YEAR-| */
//	/* |--------TIME--------|------------DATE-----------| */
//	/* |----0-|----1-|----2-|----3-|----4-|----5-|----6-| */
//
//#ifdef RV3049_BCD_CONVERSION
//	data[0] = (uint8_t)(((data[0] / 10) << 4) + (data[0] % 10));
//	data[1] = (uint8_t)(((data[1] / 10) << 4) + (data[1] % 10));
//	data[2] = (uint8_t)(((data[2] / 10) << 4) + (data[2] % 10));
//	data[3] = (uint8_t)(((data[3] / 10) << 4) + (data[3] % 10));
//	data[4] = (uint8_t)(((data[4] / 10) << 4) + (data[4] % 10));
//	data[5] = (uint8_t)(((data[5] / 10) << 4) + (data[5] % 10));
//	data[6] = (uint8_t)(((data[6] / 10) << 4) + (data[6] % 10));
//#endif
//
//	/* Write appropriate data registers */
//	spiWrite(RV3049_SECONDS_REG, data, 7);
//
//	/* Return status */
//	return rv3049Ok;
//}
//
///***************************************************************************************************************************************************/
//RV3049_Params RV3049_GetParams(void)
//{
//	return rv3049Params;
//}
//
///***************************************************************************************************************************************************/
//__STATIC_INLINE void spiWrite(uint8_t ctrlReg, uint8_t *writeData, uint8_t len)
//{
//	uint8_t txBuffer[len + 1];
//	uint8_t rxBuffer[len + 1];
//
//	/* Set write indication bit */
//	ctrlReg &= ~(1 << 7);
//	txBuffer[0] = ctrlReg;
//
//	/* Move data buffers and transfer data */
//	memcpy((uint8_t *)(txBuffer + 1), writeData, len);
//	BUS_SPI_TransferNBytes(RV3049, txBuffer, rxBuffer, (len + 1));
//}
//
///***************************************************************************************************************************************************/
//__STATIC_INLINE void spiRead(uint8_t ctrlReg, uint8_t *readData, uint8_t len)
//{
//	uint8_t txBuffer[len + 1];
//	uint8_t rxBuffer[len + 1];
//
//	/* Clear read indication bit */
//	ctrlReg |= (1 << 7);
//	txBuffer[0] = ctrlReg;
//
//	/* Move data buffers and transfer data */
//	BUS_SPI_TransferNBytes(RV3049, txBuffer, rxBuffer, (len + 1));
//	memcpy(readData, (uint8_t *)(rxBuffer + 1), len);
//}
