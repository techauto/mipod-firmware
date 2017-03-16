/************************************************************************************************************************************************//**
 * @file bus.h
 * @brief Bus interface driver to communicate with different sensors types.
 * @version 1.0
 * @author Peter Blank
 ****************************************************************************************************************************************************
 * @section License
 * <b>TODO license need to be added here...</b>
 ***************************************************************************************************************************************************/

#ifndef BUS_H_
#define BUS_H_

#define BUS_DEBUG

/****************************************************************************************************************************************************
 ***   INCLUDES   ***********************************************************************************************************************************
 ***************************************************************************************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "em_usart.h"
#include "em_common.h"

#include "common.h"
#include "debug.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

/**
 * @brief BUS sensor type.
 *
 * This enumeration indicates the hardware type of sensor the used sensor.
 */
typedef enum
{
	STC3105			= 0, /**< Type for STC3105 power measuring chip. */
	RV3049			= 1, /**< Type for RV2049 real time clock. */
	TFBS4711		= 2, /**< Type for TFBS4711 IR transceiver. */
	BMX055_ACC		= 3, /**< Type for BMX055 9 degrees of freedom IMU sensor. ACCEL */
	BMX055_GYRO		= 4, /**< Type for BMX055 9 degrees of freedom IMU sensor. GYRO  */
	BMX055_MAG		= 5, /**< Type for BMX055 9 degrees of freedom IMU sensor. MAGET */
	ADXL375			= 6, /**< Type for ADXL375 high range accelerometer. */
	NRF51422		= 7, /**< Type for NRF51422 BT Low Energy and ANT transceiver. */
} BUS_Sensor_TypeDef;

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

void BUS_Init(BUS_Sensor_TypeDef sensor, bool en);

I2C_TransferReturn_TypeDef BUS_I2C_ReadNBytes(BUS_Sensor_TypeDef sensor, uint8_t devAddr, uint8_t reg, uint8_t *data, uint16_t n);
I2C_TransferReturn_TypeDef BUS_I2C_WriteNBytes(BUS_Sensor_TypeDef sensor, uint8_t devAddr, uint8_t reg, uint8_t *data, uint16_t len);

void BUS_SPI_TransferNBytes(BUS_Sensor_TypeDef sensor, uint8_t *dataTx, uint8_t *dataRx, uint8_t len);

/***************************************************************************************************************************************************/
#endif /* BUS_H_ */
