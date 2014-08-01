/**
 * @file core.h
 *
 * This module defines interfaces for Embedded Calibration Library module.
 *
 * Copyright Authentec Inc 
 *
 */

#ifndef _ECL_API_H_
#define _ECL_API_H_

#include "types.h"

/**---------------------------------------------------------------------------
* NAME: calibrate
*
* Description:	
*   The function does full calibration 
*   and stores the calibration data into NVM
*
* @return
*         STERR_OK
*         STERR_ERROR
*         STERR_NODEVICE
*         STERR_BADPARAMETER
*         STERR_TCPWR
*         STERR_BUFFER_TOO_SMALL
*         STERR_OFFSET_TOO_LOW
*         STERR_CHARGE_TOO_LOW
*         STERR_TOO_MANY_BAD_LINES
*         STERR_CHIP_IS_UNREPAIRABLE
*         STERR_GAIN_OFFSET
*         STERR_MEMORY_MALLOC_FAIL
*----------------------------------------------------------------------------*/

sint32 calibrate(void);

/**---------------------------------------------------------------------------
* NAME: GetBadLinesReport
*
* Description:
*   The function reads the bad line information detected by the calibration 
*   routine which is stored inside NVM
*
* Note:
*   - Calibration must have been performed before this function
*   - New bad lines are not detected or repaired in this function
*   - ESD lines are not reported as bad lines
*
* @param BadCol     Bad columns buffer.  The size of this buffer has to be at
*                   least 32 bytes.  The output format of this array is:
*                   [0] = # of bad columns detected during calibration
*                   [1] = 1st bad column detected during calibration
*                   [2] = 2nd bad column detected during calibration
*                   ...
*
* @param BadRow     Bad rows buffer.  The size of this buffer has to be at
*                   least 32 bytes.  The output format of this array is:
*                   [0] = # of bad rows detected during calibration
*                   [1] = 1st bad row detected during calibration
*                   [2] = 2nd bad row detected during calibration
*                   ...
*
* @return
*         STERR_OK
*         STERR_ERROR
*         STERR_NODEVICE
*         STERR_BADPARAMETER
*         STERR_TCPWR
*         STERR_BUFFER_TOO_SMALL
*         STERR_MEMORY_MALLOC_FAIL
*----------------------------------------------------------------------------*/

sint32 GetBadLinesReport(sint32 *BadCol, sint32 *BadRow);


#endif /* _ECL_API_H_*/
