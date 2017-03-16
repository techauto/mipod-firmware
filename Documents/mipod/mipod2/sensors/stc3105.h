/************************************************************************************************************************************************//**
 * @file stc3105.h
 * @brief Hardware driver for the power measurement chip STC2105.
 * @version 1.0
 * @author Peter Blank
 ****************************************************************************************************************************************************
 * @section License
 * <b>TODO license need to be added here...</b>
 ***************************************************************************************************************************************************/

#ifndef STC3105_H_
#define STC3105_H_

#define STC3105_DEBUG

/****************************************************************************************************************************************************
 ***   INCLUDES   ***********************************************************************************************************************************
 ***************************************************************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "em_common.h"

#include "common.h"
#include "bus.h"
#include "debug.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

/**
 * @brief STC3105 power mode options.
 *
 * This enumeration indicates the power mode settings.
 */
typedef enum
{
	stc3105ActiceMode					= (0x00 << 2),	/**< Sets sensor into active mode, current consumption: ~ 100 uA. */
	stc3105PowerSavingMode				= (0x01 << 2),	/**< Sets sensor into power saving mode, current consumption: ~ 70 uA. */
} STC3105_PowerMode_TypeDef;

/**
 * @brief STC3105 alarm options.
 *
 * This enumeration indicates the alarm settings.
 */
typedef enum
{
	stc3105AlarmDisabled				= (0x00 << 3),	/**< Disables alarm functionality. */
	stc3105AlarmEnabled					= (0x01 << 3),	/**< Enables alarm functionality. */
} STC3105_Alarm_TypeDef;

/**
 * @brief STC3105 run mode options.
 *
 * This enumeration indicates the run mode settings.
 */
typedef enum
{
	stc3105StandbyMode					= (0x00 << 4),	/**< Sets sensor into standby mode, current consumption: ~ 2 uA. */
	stc3105OperatingMode				= (0x01 << 4),	/**< Sets sensor into normal operating mode. */
} STC3105_RunMode_TypeDef;

/**
 * @brief STC3105 alarm pin output options.
 *
 * This enumeration indicates the alarm pin output settings.
 */
typedef enum
{
	stc3105AlarmPinOuputForcedLow		= (0x00 << 0),	/**< Forces alarm pin to be low. */
	stc3105AlarmPinOuputByConditions	= (0x01 << 0),	/**< Sets alarm pin by alarm conditions. */
} STC3105_AlarmPinOutput_TypeDef;

/**
 * @brief STC3105 reset options.
 *
 * This enumeration indicates the reset options.
 */
typedef enum
{
	stc3105Reset						= (0x01 << 1),	/**< Reset the charge accumulator and conversion counter. */
	stc3105ReleaseSoftReset				= (0x00 << 4),	/**< Releases the soft reset and clears the power on detection. */
	stc3105SetSoftReset					= (0x01 << 4),	/**< Asserts the soft reset. */
} STC3105_Reset_TypeDef;

/**
 * @brief STC3105 internal sensor selection.
 *
 * This enumeration selects the internal sensors.
 */
typedef enum
{
	stc3105Soc							=  0,	/**< Use SOC sensor. */
	stc3105Curr							=  1,	/**< Use current sensor. */
	stc3105Volt							=  2,	/**< use voltage sensor. */
	stc3105SocCurr						=  3,	/**< Use SOC and current sensors. */
	stc3105SocVolt						=  4,	/**< Use SOC and voltage sensors. */
	stc3105CurrVolt						=  5,	/**< Use current and voltage sensors. */
	stc3105SocCurrVolt					=  6,	/**< Use SOC, current and voltage sensors. */
	stc3105None							=  7,	/**< Use no sensors. */
} STC3105_Sensors_TypeDef;

/**
 * @brief STC3105 data representation.
 *
 * This enumeration indicates data representation and holds the necessary number of bytes.
 */
typedef enum
{
	stc3105RawData						=  6,	/**< Number of bytes required for raw data acquisition. */
	stc3105ConvertedData				= 12,	/**< Number of bytes required of converted data acquisition. */
} STC3105_DataType_TypeDef;

/**
 * @brief STC3105 alarm occurrences.
 *
 * This enumeration indicates possible alarm occurrences.
 */
typedef enum
{
	stc3105LowSocAlarm					=  0,	/**< Low SOC alarm occurred. */
	stc3105LowVoltageAlarm				=  1,	/**< Low voltage alarm occurred. */
	stc3105LowSocLowVoltageAlarm		=  2,	/**< Low SOC and low voltage alarm occurred. */
	stc3105NoAlarm						=  3,	/**< No alarm occurred. */
} STC3105_AlarmType_TypeDef;

/**
 * @brief STC3105 return status.
 *
 * This enumeration indicates return status.
 */
typedef enum
{
	stc3105Ok							=  0,	/**< No operation error. */
	stc3105InitError					= -1,	/**< Error during initialization. */
	stc3105ReadError					= -2,	/**< Error during read operation. */
	stc3105WriteError					= -3,	/**< Error during write operation. */
} STC3105_Return_TypeDef;

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

STC3105_Return_TypeDef STC3105_InitDefault(void);
STC3105_Return_TypeDef STC3105_Reset(STC3105_Reset_TypeDef reset);											/// @warning <b>Untested function!</b>

STC3105_Return_TypeDef STC3105_SetRunMode(STC3105_RunMode_TypeDef mode);
STC3105_Return_TypeDef STC3105_SetInternalSensors(STC3105_Sensors_TypeDef sensors);
STC3105_Return_TypeDef STC3105_SetPowerMode(STC3105_PowerMode_TypeDef powerMode);
STC3105_Return_TypeDef STC3105_SetAlarm(STC3105_Alarm_TypeDef alarm, STC3105_AlarmPinOutput_TypeDef pin);	/// @warning <b>Untested function!</b>
STC3105_Return_TypeDef STC3105_SetSOCBaseLevel(uint16_t data);												/// @warning <b>Untested function!</b>
STC3105_Return_TypeDef STC3105_SetLowSOCAlarmLevel(uint16_t data);											/// @warning <b>Untested function!</b>
STC3105_Return_TypeDef STC3105_SetLowVoltageAlarmLevel(uint8_t data);										/// @warning <b>Untested function!</b>
STC3105_Return_TypeDef STC3105_SetCurrentThresholdforRelaxation(uint8_t data);								/// @warning <b>Untested function!</b>

STC3105_Return_TypeDef STC3105_GetRelaxationTime(uint16_t *sec);											/// @warning <b>Untested function!</b>
STC3105_Return_TypeDef STC3105_GetConversions(uint16_t *n);
STC3105_Return_TypeDef STC3105_GetSensorData(STC3105_DataType_TypeDef type, uint8_t *data);

STC3105_Return_TypeDef STC3105_GetAlarm(bool *alarm);														/// @warning <b>Untested function!</b>

STC3105_Return_TypeDef STC3105_ClearLowSOCAlarm(void);														/// @warning <b>Untested function!</b>
STC3105_Return_TypeDef STC3105_ClearLowVoltageAlarm(void);													/// @warning <b>Untested function!</b>

/***************************************************************************************************************************************************/
#endif /* STC3105_H_ */
