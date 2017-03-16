///***************************************************************************************************************************************************/
//#ifndef BMG055_H_
//#define BMG055_H_
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
//#define BMG055_DEBUGGING
//
///* Register addresses */
//#define BMG055_DEVID_DEFAULT	0x0F
//#define BMG055_DEVID_REG        0x00  // should return 0x0F
////#define BMG055_Reserved       0x01
//#define BMG055_DATAX0_REG       0x02
//#define BMG055_DATAX1_REG	   	0x03
//#define BMG055_DATAY0_REG       0x04
//#define BMG055_DATAY1_REG       0x05
//#define BMG055_DATAZ0_REG       0x06
//#define BMG055_DATAZ1_REG       0x07
////#define BMG055_Reserved       0x08
//#define BMG055_INT_STATUS_0  0x09
//#define BMG055_INT_STATUS_1  0x0A
//#define BMG055_INT_STATUS_2  0x0B
//#define BMG055_INT_STATUS_3  0x0C
////#define BMG055_Reserved    0x0D
//#define BMG055_FIFO_STATUS   0x0E
//#define BMG055_RANGE         0x0F
//#define BMG055_BW            0x10
//#define BMG055_LPM1          0x11
//#define BMG055_LPM2          0x12
//#define BMG055_RATE_HBW      0x13
//#define BMG055_BGW_SOFTRESET 0x14
//#define BMG055_INT_EN_0      0x15
//#define BMG055_INT_EN_1      0x16
//#define BMG055_INT_MAP_0     0x17
//#define BMG055_INT_MAP_1     0x18
//#define BMG055_INT_MAP_2     0x19
//#define BMG055_INT_SRC_1     0x1A
//#define BMG055_INT_SRC_2     0x1B
//#define BMG055_INT_SRC_3     0x1C
////#define BMG055_Reserved    0x1D
//#define BMG055_FIFO_EN       0x1E
////#define BMG055_Reserved    0x1F
////#define BMG055_Reserved    0x20
//#define BMG055_INT_RST_LATCH 0x21
//#define BMG055_HIGH_TH_X     0x22
//#define BMG055_HIGH_DUR_X    0x23
//#define BMG055_HIGH_TH_Y     0x24
//#define BMG055_HIGH_DUR_Y    0x25
//#define BMG055_HIGH_TH_Z     0x26
//#define BMG055_HIGH_DUR_Z    0x27
////#define BMG055_Reserved    0x28
////#define BMG055_Reserved    0x29
////#define BMG055_Reserved    0x2A
//#define BMG055_SOC           0x31
//#define BMG055_A_FOC         0x32
//#define BMG055_TRIM_NVM_CTRL 0x33
//#define BMG055_BGW_SPI3_WDT  0x34
////#define BMG055_Reserved    0x35
//#define BMG055_OFC1          0x36
//#define BMG055_OFC2          0x37
//#define BMG055_OFC3          0x38
//#define BMG055_OFC4          0x39
//#define BMG055_TRIM_GP0      0x3A
//#define BMG055_TRIM_GP1      0x3B
//#define BMG055_BIST          0x3C
//#define BMG055_FIFO_CONFIG_0 0x3D
//#define BMG055_FIFO_CONFIG_1 0x3E
///***************************************************************************************************************************************************/
///* Offset constant */
//#define BMG055_OFFSET_CONST					80  /* in [mg/LSB] */
///* Data scale factor */
//#define BMG055_DATA_SCALE_FACTOR			128000   /* in [mg/LSB] for 16g */
///* Length of data frame in bytes */
//#define BMG055_FRAME						6
//
///* Set Gyro range type definitions */
//typedef enum
//{
//	BMG055_2000DPS 					=    0,
//	BMG055_1000DPS 					=    1,
//	BMG055_500DPS 					=    2,
//	BMG055_250DPS 					= 	 3,
//	BMG055_125DPS 					= 	 4,
//} BMG055_Gyro_TypeDef;
//
///* Frequency type definitions */
//typedef enum
//{
//	BMG055_2000Hz523Hz						=    0,
//	BMG055_2000Hz230Hz						=    1,
//	BMG055_1000Hz116Hz						=    2,
//	BMG055_400Hz47Hz						=    3,
//	BMG055_200Hz23Hz						=    4,
//	BMG055_100Hz12Hz						=    5,
//	BMG055_200Hz64Hz						=    6,
//	BMG055_100Hz32Hz						=    7,
//
//} BMG055_Frequency_TypeDef;
//
///* Standby type definitions */
//typedef enum
//{
//	BMG055Standby							=    0,
//	BMG055Active							=    8,
//} BMG055_Standby_TypeDef;
//
///* Return type definitions */
//typedef enum
//{
//	BMG055Ok							=   0,
//	BMG055InitError						=  -1,
//	BMG055ReadError						=  -2,
//	BMG055WriteError					=  -3,
//	BMG055FifoError						=  -4,
//	BMG055IrqMapError					=  -5,
//	BMG055ActivityThresholdError		=  -6,
//	BMG055ShockThresholdError			=  -7,
//	BMG055OffsetError					=  -8,
//} BMG055_Return_TypeDef;
//
///* Sensor selection type definitions (2^n - 1) */
//typedef enum
//{
//	BMG055GyroX								=   0,
//	BMG055GyroY								=   1,
//	BMG055GyroZ								=   2,
//	BMG055GyroXGyroY						=   3,
//	BMG055GyroXGyroZ						=   4,
//	BMG055GyroYGyroZ						=   5,
//	BMG055GyroXGyroYGyroZ					=   6,
//	BMG055None								=   7,
//} BMG055_Sensors_TypeDef;
//
//
//
///* Global BMG055 parameters */
//SL_PACK_START(1)
//typedef struct
//{
//	bool 									init;
//	BMG055_Sensors_TypeDef					sensors;
//	BMG055_Frequency_TypeDef				freq;
//	uint8_t 								id;
//} __attribute__ ((packed)) BMG055_Params;
//SL_PACK_END()
//
///****************************************************************************************************************************************************
// ***   PROTOTYPES   *********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
//BMG055_Return_TypeDef BMG055_Init(BMG055_Sensors_TypeDef sensors, BMG055_Frequency_TypeDef freq);
//BMG055_Return_TypeDef BMG055_SetCalibrationOffsets(int32_t offX, int32_t offY, int32_t offZ);
//BMG055_Return_TypeDef getDataGyroXYZ(uint8_t *temp);
//#endif /* BMG055_H_ */
