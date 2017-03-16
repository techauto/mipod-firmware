///**************************************************************************************************
// ***   INCLUDES   *********************************************************************************
// *************************************************************************************************/
//
//#include <stdbool.h>
//#include <stdint.h>
//
//#include "em_cmu.h"
//#include "em_ebi.h"
//#include "em_gpio.h"
//#include "em_int.h"
//
//#include "common.h"
//#include "debug.h"
//#include "s34ml04g2.h"
//
///**************************************************************************************************
// ***   GLOBAL VARIABLES   *************************************************************************
// *************************************************************************************************/
//
///* Information structure */
//static NANDFLASH_Info_TypeDef flashInfo;
//
///* NANDFLASH data access pointer */
//static uint8_t volatile *pNandData8;
//static uint16_t volatile *pNandData16;
//static uint32_t volatile *pNandData32;
//
///* NANDFLASH command and address access pointer */
//static uint8_t volatile *pNandAddr;
//static uint8_t volatile *pNandCmd;
//
///* NAND flash initialization flag */
//bool NANDFLASH_Inited = false;
//
//#ifdef S34ML04G2_TIMEOUT
///* S34ML04G2 timeout variable */
//uint32_t volatile S34ML04G2_Timeout;
//#endif
//
///**************************************************************************************************
// ***   INTERRUPTS  ********************************************************************************
// *************************************************************************************************/
//
///**************************************************************************************************
// ***   PROTOTYPES   *******************************************************************************
// *************************************************************************************************/
//
//void NANDFLASH_ChipEnable(bool enable);
//void NANDFLASH_WriteProtect(bool enable);
//
//static void NANDFLASH_Reset(void);
//static void NANDFLASH_WaitReady(void);
//
//static uint8_t NANDFLASH_ReadStatus(void);
////static uint64_t NANDFLASH_GetAddr(uint16_t col, uint8_t page, uint16_t block, uint8_t plane);
//static uint64_t NANDFLASH_GetPageAddr(uint32_t page);
//static uint64_t NANDFLASH_GetBlockAddr(uint32_t block);
//
//static NANDFLASH_Status_TypeDef NANDFLASH_ReadID(uint8_t *data, uint8_t len);
//static NANDFLASH_Status_TypeDef NANDFLASH_CorrectECC(uint32_t gEcc, uint32_t rEcc, uint8_t *data);
//
///**************************************************************************************************
// ***   FUNCTIONS   ********************************************************************************
// *************************************************************************************************/
//
//NANDFLASH_Status_TypeDef NANDFLASH_Init(void) {
//	/* NAND flash, bank0, base address 0x80000000 */
//	EBI_Init_TypeDef ebiConfig = {
//		ebiModeD8A8,       /* 8 bit address, 8 bit data 		*/
//		ebiActiveLow,      /* ARDY polarity 					*/
//		ebiActiveLow,      /* ALE polarity 						*/
//		ebiActiveLow,      /* WE polarity 						*/
//		ebiActiveLow,      /* RE polarity 						*/
//		ebiActiveLow,      /* CS polarity 						*/
//		ebiActiveLow,      /* BL polarity 						*/
//		false,             /* disable BL 						*/
//		true,              /* enable NOIDLE 					*/
//		false,             /* disable ARDY 						*/
//		true,              /* disable ARDY timeout 				*/
//		EBI_BANK0,         /* enable bank 0 					*/
//		0,                 /* no chip select 					*/
//		0,                 /* address setup cycles 				*/
//		0,                 /* address hold cycles 				*/
//		false,             /* disable half cycle ALE strobe 	*/
//		0,                 /* read setup cycles 				*/
//		2,                 /* read strobe cycles 				*/
//		1,                 /* read hold cycles 					*/
//		false,             /* disable page mode					*/
//		false,             /* disable prefetch 					*/
//		false,             /* disable half cycle REn strobe 	*/
//		0,                 /* write setup cycles 				*/
//		2,                 /* write strobe cycles 				*/
//		1,                 /* write hold cycles 				*/
//		false,             /* enable the write buffer 			*/
//		false,             /* disable half cycle WEn strobe 	*/
//		ebiALowA24,        /* ALB - Low bound, address lines 	*/
//	  	ebiAHighA26,       /* APEN - High bound, address lines	*/
//	  	ebiLocation1,      /* Use Location 1 					*/
//	  	true,              /* enable EBI 						*/
//	};
//	/* Enable clocks */
//	CMU_ClockEnable(cmuClock_HFPER, true);
//	CMU_ClockEnable(cmuClock_GPIO, true);
//	CMU_ClockEnable(cmuClock_EBI, true);
//	/* ALE and CLE pins */
//	GPIO_PinModeSet(S34ML04G2_ALE_PORT, S34ML04G2_ALE_PIN, gpioModePushPull, 0);
//	GPIO_PinModeSet(S34ML04G2_CLE_PORT, S34ML04G2_CLE_PIN, gpioModePushPull, 0);
//	/* WP, CE and R/B pins */
//	GPIO_PinModeSet(S34ML04G2_WP_PORT, S34ML04G2_WP_PIN, gpioModePushPull, 0);   /* active low write-protect	*/
//	GPIO_PinModeSet(S34ML04G2_CS_PORT, S34ML04G2_CS_PIN, gpioModePushPull, 1);   /* active low chip-enable 		*/
//	GPIO_PinModeSet(S34ML04G2_RB_PORT, S34ML04G2_RB_PIN, gpioModeInput, 0);      /* ready/busy 					*/
//	/* I/O pins */
//	GPIO_PinModeSet(gpioPortE, 8, gpioModePushPull, 0);
//	GPIO_PinModeSet(gpioPortE, 9, gpioModePushPull, 0);
//	GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 0);
//	GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);
//	GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);
//	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0);
//	GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 0);
//	GPIO_PinModeSet(gpioPortE, 15, gpioModePushPull, 0);
//	/* WE and RE pins */
//	GPIO_PinModeSet(S34ML04G2_WE_PORT, S34ML04G2_WE_PIN, gpioModePushPull, 1);
//	GPIO_PinModeSet(S34ML04G2_RE_PORT, S34ML04G2_RE_PIN, gpioModePushPull, 1);
//	/* NANDFLASH power enable pin, switched off */
//	GPIO_PinModeSet(S34ML04G2_PWR_EN_PORT, S34ML04G2_PWR_EN_PIN, gpioModePushPull, 0);
//	/* Initialize configuration */
//	EBI_Init(&ebiConfig);
//	EBI->NANDCTRL = (EBI_NANDCTRL_BANKSEL_BANK0 | EBI_NANDCTRL_EN);
//	/* Set memory addresses */
//	flashInfo.baseAddress = EBI_BankAddress(EBI_BANK0);
//	pNandData8 = (uint8_t volatile*) flashInfo.baseAddress;
//	pNandData16 = (uint16_t volatile*) pNandData8;
//	pNandData32 = (uint32_t volatile*) pNandData8;
//	pNandAddr = pNandData8 + (1 << NAND_ALE_BIT);
//	pNandCmd = pNandData8 + (1 << NAND_CLE_BIT);
//	/* Power on */
//	NANDFLASH_Power(true);
//	NANDFLASH_ChipEnable(true);
//	NANDFLASH_WriteProtect(false);
//	NANDFLASH_Reset();
//	NANDFLASH_ChipEnable(true);
//	NANDFLASH_WriteProtect(false);
//	/* Set NAND flash parameter */
//	flashInfo.pageSize = S34ML04G2_PAGESIZE;
//	flashInfo.spareSize = S34ML04G2_SPARESIZE;
//	flashInfo.blockSize = S34ML04G2_BLOCKSIZE;
//	flashInfo.planeSize = S34ML04G2_PLANESIZE;
//	flashInfo.deviceSize = S34ML04G2_SIZE;
//	flashInfo.badBlockPos = S34ML04G2_SPARE_BADBLOCK_POS;
//	flashInfo.ecc0Pos = S34ML04G2_SPARE_ECC0_POS;
//	flashInfo.ecc1Pos = S34ML04G2_SPARE_ECC1_POS;
//	flashInfo.ecc2Pos = S34ML04G2_SPARE_ECC2_POS;
//	flashInfo.pagesPerBlock = S34ML04G2_PAGES_PER_BLOCK;
//	flashInfo.totalBlocks = S34ML04G2_TOTAL_BLOCKS;
//	flashInfo.pagesPerPlane = S34ML04G2_PAGES_PER_PLANE;
//	flashInfo.blocksPerPlane = S34ML04G2_BLOCKS_PER_PLANE;
//#ifdef S34ML04G2_TIMEOUT
//	/* Set timeout */
//	S34ML04G2_Timeout = ((CMU_ClockFreqGet(cmuClock_HFPER) / 1000) * S34ML04G2_TIMEOUT_MSEC);
//#endif
//	/* Read signature and ID */
//	uint8_t data[S34ML04G2_ID_LENGTH];
//	NANDFLASH_Status_TypeDef status = NANDFLASH_ReadID(data, sizeof(data));
//	if (status != NANDFLASH_OK) {
//		/* Handle error */
//		NANDFLASH_Inited = false;
//		S34ML04G2_DEBUG("NANDFLASH_Init: device could not be initialized!\n");
//		return NANDFLASH_INIT_ERROR;
//	} else {
//		S34ML04G2_DEBUG("NANDFLASH_Init: manufacturer code: %#x\n", (int)data[S34ML04G2_MANUFACTURER_CODE_POS]);
//		S34ML04G2_DEBUG("NANDFLASH_Init: device code: %#x\n", (int)data[S34ML04G2_DEVICE_IDENTIFIER_POS]);
//		S34ML04G2_DEBUG("NANDFLASH_Init: internal chip: %#x\n", (int)data[S34ML04G2_INTERNAL_CHIP_NUMBER_POS]);
//		S34ML04G2_DEBUG("NANDFLASH_Init: organization: %#x\n", (int)data[S34ML04G2_ORGANIZATION_POS]);
//		S34ML04G2_DEBUG("NANDFLASH_Init: plane info: %#x\n", (int)data[S34ML04G2_PLANE_INFORMATION_POS]);
//		/* Success */
//		NANDFLASH_Inited = true;
//		S34ML04G2_DEBUG("NANDFLASH_Init: device successfully initialized!\n");
//		return NANDFLASH_OK;
//	}
//}
//
///*************************************************************************************************/
//bool NANDFLASH_IsOff(void) {
//	if (NANDFLASH_Inited == false) {
//		return true;
//	} else {
//		return false;
//	}
//}
//
///*************************************************************************************************/
//static void NANDFLASH_Reset(void) {
//	INT_Disable();
//	NAND_CMD = NANDFLASH_RESET_CMD;
//	INT_Enable();
//	NANDFLASH_WaitReady();
//}
//
///*************************************************************************************************/
//void NANDFLASH_ChipEnable(bool enable) {
//	if (enable == true) {
//		GPIO_PinOutClear(S34ML04G2_CS_PORT, S34ML04G2_CS_PIN);
//	} else {
//		GPIO_PinOutSet(S34ML04G2_CS_PORT, S34ML04G2_CS_PIN);
//	}
//	NANDFLASH_WaitReady();
//}
//
///*************************************************************************************************/
//static void NANDFLASH_WaitReady(void) {
//#ifdef S34ML04G2_TIMEOUT
//	/* Set timeout */
//	uint32_t timeout = S34ML04G2_Timeout;
//	/* Wait for EBI idle in case of EBI writeBuffer is enabled */
//	while ((EBI->STATUS & EBI_STATUS_AHBACT) && (timeout--));
//	/* Wait on R/B pin to become high */
//	while (((GPIO->P[S34ML04G2_RB_PORT].DIN >> S34ML04G2_RB_PIN) & 0x01) == 0);
//#else
//	/* Wait for EBI idle in case of EBI writeBuffer is enabled */
//	while (EBI->STATUS & EBI_STATUS_AHBACT);
//	/* Wait on R/B pin to become high */
//	while (((GPIO->P[S34ML04G2_RB_PORT].DIN >> S34ML04G2_RB_PIN) & 0x01) == 0);
//#endif
//}
//
///*************************************************************************************************/
//void NANDFLASH_WriteProtect(bool enable) {
//	if (enable == true) {
//		GPIO_PinOutClear(S34ML04G2_WP_PORT, S34ML04G2_WP_PIN);
//	} else {
//		GPIO_PinOutSet(S34ML04G2_WP_PORT, S34ML04G2_WP_PIN);
//	}
//	NANDFLASH_WaitReady();
//}
//
///*************************************************************************************************/
//void NANDFLASH_Power(bool enable) {
//	if (enable == true) {
//		GPIO_PinOutClear(S34ML04G2_PWR_EN_PORT, S34ML04G2_PWR_EN_PIN);
//	} else {
//		GPIO_PinOutSet(S34ML04G2_PWR_EN_PORT, S34ML04G2_PWR_EN_PIN);
//	}
//	NANDFLASH_WaitReady();
//}
//
///*************************************************************************************************/
//static uint8_t NANDFLASH_ReadStatus(void) {
//	uint8_t status;
//	INT_Disable();
//	NAND_CMD = NANDFLASH_READ_STATUS_REG_CMD;
//	status = NAND_DATA8;
//	INT_Enable();
//	NANDFLASH_WaitReady();
//	return status;
//}
//
///*************************************************************************************************/
//static NANDFLASH_Status_TypeDef NANDFLASH_ReadID(uint8_t *data, uint8_t len) {
//	/* Device information must be 5 bytes */
//	if (len != 5) {
//		return NANDFLASH_ID_ERROR;
//	}
//	INT_Disable();
//	NAND_CMD = NANDFLASH_READ_ID_CMD;
//	NAND_ADDR = 0x00;
//	NANDFLASH_WaitReady();
//	uint8_t i;
//	uint8_t *p = data;
//	for (i = 0; i < len; i++) {
//		*p++ = NAND_DATA8;
//	}
//	INT_Enable();
//	NANDFLASH_WaitReady();
//	/* Check IDs */
//	if ((data[S34ML04G2_MANUFACTURER_CODE_POS] != NANDFLASH_MANUFACTURER_ID) &&
//		(data[S34ML04G2_DEVICE_IDENTIFIER_POS] != NANDFLASH_DEVICE_ID) &&
//		(data[S34ML04G2_INTERNAL_CHIP_NUMBER_POS] != NANDFLASH_INTERNAL_CHIP_ID) &&
//		(data[S34ML04G2_ORGANIZATION_POS] != NANDFLASH_ORGANIZATION_ID) &&
//		(data[S34ML04G2_PLANE_INFORMATION_POS] != NANDFLASH_PLANE_INFO_ID)) {
//		S34ML04G2_DEBUG("NANDFLASH_ReadID: Could not read ID!\n");
//		return NANDFLASH_ID_ERROR;
//	} else {
//		S34ML04G2_DEBUG("NANDFLASH_ReadID: ID successfully read!\n");
//		return NANDFLASH_OK;
//	}
//}
//
/////*************************************************************************************************/
////NANDFLASH_Status_TypeDef NANDFLASH_CheckBlock(uint16_t block, uint8_t plane) {
////	/* Get correct block address */
////	uint64_t addr = NANDFLASH_GetAddr(0, 0, block, plane);
////	uint64_t spareAddr = (flashInfo.pageSize + flashInfo.badBlockPos);
////	uint64_t markerAddr;
////	uint16_t marker = 0;
////	/* Read spare data of first, second and last page of the block */
////	uint8_t i;
////	for (i = 0; i < 3; i++) {
////		switch(i) {
////			case 0:
////				markerAddr = addr | spareAddr | (uint64_t)(0 << 16);
////				break;
////			case 1:
////				markerAddr = addr | spareAddr | (uint64_t)(1 << 16);
////				break;
////			case 2:
////				markerAddr = addr | spareAddr | (uint64_t)((flashInfo.pagesPerBlock - 1) << 16);
////				break;
////			default:
////				break;
////		}
////		/* Disable interrupts */
////		INT_Disable();
////		NAND_CMD = NANDFLASH_PAGE_READ_CMD_A;
////		NAND_ADDR = (uint8_t)(markerAddr >> 0);
////		NAND_ADDR = (uint8_t)(markerAddr >> 8);
////		NAND_ADDR = (uint8_t)(markerAddr >> 16);
////		NAND_ADDR = (uint8_t)(markerAddr >> 24);
////		NAND_ADDR = (uint8_t)(markerAddr >> 32);
////		NAND_CMD = NANDFLASH_PAGE_READ_CMD_B;
////		/* Enable interrupts */
////		INT_Enable();
////		NANDFLASH_WaitReady();
////		marker += NAND_DATA8;
////		S34ML04G2_DEBUG("NANDFLASH_CheckBlock: status: %#x\n", (int)NANDFLASH_ReadStatus());
////	}
////    S34ML04G2_DEBUG("NANDFLASH_CheckBlock: marker: %#x\n", (int)marker);
////	/* Check marker */
////	if (marker == (3 * NAND_BAD_BLOCK_MARKER)) {
////		S34ML04G2_DEBUG("NANDFLASH_CHECK_BAD_BLOCK: no bad block detected!\n");
////		return NANDFLASH_OK;
////	} else {
////		S34ML04G2_DEBUG("NANDFLASH_CHECK_BAD_BLOCK: this is a block!\n");
////		return NANDFLASH_BAD_BLOCK_ERROR;
////	}
////}
//
///*************************************************************************************************/
////NANDFLASH_Status_TypeDef NANDFLASH_ReadPage(uint8_t page, uint16_t block, uint8_t plane, uint8_t *buffer) {
//NANDFLASH_Status_TypeDef NANDFLASH_ReadPage (uint32_t page, uint8_t *buffer) {
//#ifdef S34ML04G2_TIMEOUT
//	/* Set timeout */
//	uint32_t timeout = S34ML04G2_Timeout;
//#endif
//	/* Get correct page address */
////	uint64_t addr = NANDFLASH_GetAddr(0, page, block, plane);
////	uint64_t addr = ((uint64_t)(block & 0x0FFF) << 16);
//	uint64_t addr = NANDFLASH_GetPageAddr(page);
//	/* Try multiple times to read out NAND flash */
//	uint8_t t;
//	for (t = 0; t < S34ML04G2_READ_PAGE_ATTEMPTS; t++) {
//		/* Disable interrupts */
//		INT_Disable();
//		/* Write commands and address */
//		NAND_CMD = NANDFLASH_PAGE_READ_CMD_A;
//		NAND_ADDR = (uint8_t)(addr >> 0);
//		NAND_ADDR = (uint8_t)(addr >> 8);
//		NAND_ADDR = (uint8_t)(addr >> 16);
//		NAND_ADDR = (uint8_t)(addr >> 24);
//		NAND_ADDR = (uint8_t)(addr >> 32);
//		NAND_CMD = NANDFLASH_PAGE_READ_CMD_B;
//		NANDFLASH_WaitReady();
//		/* Start ECC generation */
//#ifdef S34ML04G2_TIMEOUT
//		while ((EBI->STATUS & EBI_STATUS_AHBACT) && (timeout--));
//#else
//		while (EBI->STATUS & EBI_STATUS_AHBACT);
//#endif
//		EBI_StartNandEccGen();
//		/* Read data */
//		uint32_t i;
//		uint32_t *p;
//		p = (uint32_t *)buffer;
//		/* Read data */
//		for (i = 0; i < 512; i++) {
//			*p++ = NAND_DATA32;
//		}
//		/* Stop ECC generation */
//#ifdef S34ML04G2_TIMEOUT
//		while ((EBI->STATUS & EBI_STATUS_AHBACT) && (timeout--));
//#else
//		while (EBI->STATUS & EBI_STATUS_AHBACT);
//#endif
//		flashInfo.ecc = EBI_StopNandEccGen();
//		S34ML04G2_DEBUG("NANDFLASH_ReadPage: generated ECC: %#x, %#x, %#x\n",
//				(int)(uint8_t)(flashInfo.ecc >> 0),
//				(int)(uint8_t)(flashInfo.ecc >> 8),
//				(int)(uint8_t)(flashInfo.ecc >> 16));
//		/* Read spare */
//		p = (uint32_t *)flashInfo.spare;
//		for (i = 0; i < 32; i++) {
//			*p++ = NAND_DATA32;
//		}
//		/* Enable interrupts */
//		INT_Enable();
//		NANDFLASH_WaitReady();
//		/* Check status */
//		S34ML04G2_DEBUG("NANDFLASH_ReadPage: status: %#x\n", (int)NANDFLASH_ReadStatus());
//		uint32_t rEcc = flashInfo.spare[flashInfo.ecc0Pos] |
//				(flashInfo.spare[flashInfo.ecc1Pos] << 8) |
//				(flashInfo.spare[flashInfo.ecc2Pos] << 16);
//		S34ML04G2_DEBUG("NANDFLASH_ReadPage: bad block marker: %#x\n",
//				(int)flashInfo.spare[flashInfo.badBlockPos]);
//		S34ML04G2_DEBUG("NANDFLASH_ReadPage: read ECC: %#x, %#x, %#x\n",
//				(int)(uint8_t)(rEcc >> 0),
//				(int)(uint8_t)(rEcc >> 8),
//				(int)(uint8_t)(rEcc >> 16));
//		/* Check for bad block */
//		if (flashInfo.spare[flashInfo.badBlockPos] == NAND_BAD_BLOCK_MARKER) {
//			/* Correct data */
//			return NANDFLASH_CorrectECC(flashInfo.ecc, rEcc, buffer);
//		}
//	}
//	/* Read out failed */
//	return NANDFLASH_READ_ERROR;
//}
//
///*************************************************************************************************/
////NANDFLASH_Status_TypeDef NANDFLASH_WritePage(uint8_t page, uint16_t block, uint8_t plane, uint8_t *buffer) {
//NANDFLASH_Status_TypeDef NANDFLASH_WritePage(uint32_t page, uint8_t *buffer) {
//#ifdef S34ML04G2_TIMEOUT
//	/* Set timeout */
//	uint32_t timeout = S34ML04G2_Timeout;
//#endif
//	/* Get correct page address */
////	uint64_t addr = NANDFLASH_GetAddr(0, page, block, plane);
////	uint64_t addr = ((uint64_t)(block & 0x0FFF) << 16);
//	uint64_t addr = NANDFLASH_GetPageAddr(page);
//	/* Disable interrupts */
//	INT_Disable();
//	/* Write commands and address */
//	NAND_CMD = NANDFLASH_PAGE_PROG_CMD_A;
//	NAND_ADDR = (uint8_t)(addr >> 0);
//	NAND_ADDR = (uint8_t)(addr >> 8);
//	NAND_ADDR = (uint8_t)(addr >> 16);
//	NAND_ADDR = (uint8_t)(addr >> 24);
//	NAND_ADDR = (uint8_t)(addr >> 32);
//#ifdef S34ML04G2_TIMEOUT
//	/* Wait for EBI module and start ECC generation */
//	while ((EBI->STATUS & EBI_STATUS_AHBACT) && (timeout--));
//#else
//	while (EBI->STATUS & EBI_STATUS_AHBACT);
//#endif
//	EBI_StartNandEccGen();
//	uint32_t i;
//	uint32_t *p;
//    p = (uint32_t *)buffer;
//    for (i = 0; i < 512; i++) {
//      NAND_DATA32 = *p++;
//    }
//    /* Wait for EBI module and stop ECC generation */
//#ifdef S34ML04G2_TIMEOUT
//    while ((EBI->STATUS & EBI_STATUS_AHBACT) && (timeout--));
//#else
//    while (EBI->STATUS & EBI_STATUS_AHBACT);
//#endif
//    flashInfo.ecc = EBI_StopNandEccGen();
//    S34ML04G2_DEBUG("NANDFLASH_WritePage: ECC: %#x, %#x, %#x\n",
//    		(uint8_t)(flashInfo.ecc >> 0),
//			(uint8_t)(flashInfo.ecc >> 8),
//			(uint8_t)(flashInfo.ecc >> 16));
//	/* Write bad block marker as 0xFF (good block) */
//	NAND_DATA8 = NAND_BAD_BLOCK_MARKER;
//	/* Write ECC to spare area */
//	NAND_DATA8 = (uint8_t)(flashInfo.ecc >> 0);
//	NAND_DATA8 = (uint8_t)(flashInfo.ecc >> 8);
//	NAND_DATA8 = (uint8_t)(flashInfo.ecc >> 16);
//	NAND_CMD = NANDFLASH_PAGE_PROG_CMD_B;
//	/* Enable interrupts */
//	INT_Enable();
//	NANDFLASH_WaitReady();
//    /* Check write page status */
//	uint8_t status = NANDFLASH_ReadStatus();
//	S34ML04G2_DEBUG("NANDFLASH_WritePage: status: %#x\n", (int)status);
//	if (status & NAND_STATUS_FAIL) {
//		return NANDFLASH_WRITE_ERROR;
//	} else {
//		return NANDFLASH_OK;
//	}
//}
//
/////*************************************************************************************************/
////NANDFLASH_Status_TypeDef NANDFLASH_RewritePage(uint8_t page, uint16_t block, uint8_t plane) {
////	/* Get correct block address */
////	uint64_t addr = NANDFLASH_GetAddr(0, page, block, plane);
////	/* Disable interrupts */
////	INT_Disable();
////	NAND_CMD = NANDFLASH_MP_PAGE_REPROG_CMD_A;
////	NAND_ADDR = (uint8_t)(addr >> 0);
////	NAND_ADDR = (uint8_t)(addr >> 8);
////	NAND_ADDR = (uint8_t)(addr >> 16);
////	NAND_ADDR = (uint8_t)(addr >> 24);
////	NAND_ADDR = (uint8_t)(addr >> 32);
////	NAND_CMD = NANDFLASH_MP_PAGE_REPROG_CMD_D;
////	/* Enable interrupts */
////	INT_Enable();
////	NANDFLASH_WaitReady();
////    /* Check rewrite page success status */
////	uint8_t status = NANDFLASH_ReadStatus();
////	S34ML04G2_DEBUG("NANDFLASH_RewritePage: status: %#x\n", (int)status);
////	if (status & NAND_STATUS_FAIL) {
////		return NANDFLASH_REWRITE_ERROR;
////	} else {
////		return NANDFLASH_OK;
////	}
////}
//
///*************************************************************************************************/
////NANDFLASH_Status_TypeDef NANDFLASH_EraseBlock(uint16_t block, uint8_t plane) {
//NANDFLASH_Status_TypeDef NANDFLASH_EraseBlock(uint32_t block) {
//	/* Get correct block address */
////	uint64_t addr = NANDFLASH_GetAddr(0, 0, block, plane);
////	uint64_t addr = ((uint64_t)(block & 0x0FFF) << 16);
//	uint64_t addr = NANDFLASH_GetBlockAddr(block);
//	/* Disable interrupts */
//	INT_Disable();
//	/* Write commands and address */
//	NAND_CMD = NANDFLASH_BLOCK_ERASE_CMD_A;
//	NAND_ADDR = (uint8_t)(addr >> 16);
//	NAND_ADDR = (uint8_t)(addr >> 24);
//	NAND_ADDR = (uint8_t)(addr >> 32);
//	NAND_CMD = NANDFLASH_BLOCK_ERASE_CMD_B;
//	/* Enable interrupts */
//	INT_Enable();
//	NANDFLASH_WaitReady();
//	/* Check status */
//	uint8_t status = NANDFLASH_ReadStatus();
//	S34ML04G2_DEBUG("NANDFLASH_EraseBlock: status: %#x\n", (int)status);
//	if (status & NAND_STATUS_FAIL) {
//		return NANDFLASH_ERASE_ERROR;
//	} else {
//		return NANDFLASH_OK;
//	}
//}
//
/////*************************************************************************************************/
////NANDFLASH_Status_TypeDef NANDFLASH_EraseBlocksOfPages(uint32_t startPage, uint32_t endPage) {
////	NANDFLASH_Status_TypeDef status;
////	uint32_t i;
////	uint32_t startBlock;
////	uint32_t endBlock;
////	/* Check if there is an "ring buffer" overflow */
////	if (endPage < startPage) {
////		/* Erase from start block to last block */
////		startBlock = (uint32_t)(startPage / flashInfo.pagesPerBlock);
////		endBlock = (flashInfo.totalBlocks - 1);
////		S34ML04G2_DEBUG("NANDFLASH_EraseBlocksOfPages: blocks: %i --> %i\n", (int)startBlock, (int)endBlock);
////		for (i = startBlock; i < (endBlock + 1); i++) {
////			status = NANDFLASH_EraseBlock(i);
////			if (status != NANDFLASH_OK) {
////				return status;
////			} else {
////				continue;
////			}
////		}
////		/* Erase from first block to end block */
////		startBlock = 0;
////		endBlock  = (uint32_t)(endPage / flashInfo.pagesPerBlock);
////		for (i = startBlock; i < (endBlock + 1); i++) {
////			status = NANDFLASH_EraseBlock(i);
////			if (status != NANDFLASH_OK) {
////				return status;
////			} else {
////				continue;
////			}
////		}
////	} else {
////		startBlock = (uint32_t)(startPage / flashInfo.pagesPerBlock);
////		endBlock  = (uint32_t)(endPage / flashInfo.pagesPerBlock);
////		S34ML04G2_DEBUG("NANDFLASH_EraseBlocksOfPages: blocks: %i --> %i\n", (int)startBlock, (int)endBlock);
////		for (i = startBlock; i < (endBlock + 1); i++) {
////			status = NANDFLASH_EraseBlock(i);
////			if (status != NANDFLASH_OK) {
////				return status;
////			} else {
////				continue;
////			}
////		}
////	}
////	/* Success */
////	return NANDFLASH_OK;
////}
//
/////*************************************************************************************************/
////NANDFLASH_Status_TypeDef NANDFLASH_EraseChip(void) {
////	uint32_t i;
////	uint8_t status;
////	for (i = 0; i < flashInfo.totalBlocks; i++) {
////		uint64_t addr = NANDFLASH_GetBlockAddr(i);
////		/* Disable interrupts */
////		INT_Disable();
////		NAND_CMD = NANDFLASH_BLOCK_ERASE_CMD_A;
////		NAND_ADDR = (uint8_t)(addr >> 16);
////		NAND_ADDR = (uint8_t)(addr >> 24);
////		NAND_ADDR = (uint8_t)(addr >> 32);
////		NAND_CMD = NANDFLASH_BLOCK_ERASE_CMD_B;
////		/* Enable interrupts */
////		INT_Enable();
////		NANDFLASH_WaitReady();
////		/* Check status */
////		status = NANDFLASH_ReadStatus();
////		S34ML04G2_DEBUG("NANDFLASH_EraseChip: block: %i status: %#x\n", (int)i, (int)status);
////		if (status & NAND_STATUS_FAIL) {
////			return NANDFLASH_ERASE_ERROR;
////		} else {
////			continue;
////		}
////	}
////	/* Success */
////	return NANDFLASH_OK;
////}
//
///*************************************************************************************************/
//static NANDFLASH_Status_TypeDef NANDFLASH_CorrectECC(uint32_t gEcc, uint32_t rEcc, uint8_t *data) {
//#ifdef S34ML04G2_TIMEOUT
//	/* Set timeout */
//	uint32_t timeout = S34ML04G2_Timeout;
//#endif
//	/* Create variables */
//	int count;
//	int bitNum;
//	int byteAddr;
//	uint32_t mask;
//	uint32_t syndrome;
//	/* 12 even ECC parity bits */
//	uint32_t eccP;
//	/* 12 odd ECC parity bits */
//	uint32_t eccPn;
//	/* Calculate syndrome */
//	syndrome = ((gEcc ^ rEcc) & 0x00FFFFFF);
//	/* No errors in data */
//	if (syndrome == 0) {
//		S34ML04G2_DEBUG("NANDFLASH_CorrectECC: no error in data!\n");
//		return NANDFLASH_OK;
//	}
//	/* Get twelve odd parity bits */
//	eccPn = (syndrome & 0x00555555);
//	/* Get twelve even parity bits */
//	eccP = ((syndrome >> 1) & 0x00555555);
//	/* 1-bit correctable error? */
//  	if ((eccPn ^ eccP) == 0x00555555) {
//  		bitNum = (eccP & 0x01) | ((eccP >> 1) & 0x02) | ((eccP >> 2) & 0x04);
//  		byteAddr = ((eccP >> 6) & 0x001) | ((eccP >> 7) & 0x002) | ((eccP >> 8) & 0x004) |
//  				((eccP >> 9) & 0x008) | ((eccP >> 10) & 0x010) | ((eccP >> 11) & 0x020) |
//  				((eccP >> 12) & 0x040) | ((eccP >> 13) & 0x080) | ((eccP >> 14) & 0x100);
//  		data[byteAddr] ^= (1 << bitNum);
//  		S34ML04G2_DEBUG("NANDFLASH_CorrectECC: 1 bit on position %i corrected!\n", bitNum);
//  		return NANDFLASH_OK;
//  	}
//  	/* Count number of 1's in the syndrome */
//  	count = 0;
//  	mask = 0x00800000;
//#ifdef S34ML04G2_TIMEOUT
//  	while ((mask) && (timeout--)) {
//#else
//  	while (mask) {
//#endif
//  		if (syndrome & mask) {
//  			count++;
//  		}
//  		mask >>= 1;
//  	}
//  	/* Error in the ECC itself */
//  	if (count == 1) {
//  		S34ML04G2_DEBUG("NANDFLASH_CorrectECC: error in the ECC itself!\n");
//  		return NANDFLASH_ECC_ERROR;
//  	}
//  	/* Unable to correct data */
//  	S34ML04G2_DEBUG("NANDFLASH_CorrectECC: too many uncorrectable errors!\n");
//  	return NANDFLASH_ECC_UNCORRECTABLE;
//}
//
///*************************************************************************************************/
//static uint64_t NANDFLASH_GetPageAddr(uint32_t page) {
//	uint64_t baseAddr = 0;
//	/* Check plane */
//	if (page >= flashInfo.pagesPerPlane) {
//		page -= flashInfo.pagesPerPlane;
//		baseAddr |= (1 << 22);
//	}
//	/* Check block */
//	uint32_t block = (uint32_t)(page / flashInfo.pagesPerBlock);
//	/* Check page */
//	page = (uint32_t)(page - (block * flashInfo.pagesPerBlock));
//	/* Set base address */
//	baseAddr |= (((uint64_t)block & 0x000007FF) << 23) | (((uint64_t)page & 0x0000003F) << 16);
//	S34ML04G2_DEBUG("NANDFLASH_GetPageAddr: 0, 0, %#x, %#x, %#x\n",
//			(int)(uint8_t)(baseAddr >> 16),
//			(int)(uint8_t)(baseAddr >> 24),
//			(int)(uint8_t)(baseAddr >> 32));
//	return baseAddr;
//}
//
///*************************************************************************************************/
//static uint64_t NANDFLASH_GetBlockAddr(uint32_t block) {
//	uint64_t baseAddr = 0;
//	/* Check plane */
//	if (block >= flashInfo.blocksPerPlane) {
//		block -= flashInfo.blocksPerPlane;
//		baseAddr |= (1 << 22);
//	}
//	/* Set base address */
//	baseAddr |= (((uint64_t)block & 0x000007FF) << 23);
//	S34ML04G2_DEBUG("NANDFLASH_GetBlockAddr: 0, 0, %#x, %#x, %#x\n",
//			(int)(uint8_t)(baseAddr >> 16),
//			(int)(uint8_t)(baseAddr >> 24),
//			(int)(uint8_t)(baseAddr >> 32));
//	return baseAddr;
//}
//
/////*************************************************************************************************/
////uint16_t NANDFLASH_GetBlockOfPage(uint32_t page) {
////	uint16_t block = 0;
////	/* Check plane */
////	if (page >= flashInfo.pagesPerPlane) {
////		page -= flashInfo.pagesPerPlane;
////	}
////}
//
/////*************************************************************************************************/
////static uint64_t NANDFLASH_GetAddr(uint16_t col, uint8_t page, uint16_t block, uint8_t plane) {
////	uint64_t baseAddr = 0;
////	/* Set column bits */
////	baseAddr |= ((col & 0x0FFF) << 0);
////	/* Set page bits */
////	baseAddr |= ((page & 0x3F) << 16);
////	/* Set plane bit */
////	baseAddr |= ((plane & 0x01) << 22);
////	/* Set block bits */
////	baseAddr |= ((block & 0x07FF) << 23);
////	/* Return combined address */
////	return baseAddr;
////}
