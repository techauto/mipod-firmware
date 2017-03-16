///****************************************************************************************************************************************************
// ***   INCLUDES   ***********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
//#include "BMA055.h"
//
///****************************************************************************************************************************************************
// ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
// ***************************************************************************************************************************************************/
//
///* Default BMA055 parameter structure */
//#define BMA055_PARAMS_DEFAULT				\
//{											\
//	false,									\
//	BMA055AccXAccYAccZ,						\
//	BMA055Normal125Hz,					\
//	0,										\
//}
//
//SL_ALIGN(4)
//SL_PACK_START(1)
//static BMA055_Params __attribute__ ((aligned(4))) BMA055Params = BMA055_PARAMS_DEFAULT;
//SL_PACK_END()
///****************************************************************************************************************************************************
// ***   PROTOTYPES   *********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//__STATIC_INLINE void getDeviceId(uint8_t *data);
//__STATIC_INLINE void setFrequency(BMA055_Frequency_TypeDef freq);
//__STATIC_INLINE void setAccelerometerRange(BMA055_Accelerometer_TypeDef afs); ///* Set accelerometer range
//__STATIC_INLINE void spiWrite(uint8_t ctrlReg, uint8_t *writeData, uint8_t len);
//__STATIC_INLINE void spiRead(uint8_t ctrlReg, uint8_t *readData, uint8_t len);
//
///****************************************************************************************************************************************************
// ***   FUNCTIONS   **********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//static volatile uint32_t msTicks; /* counts 1ms timeTicks */
//
//static void Delay(uint32_t dlyTicks);
//
///**************************************************************************//**
// * @brief SysTick_Handler
// * Interrupt Service Routine for system tick counter
// *****************************************************************************/
//void SysTick_Handler(void)
//{
//  msTicks++;       /* increment counter necessary in Delay()*/
//}
//
///**************************************************************************//**
// * @brief Delays number of msTick Systicks (typically 1 ms)
// * @param dlyTicks Number of ticks to delay
// *****************************************************************************/
//
//static void Delay(uint32_t dlyTicks)
//{
//  uint32_t curTicks;
//
//  curTicks = msTicks;
//  while ((msTicks - curTicks) < dlyTicks) ;
//}
//
//BMA055_Return_TypeDef BMA055_Init(BMA055_Sensors_TypeDef sensors, BMA055_Frequency_TypeDef freq)
//{
//
//
//		BMA055_Return_TypeDef retStatus;
//
//		/* Set sensor selection */
//		BMA055Params.sensors = sensors;
//
//		/* Set sampling frequency */
//		BMA055Params.freq = freq;
//
//			/*Reset BMA055*/
//			uint8_t BMX_reset;
//			uint8_t BMX_Power;
//			BMX_Power=0;
//			BMX_reset=182;											//182=0xB6
//			spiWrite(BMA055_BGW_SOFTRESET,&BMX_reset,1);			// reset accelerometer
//			Delay(1000);											// Wait for all registers to reset
//			setAccelerometerRange(BMA055Accelerometer_16G);			// Set accelerometer full range
//			setFrequency(BMA055Normal125Hz);						// Set accelerometer bandwidth
//			//spiWrite(BMA055_POWER_CTL_REG,&BMX_Power,1);			// Use filtered data
//			/* Set sensor selection */
//			BMA055Params.sensors = sensors;
//			/* Set sampling frequency */
//			BMA055Params.freq = freq;
//
//		/* Get sensor ID */
//		getDeviceId(&BMA055Params.id);
//		/* Set initialization flag */
//		if(BMA055Params.id == BMA055_DEVID_DEFAULT)
//		{
//			retStatus = BMA055Ok;
//			BMA055Params.init = true;
//		}
//		else
//		{
//			retStatus = BMA055InitError;
//			BMA055Params.init = false;
//		}
//		/* Print debug information */
//	#ifdef BMA055_DEBUGGING
//		printf("BMA055_Init(): params.sensors = %i\n", (int)BMA055Params.sensors);
//		printf("BMA055_Init(): params.freq    = %i\n", (int)BMA055Params.freq);
//		printf("BMA055_Init(): params.init    = %i\n", (int)BMA055Params.init);
//		printf("BMA055_Init(): params.id      = %i\n", (int)BMA055Params.id);
//	#endif
//
//		/* Return status */
//		return retStatus;
//}
///***************************************************************************************************************************************************/
////BMA055_Return_TypeDef BMA055_SetCalibrationOffsets(int32_t offX, int32_t offY, int32_t offZ)
////{
////	/* Input format offX, offY, offZ in [mg] */
////
////	uint8_t regOffX = (uint8_t)(COMMON_devisionToNearestInteger(offX, BMA055_OFFSET_CONST));
////	uint8_t regOffY = (uint8_t)(COMMON_devisionToNearestInteger(offY, BMA055_OFFSET_CONST));
////	uint8_t regOffZ = (uint8_t)(COMMON_devisionToNearestInteger(offZ, BMA055_OFFSET_CONST));
////
////	if((regOffX > 255) || (regOffY > 255) || (regOffZ > 255))
////	{
////		return BMA055OffsetError;
////	}
////
////	/* Print debug information */
////#ifdef BMA055_DEBUGGING
////	printf("BMA055_SetCalibrationOffsets(): xOff = %i\n", (int)regOffX);
////	printf("BMA055_SetCalibrationOffsets(): yOff = %i\n", (int)regOffY);
////	printf("BMA055_SetCalibrationOffsets(): zOff = %i\n", (int)regOffZ);
////#endif
////
////	spiWrite(BMA055_OFSX_REG, &regOffX, 1);
////	spiWrite(BMA055_OFSY_REG, &regOffY, 1);
////	spiWrite(BMA055_OFSZ_REG, &regOffZ, 1);
////
////	return BMA055Ok;
////}
//
///***************************************************************************************************************************************************/
//__STATIC_INLINE void getDeviceId(uint8_t *data)
//{
//	spiRead(BMA055_DEVID_REG, data, 1);
//
//}
///***************************************************************************************************************************************************/
//BMA055_Return_TypeDef  getDataAccXYZ(uint8_t *temp)
//{
//	//uint8_t temp[6];
//
//	spiRead(BMA055_DATAX0_REG,&temp[0] , 1);
//	spiRead(BMA055_DATAX1_REG,&temp[1] , 1);
//	spiRead(BMA055_DATAY0_REG,&temp[2] , 1);
//	spiRead(BMA055_DATAX1_REG,&temp[3] , 1);
//	spiRead(BMA055_DATAZ0_REG,&temp[4] , 1);
//	spiRead(BMA055_DATAZ1_REG,&temp[5] , 1);
//
//#ifdef BMA055_DEBUGGING
//
//	printf("AX = %i",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)temp[1] << 8) | temp[0])));
//	printf(" AY = %i",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)temp[3] << 8) | temp[2])));
//	printf(" AZ = %i\n",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)temp[5] << 8) | temp[4])));
//
//	//printf("AX = %i",	(int16_t) (((int16_t)temp[1] << 8) | temp[0]));//*16*1000/2048);
//	//printf(" AY = %i",	(int16_t) (((int16_t)temp[3] << 8) | temp[2]));//*16*1000/2048);
//	//printf(" AZ = %i\n",(int16_t) (((int16_t)temp[5] << 8) | temp[3]));//*16*1000/2048);
//
//
//#endif
//	return BMA055Ok;
//}
//
///***************************************************************************************************************************************************/
//__STATIC_INLINE void setFrequency(BMA055_Frequency_TypeDef freq)
//{
//	spiWrite(BMA055_BW_RATE_REG, &freq, 1);
//}
///***************************************************************************************************************************************************/
//__STATIC_INLINE void setAccelerometerRange(BMA055_Accelerometer_TypeDef afs)
//{
//	spiWrite(BMA055_PMU_RANGE, &afs, 1);
//}
///***************************************************************************************************************************************************/
//__STATIC_INLINE void spiWrite(uint8_t ctrlReg, uint8_t *writeData, uint8_t len)
//{
//	uint8_t txBuffer[len + 1];
//	uint8_t rxBuffer[len + 1];
//
//	/* Clear write indication bit */
//	ctrlReg &= ~(1 << 7);
//
//	/* Set multiple byte write bit */
//	if(len > 1)
//	{
//		ctrlReg |= (1 << 6);
//	}
//
//	txBuffer[0] = ctrlReg;
//
//	/* Move data buffers and transfer data */
//	memcpy((uint8_t *)(txBuffer + 1), writeData, len);
//	BUS_SPI_TransferNBytes(BMA055, txBuffer, rxBuffer, (len + 1));
//}
///***************************************************************************************************************************************************/
//__STATIC_INLINE void spiRead(uint8_t ctrlReg, uint8_t *readData, uint8_t len)
//{
//	uint8_t txBuffer[len + 1];
//	uint8_t rxBuffer[len + 1];
//
//	/* Set read indication bit */
//	ctrlReg |= (1 << 7);
//
//	/* Set multiple byte read bit */
//	if(len > 1)
//	{
//		ctrlReg |= (1 << 6);
//	}
//
//	txBuffer[0] = ctrlReg;
//
//	/* Move data buffers and transfer data */
//	BUS_SPI_TransferNBytes(BMA055, txBuffer, rxBuffer, (len + 1));
//	memcpy(readData, (uint8_t *)(rxBuffer + 1), len);
//}
