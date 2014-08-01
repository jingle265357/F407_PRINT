/**
 * @file epp_errors.h
 *
 * This file defines the error codes used by embedded TCI 
 *
 * Copyright Authentec Inc
 *
 */

#ifndef _EPP_ERRORS_H_
#define _EPP_ERRORS_H_

/**
 * @name Error Code for embedded TCI
 */
//@{
#define STERR_OK                     0  ///< no error
#define STERR_ERROR                  1  ///< spi error
#define STERR_NODEVICE              -3  ///< no device
#define STERR_BADPARAMETER          -5  ///< bad input parameters
#define STERR_TCPWR                 -6  ///< sensor power problem
#define STERR_BUFFER_TOO_SMALL      -20 ///< buffer is too small

#define STERR_OFFSET_TOO_LOW        -50 ///< offset too low
#define STERR_CHARGE_TOO_LOW        -51 ///< charge too low
#define STERR_TOO_MANY_BAD_LINES    -52 ///< too many bad lines
#define STERR_CHIP_IS_UNREPAIRABLE	-54 ///< sensor is unrepairable
#define STERR_GAIN_OFFSET           -57 ///< gain offset calibraion fails
#define STERR_MEMORY_MALLOC_FAIL    -73 ///< Memory allocation failure

#define STERR_NO_FINGER_DETECTED    -30
//@}

#endif /*_EPP_ERRORS_H_*/
