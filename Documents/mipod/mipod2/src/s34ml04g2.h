///*************************************************************************************************/
//#ifndef S34ML04G2_H_
//#define S34ML04G2_H_
//
///**************************************************************************************************
// ***   INCLUDES   *********************************************************************************
// *************************************************************************************************/
//
//#include <stdbool.h>
//#include <stdint.h>
//
//#define S34ML04G2_TIMEOUT
//
///**************************************************************************************************
// ***   GLOBAL VARIABLES / DEFINES  ****************************************************************
// *************************************************************************************************/
//
///* NAND flash ID macros */
//#define S34ML04G2_MANUFACTURER_CODE_POS		0
//#define S34ML04G2_DEVICE_IDENTIFIER_POS		1
//#define S34ML04G2_INTERNAL_CHIP_NUMBER_POS	2
//#define S34ML04G2_ORGANIZATION_POS			3
//#define S34ML04G2_PLANE_INFORMATION_POS		4
//#define S34ML04G2_ID_LENGTH					5
//#define S34ML04G2_ONFI_LENGTH				4
//
///* NAND flash sizes in bytes */
//#define S34ML04G2_PLANES					2
//#define S34ML04G2_BLOCKS_PER_PLANE			2048
//#define S34ML04G2_PAGES_PER_BLOCK			64
//#define S34ML04G2_PAGES_PER_PLANE			131072
//#define S34ML04G2_TOTAL_BLOCKS				4096		/* (2 * 2048) 								*/
//#define S34ML04G2_TOTAL_PAGES				262144		/* (2 * 2048 * 64) 							*/
//#define S34ML04G2_SPARESIZE					128
//#define S34ML04G2_PAGESIZE     				2048
//#define S34ML04G2_BLOCKSIZE    				131072 		/* (64 * 2048) 								*/
//#define S34ML04G2_PLANESIZE   				268435456 	/* (2048 * 64 * 2048) 						*/
//#define S34ML04G2_SIZE						536870912 	/* (2 * 2048 * 64* 2048)					*/
//
///* Generic NAND flash definitions */
//#define NAND_PAGEADDR_MASK      			(S34ML04G2_PAGESIZE - 1)
//#define NAND_BLOCKADDR_MASK     			(S34ML04G2_BLOCKSIZE - 1)
//
///* Byte positions in the spare area */
//#define S34ML04G2_SPARE_BADBLOCK_POS    	0			/* Spare area position of bad-block marker	*/
//#define S34ML04G2_SPARE_ECC0_POS        	1 			/* Spare area position of ECC byte 0 (LSB)	*/
//#define S34ML04G2_SPARE_ECC1_POS        	2 			/* Spare area position of ECC byte 1       	*/
//#define S34ML04G2_SPARE_ECC2_POS        	3 			/* Spare area position of ECC byte 2 (MSB) 	*/
//
///* NAND flash commands */
//#define NANDFLASH_PAGE_READ_CMD_A			0x00
//#define NANDFLASH_PAGE_READ_CMD_B			0x30
//#define NANDFLASH_PAGE_PROG_CMD_A			0x80
//#define NANDFLASH_PAGE_PROG_CMD_B			0x10
//#define NANDFLASH_PAGE_REPROG_CMD_A			0x8B
//#define NANDFLASH_PAGE_REPROG_CMD_B			0x10
//#define NANDFLASH_MP_PAGE_PROG_CMD_A		0x80
//#define NANDFLASH_MP_PAGE_PROG_CMD_B		0x11
//#define NANDFLASH_MP_PAGE_PROG_CMD_C		0x81
//#define NANDFLASH_MP_PAGE_PROG_CMD_D		0x10
//#define NANDFLASH_MP_PAGE_REPROG_CMD_A		0x8B
//#define NANDFLASH_MP_PAGE_REPROG_CMD_B		0x11
//#define NANDFLASH_MP_PAGE_REPROG_CMD_C		0x8B
//#define NANDFLASH_MP_PAGE_REPROG_CMD_D		0x10
//#define NANDFLASH_BLOCK_ERASE_CMD_A			0x60
//#define NANDFLASH_BLOCK_ERASE_CMD_B			0xD0
//#define NANDFLASH_MP_BLOCK_ERASE_CMD_A		0x60
//#define NANDFLASH_MP_BLOCK_ERASE_CMD_B		0x60
//#define NANDFLASH_MP_BLOCK_ERASE_CMD_C		0xD0
//#define NANDFLASH_COPYBACK_READ_CMD_A		0x00
//#define NANDFLASH_COPYBACK_READ_CMD_B		0x35
//#define NANDFLASH_COPYBACK_PROG_CMD_A		0x85
//#define NANDFLASH_COPYBACK_PROG_CMD_B		0x10
//#define NANDFLASH_MP_COPYBACK_PROG_CMD_A	0x85
//#define NANDFLASH_MP_COPYBACK_PROG_CMD_B	0x11
//#define NANDFLASH_MP_COPYBACK_PROG_CMD_C	0x81
//#define NANDFLASH_MP_COPYBACK_PROG_CMD_D	0x10
//#define NANDFLASH_COPYBACK_SPEC_READ_CMD_A	0x00
//#define NANDFLASH_COPYBACK_SPEC_READ_CMD_B	0x36
//#define NANDFLASH_READ_EDC_STATUS_REG_CMD	0x7B
//#define NANDFLASH_READ_STATUS_REG_CMD		0x70
//#define NANDFLASH_READ_STATUS_ENH_REG_CMD	0x78
//#define NANDFLASH_RESET_CMD					0xFF
//#define NANDFLASH_READ_CACHE_CMD			0x31
//#define NANDFLASH_READ_CACHE_ENH_CMD_A		0x00
//#define NANDFLASH_READ_CACHE_ENH_CMD_B		0x31
//#define NANDFLASH_READ_CACHE_END_CMD		0x3F
//#define NANDFLASH_CACHE_PROG_S_C_CMD_A		0x80
//#define NANDFLASH_CACHE_PROG_S_C_CMD_B		0x15
//#define NANDFLASH_CACHE_PROG_E_CMD_A		0x80
//#define NANDFLASH_CACHE_PROG_E_CMD_B		0x10
//#define NANDFLASH_MP_CACHE_PROG_S_C_CMD_A	0x80
//#define NANDFLASH_MP_CACHE_PROG_S_C_CMD_B	0x11
//#define NANDFLASH_MP_CACHE_PROG_S_C_CMD_C	0x81
//#define NANDFLASH_MP_CACHE_PROG_S_C_CMD_D	0x15
//#define NANDFLASH_MP_CACHE_PROG_E_CMD_A		0x80
//#define NANDFLASH_MP_CACHE_PROG_E_CMD_B		0x11
//#define NANDFLASH_MP_CACHE_PROG_E_CMD_C		0x81
//#define NANDFLASH_MP_CACHE_PROG_E_CMD_D		0x11
//#define NANDFLASH_READ_ID_CMD				0x90
//#define NANDFLASH_READ_ONFI_CMD				0x90
//#define NANDFLASH_READ_PARAM_PAGE_CMD		0xEC
//
///* NAND flash manufacturer IDs */
//#define NANDFLASH_MANUFACTURER_ID			0x01
//#define NANDFLASH_DEVICE_ID					0xDC
//#define NANDFLASH_INTERNAL_CHIP_ID			0x90
//#define NANDFLASH_ORGANIZATION_ID			0x95
//#define NANDFLASH_PLANE_INFO_ID				0x56
//
///* NAND flash status */
//#define NAND_STATUS_UNPROTECTED       		0x80
//#define NAND_STATUS_READY	        		0x40
//#define NAND_STATUS_IDLE	        		0x20
//#define NAND_STATUS_FAIL        			0x01
//
///* Bad block marker (0xFF = good, else bad) */
//#define NAND_BAD_BLOCK_MARKER				0xFF
//
///* Maximum page read attempts */
//#define S34ML04G2_READ_PAGE_ATTEMPTS		2
//
///* NAND flash access macros */
//#define NAND_DATA8              			*pNandData8
//#define NAND_DATA16             			*pNandData16
//#define NAND_DATA32             			*pNandData32
//#define NAND_ADDR               			*pNandAddr
//#define NAND_CMD                			*pNandCmd
//
///* NAND flash control bits */
//#define NAND_ALE_BIT            			24
//#define NAND_CLE_BIT            			25
//
///* NANDFLASH device information structure */
//typedef struct {
//	uint32_t baseAddress;					/* The device base address in CPU memory map					*/
//	uint32_t deviceSize;                	/* Total device size in bytes                  					*/
//	uint32_t pageSize;                  	/* Device page size in bytes                   					*/
//	uint32_t spareSize;                 	/* Device page spare size in bytes             					*/
//	uint32_t blockSize;                 	/* Device block size in bytes									*/
//	uint32_t planeSize;                 	/* Device block size in bytes									*/
//	uint32_t ecc;                       	/* Result of ECC generation from last read/written page 		*/
//	uint8_t  badBlockPos;					/* Position of bad block marker 								*/
//	uint8_t  ecc0Pos;						/* Position of first ECC byte 									*/
//	uint8_t  ecc1Pos;						/* Position of second ECC byte 									*/
//	uint8_t  ecc2Pos;						/* Position of third ECC byte 									*/
//	uint32_t pagesPerBlock;					/* Number of pages per one block 								*/
//	uint32_t totalBlocks;					/* Total number of blocks 										*/
//	uint32_t pagesPerPlane;					/* Number of pages per plane 									*/
//	uint32_t blocksPerPlane;				/* Number of blocks per plane 									*/
//	uint8_t  spare[S34ML04G2_SPARESIZE];	/* Spare area content from last read page or spare operation	*/
//} NANDFLASH_Info_TypeDef;
//
///* NANDFLASH status enumerator */
//typedef enum {
//	NANDFLASH_OK         		=  0,		/* No errors detected                      	*/
//	NANDFLASH_WRITE_ERROR       = -1,   	/* NAND flash write error, block is "bad"	*/
//	NANDFLASH_REWRITE_ERROR    	= -2,   	/* NAND flash rewrite error, block is "bad"	*/
//	NANDFLASH_READ_ERROR    	= -3,   	/* NAND flash read error, block is "bad"	*/
//	NANDFLASH_ECC_ERROR         = -4,   	/* Illegal ECC value read from spare area  	*/
//	NANDFLASH_ECC_UNCORRECTABLE	= -5,   	/* Uncorrectable error in page        		*/
//	NANDFLASH_BAD_BLOCK_ERROR	= -6,   	/* NAND flash has detected a bad block 		*/
//	NANDFLASH_ERASE_ERROR		= -7,		/* NAND flash unable to erase block			*/
//	NANDFLASH_INIT_ERROR		= -8,		/* NAND flash initializing error			*/
//	NANDFLASH_ID_ERROR			= -9,		/* NAND flash ID error						*/
//} NANDFLASH_Status_TypeDef;
//
//#ifdef S34ML04G2_TIMEOUT
///* S34ML04G2 timeout macro in [ms] */
//#define S34ML04G2_TIMEOUT_MSEC				50
//#endif
//
///* Macro for debug output */
//#ifdef S34ML04G2_DEBUGGING
//#include <stdio.h>
//#define S34ML04G2_DEBUG(info, ...)		\
//	do {								\
//		printf(info, ## __VA_ARGS__);	\
//	} while(0)
//#else
//#define S34ML04G2_DEBUG(info, ...)
//#endif
//
///**************************************************************************************************
// ***   PROTOTYPES   *******************************************************************************
// *************************************************************************************************/
//
//NANDFLASH_Status_TypeDef NANDFLASH_Init(void);
//bool NANDFLASH_IsOff(void);
//
//void NANDFLASH_Power(bool enable);
//void NANDFLASH_ChipEnable(bool enable);
//void NANDFLASH_WriteProtect(bool enable);
//
////NANDFLASH_Status_TypeDef NANDFLASH_ReadPage(uint8_t page, uint16_t block, uint8_t plane, uint8_t *buffer);
////NANDFLASH_Status_TypeDef NANDFLASH_WritePage(uint8_t page, uint16_t block, uint8_t plane, uint8_t *buffer);
//
//NANDFLASH_Status_TypeDef NANDFLASH_ReadPage(uint32_t page, uint8_t *buffer);
//NANDFLASH_Status_TypeDef NANDFLASH_WritePage(uint32_t page, uint8_t *buffer);
//
////NANDFLASH_Status_TypeDef NANDFLASH_RewritePage(uint8_t page, uint16_t block, uint8_t plane);
////NANDFLASH_Status_TypeDef NANDFLASH_EraseBlock(uint16_t block, uint8_t plane);
//
//NANDFLASH_Status_TypeDef NANDFLASH_EraseBlock(uint32_t block);
//
////NANDFLASH_Status_TypeDef NANDFLASH_CheckBlock(uint16_t block, uint8_t plane);
////NANDFLASH_Status_TypeDef NANDFLASH_EraseChip(void);
//
///*************************************************************************************************/
//#endif /* S34ML04G2_H_ */
