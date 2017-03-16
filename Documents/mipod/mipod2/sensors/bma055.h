///***************************************************************************************************************************************************/
//#ifndef BMA055_H_
//#define BMA055_H_
//
///****************************************************************************************************************************************************
// ***   INCLUDES   ***********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
//#include <stdint.h>
//#include <stdbool.h>
//#include <string.h>
//#include "em_common.h"
//#include "common.h"
//#include "bus.h"
//#include "debug.h"
//
///****************************************************************************************************************************************************
// ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
// ***************************************************************************************************************************************************/
//#define BMA055_DEBUGGING
//
///* Register addresses */
//#define BMA055_DEVID_REG					0x00
//#define BMA055_DATAX0_REG					0x02
//#define BMA055_DATAX1_REG					0x03
//#define BMA055_DATAY0_REG					0x04
//#define BMA055_DATAY1_REG					0x05
//#define BMA055_DATAZ0_REG					0x06
//#define BMA055_DATAZ1_REG					0x07
//#define BMA055_PMU_RANGE					0x0F
//#define BMA055_BW_RATE_REG					0x10
//#define BMA055_POWER_CTL_REG				0x11
//#define BMA055_DATA_FORMAT_REG				0x34
//#define BMA055_OFSX_REG						0x38
//#define BMA055_OFSY_REG						0x39
//#define BMA055_OFSZ_REG						0x3A
//#define BMA055_FIFO_CTL_REG					0x3E
////need modify
////reset BMA055
//#define BMA055_BGW_SOFTRESET 				0x14
//#define BMA055_RESET						0xB6
//#define BMA055_D_HBW						0x13
//
//
////#define BMX055_ACC_Reserved    0x3D
//#define BMX055_ACC_FIFO_CONFIG_1 			0x3E
//#define BMX055_ACC_FIFO_DATA     			0x3F
//
///* Register default values */
//#define BMA055_GENERAL_DEFAULT				0x00
//#define BMA055_DEVID_DEFAULT				0xFA
//#define BMA055_BW_RATE_DEFAULT				0x0C
//#define BMA055_INT_SOURCE_DEFAULT			0x02
//#define BMA055_DATA_FORMAT_DEFAULT			0x0B
///***************************************************************************************************************************************************/
//#define BMA055_FRAME						6
//
///* Set accelerometer range type definitions */
//typedef enum
//{
//	BMA055Accelerometer_2G 					=    3,
//	BMA055Accelerometer_4G 					=    5,
//	BMA055Accelerometer_8G 					=    8,
//	BMA055Accelerometer_16G 				=    12,
//} BMA055_Accelerometer_TypeDef;
//
///* Frequency type definitions */
//typedef enum
//{
//	BMA055Normal7_81Hz						=    8,
//	BMA055Normal15_63Hz						=    9,
//	BMA055Normal31_25Hz						=    10,
//	BMA055Normal62_5Hz						=    11,
//	BMA055Normal125Hz						=    12,
//	BMA055Normal250Hz						=    13,
//	BMA055Normal500Hz						=    14,
//	BMA055Normal1000Hz						=    15,
//
//} BMA055_Frequency_TypeDef;
//
///* Standby type definitions */
//typedef enum
//{
//	BMA055Standby							=    0,
//	BMA055Active							=    8,
//} BMA055_Standby_TypeDef;
//
///* Return type definitions */
//typedef enum
//{
//	BMA055Ok							=   0,
//	BMA055InitError						=  -1,
//	BMA055ReadError						=  -2,
//	BMA055WriteError					=  -3,
//	BMA055FifoError						=  -4,
//	BMA055IrqMapError					=  -5,
//	BMA055ActivityThresholdError		=  -6,
//	BMA055ShockThresholdError			=  -7,
//	BMA055OffsetError					=  -8,
//} BMA055_Return_TypeDef;
//
///* Sensor selection type definitions (2^n - 1) */
//typedef enum
//{
//	BMA055AccX								=   0,
//	BMA055AccY								=   1,
//	BMA055AccZ								=   2,
//	BMA055AccXAccY							=   3,
//	BMA055AccXAccZ							=   4,
//	BMA055AccYAccZ							=   5,
//	BMA055AccXAccYAccZ						=   6,
//	BMA055None								=   7,
//} BMA055_Sensors_TypeDef;
//
//
//
///* Global BMA055 parameters */
//SL_PACK_START(1)
//typedef struct
//{
//	bool 									init;
//	BMA055_Sensors_TypeDef					sensors;
//	BMA055_Frequency_TypeDef				freq;
//	uint8_t 								id;
//} __attribute__ ((packed)) BMA055_Params;
//SL_PACK_END()
//
///****************************************************************************************************************************************************
// ***   PROTOTYPES   *********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
//BMA055_Return_TypeDef BMA055_Init(BMA055_Sensors_TypeDef sensors, BMA055_Frequency_TypeDef freq);
//BMA055_Return_TypeDef BMA055_SetCalibrationOffsets(int32_t offX, int32_t offY, int32_t offZ);
//BMA055_Return_TypeDef BMA055_GetData(uint8_t *data);
//BMA055_Return_TypeDef  getDataAccXYZ(uint8_t *temp);
//#endif /* BMA055_H_ */
