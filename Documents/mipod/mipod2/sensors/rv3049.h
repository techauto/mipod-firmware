///************************************************************************************************************************************************//**
// * @file rv3049.h
// * @brief Hardware driver for the real time clock chip RV3049.
// * @version 1.0
// * @author Peter Blank
// ****************************************************************************************************************************************************
// * @section License
// * <b>TODO license need to be added here...</b>
// ***************************************************************************************************************************************************/
//
//#ifndef RV3049_H_
//#define RV3049_H_
//
//#define RV3049_DEBUG
//
///****************************************************************************************************************************************************
// ***   INCLUDES   ***********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
//#include <stdint.h>
//#include <stdbool.h>
//#include <string.h>
//
//#include "em_common.h"
//
//#include "common.h"
//#include "bus.h"
//#include "debug.h"
//
///****************************************************************************************************************************************************
// ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
// ***************************************************************************************************************************************************/
//
//typedef enum {
//	rv3049IntToClkoutPin					= (0x00 << 7),	/**< <b>@TODO</b>. */
//	rv3049ClkoutToClkoutPin					= (0x01 << 7),	/**< <b>@TODO</b>. */
//} RV3049_CLKOUTPinFunction_TypeDef;
//
//typedef enum {
//	rv3049TimerClock32Hz					= (0x00 << 5),	/**< <b>@TODO</b>. */
//	rv3049TimerClock8Hz						= (0x01 << 5),	/**< <b>@TODO</b>. */
//	rv3049TimerClock1Hz						= (0x02 << 5),	/**< <b>@TODO</b>. */
//	rv3049TimerClock0Div5Hz					= (0x03 << 5),	/**< <b>@TODO</b>. */
//} RV3049_TimerClockSource_TypeDef;
//
//typedef enum {
//	rv3049DisableSelfRecovery				= (0x00 << 4),	/**< <b>@TODO</b>. */
//	rv3049EnableSelfRecovery				= (0x01 << 4),	/**< <b>@TODO</b>. */
//} RV3049_SelfRecovery_TypeDef;
//
//typedef enum {
//	rv3049DisableEEPROMRefresh				= (0x00 << 3), 	/**< <b>@TODO</b>. */
//	rv3049EnableEEPROMRefresh				= (0x01 << 3),	/**< <b>@TODO</b>. */
//} RV3049_EEPROMRefresh_TypeDef;
//
//typedef enum {
//	rv3049DisableCountdownTimerAutoReload	= (0x00 << 2),	/**< <b>@TODO</b>. */
//	rv3049EnableCountdownTimerAutoReload	= (0x01 << 2),	/**< <b>@TODO</b>. */
//} RV3049_CountdownTimerAutoReload_TypeDef;
//
//typedef enum {
//	rv3049DisableCountdownTimer				= (0x00 << 1),	/**< <b>@TODO</b>. */
//	rv3049EnableCountdownTimer				= (0x01 << 1),	/**< <b>@TODO</b>. */
//} RV3049_CountdownTimer_TypeDef;
//
//typedef enum {
//	rv3049Disable1HzWatchClockSource		= (0x00 << 0),	/**< <b>@TODO</b>. */
//	rv3049Enable1HzWatchClockSource			= (0x01 << 0),	/**< <b>@TODO</b>. */
//} RV3049_WatchClockSource_TypeDef;
//
//typedef enum {
//	rv3049DisableSelfRecoveryInterrupt		= (0x00 << 4),	/**< <b>@TODO</b>. */
//	rv3049EnableSelfRecoveryInterrupt		= (0x01 << 4),	/**< <b>@TODO</b>. */
//} RV3049_SelfRecoveryInterrupt_TypeDef;
//
//typedef enum {
//	rv3049DisableVLOW2Interrupt				= (0x00 << 3),	/**< <b>@TODO</b>. */
//	rv3049EnableVLOW2Interrupt				= (0x01 << 3),	/**< <b>@TODO</b>. */
//} RV3049_VLOW2Interrupt_TypeDef;
//
//typedef enum {
//	rv3049DisableVLOW1Interrupt				= (0x00 << 2),	/**< <b>@TODO</b>. */
//	rv3049EnableVLOW1Interrupt				= (0x01 << 2),	/**< <b>@TODO</b>. */
//} RV3049_VLOW1Interrupt_TypeDef;
//
//typedef enum {
//	rv3049DisableCountdownTimerInterrupt	= (0x00 << 1),	/**< <b>@TODO</b>. */
//	rv3049EnableCountdownTimerInterrupt		= (0x01 << 1),	/**< <b>@TODO</b>. */
//} RV3049_CountdownTimerInterrupt_TypeDef;
//
//typedef enum {
//	rv3049DisableAlarmInterrupt				= (0x00 << 0),	/**< <b>@TODO</b>. */
//	rv3049EnableAlarmInterrupt				= (0x01 << 0),	/**< <b>@TODO</b>. */
//} RV3049_AlarmInterrupt_TypeDef;
//
//typedef enum {
//	rv3049SelfRecoveryInterrupt				= 0, 			/**< <b>@TODO</b>. */
//	rv3049VLOW2Interrupt					= 1, 			/**< <b>@TODO</b>. */
//	rv3049VLOW1Interrupt					= 2, 			/**< <b>@TODO</b>. */
//	rv3049TimerInterrupt					= 3, 			/**< <b>@TODO</b>. */
//	rv3049AlarmInterrupt					= 4, 			/**< <b>@TODO</b>. */
//} STC3105_InterruptType_TypeDef;
//
//typedef enum {
//	rv3049EEPROMBusy						= 0, 			/**< <b>@TODO</b>. */
//	rv3049PowerOnReset						= 1, 			/**< <b>@TODO</b>. */
//	rv3049SelfRecoveryReset					= 2, 			/**< <b>@TODO</b>. */
//	rv3049VLOW2								= 3, 			/**< <b>@TODO</b>. */
//	rv3049VLOW1								= 4, 			/**< <b>@TODO</b>. */
//} STC3105_StatusType_TypeDef;
//
//typedef enum {
//	rv3049NoReset							= (0x00 << 4), 	/**< <b>@TODO</b>. */
//	rv3049Reset								= (0x01 << 4), 	/**< <b>@TODO</b>. */
//} RV3049_Reset_TypeDef;
//
//typedef enum {
//	rv3049DisableSecondAlarm				= (0x00 << 7), 	/**< <b>@TODO</b>. */
//	rv3049EnableSecondAlarm					= (0x01 << 7), 	/**< <b>@TODO</b>. */
//} RV3049_SecondAlarm_TypeDef;
//
//typedef enum {
//	rv3049DisableMinuteAlarm				= (0x00 << 7), 	/**< <b>@TODO</b>. */
//	rv3049EnableMinuteAlarm					= (0x01 << 7), 	/**< <b>@TODO</b>. */
//} RV3049_MinuteAlarm_TypeDef;
//
//typedef enum {
//	rv3049DisableHourAlarm					= (0x00 << 7), 	/**< <b>@TODO</b>. */
//	rv3049EnableHourAlarm					= (0x01 << 7), 	/**< <b>@TODO</b>. */
//} RV3049_HourAlarm_TypeDef;
//
//typedef enum {
//	rv3049DisableDayAlarm					= (0x00 << 7), 	/**< <b>@TODO</b>. */
//	rv3049EnableDayAlarm					= (0x01 << 7), 	/**< <b>@TODO</b>. */
//} RV3049_DayAlarm_TypeDef;
//
//typedef enum {
//	rv3049DisableWeekdayAlarm				= (0x00 << 7), 	/**< <b>@TODO</b>. */
//	rv3049EnableWeekdayAlarm				= (0x01 << 7), 	/**< <b>@TODO</b>. */
//} RV3049_WeekdayAlarm_TypeDef;
//
//typedef enum {
//	rv3049DisableMonthAlarm					= (0x00 << 7), 	/**< <b>@TODO</b>. */
//	rv3049EnableMonthAlarm					= (0x01 << 7), 	/**< <b>@TODO</b>. */
//} RV3049_MonthAlarm_TypeDef;
//
//typedef enum {
//	rv3049DisableYearAlarm					= (0x00 << 7), 	/**< <b>@TODO</b>. */
//	rv3049EnableYearAlarm					= (0x01 << 7), 	/**< <b>@TODO</b>. */
//} RV3049_YearAlarm_TypeDef;
//
//typedef enum {
//	rv3049RNone								= (0x00 << 4), 	/**< <b>@TODO</b>. */
//	rv3049R1k								= (0x01 << 4), 	/**< <b>@TODO</b>. */
//	rv3049R5k								= (0x02 << 4), 	/**< <b>@TODO</b>. */
//	rv3049R20k								= (0x04 << 4), 	/**< <b>@TODO</b>. */
//	rv3049R80k								= (0x08 << 4), 	/**< <b>@TODO</b>. */
//} RV3049_TrickleChargeResistor_TypeDef;
//
//typedef enum {
//	rv3049Clkout32786Hz						= (0x00 << 2), 	/**< <b>@TODO</b>. */
//	rv3049Clkout1024Hz						= (0x01 << 2), 	/**< <b>@TODO</b>. */
//	rv3049Clkout32Hz						= (0x02 << 2), 	/**< <b>@TODO</b>. */
//	rv3049Clkout1Hz							= (0x03 << 2), 	/**< <b>@TODO</b>. */
//} RV3049_CLKOUTSource_TypeDef;
//
//typedef enum {
//	rv3049DisableThermometer				= (0x00 << 1), 	/**< <b>@TODO</b>. */
//	rv3049EnableThermometer					= (0x01 << 1), 	/**< <b>@TODO</b>. */
//} RV3049_Thermometer_TypeDef;
//
//typedef enum {
//	rv3049ThermometerInterval1s				= (0x00 << 0), 	/**< <b>@TODO</b>. */
//	rv3049ThermometerInterval16s			= (0x01 << 0), 	/**< <b>@TODO</b>. */
//} RV3049_ThermometerInterval_TypeDef;
//
///**
// * @brief RV3049 internal sensor selection.
// *
// * This enumeration selects the internal sensors.
// */
//typedef enum {
//	rv3049Time								=  0,			/**< <b>@TODO</b>. */
//	rv3049Date								=  1,			/**< <b>@TODO</b>. */
//	rv3049Temp								=  2,			/**< <b>@TODO</b>. */
//	rv3049TimeDate							=  3,			/**< <b>@TODO</b>. */
//	rv3049TimeTemp							=  4,			/**< <b>@TODO</b>. */
//	rv3049DateTemp							=  5,			/**< <b>@TODO</b>. */
//	rv3049TimeDateTemp						=  6,			/**< <b>@TODO</b>. */
//	rv3049None								=  7,			/**< <b>@TODO</b>. */
//} RV3049_Sensors_TypeDef;
//
///**
// * @brief RV3049 data representation.
// *
// * This enumeration indicates data representation and holds the necessary number of bytes.
// */
//typedef enum {
//	rv3049RawData							=  8,			/**< Number of bytes required for raw data acquisition. */
//	rv3049ConvertedData						=  8,			/**< Number of bytes required of converted data acquisition. */
//} RV3049_DataType_TypeDef;
//
///**
// * @brief RV3049 return status.
// *
// * This enumeration indicates return status.
// */
//typedef enum {
//	rv3049Ok								=  0,			/**< No operation error. */
//	rv3049InitError							= -1,			/**< Error during initialization. */
//	rv3049ReadError							= -2,			/**< Error during read operation. */
//	rv3049WriteError						= -3,			/**< Error during write operation. */
//} RV3049_Return_TypeDef;
//
///****************************************************************************************************************************************************
// ***   PROTOTYPES   *********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
//RV3049_Return_TypeDef RV3049_InitDefault(void);
//
//RV3049_Return_TypeDef RV3049_GetSensorData(uint8_t *data);
//RV3049_Return_TypeDef RV3049_SetSensorData(uint8_t *data);
//
//
///***************************************************************************************************************************************************/
//#endif /* RV3049_H_ */
