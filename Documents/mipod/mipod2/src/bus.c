/************************************************************************************************************************************************//**
 * @file bus.c
 * @brief Bus interface driver to communicate with different sensors types.
 * @version 1.0
 * @author Peter Blank
 ****************************************************************************************************************************************************
 * @section License
 * <b>TODO license need to be added here...</b>
 ***************************************************************************************************************************************************/

#include "bus.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

/**
 * @brief BUS parameters.
 *
 * This structure indicates the global parameters of the BUS module.
 */
SL_PACK_START(1)
typedef struct
{
	bool stc3105BusInit;		/**< Initialization status for STC3105. */
	bool rv3049BusInit;			/**< Initialization status for RV3049. */
	bool tfbs4711BusInit;		/**< Initialization status for TFBS4711. */
	bool bmx055accBusInit;		/**< Initialization status for BMX055.ACCEL */
	bool bmx055gyroBusInit;		/**< Initialization status for BMX055.GYRO */
	bool bmx055magBusInit;		/**< Initialization status for BMX055.MAG */
	bool adxl375BusInit;		/**< Initialization status for ADXL375. */
	bool nrf51422BusInit;		/**< Initialization status for NRF51422. */
} __attribute__ ((packed)) BUS_Params;
SL_PACK_END()

/**
 * @brief BUS default parameter.
 *
 * This macro indicates the default parameters of the BUS module.
 */
#define BUS_PARAMS_DEFAULT \
{ \
	false, /**< STC3105 not initialized. */ \
	false, /**< RV3049 not initialized. */ \
	false, /**< TFBC4711 not initialized. */ \
	false, /**< BMX055_ACC not initialized. */ \
	false, /**< BMX055_GYRO not initialized. */ \
	false, /**< BMX055_MAG not initialized. */ \
	false, /**< ADXL375 not initialized. */ \
	false, /**< NRF51422 not initialized. */ \
}

SL_ALIGN(4)
SL_PACK_START(1)
static BUS_Params __attribute__ ((aligned(4))) busParams = BUS_PARAMS_DEFAULT;
SL_PACK_END()

/**
 * @brief I2C bus default parameter for the STC3105 chip.
 *
 * This macro indicates the I2C bus module's default parameter for the STC3105 chip.
 */
#define I2C_STC3105_INIT \
{ \
	true,					/**< Enable module when initialization done. */ \
	true,					/**< Set module to master mode. */ \
	0,						/**< Use current configured hardware clock. */ \
	I2C_STC3105_SPEED,		/**< Set speed defined in the hardware configuration. */ \
	i2cClockHLRAsymetric,	/**< Set asymmetric duty cycle for the clock signal. */ \
}

/**
 * @brief SPI bus default parameter for the RV3049 chip.
 *
 * This macro indicates the SPI bus module's default parameter for the RV3049 chip.
 */
#define SPI_RV3049_INIT \
{ \
	usartEnable,		/**< Enable module when initialization done. */ \
	0,					/**< Use current configured hardware clock. */ \
	SPI_RV3049_SPEED,	/**< Set speed defined in hardware configuration. */ \
	usartDatabits8,		/**< Set data frame to 8 bits. */ \
	true,				/**< Set module to master mode. */ \
	true,				/**< Send most significant bit first. */ \
	usartClockMode0,	/**< Set clock to idle low and sample on rising edge. */ \
	false,				/**< Disable PRS input. */ \
	usartPrsRxCh0,		/**< Select PRS channel 0. */ \
	false,				/**< Disable auto transmit mode. */ \
}

/**
 * @brief SPI bus default parameter for the ADXL375 chip.
 *
 * This macro indicates the SPI bus module's default parameter for the ADXL375 chip.
 */
#define SPI_ADXL375_INIT \
{ \
	usartEnable,		/**< Enable module when initialization done. */ \
	0,					/**< Use current configured hardware clock. */ \
	SPI_ADXL375_SPEED,	/**< Set speed defined in the hardware configuration. */ \
	usartDatabits8,		/**< Set data frame to 8 bits. */ \
	true,				/**< Set module to master mode. */ \
	true,				/**< Send most significant bit first. */ \
	usartClockMode3,	/**< Set clock to idle high and sample on rising edge. */ \
	false,				/**< Disable PRS input. */ \
	usartPrsRxCh0,		/**< Select PRS channel 0. */ \
	false,				/**< Disable auto transmit mode. */ \
}
/**
 * @brief SPI bus default parameter for the BMX055 chip.
 *
 * This macro indicates the SPI bus module's default parameter for the BMX055 chip.
 */
#define SPI_BMX055_INIT \
{ \
	usartEnable,		/**< Enable module when initialization done. */ \
	0,					/**< Use current configured hardware clock. */ \
	SPI_BMX055_SPEED,	/**< Set speed defined in the hardware configuration. */ \
	usartDatabits8,		/**< Set data frame to 8 bits. */ \
	true,				/**< Set module to master mode. */ \
	true,				/**< Send most significant bit first. */ \
	usartClockMode3,	/**< Set clock to idle high and sample on rising edge. */ \
	false,				/**< Disable PRS input. */ \
	usartPrsRxCh0,		/**< Select PRS channel 0. */ \
	false,				/**< Disable auto transmit mode. */ \
}

/**
 * @brief SPI bus default parameter for the NRF51422 chip.
 *
 * This macro indicates the SPI bus module's default parameter for the NRF51422 chip.
 */
#define SPI_NRF51422_INIT \
{ \
	usartEnable,		/**< Enable module when initialization done. */ \
	0,					/**< Use current configured hardware clock. */ \
	SPI_NRF51422_SPEED,	/**< Set speed defined in the hardware configuration. */ \
	usartDatabits8,		/**< Set data frame to 8 bits. */ \
	true,				/**< Set module to master mode. */ \
	true,				/**< Send most significant bit first. */ \
	usartClockMode3,	/**< Set clock to idle high and sample on rising edge. */ \
	false,				/**< Disable PRS input. */ \
	usartPrsRxCh0,		/**< Select PRS channel 0. */ \
	false,				/**< Disable auto transmit mode. */ \
}
/****************************************************************************************************************************************************
 ***   INTERRUPTS   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

/* I2C_STC3105 return status */
I2C_TransferReturn_TypeDef volatile I2C_STC3105_ReturnStatus = 0;

/* I2C1 interrupt handler */
void I2C1_IRQHandler(void)
{
	/* Run the I2C_Transfer function that checks interrupts flags and returns status */
	I2C_STC3105_ReturnStatus = I2C_Transfer(I2C_STC3105_MODULE);
}

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

/****************************************************************************************************************************************************
 ***   FUNCTIONS   **********************************************************************************************************************************
 ***************************************************************************************************************************************************/

/**
 * @brief Initialization of specific bus interfaces.
 *
 * This function initializes a specific interface bus depending on the selected chip.
 *
 * @param BUS_Sensor_Type sensor	specific chip
 * @param bool en					enables or disables the interface
 */
void BUS_Init(BUS_Sensor_TypeDef sensor, bool en)
{
	switch(sensor)
	{
		case STC3105:
			/* Enable oder disable interface */
			if(en == false)
			{
				I2C_Reset(I2C_STC3105_MODULE);
				CMU_ClockEnable(I2C_STC3105_CLOCK, false);
				busParams.stc3105BusInit = false;
			}
			else
			{
				/* Check if interface is already initialized */
				if(busParams.stc3105BusInit == false)
				{
					/* Enable peripheral and I2C_STC3105_MODULE clock, setup pins */
					I2C_Reset(I2C_STC3105_MODULE);
					CMU_ClockEnable(cmuClock_HFPER, true);
					CMU_ClockEnable(I2C_STC3105_CLOCK, true);
					CMU_ClockEnable(cmuClock_GPIO, true);
					GPIO_PinModeSet(I2C_STC3105_SCL_PORT, I2C_STC3105_SCL_PIN, gpioModeWiredAnd, 1);
					GPIO_PinModeSet(I2C_STC3105_SDA_PORT, I2C_STC3105_SDA_PIN, gpioModeWiredAnd, 1);
					/* Initialize I2C_STC3105_MODULE module */
					I2C_Init_TypeDef init = I2C_STC3105_INIT;
					I2C_Init(I2C_STC3105_MODULE, &init);
					busParams.stc3105BusInit = true;
					/* Enable pins at specific location */
					I2C_STC3105_MODULE->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | I2C_STC3105_LOCATION;
					/* Set I2C_STC3105_MODULE bus frequency */
					I2C_BusFreqSet(I2C_STC3105_MODULE, 0, I2C_STC3105_SPEED, i2cClockHLRAsymetric);
					/* Clear all interrupts from I2C_STC3105_MODULE module */
					NVIC_ClearPendingIRQ(I2C_STC3105_IRQ);
				}
			}
			break;

		case RV3049:
			/* Enable oder disable interface */
			if(en == false)
			{
				USART_Reset(SPI_RV3049_MODULE);
				CMU_ClockEnable(SPI_RV3049_CLOCK, false);
				busParams.rv3049BusInit = false;
			}
			else
			{
				/* Check if interface is already initialized */
				if(busParams.rv3049BusInit == false)
				{
					/* Enable peripheral and SPI_RV3049_MODULE clock, setup pins */
					USART_Reset(SPI_RV3049_MODULE);
					CMU_ClockEnable(cmuClock_HFPER, true);
					CMU_ClockEnable(SPI_RV3049_CLOCK, true);
					CMU_ClockEnable(cmuClock_GPIO, true);
					GPIO_PinModeSet(SPI_RV3049_MOSI_PORT, SPI_RV3049_MOSI_PIN, gpioModePushPull, 0);
					GPIO_PinModeSet(SPI_RV3049_MISO_PORT, SPI_RV3049_MISO_PIN, gpioModeInput, 0);
					GPIO_PinModeSet(SPI_RV3049_CLK_PORT, SPI_RV3049_CLK_PIN, gpioModePushPull, 0);
					GPIO_PinModeSet(SPI_RV3049_CS_PORT, SPI_RV3049_MISO_PIN, gpioModePushPull, 0);
					/* Initialize SPI_RV3049_MODULE as synchronous module */
					USART_InitSync_TypeDef init = SPI_RV3049_INIT;
					USART_InitSync(SPI_RV3049_MODULE, &init);
					busParams.rv3049BusInit = true;
					/* Enable pins at specific location */
					SPI_RV3049_MODULE->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | SPI_RV3049_LOCATION;
					/* Set SPI_RV3049_MODULE bus frequency */
					USART_BaudrateSyncSet(SPI_RV3049_MODULE, 0, SPI_RV3049_SPEED);
					/* Clear all interrupts from SPI_RV3049_MODULE module */
					NVIC_ClearPendingIRQ(SPI_RV3049_IRQ_TX);
					NVIC_ClearPendingIRQ(SPI_RV3049_IRQ_RX);
				}
			}
			break;

		case TFBS4711:
			break;

		case BMX055_ACC:   //(for Accelerometer)
			/* Enable oder disable interface */
						if(en == false)
						{
							USART_Reset(SPI_BMX055_MODULE);
							CMU_ClockEnable(SPI_BMX055_CLOCK, false);
							busParams.bmx055accBusInit = false;
						}
						else
						{
							/* Check if interface is already initialized */
							if(busParams.bmx055accBusInit == false)
							{
								/* Enable peripheral and SPI_BMX055_MODULE clock, setup pins */
								USART_Reset(SPI_BMX055_MODULE);
								CMU_ClockEnable(cmuClock_HFPER, true);
								CMU_ClockEnable(SPI_BMX055_CLOCK, true);
								CMU_ClockEnable(cmuClock_GPIO, true);
								GPIO_PinModeSet(SPI_BMX055_MOSI_PORT, SPI_BMX055_MOSI_PIN, gpioModePushPull, 0);
								GPIO_PinModeSet(SPI_BMX055_MISO_PORT, SPI_BMX055_MISO_PIN, gpioModeInput, 0);
								GPIO_PinModeSet(SPI_BMX055_CLK_PORT, SPI_BMX055_CLK_PIN, gpioModePushPull, 1);
								GPIO_PinModeSet(SPI_BMX055_CS_ACC_PORT, SPI_BMX055_CS_ACC_PIN, gpioModePushPull, 1);
								/* Initialize SPI_BMX055_MODULE as synchronous module */
								USART_InitSync_TypeDef init = SPI_BMX055_INIT;
								USART_InitSync(SPI_BMX055_MODULE, &init);
								busParams.bmx055accBusInit = true;
								/* Enable pins at specific location */
								SPI_BMX055_MODULE->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | SPI_BMX055_LOCATION;
								/* Set SPI_BMX055_MODULE bus frequency */
								USART_BaudrateSyncSet(SPI_BMX055_MODULE, 0, SPI_BMX055_SPEED);
								/* Clear all interrupts from SPI_BMX055_MODULE module */
								NVIC_ClearPendingIRQ(SPI_BMX055_IRQ_TX);
								NVIC_ClearPendingIRQ(SPI_BMX055_IRQ_RX);
							}
						}
						break;
		case BMX055_GYRO:   //(for Accelerometer)
					/* Enable oder disable interface */
					if(en == false)
					{
						USART_Reset(SPI_BMX055_MODULE);
						CMU_ClockEnable(SPI_BMX055_CLOCK, false);
						busParams.bmx055gyroBusInit = false;
					}
					else
					{
						/* Check if interface is already initialized */
						if(busParams.bmx055gyroBusInit == false)
						{
							/* Enable peripheral and SPI_BMX055_MODULE clock, setup pins */
							USART_Reset(SPI_BMX055_MODULE);
							CMU_ClockEnable(cmuClock_HFPER, true);
							CMU_ClockEnable(SPI_BMX055_CLOCK, true);
							CMU_ClockEnable(cmuClock_GPIO, true);
							GPIO_PinModeSet(SPI_BMX055_MOSI_PORT, SPI_BMX055_MOSI_PIN, gpioModePushPull, 0);
							GPIO_PinModeSet(SPI_BMX055_MISO_PORT, SPI_BMX055_MISO_PIN, gpioModeInput, 0);
							GPIO_PinModeSet(SPI_BMX055_CLK_PORT, SPI_BMX055_CLK_PIN, gpioModePushPull, 1);
							GPIO_PinModeSet(SPI_BMX055_CS_GYRO_PORT, SPI_BMX055_CS_GYRO_PIN, gpioModePushPull, 1);
							/* Initialize SPI_BMG055_MODULE as synchronous module */
							USART_InitSync_TypeDef init = SPI_BMX055_INIT;
							USART_InitSync(SPI_BMX055_MODULE, &init);
							busParams.bmx055gyroBusInit = true;
							/* Enable pins at specific location */
							SPI_BMX055_MODULE->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | SPI_BMX055_LOCATION;
							/* Set SPI_BMG055_MODULE bus frequency */
							USART_BaudrateSyncSet(SPI_BMX055_MODULE, 0, SPI_BMX055_SPEED);
							/* Clear all interrupts from SPI_BMG055_MODULE module */
							NVIC_ClearPendingIRQ(SPI_BMX055_IRQ_TX);
							NVIC_ClearPendingIRQ(SPI_BMX055_IRQ_RX);
						}
					}
						break;
		case BMX055_MAG:   //(for Accelerometer)
					/* Enable oder disable interface */
			/* Enable oder disable interface */
			if(en == false)
			{
				USART_Reset(SPI_BMX055_MODULE);
				CMU_ClockEnable(SPI_BMX055_CLOCK, false);
				busParams.bmx055magBusInit = false;
			}
			else
			{
				/* Check if interface is already initialized */
				if(busParams.bmx055magBusInit == false)
				{
					/* Enable peripheral and SPI_BMX055_MODULE clock, setup pins */
					USART_Reset(SPI_BMX055_MODULE);
					CMU_ClockEnable(cmuClock_HFPER, true);
					CMU_ClockEnable(SPI_BMX055_CLOCK, true);
					CMU_ClockEnable(cmuClock_GPIO, true);
					GPIO_PinModeSet(SPI_BMX055_MOSI_PORT, SPI_BMX055_MOSI_PIN, gpioModePushPull, 0);
					GPIO_PinModeSet(SPI_BMX055_MISO_PORT, SPI_BMX055_MISO_PIN, gpioModeInput, 0);
					GPIO_PinModeSet(SPI_BMX055_CLK_PORT, SPI_BMX055_CLK_PIN, gpioModePushPull, 1);
					GPIO_PinModeSet(SPI_BMX055_CS_MAG_PORT, SPI_BMX055_CS_MAG_PIN, gpioModePushPull, 1);
					/* Initialize SPI_BMG055_MODULE as synchronous module */
					USART_InitSync_TypeDef init = SPI_BMX055_INIT;
					USART_InitSync(SPI_BMX055_MODULE, &init);
					busParams.bmx055magBusInit = true;
					/* Enable pins at specific location */
					SPI_BMX055_MODULE->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | SPI_BMX055_LOCATION;
					/* Set SPI_BMG055_MODULE bus frequency */
					USART_BaudrateSyncSet(SPI_BMX055_MODULE, 0, SPI_BMX055_SPEED);
					/* Clear all interrupts from SPI_BMG055_MODULE module */
					NVIC_ClearPendingIRQ(SPI_BMX055_IRQ_TX);
					NVIC_ClearPendingIRQ(SPI_BMX055_IRQ_RX);
				}
			}
				break;

		case ADXL375:
			/* Enable oder disable interface */
			if(en == false)
			{
				USART_Reset(SPI_ADXL375_MODULE);
				CMU_ClockEnable(SPI_ADXL375_CLOCK, false);
				busParams.adxl375BusInit = false;
			}
			else
			{
				/* Check if interface is already initialized */
				if(busParams.adxl375BusInit == false)
				{
					/* Enable peripheral and SPI_ADXL375_MODULE clock, setup pins */
					USART_Reset(SPI_ADXL375_MODULE);
					CMU_ClockEnable(cmuClock_HFPER, true);
					CMU_ClockEnable(SPI_ADXL375_CLOCK, true);
					CMU_ClockEnable(cmuClock_GPIO, true);
					GPIO_PinModeSet(SPI_ADXL375_MOSI_PORT, SPI_ADXL375_MOSI_PIN, gpioModePushPull, 0);
					GPIO_PinModeSet(SPI_ADXL375_MISO_PORT, SPI_ADXL375_MISO_PIN, gpioModeInput, 0);
					GPIO_PinModeSet(SPI_ADXL375_CLK_PORT, SPI_ADXL375_CLK_PIN, gpioModePushPull, 1);
					GPIO_PinModeSet(SPI_ADXL375_CS_PORT, SPI_ADXL375_CS_PIN, gpioModePushPull, 1);
					/* Initialize SPI_ADXL375_MODULE as synchronous module */
					USART_InitSync_TypeDef init = SPI_ADXL375_INIT;
					USART_InitSync(SPI_ADXL375_MODULE, &init);
					busParams.adxl375BusInit = true;
					/* Enable pins at specific location */
					SPI_ADXL375_MODULE->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | SPI_ADXL375_LOCATION;
					/* Set SPI_ADXL375_MODULE bus frequency */
					USART_BaudrateSyncSet(SPI_ADXL375_MODULE, 0, SPI_ADXL375_SPEED);
					/* Clear all interrupts from SPI_ADXL375_MODULE module */
					NVIC_ClearPendingIRQ(SPI_ADXL375_IRQ_TX);
					NVIC_ClearPendingIRQ(SPI_ADXL375_IRQ_RX);
				}
			}
			break;

		case NRF51422:
			/* Enable oder disable interface */
						if(en == false)
						{
							USART_Reset(SPI_NRF51422_MODULE);
							CMU_ClockEnable(SPI_NRF51422_CLOCK, false);
							busParams.nrf51422BusInit = false;
						}
						else
						{
							/* Check if interface is already initialized */
							if(busParams.nrf51422BusInit == false)
							{
								/* Enable peripheral and SPI_NRF51422_MODULE clock, setup pins */
								USART_Reset(SPI_NRF51422_MODULE);
								CMU_ClockEnable(cmuClock_HFPER, true);
								CMU_ClockEnable(SPI_NRF51422_CLOCK, true);
								CMU_ClockEnable(cmuClock_GPIO, true);
								GPIO_PinModeSet(SPI_NRF51422_MOSI_PORT, SPI_NRF51422_MOSI_PIN, gpioModePushPull, 0);
								GPIO_PinModeSet(SPI_NRF51422_MISO_PORT, SPI_NRF51422_MISO_PIN, gpioModeInput, 0);
								GPIO_PinModeSet(SPI_NRF51422_CLK_PORT, SPI_NRF51422_CLK_PIN, gpioModePushPull, 1);
								GPIO_PinModeSet(SPI_NRF51422_CS_PORT, SPI_NRF51422_CS_PIN, gpioModePushPull, 1);
								/* Initialize SPI_NRF51422_MODULE as synchronous module */
								USART_InitSync_TypeDef init = SPI_NRF51422_INIT;
								USART_InitSync(SPI_NRF51422_MODULE, &init);
								busParams.nrf51422BusInit = true;
								/* Enable pins at specific location */
								SPI_NRF51422_MODULE->ROUTE = USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN | SPI_NRF51422_LOCATION;
								/* Set SPI_NRF51422_MODULE bus frequency */
								USART_BaudrateSyncSet(SPI_NRF51422_MODULE, 0, SPI_NRF51422_SPEED);
								/* Clear all interrupts from SPI_NRF51422_MODULE module */
								NVIC_ClearPendingIRQ(SPI_NRF51422_IRQ_TX);
								NVIC_ClearPendingIRQ(SPI_NRF51422_IRQ_RX);
							}
						}
						break;


		default:
			break;
	}

	/* Print debug information */
#ifdef BUS_DEBUG
	printf("BUS_Init(): initialized stc3105  	= %i\n", (int)busParams.stc3105BusInit);
	printf("BUS_Init(): initialized rv3049   	= %i\n", (int)busParams.rv3049BusInit);
	printf("BUS_Init(): initialized tfbs4711 	= %i\n", (int)busParams.tfbs4711BusInit);
	printf("BUS_Init(): initialized bmx055_acc  = %i\n", (int)busParams.bmx055accBusInit);
	printf("BUS_Init(): initialized bmx055_gyro = %i\n", (int)busParams.bmx055gyroBusInit);
	printf("BUS_Init(): initialized bmx055_mag  = %i\n", (int)busParams.bmx055magBusInit);
	printf("BUS_Init(): initialized adxl375  	= %i\n", (int)busParams.adxl375BusInit);
	printf("BUS_Init(): initialized nrf51422 	= %i\n", (int)busParams.nrf51422BusInit);
#endif
}

/**
 *
 * This function performs an I2C data receive transfer.
 *
 * @param BUS_Sensor_Type sensor		specific chip
 * @param uint8_t devAddr				I2C chip address
 * @param uint8_t reg					start register address to read from
 * @param uint8_t *data					received data
 * @param uint16_t n					number of bytes to be read
 * @return I2C_TransferReturn_TypeDef 	status of the performed transmission
 */
I2C_TransferReturn_TypeDef BUS_I2C_ReadNBytes(BUS_Sensor_TypeDef sensor, uint8_t devAddr, uint8_t reg, uint8_t *data, uint16_t n)
{
	/* Create transfer sequence */
	I2C_TransferSeq_TypeDef sequence;
	sequence.addr = devAddr;
	sequence.flags = I2C_FLAG_WRITE_READ;
	/* Output control buffer */
	sequence.buf[0].data = &reg;
	sequence.buf[0].len = 1;
	/* Output data buffer */
	sequence.buf[1].data = data;
	sequence.buf[1].len = n;
	/* Select sensor type */
	switch(sensor)
	{
		case STC3105:
			/* Enable interrupt */
			NVIC_EnableIRQ(I2C_STC3105_IRQ);
			/* Initialize bus transfer */
			I2C_STC3105_ReturnStatus = I2C_TransferInit(I2C_STC3105_MODULE, &sequence);
			/* Do acknowledge polling */
			while(I2C_STC3105_ReturnStatus == i2cTransferInProgress)
			{
				/* Save energy and go to sleep */
				EMU_EnterEM1();
			}
			/* Disable interrupt */
			NVIC_DisableIRQ(I2C_STC3105_IRQ);
			break;

		default:
			break;
	}
	/* Error handling, should be more differentiated */
	return I2C_STC3105_ReturnStatus;
}

/**
 * @brief I2C write function.
 *
 * This function performs an I2C data transmit transfer.
 *
 * @param BUS_Sensor_Type sensor		specific chip
 * @param uint8_t devAddr				I2C chip address
 * @param uint8_t reg					start register address to write to
 * @param uint8_t *data					data to be written
 * @param uint16_t n					number of bytes to be written
 * @return I2C_TransferReturn_TypeDef	status of the performed transmission
 */
I2C_TransferReturn_TypeDef BUS_I2C_WriteNBytes(BUS_Sensor_TypeDef sensor, uint8_t devAddr, uint8_t reg, uint8_t *data, uint16_t len)
{
	/* Create transfer sequence */
	I2C_TransferSeq_TypeDef sequence;
	sequence.addr = devAddr;
	sequence.flags = I2C_FLAG_WRITE_WRITE;
	/* Output control buffer */
	sequence.buf[0].data = &reg;
	sequence.buf[0].len = 1;
	/* Output data buffer */
	sequence.buf[1].data = data;
	sequence.buf[1].len = len;
	/* Select sensor type */
	switch(sensor)
	{
		case STC3105:
			/* Enable interrupt */
			NVIC_EnableIRQ(I2C_STC3105_IRQ);
			/* Initialize bus transfer */
			I2C_STC3105_ReturnStatus = I2C_TransferInit(I2C_STC3105_MODULE, &sequence);
			/* Do acknowledge polling */
			while(I2C_STC3105_ReturnStatus == i2cTransferInProgress)
			{
				/* Save energy and go to sleep */
				EMU_EnterEM1();
			}
			/* Disable interrupt */
			NVIC_DisableIRQ(I2C_STC3105_IRQ);
			break;

		default:
			break;
	}
	/* Error handling, should be more differentiated */
	return I2C_STC3105_ReturnStatus;
}

/**
 * @brief SPI transfer function.
 *
 * This function performs a SPI data transfer.
 *
 * @param BUS_Sensor_Type sensor	specific chip
 * @param uint8_t *dataTx			data to be written
 * @param uint8_t *dataRx			data to be read
 * @param uint16_t n				number of bytes to be transferred
 */
void BUS_SPI_TransferNBytes(BUS_Sensor_TypeDef sensor, uint8_t *dataTx, uint8_t *dataRx, uint8_t len)
{
	uint8_t i;
	/* Select sensor type */
	switch(sensor)
	{
		case RV3049:
			/* Initiate SPI data transfer */
			GPIO_PinOutSet(SPI_RV3049_CS_PORT, SPI_RV3049_CS_PIN);
			/* Do SPI data transfer */
			for(i = 0; i < len; i++)
			{
				dataRx[i] = USART_SpiTransfer(SPI_RV3049_MODULE, dataTx[i]);
			}
			/* Stop SPI data transfer */
			GPIO_PinOutClear(SPI_RV3049_CS_PORT, SPI_RV3049_CS_PIN);
			break;

		case TFBS4711:
			break;

		case BMX055_ACC:
			/* Initiate SPI data transfer */
			GPIO_PinOutClear(SPI_BMX055_CS_ACC_PORT, SPI_BMX055_CS_ACC_PIN);
			/* Do SPI data transfer */
			for(i = 0; i < len; i++)
			{
				dataRx[i] = USART_SpiTransfer(SPI_BMX055_MODULE, dataTx[i]);
			}
			/* Stop SPI data transfer */
			GPIO_PinOutSet(SPI_BMX055_CS_ACC_PORT, SPI_BMX055_CS_ACC_PIN);
			break;

		case BMX055_GYRO:
			/* Initiate SPI data transfer */
			GPIO_PinOutClear(SPI_BMX055_CS_GYRO_PORT, SPI_BMX055_CS_GYRO_PIN);
			/* Do SPI data transfer */
			for(i = 0; i < len; i++)
			{
				dataRx[i] = USART_SpiTransfer(SPI_BMX055_MODULE, dataTx[i]);
			}
			/* Stop SPI data transfer */
			GPIO_PinOutSet(SPI_BMX055_CS_GYRO_PORT, SPI_BMX055_CS_GYRO_PIN);
			break;
		case BMX055_MAG:
			/* Initiate SPI data transfer */
			GPIO_PinOutClear(SPI_BMX055_CS_MAG_PORT, SPI_BMX055_CS_MAG_PIN);
			/* Do SPI data transfer */
			for(i = 0; i < len; i++)
			{
				dataRx[i] = USART_SpiTransfer(SPI_BMX055_MODULE, dataTx[i]);
			}
			/* Stop SPI data transfer */
			GPIO_PinOutSet(SPI_BMX055_CS_MAG_PORT, SPI_BMX055_CS_MAG_PIN);
			break;
		case ADXL375:
			/* Initiate SPI data transfer */
			GPIO_PinOutClear(SPI_ADXL375_CS_PORT, SPI_ADXL375_CS_PIN);
			/* Do SPI data transfer */
			for(i = 0; i < len; i++)
			{
				dataRx[i] = USART_SpiTransfer(SPI_ADXL375_MODULE, dataTx[i]);
			}
			/* Stop SPI data transfer */
			GPIO_PinOutSet(SPI_ADXL375_CS_PORT, SPI_ADXL375_CS_PIN);
			break;

		case NRF51422:
			/* Initiate SPI data transfer */
			GPIO_PinOutClear(SPI_NRF51422_CS_PORT, SPI_NRF51422_CS_PIN);
			/* Do SPI data transfer */
			for(i = 0; i < len; i++)
			{
				dataRx[i] = USART_SpiTransfer(SPI_NRF51422_MODULE, dataTx[i]);
			}
			/* Stop SPI data transfer */
			GPIO_PinOutSet(SPI_NRF51422_CS_PORT, SPI_NRF51422_CS_PIN);
			break;

		default:
			break;
	}
}
