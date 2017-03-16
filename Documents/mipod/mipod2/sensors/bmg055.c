///****************************************************************************************************************************************************
// ***   INCLUDES   ***********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//
//#include "BMG055.h"
//
///****************************************************************************************************************************************************
// ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
// ***************************************************************************************************************************************************/
//
///* Default BMG055 parameter structure */
//#define BMG055_PARAMS_DEFAULT				\
//{											\
//	false,									\
//	BMG055GyroXGyroYGyroZ,						\
//	BMG055_100Hz12Hz,					\
//	0,										\
//}
//
//SL_ALIGN(4)
//SL_PACK_START(1)
//static BMG055_Params __attribute__ ((aligned(4))) BMG055Params = BMG055_PARAMS_DEFAULT;
//SL_PACK_END()
///****************************************************************************************************************************************************
// ***   PROTOTYPES   *********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//__STATIC_INLINE void getDeviceId(uint8_t *data);
//__STATIC_INLINE void setFrequency(BMG055_Frequency_TypeDef freq);
//__STATIC_INLINE void setGyroRange(BMG055_Gyro_TypeDef afs); ///* Set Gyro range
//__STATIC_INLINE void spiWrite(uint8_t ctrlReg, uint8_t *writeData, uint8_t len);
//__STATIC_INLINE void spiRead(uint8_t ctrlReg, uint8_t *readData, uint8_t len);
//
///****************************************************************************************************************************************************
// ***   FUNCTIONS   **********************************************************************************************************************************
// ***************************************************************************************************************************************************/
//BMG055_Return_TypeDef BMG055_Init(BMG055_Sensors_TypeDef sensors, BMG055_Frequency_TypeDef freq)
//{
//
//
//		BMG055_Return_TypeDef retStatus;
//
//		/* Set sensor selection */
//		BMG055Params.sensors = sensors;
//
//		/* Set sampling frequency */
//		BMG055Params.freq = freq;
//
//		setGyroRange(BMG055_1000DPS);  						// set GYRO FS range
//		setFrequency(BMG055_1000Hz116Hz);      				// set GYRO ODR and Bandwidth
//
//		/* Set sensor selection */
//			BMG055Params.sensors = sensors;
//			/* Set sampling frequency */
//			BMG055Params.freq = freq;
//
//		/* Get sensor ID */
//		getDeviceId(&BMG055Params.id);
//		/* Set initialization flag */
//		if(BMG055Params.id == BMG055_DEVID_DEFAULT)
//		{
//			retStatus = BMG055Ok;
//			BMG055Params.init = true;
//		}
//		else
//		{
//			retStatus = BMG055InitError;
//			BMG055Params.init = false;
//		}
//		/* Print debug information */
//	#ifdef BMG055_DEBUGGING
//		printf("BMG055_Init(): params.sensors = %i\n", (int)BMG055Params.sensors);
//		printf("BMG055_Init(): params.freq    = %i\n", (int)BMG055Params.freq);
//		printf("BMG055_Init(): params.init    = %i\n", (int)BMG055Params.init);
//		printf("BMG055_Init(): params.id      = %i\n", (int)BMG055Params.id);
//	#endif
//
//		/* Return status */
//		return retStatus;
//}
//BMG055_Return_TypeDef getDataGyroXYZ(uint8_t *temp)
//{
//
//	spiRead(BMG055_DATAX0_REG,&temp[0] , 1);
//	spiRead(BMG055_DATAX1_REG,&temp[1] , 1);
//	spiRead(BMG055_DATAY0_REG,&temp[2] , 1);
//	spiRead(BMG055_DATAX1_REG,&temp[3] , 1);
//	spiRead(BMG055_DATAZ0_REG,&temp[4] , 1);
//	spiRead(BMG055_DATAZ1_REG,&temp[5] , 1);
//
//#ifdef BMG055_DEBUGGING
//	printf("GX = %i",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)temp[1] << 8) | temp[0])));
//	printf(" GY = %i",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)temp[3] << 8) | temp[2])));
//	printf(" GZ = %i\n",	(int)(COMMON_convert16bitComplementaryIntoSignedInt(((uint16_t)temp[5] << 8) | temp[4])));
//
//
//return BMG055Ok;
//#endif
//
//}
///***************************************************************************************************************************************************/
//__STATIC_INLINE void setFrequency(BMG055_Frequency_TypeDef freq)
//{
//	spiWrite(BMG055_BW, &freq, 1);
//}
///***************************************************************************************************************************************************/
//__STATIC_INLINE void setGyroRange(BMG055_Gyro_TypeDef afs)
//{
//	spiWrite(BMG055_RANGE, &afs, 1);
//}
///***************************************************************************************************************************************************/
//__STATIC_INLINE void getDeviceId(uint8_t *data)
//{
//	spiRead(BMG055_DEVID_REG, data, 1);
//
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
//	BUS_SPI_TransferNBytes(BMG055, txBuffer, rxBuffer, (len + 1));
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
//	BUS_SPI_TransferNBytes(BMG055, txBuffer, rxBuffer, (len + 1));
//	memcpy(readData, (uint8_t *)(rxBuffer + 1), len);
//}
