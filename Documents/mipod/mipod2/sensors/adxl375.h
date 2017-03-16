/***************************************************************************************************************************************************/
#ifndef ADXL375_H_
#define ADXL375_H_

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
#define ADXL375_DEBUGGING

/* Register addresses */
#define ADXL375_DEVID_REG					0x00
#define ADXL375_THRESH_SHOCK_REG			0x1D
#define ADXL375_OFSX_REG					0x1E
#define ADXL375_OFSY_REG					0x1F
#define ADXL375_OFSZ_REG					0x20
#define ADXL375_DUR_REG						0x21
#define ADXL375_LATENT_REG					0x22
#define ADXL375_WINDOW_REG					0x23
#define ADXL375_THRESH_ACT_REG				0x24
#define ADXL375_THRESH_INACT_REG			0x25
#define ADXL375_TIME_INCAT_REG				0x26
#define ADXL375_ACT_INACT_CTL_REG			0x27
#define ADXL375_SHOCK_AXES_REG				0x2A
#define ADXL375_ACT_SHOCK_STATUS_REG		0x2B
#define ADXL375_BW_RATE_REG					0x2C
#define ADXL375_POWER_CTL_REG				0x2D
#define ADXL375_INT_ENABLE_REG				0x2E
#define ADXL375_INT_MAP_REG					0x2F
#define ADXL375_INT_SOURCE_REG				0x30
#define ADXL375_DATA_FORMAT_REG				0x31
#define ADXL375_DATAX0_REG					0x32
#define ADXL375_DATAX1_REG					0x33
#define ADXL375_DATAY0_REG					0x34
#define ADXL375_DATAY1_REG					0x35
#define ADXL375_DATAZ0_REG					0x36
#define ADXL375_DATAZ1_REG					0x37
#define ADXL375_FIFO_CTL_REG				0x38
#define ADXL375_FIFO_STATUS_REG				0x39

/* Register default values */
#define ADXL375_GENERAL_DEFAULT				0x00
#define ADXL375_DEVID_DEFAULT				0xE5
#define ADXL375_BW_RATE_DEFAULT				0x0A
#define ADXL375_INT_SOURCE_DEFAULT			0x02
#define ADXL375_DATA_FORMAT_DEFAULT			0x0B

/* ACT_SHOCK_STATUS register bit masks */
#define __ADXL375_ACT_SHOCK_STATUS_ACT_X	0x40
#define __ADXL375_ACT_SHOCK_STATUS_ACT_Y	0x20
#define __ADXL375_ACT_SHOCK_STATUS_ACT_Z	0x10
#define __ADXL375_ACT_SHOCK_STATUS_ASLEEP	0x08
#define __ADXL375_ACT_SHOCK_STATUS_SHOCK_X	0x04
#define __ADXL375_ACT_SHOCK_STATUS_SHOCK_Y	0x02
#define __ADXL375_ACT_SHOCK_STATUS_SHOCK_Z	0x01

/* INT_SOURCE register bit masks */
#define __ADXL375_INT_SOURCE_DATA_READY		0x80
#define __ADXL375_INT_SOURCE_SINGLE_SHOCK	0x40
#define __ADXL375_INT_SOURCE_DOUBLE_SHOCK	0x20
#define __ADXL375_INT_SOURCE_ACTIVITY		0x10
#define __ADXL375_INT_SOURCE_INACTIVITY		0x08
#define __ADXL375_INT_SOURCE_WATERMARK		0x02
#define __ADXL375_INT_SOURCE_OVERRUN		0x01

/* FIFO_STATUS register bit masks */
#define __ADXL375_FIFO_TRIGGER_EVENT		0x80
#define __ADXL375_FIFO_SAMPLES				0x3F

/* Length of data frame in bytes */
#define ADXL375_FRAME						6

/* Offset constant */
#define ADXL375_OFFSET_CONST				196  /* in [mg/LSB] */

/* Activity and Inactivity threshold constant */
#define ADXL375_ACT_INACT_TH_CONST			780  /* in [mg/LSB] */

/* Shock threshold constant */
#define ADXL375_SHOCK_TH_CONST				780  /* in [ms/LSB] */

/* Shock duration constant */
#define ADXL375_SHOCK_DURATION_CONST		625  /* in [us/LSB] */

/* Shock latency constant */
#define ADXL375_SHOCK_LATENCY_CONST			1250 /* in [ms/LSB] */

/* Shock window constant */
#define ADXL375_SHOCK_WINDOW_CONST			1250 /* in [ms/LSB] */

/* Data scale factor */
#define ADXL375_DATA_SCALE_FACTOR			49   /* in [mg/LSB] */

/* Number of samples of auto calibration */
#define ADXL375_CALIBRATION_SAMPLES			100

/* Activity axes type definitions */
typedef enum
{
	adxl375ActivityNone						= 	 0,
	adxl375DcActivityZ						=   16,
	adxl375DcActivityY						=   32,
	adxl375DcActivityYZ						=   48,
	adxl375DcActivityX						=   64,
	adxl375DcActivityXZ						=   80,
	adxl375DcActivityXY						=   96,
	adxl375DcActivityXYZ					=  112,
	adxl375AcActivityZ						=  144,
	adxl375AcActivityY						=  160,
	adxl375AcActivityYZ						=  176,
	adxl375AcActivityX						=  192,
	adxl375AcActivityXZ						=  208,
	adxl375AcActivityXY						=  224,
	adxl375AcActivityXYZ					=  240,
} ADXL375_ActivityAxes_TypeDef;

/* Inactivity axes type definitions */
typedef enum
{
	adxl375InactivityNone					= 	 0,
	adxl375DcInactivityZ					=    1,
	adxl375DcInactivityY					=    2,
	adxl375DcInactivityYZ					=    3,
	adxl375DcInactivityX					=    4,
	adxl375DcInactivityXZ					=    5,
	adxl375DcInactivityXY					=    6,
	adxl375DcInactivityXYZ					=    7,
	adxl375AcInactivityZ					=    8,
	adxl375AcInactivityY					=    9,
	adxl375AcInactivityYZ					=   10,
	adxl375AcInactivityX					=   11,
	adxl375AcInactivityXZ					=   12,
	adxl375AcInactivityXY					=   14,
	adxl375AcInactivityXYZ					=   15,
} ADXL375_InactivityAxes_TypeDef;

/* Selects shock detection mode type definitions */
typedef enum
{
	adxl375DoubleShock						= 	 0,
	adxl375SingleShock						=    8,
} ADXL375_ShockDetectionMode_TypeDef;

/* Shock axes type definitions */
typedef enum
{
	adxl375ShockNone						= 	 0,
	adxl375ShockZ							=    1,
	adxl375ShockY							=    2,
	adxl375ShockYZ							=    3,
	adxl375ShockX							=    4,
	adxl375ShockXZ							=    5,
	adxl375ShockXY							=    6,
	adxl375ShockXYZ							=    7,
} ADXL375_ShockAxes_TypeDef;

/* Frequency type definitions */
typedef enum
{
	adxl375Normal0_1Hz						=    0,
	adxl375Normal0_2Hz						=    1,
	adxl375Normal0_39Hz						=    2,
	adxl375Normal0_78Hz						=    3,
	adxl375Normal1_56Hz						=    4,
	adxl375Normal3_13Hz						=    5,
	adxl375Normal6_25Hz						=    6,
	adxl375Normal12_5Hz						=    7,
	adxl375Normal25Hz						=    8,
	adxl375Normal50Hz						=    9,
	adxl375Normal100Hz						=   10,
	adxl375Normal200Hz						=   11,
	adxl375Normal400Hz						=   12,
	adxl375Normal800Hz						=   13,
	adxl375Normal1600Hz						=   14,
	adxl375Normal3200Hz						=   15,
	adxl375LowPower12_5Hz					=   23,
	adxl375LowPower25Hz						=   24,
	adxl375LowPower50Hz						=   25,
	adxl375LowPower100Hz					=   26,
	adxl375LowPower200Hz					=   27,
	adxl375LowPower400Hz					=   28,
} ADXL375_Frequency_TypeDef;

/* Link activities type definitions */
typedef enum
{
	adxl375LinkAvtivitiesConcurrent			=    0,
	adxl375LinkAvtivitiesSequential			=   32,
	adxl375LinkAvtivitiesSequentialAuto		=   48,
} ADXL375_LinkActivities_TypeDef;

/* Sleep mode type definitions */
typedef enum
{
	adxl375SleepDisabled					=    0,
	adxl375Sleep8Hz							=    4,
	adxl375Sleep4Hz							=    5,
	adxl375Sleep2Hz							=    6,
	adxl375Sleep1Hz							=    7,
} ADXL375_Sleep_TypeDef;

/* Standby type definitions */
typedef enum
{
	adxl375Standby							=    0,
	adxl375Active							=    8,
} ADXL375_Standby_TypeDef;

/* Interrupt enable type definitions */
typedef enum
{
	adxl375IrqEnableAll						=    0,
	adxl375IrqEnableOverrun					=    1,
	adxl375IrqEnableWatermark				=    2,
	adxl375IrqEnableInactivity				=    8,
	adxl375IrqEnableActivity				=   16,
	adxl375IrqEnableDoubleShock				=   32,
	adxl375IrqEnableSingleShock				=   64,
	adxl375IrqEnableDataReady				=  128,
} ADXL375_IrqEnable_TypeDef;

/* Interrupt map to output pin type definitions */
typedef enum
{
	adxl375IrqMapNone						=    0,
	adxl375IrqMapOverrun					=    1,
	adxl375IrqMapWatermark					=    2,
	adxl375IrqMapInactivity					=    8,
	adxl375IrqMapActivity					=   16,
	adxl375IrqMapDoubleShock				=   32,
	adxl375IrqMapSingleShock				=   64,
	adxl375IrqMapDataReady					=  128,
} ADXL375_IrqMap_TypeDef;

/* Self test bit type definitions */
typedef enum
{
	adxl375SelfTestDisabled					=    0,
	adxl375SelfTestEnabled					=  128,
} ADXL375_SelfTest_TypeDef;

/* SPI mode bit type definitions */
typedef enum
{
	adxl375Spi4Wires						=    0,
	adxl375Spi3Wires						=   64,
} ADXL375_Spi_TypeDef;

/* Interrupt level mode type definitions */
typedef enum
{
	adxl375IrqActiveHigh					=    0,
	adxl375IrqActiveLow						=   32,
} ADXL375_IrqLevel_TypeDef;

/* Align transmitting bit type definitions */
typedef enum
{
	adxl375RightAligend						=    0,
	adxl375LeftAligned						=    4,
} ADXL375_Align_TypeDef;

/* FIFO mode type definitions */
typedef enum
{
	adxl375FifoBypass						=    0,
	adxl375Fifo32Bytes						=   64,
	adxl375FifoStream						=  128,
	adxl375FifoTrigger						=  192,
} ADXL375_FifoMode_TypeDef;

/* FIFO trigger type definitions */
typedef enum
{
	adxl375TriggerEventInt0					=   0,
	adxl375TriggerEventInt1					=  32,
} ADXL375_FifoTriggerEvent_TypeDef;

/* Sensor selection type definitions (2^n - 1) */
typedef enum
{
	adxl375AccX								=   0,
	adxl375AccY								=   1,
	adxl375AccZ								=   2,
	adxl375AccXAccY							=   3,
	adxl375AccXAccZ							=   4,
	adxl375AccYAccZ							=   5,
	adxl375AccXAccYAccZ						=   6,
	adxl375None								=   7,
} ADXL375_Sensors_TypeDef;

/* Return type definitions */
typedef enum
{
	adxl375Ok								=   0,
	adxl375InitError						=  -1,
	adxl375ReadError						=  -2,
	adxl375WriteError						=  -3,
	adxl375FifoError						=  -4,
	adxl375IrqMapError						=  -5,
	adxl375ActivityThresholdError			=  -6,
	adxl375ShockThresholdError				=  -7,
	adxl375OffsetError						=  -8,
} ADXL375_Return_TypeDef;

/* Global ADXL375 parameters */
SL_PACK_START(1)
typedef struct
{
	bool 									init;
	ADXL375_Sensors_TypeDef					sensors;
	ADXL375_Frequency_TypeDef				freq;
	uint8_t 								id;
} __attribute__ ((packed)) ADXL375_Params;
SL_PACK_END()

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

ADXL375_Return_TypeDef ADXL375_Init(ADXL375_Sensors_TypeDef sensors, ADXL375_Frequency_TypeDef freq);

ADXL375_Return_TypeDef ADXL375_SetCalibrationOffsets(int32_t offX, int32_t offY, int32_t offZ);
ADXL375_Return_TypeDef ADXL375_AutoCalibrate(void);

ADXL375_Return_TypeDef ADXL375_SetShockDetection(
		ADXL375_ShockDetectionMode_TypeDef mode,
		ADXL375_ShockAxes_TypeDef axes,
		int32_t shockTh,
		uint32_t duration,
		uint32_t latency,
		uint32_t window);

ADXL375_Return_TypeDef ADXL375_SetActivityDetection(
		ADXL375_ActivityAxes_TypeDef axesAct,
		ADXL375_InactivityAxes_TypeDef axesInact,
		int32_t actTh,
		int32_t inactTh,
		uint8_t inactTime,
		ADXL375_LinkActivities_TypeDef link,
		ADXL375_Sleep_TypeDef sleep,
		ADXL375_Standby_TypeDef standby);

ADXL375_Return_TypeDef ADXL375_SetInterruptEnable(ADXL375_IrqEnable_TypeDef irq, bool en);
ADXL375_Return_TypeDef ADXL375_SetInterruptMap(ADXL375_IrqMap_TypeDef irq, uint8_t pin);

ADXL375_Return_TypeDef ADXL375_GetActivityShockSource(uint8_t *data);
ADXL375_Return_TypeDef ADXL375_GetInterruptSource(uint8_t *data);
ADXL375_Return_TypeDef ADXL375_GetFifoTriggerEvent(uint8_t *data);
ADXL375_Return_TypeDef ADXL375_GetFifoBytes(uint8_t *data);

ADXL375_Return_TypeDef ADXL375_GetData(uint8_t *data);

/***************************************************************************************************************************************************/
#endif /* ADXL375_H_ */
