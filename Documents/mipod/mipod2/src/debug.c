/************************************************************************************************************************************************//**
 * @file debug.c
 * @brief Debug driver for printf() using the serial LEURT module.
 * @version 1.0
 * @author Peter Blank
 ****************************************************************************************************************************************************
 * @section License
 * <b>TODO license need to be added here...</b>
 ***************************************************************************************************************************************************/

#include "debug.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

#ifdef RETARGET_PRINTF_TO_LEUART
/**
 * @brief LEURT default parameter.
 *
 * This define indicates the LEURT module's default parameter.
 */
#define LEUART_INIT_STRUCTURE \
{ \
	leuartDisable,		/**< Disable module when initialization done. */ \
	0,					/**< Use current configured hardware  clock. */ \
	115200,				/**< Set output rate to 115200 baud. */ \
	leuartDatabits8,	/**< Set data frame to 8 bits. */ \
	leuartNoParity,		/**< Set 0 parity bits. */ \
	leuartStopbits1,	/**< Set 1 stop bits. */ \
}
#endif

#ifdef RETARGET_PRINTF_TO_LEUART
#undef errno
extern int errno;
#endif

/****************************************************************************************************************************************************
 ***   FUNCTIONS   **********************************************************************************************************************************
 ***************************************************************************************************************************************************/

#ifdef PRINT_TO_SWO
void DEBUG_Init(void)
{
    /* Enable GPIO clock. */
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
    /* Enable Serial wire output pin */
    GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;
      /* Set location 0 */
      GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC0;
      /* Enable output on pin - GPIO Port F, Pin 2 */
      GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
      GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
      /* Enable debug clock AUXHFRCO */
    CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;
    /* Wait until clock is ready */
    while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY)) ;
    /* Enable trace in core debug */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    ITM->LAR = 0xC5ACCE55;
    ITM->TER = 0x00;
    ITM->TCR = 0x00;
    TPI->SPPR = 0x02;
    TPI->ACPR = 0x0F;
    ITM->TPR = 0x00;
    DWT->CTRL = 0x400003FE;
    ITM->TCR = 0x0001000D;
    TPI->FFCR = 0x00000100;
    ITM->TER = 0x01;
}

int _write(int file, const char *ptr, int len)
{
    int x;
    for (x = 0; x < len; x++)
    ITM_SendChar (*ptr++);
    return (len);
}
#endif




#ifdef RETARGET_PRINTF_TO_LEUART
/**
 * @brief Debug initialization.
 *
 * This function initializes the debug module.
 */
void DEBUG_Init(void)
{
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_HFLE, true);
	CMU_ClockEnable(cmuClock_LEUART0, true);
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
	LEUART_Init_TypeDef initLEUART = LEUART_INIT_STRUCTURE;
	LEUART_Init(LEUART0, &initLEUART);
	LEUART0->ROUTE = LEUART_ROUTE_TXPEN | DEBUG_LOCATION;
	GPIO_PinModeSet(DEBUG_PORT, DEBUG_PIN, gpioModePushPull, 1);
	LEUART_Enable(LEUART0, leuartEnable);
}

/**
 * @brief Printf() functions.
 *
 * This set of functions redirect the debug output to printf().
 *
 * @{
 */
int _write(int file, char *ptr, int len)
{
	(void) file;
	(void) (*ptr);
	int n;
	for (n = 0; n < len; n++)
	{
		LEUART_Tx(LEUART0, ptr[n]);
	}
	return len;
}

int _read(int file, char *ptr, int len)
{
	(void) (*ptr);
	(void) len;
	switch (file)
	{
		case STDIN_FILENO:
			return 1;
		default:
			errno = EBADF;
			return -1;
	}
}

int _lseek(int file, int ptr, int dir)
{
	(void) file;
	(void) ptr;
	(void) dir;
	return 0;
}

int _isatty(int file)
{
	switch (file)
	{
		case STDOUT_FILENO:
		case STDERR_FILENO:
		case STDIN_FILENO:
			return 1;
		default:
			errno = EBADF;
			return 0;
	}
}

int _fstat(int file, struct stat *st)
{
	(void) file;
	st->st_mode = S_IFCHR;
	return 0;
}

int _close(int file)
{
	(void) file;
	return -1;
}
/**
 * @}
 */

/**
 * @brief Print byte as binary string.
 *
 * This function prints a 8 bit unsigned integer as a string of 0's and 1's.
 *
 * @param uint8_t x		input data
 */
void DEBUG_Print8bitIntAsBinary(uint8_t x)
{
	uint8_t i;
	for(i = 8; i > 0; i--)
	{
		if(((x >> (i - 1)) & 0x01) == 1)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
}

/**
 * @brief Print word as binary string.
 *
 * This function prints a 16 bit unsigned integer as a string of 0's and 1's.
 *
 * @param uint16_t x	input data
 */
void DEBUG_Print16bitIntAsBinary(uint16_t x)
{
	uint8_t i;
	for(i = 16; i > 0; i--)
	{
		if(((x >> (i - 1)) & 0x0001) == 1)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
}

/**
 * @brief Print double word as binary string.
 *
 * This function prints a 32 bit unsigned integer as a string of 0's and 1's.
 *
 * @param uint32_t x	input data
 */
void DEBUG_Print32bitIntAsBinary(uint32_t x)
{
	uint8_t i;
	for(i = 32; i > 0; i--)
	{
		if(((x >> (i - 1)) & 0x00000001) == 1)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
}
#endif
