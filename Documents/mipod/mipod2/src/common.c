/************************************************************************************************************************************************//**
 * @file common.c
 * @brief Hardware configuration and commonly used functions.
 * @version 1.0
 * @author Peter Blank
 ****************************************************************************************************************************************************
 * @section License
 * <b>TODO license need to be added here...</b>
 ***************************************************************************************************************************************************/

#include "common.h"

/****************************************************************************************************************************************************
 ***   TYPEDEF'S, DEFINES AND VARIABLES   ***********************************************************************************************************
 ***************************************************************************************************************************************************/

/****************************************************************************************************************************************************
 ***   INTERRUPTS   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

/****************************************************************************************************************************************************
 ***   PROTOTYPES   *********************************************************************************************************************************
 ***************************************************************************************************************************************************/

/****************************************************************************************************************************************************
 ***   FUNCTIONS   **********************************************************************************************************************************
 ***************************************************************************************************************************************************/

/**
 * @brief Conversion of a two's complement byte.
 *
 * This function converts a 8 bit two's complement value into a signed integer value.
 *
 * @param uint8_t x	two's complement number
 * @return int		binary integer
 */
int COMMON_convert8bitComplementaryIntoSignedInt(uint8_t x)
{
	if((x >> 7) == 1)
	{
		x = ~x + 1;
		return (int)x * -1;
	}
	else
	{
		return (int)x;
	}
}

/**
 * @brief Conversion of a two's complement word.
 *
 * This function converts a 16 bit two's complement value into a signed integer value.
 *
 * @param uint16_t x	two's complement number
 * @return int			binary integer
 */
int COMMON_convert16bitComplementaryIntoSignedInt(uint16_t x)
{
	if((x >> 15) == 1)
	{
		x = ~x + 1;
		return (int)x * -1;
	}
	else
	{
		return (int)x;
	}
}

/**
 * @brief Conversion of a two's complement double word.
 *
 * This function converts a 32 bit two's complement value into a signed integer value.
 *
 * @param uint32_t x	two's complement number
 * @return int			binary integer
 */
int COMMON_convert32bitComplementaryIntoSignedInt(uint32_t x)
{
	if((x >> 31) == 1)
	{
		x = ~x + 1;
		return (int)x * -1;
	}
	else
	{
		return (int)x;
	}
}

/**
 * @brief Division to nearest integer.
 *
 * This function divides an integer value to the nearest integer value (round to nearest integer).
 *
 * @param int n		number to be divided
 * @param int d		divisor
 * @return int		nearest integer
 */
int COMMON_devisionToNearestInteger(int n, int d)
{
	return ((n < 0) ^ (d < 0)) ? ((n - (d / 2)) / d) : ((n + (d / 2)) / d);
}
