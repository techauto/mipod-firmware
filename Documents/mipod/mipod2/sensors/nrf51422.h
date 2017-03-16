/***************************************************************************************************************************************************/
#ifndef NRF51422_H_
#define NRF51422_H_

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
#define NRF51422_DEBUGGING

/* Register addresses */
#define NRF51422_DEVID_REG					0x40003000
#define NRF51422_FRAME						6

#endif
