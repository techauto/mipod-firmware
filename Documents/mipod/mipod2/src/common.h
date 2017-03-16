/************************************************************************************************************************************************//**
 * @file common.h
 * @brief Hardware configuration and commonly used functions.
 * @version 1.0
 * @author Peter Blank
 ****************************************************************************************************************************************************
 * @section License
 * <b>TODO license need to be added here...</b>
 ***************************************************************************************************************************************************/

#ifndef COMMON_H_
#define COMMON_H_

/****************************************************************************************************************************************************
 ***   INCLUDES   ***********************************************************************************************************************************
 ***************************************************************************************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#include "em_gpio.h"
#include "em_i2c.h"

#include "debug.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

/**
 * @name Hardware configuration.
 *
 * @{
 */
/* Hardware configuration for LED  */
#define LED_PORT					gpioPortC						/**< LED port on GPIO port number C					*/
#define LED_PIN						13								/**< LED pin on GPIO pin number 13					*/
/* Hardware configuration for debug output */
#define DEBUG_PORT					gpioPortF						/**< LEUART TX port on GPIO port number F			*/
#define DEBUG_PIN					2								/**< LEUART TX pin on GPIO pin number 2				*/
#define DEBUG_LOCATION				LEUART_ROUTE_LOCATION_LOC4		/**< LEUART hardware location 4						*/
/* Hardware configuration for the STC3105 chip */
#define I2C_STC3105_SDA_PORT		gpioPortB						/**< STC3105 I2C SDA port on GPIO port number B		*/
#define I2C_STC3105_SCL_PORT		gpioPortB						/**< STC3105 I2C SCL port on GPIO port number B		*/
#define I2C_STC3105_SDA_PIN			11								/**< STC3105 I2C SDA pin on GPIO pin number 11		*/
#define I2C_STC3105_SCL_PIN			12								/**< STC3105 I2C SCL pin on GPIO pin number 12		*/
#define I2C_STC3105_MODULE			I2C1							/**< STC3105 I2C hardware module I2C1				*/
#define I2C_STC3105_LOCATION		I2C_ROUTE_LOCATION_LOC1			/**< STC3105 I2C hardware location 1				*/
#define I2C_STC3105_IRQ				I2C1_IRQn						/**< STC3105 I2C interrupt vector I2C1				*/
#define I2C_STC3105_CLOCK			cmuClock_I2C1					/**< STC3105 I2C hardware clock I2C1				*/
#define I2C_STC3105_SPEED			I2C_FREQ_FAST_MAX				/**< STC3105 I2C maximum speed: ~ 400 kHz			*/
#define STC3105_ADDR				0xE0							/**< STC3105 chip address: 0xE0						*/
#define STC3105_SHUNT				10								/**< STC3105 current measurement shunt: ~ 10 mOhm	*/
/* Hardware configuration for the RV3049 chip */
#define SPI_RV3049_MISO_PORT		gpioPortE						/**< RV3049 SPI MISO port on GPIO port number E		*/
#define SPI_RV3049_MOSI_PORT		gpioPortE						/**< RV3049 SPI MOSI port on GPIO port number E		*/
#define SPI_RV3049_CLK_PORT			gpioPortC						/**< RV3049 SPI CLK port on GPIO port number C		*/
#define SPI_RV3049_CS_PORT			gpioPortC						/**< RV3049 SPI CS port on GPIO port number C		*/
#define SPI_RV3049_MISO_PIN			12								/**< RV3049 SPI MISO pin on GPIO pin number 12		*/
#define SPI_RV3049_MOSI_PIN			13								/**< RV3049 SPI MOSI pin on GPIO pin number 13		*/
#define SPI_RV3049_CLK_PIN			15								/**< RV3049 SPI CLK pin on GPIO pin number 15		*/
#define SPI_RV3049_CS_PIN			14								/**< RV3049 SPI CS pin on GPIO pin number 14		*/
#define SPI_RV3049_MODULE			USART0							/**< RV3049 SPI hardware module USART0				*/
#define SPI_RV3049_LOCATION			USART_ROUTE_LOCATION_LOC3		/**< RV3049 SPI hardware location 3					*/
#define SPI_RV3049_IRQ_TX			USART0_TX_IRQn					/**< RV3049 SPI TX interrupt vector USART0			*/
#define SPI_RV3049_IRQ_RX			USART0_RX_IRQn					/**< RV3049 SPI RX interrupt vector USART0			*/
#define SPI_RV3049_CLOCK			cmuClock_USART0					/**< RV3049 SPI hardware clock USART0				*/
#define SPI_RV3049_SPEED			1000000UL						/**< RV3049 SPI maximum speed: ~ 1 MHz				*/
/* Hardware configuration for the ADXL375 chip */
#define SPI_ADXL375_MISO_PORT		gpioPortD						/**< ADXL375 SPI MISO port on GPIO port number D	*/
#define SPI_ADXL375_MOSI_PORT		gpioPortD						/**< ADXL375 SPI MOSI port on GPIO port number D	*/
#define SPI_ADXL375_CLK_PORT		gpioPortD						/**< ADXL375 SPI CLK port on GPIO port number D		*/
#define SPI_ADXL375_CS_PORT			gpioPortC						/**< ADXL375 SPI CS port on GPIO port number C		*/
#define SPI_ADXL375_MISO_PIN		1								/**< ADXL375 SPI MISO pin on GPIO pin number 1		*/
#define SPI_ADXL375_MOSI_PIN		0								/**< ADXL375 SPI MOSI pin on GPIO pin number 0		*/
#define SPI_ADXL375_CLK_PIN			2								/**< ADXL375 SPI CLK pin on GPIO pin number 2		*/
#define SPI_ADXL375_CS_PIN			4								/**< ADXL375 SPI CS pin on GPIO pin number 4		*/
#define SPI_ADXL375_MODULE			USART1							/**< ADXL375 SPI hardware module USART1				*/
#define SPI_ADXL375_LOCATION		USART_ROUTE_LOCATION_LOC1		/**< ADXL375 SPI hardware location 1				*/
#define SPI_ADXL375_IRQ_TX			USART1_TX_IRQn					/**< ADXL375 SPI TX interrupt vector USART1			*/
#define SPI_ADXL375_IRQ_RX			USART1_RX_IRQn					/**< ADXL375 SPI RX interrupt vector USART1			*/
#define SPI_ADXL375_CLOCK			cmuClock_USART1					/**< ADXL375 SPI hardware clock USART1				*/
#define SPI_ADXL375_SPEED			1000000UL						/**< ADXL375 SPI maximum speed: ~ 1 MHz 			*/
/*Hardware configuration for the BMX055 chip */

#define SPI_BMX055_MISO_PORT		gpioPortB						/**< BMX055 SPI MISO port on GPIO port number B		*/
#define SPI_BMX055_MOSI_PORT		gpioPortB						/**< BMX055 SPI MOSI port on GPIO port number B		*/
#define SPI_BMX055_CLK_PORT			gpioPortB						/**< BMX055 SPI CLK port on GPIO port number B		*/
#define SPI_BMX055_CS_ACC_PORT		gpioPortA						/**< BMX055 SPI CS ACC port on GPIO port number A	*/
#define SPI_BMX055_CS_GYRO_PORT		gpioPortB						/**< BMX055 SPI CS GYRO port on GPIO port number A	*/
#define SPI_BMX055_CS_MAG_PORT		gpioPortA						/**< BMX055 SPI CS MAG port on GPIO port number A	*/

#define SPI_BMX055_MISO_PIN			4								/**< BMX055 SPI MISO pin on GPIO pin number 4		*/
#define SPI_BMX055_MOSI_PIN			3								/**< BMX055 SPI MOSI pin on GPIO pin number 3		*/
#define SPI_BMX055_CLK_PIN			5								/**< BMX055 SPI CLK pin on GPIO pin number 5		*/
#define SPI_BMX055_CS_ACC_PIN		4								/**< BMX055 SPI CS ACC pin on GPIO pin number 4		*/
#define SPI_BMX055_CS_GYRO_PIN		6								/**< BMX055 SPI CS GYRO pin on GPIO pin number 4		*/
#define SPI_BMX055_CS_MAG_PIN		1								/**< BMX055 SPI CS MAG pin on GPIO pin number 4		*/

#define SPI_BMX055_MODULE			USART2							/**< BMX055 SPI hardware module USART2				*/
#define SPI_BMX055_LOCATION			USART_ROUTE_LOCATION_LOC1		/**< BMX055 SPI hardware location 1					*/
#define SPI_BMX055_IRQ_TX			USART2_TX_IRQn					/**< BMX055 SPI TX interrupt vector USART2			*/
#define SPI_BMX055_IRQ_RX			USART2_RX_IRQn					/**< BMX055 SPI RX interrupt vector USART2			*/
#define SPI_BMX055_CLOCK			cmuClock_USART2					/**< BMX055 SPI hardware clock USART2				*/
#define SPI_BMX055_SPEED			1000000UL						/**< BMX055 SPI maximum speed: ~ 1 MHz 				*/

/* Hardware configuration for the NRF51422 chip */
#define SPI_NRF51422_MISO_PORT		gpioPortD						/**< NRF51422 SPI MISO port on GPIO port number D	*/
#define SPI_NRF51422_MOSI_PORT		gpioPortD						/**< NRF51422 SPI MOSI port on GPIO port number D	*/
#define SPI_NRF51422_CLK_PORT		gpioPortD						/**< NRF51422 SPI CLK port on GPIO port number D	*/
#define SPI_NRF51422_CS_PORT		gpioPortD						/**< NRF51422 SPI CS port on GPIO port number D		*/
#define SPI_NRF51422_MISO_PIN		1								/**< NRF51422 SPI MISO pin on GPIO pin number 1		*/
#define SPI_NRF51422_MOSI_PIN		0								/**< NRF51422 SPI MOSI pin on GPIO pin number 0		*/
#define SPI_NRF51422_CLK_PIN		2								/**< NRF51422 SPI CLK pin on GPIO pin number 2		*/
#define SPI_NRF51422_CS_PIN			3								/**< NRF51422 SPI CS pin on GPIO pin number 3		*/
#define SPI_NRF51422_MODULE			USART1							/**< NRF51422 SPI hardware module USART1				*/
#define SPI_NRF51422_LOCATION		USART_ROUTE_LOCATION_LOC1		/**< NRF51422 SPI hardware location 1				*/
#define SPI_NRF51422_IRQ_TX			USART1_TX_IRQn					/**< NRF51422 SPI TX interrupt vector USART1			*/
#define SPI_NRF51422_IRQ_RX			USART1_RX_IRQn					/**< NRF51422 SPI RX interrupt vector USART1			*/
#define SPI_NRF51422_CLOCK			cmuClock_USART1					/**< NRF51422 SPI hardware clock USART1				*/
#define SPI_NRF51422_SPEED			1000000UL						/**< NRF51422 SPI maximum speed: ~ 1 MHz 			*/

/* General hardware configuration */
#define BAT_CAP						110								/**< Battery capacity: ~ 110 mAh					*/
#define SENSOR_ID					0x42							/**< Sensor default ID: 0x42						*/
#define EXT_CRYSTAL_FREQ			48								/**< Frequency of external crystal: ~ 48 MHz		*/
/**
 * @}
 */

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

int COMMON_convert8bitComplementaryIntoSignedInt(uint8_t x);
int COMMON_convert16bitComplementaryIntoSignedInt(uint16_t x);
int COMMON_convert32bitComplementaryIntoSignedInt(uint32_t x);

int COMMON_devisionToNearestInteger(int n, int d);

/***************************************************************************************************************************************************/
#endif /* COMMON_H_ */
