/**
 * @file erl.h
 *
 * This module defines interfaces for ERL module.
 *
 * Copyright Authentec 2011
 *
 * @author Zhaoliang Wang
 */

#ifndef _ERL_API_H_
#define _ERL_API_H_

/**---------------------------------------------------------------------------
* NAME: ERL
*
* Description:	
*   The function does offline bad line calibration 
*   and stores the calibration data into NVM
*
* @return
*         STERR_OK
*         STERR_ERROR
*         STERR_NODEVICE
*         STERR_BADPARAMETER
*----------------------------------------------------------------------------*/

sint32 ERL(void);

#endif /* _ERL_API_H_*/
