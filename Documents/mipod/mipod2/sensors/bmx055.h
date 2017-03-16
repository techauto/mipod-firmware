/************************************************************************************************************************************************//**
 * @file bmx055.h
 * @brief Hardware driver for the BMX055 sensor.
 * @version 1.0
 * @author Tony Nguyen
 ****************************************************************************************************************************************************
 * @section License
 * <b>TODO license need to be added here...</b>
 *
 *
 *
 *
 *
 ***************************************************************************************************************************************************/
#ifndef BMX055_H_
#define BMX055_H_

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
#define BMX055_DEBUGGING
// BMX055 data sheet http://ae-bst.resource.bosch.com/media/products/dokumente/bmx055/BST-BMX055-DS000-01v2.pdf
// The BMX055 is a conglomeration of three separate motion sensors packaged together but
// addressed and communicated with separately by design
// Accelerometer registers
#define BMX055_SOFTRESET 		 0x14
#define BMX055_RESET_VALUE		 0xB6

#define BMX055_ACC_WHOAMI        0x00   // should return 0xFA
#define BMX055_ACC_ID	         0xFA
//#define BMX055_ACC_Reserved    0x01
#define BMX055_ACC_D_X_LSB       0x02
#define BMX055_ACC_D_X_MSB       0x03
#define BMX055_ACC_D_Y_LSB       0x04
#define BMX055_ACC_D_Y_MSB       0x05
#define BMX055_ACC_D_Z_LSB       0x06
#define BMX055_ACC_D_Z_MSB       0x07
#define BMX055_ACC_D_TEMP        0x08
#define BMX055_ACC_INT_STATUS_0  0x09
#define BMX055_ACC_INT_STATUS_1  0x0A
#define BMX055_ACC_INT_STATUS_2  0x0B
#define BMX055_ACC_INT_STATUS_3  0x0C
//#define BMX055_ACC_Reserved    0x0D
#define BMX055_ACC_FIFO_STATUS   0x0E
#define BMX055_ACC_PMU_RANGE     0x0F
#define BMX055_ACC_PMU_BW        0x10
#define BMX055_ACC_PMU_LPW       0x11
#define BMX055_ACC_PMU_LOW_POWER 0x12
#define BMX055_ACC_D_HBW         0x13
//#define BMX055_ACC_BGW_SOFTRESET 0x14
//#define BMX055_ACC_Reserved    0x15
#define BMX055_ACC_INT_EN_0      0x16
#define BMX055_ACC_INT_EN_1      0x17
#define BMX055_ACC_INT_EN_2      0x18
#define BMX055_ACC_INT_MAP_0     0x19
#define BMX055_ACC_INT_MAP_1     0x1A
#define BMX055_ACC_INT_MAP_2     0x1B
//#define BMX055_ACC_Reserved    0x1C
//#define BMX055_ACC_Reserved    0x1D
#define BMX055_ACC_INT_SRC       0x1E
//#define BMX055_ACC_Reserved    0x1F
#define BMX055_ACC_INT_OUT_CTRL  0x20
#define BMX055_ACC_INT_RST_LATCH 0x21
#define BMX055_ACC_INT_0         0x22
#define BMX055_ACC_INT_1         0x23
#define BMX055_ACC_INT_2         0x24
#define BMX055_ACC_INT_3         0x25
#define BMX055_ACC_INT_4         0x26
#define BMX055_ACC_INT_5         0x27
#define BMX055_ACC_INT_6         0x28
#define BMX055_ACC_INT_7         0x29
#define BMX055_ACC_INT_8         0x2A
#define BMX055_ACC_INT_9         0x2B
#define BMX055_ACC_INT_A         0x2C
#define BMX055_ACC_INT_B         0x2D
#define BMX055_ACC_INT_C         0x2E
#define BMX055_ACC_INT_D         0x2F
#define BMX055_ACC_FIFO_CONFIG_0 0x30
//#define BMX055_ACC_Reserved    0x31
#define BMX055_ACC_PMU_SELF_TEST 0x32
#define BMX055_ACC_TRIM_NVM_CTRL 0x33
#define BMX055_ACC_BGW_SPI3_WDT  0x34
//#define BMX055_ACC_Reserved    0x35
#define BMX055_ACC_OFC_CTRL      0x36
#define BMX055_ACC_OFC_SETTING   0x37
#define BMX055_ACC_OFC_OFFSET_X  0x38
#define BMX055_ACC_OFC_OFFSET_Y  0x39
#define BMX055_ACC_OFC_OFFSET_Z  0x3A
#define BMX055_ACC_TRIM_GPO      0x3B
#define BMX055_ACC_TRIM_GP1      0x3C
//#define BMX055_ACC_Reserved    0x3D
#define BMX055_ACC_FIFO_CONFIG_1 0x3E
#define BMX055_ACC_FIFO_DATA     0x3F

// BMX055 Gyroscope Registers
#define BMX055_GYRO_WHOAMI           0x00  // should return 0x0F
#define BMX055_GYRO_ID				 0x0F
//#define BMX055_GYRO_Reserved       0x01
#define BMX055_GYRO_RATE_X_LSB       0x02
#define BMX055_GYRO_RATE_X_MSB       0x03
#define BMX055_GYRO_RATE_Y_LSB       0x04
#define BMX055_GYRO_RATE_Y_MSB       0x05
#define BMX055_GYRO_RATE_Z_LSB       0x06
#define BMX055_GYRO_RATE_Z_MSB       0x07
//#define BMX055_GYRO_Reserved       0x08
#define BMX055_GYRO_INT_STATUS_0  0x09
#define BMX055_GYRO_INT_STATUS_1  0x0A
#define BMX055_GYRO_INT_STATUS_2  0x0B
#define BMX055_GYRO_INT_STATUS_3  0x0C
//#define BMX055_GYRO_Reserved    0x0D
#define BMX055_GYRO_FIFO_STATUS   0x0E
#define BMX055_GYRO_RANGE         0x0F
#define BMX055_GYRO_BW            0x10
#define BMX055_GYRO_LPM1          0x11
#define BMX055_GYRO_LPM2          0x12
#define BMX055_GYRO_RATE_HBW      0x13
//#define BMX055_GYRO_BGW_SOFTRESET 0x14
#define BMX055_GYRO_INT_EN_0      0x15
#define BMX055_GYRO_INT_EN_1      0x16
#define BMX055_GYRO_INT_MAP_0     0x17
#define BMX055_GYRO_INT_MAP_1     0x18
#define BMX055_GYRO_INT_MAP_2     0x19
#define BMX055_GYRO_INT_SRC_1     0x1A
#define BMX055_GYRO_INT_SRC_2     0x1B
#define BMX055_GYRO_INT_SRC_3     0x1C
//#define BMX055_GYRO_Reserved    0x1D
#define BMX055_GYRO_FIFO_EN       0x1E
//#define BMX055_GYRO_Reserved    0x1F
//#define BMX055_GYRO_Reserved    0x20
#define BMX055_GYRO_INT_RST_LATCH 0x21
#define BMX055_GYRO_HIGH_TH_X     0x22
#define BMX055_GYRO_HIGH_DUR_X    0x23
#define BMX055_GYRO_HIGH_TH_Y     0x24
#define BMX055_GYRO_HIGH_DUR_Y    0x25
#define BMX055_GYRO_HIGH_TH_Z     0x26
#define BMX055_GYRO_HIGH_DUR_Z    0x27
//#define BMX055_GYRO_Reserved    0x28
//#define BMX055_GYRO_Reserved    0x29
//#define BMX055_GYRO_Reserved    0x2A
#define BMX055_GYRO_SOC           0x31
#define BMX055_GYRO_A_FOC         0x32
#define BMX055_GYRO_TRIM_NVM_CTRL 0x33
#define BMX055_GYRO_BGW_SPI3_WDT  0x34
//#define BMX055_GYRO_Reserved    0x35
#define BMX055_GYRO_OFC1          0x36
#define BMX055_GYRO_OFC2          0x37
#define BMX055_GYRO_OFC3          0x38
#define BMX055_GYRO_OFC4          0x39
#define BMX055_GYRO_TRIM_GP0      0x3A
#define BMX055_GYRO_TRIM_GP1      0x3B
#define BMX055_GYRO_BIST          0x3C
#define BMX055_GYRO_FIFO_CONFIG_0 0x3D
#define BMX055_GYRO_FIFO_CONFIG_1 0x3E

// BMX055 magnetometer registers
#define BMX055_MAG_WHOAMI         0x40  // should return 0x32
#define BMX055_MAG_ID			  0x32
#define BMX055_MAG_Reserved       0x41
#define BMX055_MAG_XOUT_LSB       0x42
#define BMX055_MAG_XOUT_MSB       0x43
#define BMX055_MAG_YOUT_LSB       0x44
#define BMX055_MAG_YOUT_MSB       0x45
#define BMX055_MAG_ZOUT_LSB       0x46
#define BMX055_MAG_ZOUT_MSB       0x47
#define BMX055_MAG_ROUT_LSB       0x48
#define BMX055_MAG_ROUT_MSB       0x49
#define BMX055_MAG_INT_STATUS     0x4A
#define BMX055_MAG_PWR_CNTL1      0x4B
#define BMX055_MAG_PWR_CNTL2      0x4C
#define BMX055_MAG_INT_EN_1       0x4D
#define BMX055_MAG_INT_EN_2       0x4E
#define BMX055_MAG_LOW_THS        0x4F
#define BMX055_MAG_HIGH_THS       0x50
#define BMX055_MAG_REP_XY         0x51
#define BMX055_MAG_REP_Z          0x52
/* Trim Extended Registers */
#define BMM050_DIG_X1             0x5D // needed for magnetic field calculation
#define BMM050_DIG_Y1             0x5E
#define BMM050_DIG_Z4_LSB         0x62
#define BMM050_DIG_Z4_MSB         0x63
#define BMM050_DIG_X2             0x64
#define BMM050_DIG_Y2             0x65
#define BMM050_DIG_Z2_LSB         0x68
#define BMM050_DIG_Z2_MSB         0x69
#define BMM050_DIG_Z1_LSB         0x6A
#define BMM050_DIG_Z1_MSB         0x6B
#define BMM050_DIG_XYZ1_LSB       0x6C
#define BMM050_DIG_XYZ1_MSB       0x6D
#define BMM050_DIG_Z3_LSB         0x6E
#define BMM050_DIG_Z3_MSB         0x6F
#define BMM050_DIG_XY2            0x70
#define BMM050_DIG_XY1            0x71

/* Length of data frame in bytes */
#define BMX055_ACC_FRAME						6
#define BMX055_GYRO_FRAME						6
#define BMX055_MAG_FRAME						6
// Set initial input parameters

/* Frequency type definitions */
typedef enum
{
		/*Accelerometer*/
		bmx055Acc7_81Hz								=    8,			//7.81Hz
		bmx055Acc15_63Hz							=    9,			//15.63Hz
		bmx055Acc31_25Hz							=    10,		//31.25Hz
		bmx055Acc62_5Hz								=    11,		//62.5Hz
		bmx055Acc125Hz								=    12,		//125Hz
		bmx055Acc250Hz								=    13,		//250Hz
		bmx055Acc500Hz								=    14,		//500Hz
		bmx055Acc1000Hz								=    15,		//1000Hz
		/*Gyro*/
} BMX055_Acc_Frequency_TypeDef;
typedef enum
{
		bmx055Gyro2000Hz523Hz						=    0,
		bmx055Gyro2000Hz230Hz						=    1,
		bmx055Gyro1000Hz116Hz						=    2,
		bmx055Gyro400Hz47Hz							=    3,
		bmx055Gyro200Hz23Hz							=    4,
		bmx055Gyro100Hz12Hz							=    5,
		bmx055Gyro200Hz64Hz							=    6,
		bmx055Gyro100Hz32Hz							=    7,
} BMX055_Gyro_Frequency_TypeDef;

typedef enum
{
		bmx055Mag10Hz								=    0x00<<3,			//Default
		bmx055Mag2Hz								=    0x01<<3,
		bmx055Mag6Hz								=    0x02<<3,
		bmx055Mag8Hz								=    0x03<<3,
		bmx055Mag15Hz								=    0x04<<3,
		bmx055Mag20Hz								=    0x05<<3,
		bmx055Mag25Hz								=    0x06<<3,
		bmx055Mag30Hz								=    0x07<<3,
} BMX055_Mag_Frequency_TypeDef;

/* Sensor selection type definitions (2^n - 1) */
typedef enum
{
	bmx055AccX								=   0,
	bmx055AccY								=   1,
	bmx055AccZ								=   2,
	bmx055AccXAccY							=   3,
	bmx055AccXAccZ							=   4,
	bmx055AccYAccZ							=   5,
	bmx055AccXAccYAccZ						=   6,
	bmx055AccNone							=   7,
} BMX055_Acc_Sensors_TypeDef;
typedef enum
{
	bmx055GyroX								=	0,
	bmx055GyroY								=	1,
	bmx055GyroZ								=	2,
	bmx055GyroXGyroY						=	3,
	bmx055GyroXGyroZ						=	4,
	bmx055GyroYGyroZ						=	5,
	bmx055GyroXGyroYGyroZ					=	6,
	bmx055GyroNone							=   7,
} BMX055_Gyro_Sensors_TypeDef;
typedef enum
{
	bmx055MagX								=	0,
	bmx055MagY								=	1,
	bmx055MagZ								=	2,
	bmx055MagXMagY							=	3,
	bmx055MagXMagZ							=	4,
	bmx055MagYMagZ							=	5,
	bmx055MagXMagYMagZ						=	6,
	bmx055MagNone							=   7,
} BMX055_Mag_Sensors_TypeDef;

// define BMX055 ACC full scale options
/* Set accelerometer range type definitions */
typedef enum
{
	bmx055Acc_2G 							=	3,
	bmx055Acc_4G 							=   5,
	bmx055Acc_8G 							=   8,
	bmx055Acc_16G 							=   12,
} BMX055_Acc_Range_TypeDef;
typedef enum
{
	bmx055Gyro_2000DPS						=  	0,
	bmx055Gyro_1000DPS						=   1,
	bmx055Gyro_500DPS						=	2,
	bmx055Gyro_250DPS	 					=	3,
	bmx055Gyro_125DPS						=	4,
} BMX055_Gyro_Range_TypeDef;

/* Return type definitions */
typedef enum
{
	bmx055Ok								=   0,
	bmx055InitError							=  -1,
	bmx055ReadError							=  -2,
	bmx055WriteError						=  -3,
	bmx055FifoError							=  -4,
	bmx055IrqMapError						=  -5,
	bmx055ActivityThresholdError			=  -6,
	bmx055ShockThresholdError				=  -7,
	bmx055OffsetError						=  -8,

} BMX055_Return_TypeDef;

/* Global BMX055 parameters */
SL_PACK_START(1)
typedef struct
{
	bool 									initAcc;
	bool 									initGyro;
	bool 									initMag;
	BMX055_Acc_Sensors_TypeDef				AccSensors;
	BMX055_Gyro_Sensors_TypeDef				GyroSensors;
	BMX055_Mag_Sensors_TypeDef				MagSensors;
	BMX055_Acc_Frequency_TypeDef			Accfreq;
	BMX055_Gyro_Frequency_TypeDef			Gyrofreq;
	BMX055_Mag_Frequency_TypeDef			Magfreq;
	BMX055_Acc_Range_TypeDef 				AccRange;
	BMX055_Gyro_Range_TypeDef 				GyroRange;
	uint8_t 								AccID;
	uint8_t 								GyroID;
	uint8_t 								MagID;
} __attribute__ ((packed)) BMX055_Params;
SL_PACK_END()

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

BMX055_Return_TypeDef BMX055_Init(BMX055_Acc_Sensors_TypeDef 		AccSensors,
								  BMX055_Gyro_Sensors_TypeDef 		GyroSensors,
								  BMX055_Mag_Sensors_TypeDef 		MagSensors,
								  BMX055_Acc_Frequency_TypeDef 		Accfreq,
								  BMX055_Gyro_Frequency_TypeDef 	Gyrofreq,
								  BMX055_Mag_Frequency_TypeDef 		Magfreq,
								  BMX055_Acc_Range_TypeDef 			AccRange,
								  BMX055_Gyro_Range_TypeDef	 		GyroRange);

BMX055_Return_TypeDef BMX055_Acc_GetData(uint8_t *data);
BMX055_Return_TypeDef BMX055_Gyro_GetData(uint8_t *data);
BMX055_Return_TypeDef BMX055_Mag_GetData(uint8_t *data);
#endif /* SENSORS_BMX055_H_ */
