/**
 * @file spi.h
 *
 * TouchChip Sensor Parallel Interfaces
 *
 * Copyright Authentec Inc
 *
 */

#ifndef __SPI_H
#define __SPI_H

//#include "stm32f4xx.h"
#include "types.h"
#include "SystemClock.h"
#include "HAL_Bus8Bit.h"
#include "HAL_Flash.h"

#include "epp_errors.h"


/**----------------------------------------------------------------
 *
 * TCDVDR_Init
 *
 * Description: Initialize the SPI layer.
 *              
 * ---------------------------------------------------------------- */
void TCDVDR_Init(void);

/**----------------------------------------------------------------
 *
 * TCDVDR_power_on_TC
 *
 * Description: Turn on the power supplied to the TC sensor.
 *              
 * ---------------------------------------------------------------- */
void TCDVDR_power_on_TC(void);

/**----------------------------------------------------------------
 *
 * TCDVDR_power_off_TC
 *
 * Description: Turn off the power supplied to the TC sensor.
 *              
 * ---------------------------------------------------------------- */
void TCDVDR_power_off_TC(void);

/**----------------------------------------------------------------
 *
 * TCDVDR_WriteRegister
 *
 * Description:  This routine allows writing of Touch-chip 
 *               register, which is always attached to SPI.
 *
 * @param        AddrPort   Address of Touch-chip register.
 *
 * @param        Data       Data which is to be written to
 *                          specified register.
 *              
 * ---------------------------------------------------------------- */
void TCDVDR_WriteRegister(uint8 AddrPort, uint32 Data);

/**----------------------------------------------------------------
 *
 * TCDVDR_ReadRegister
 *
 * Description:  This routine allows reading of touch-chip register
 *               which is always attached to SPI.
 *
 * @param        AddrPort   Address of Touch-chip register.
 *
 * @param        Data       Pointer to location into which should
 *                          be stored contents of given Touch-chip
 *                          register.
 *              
 * ---------------------------------------------------------------- */
void TCDVDR_ReadRegister(uint8 AddrPort, uint32 *Data);

/**----------------------------------------------------------------
 *
 * TCDVDR_GrabImage
 *
 * Description:  This is a blocking routine which programs DMA
 *               to receive data from SPIX which in turn receives
 *               data from touch-chip.  While DMA is doing the
 *               work, this routine monitors its progress and
 *               only exits when all data has been transferred.
 *
 * @param        ImgBuf   Pointer to buffer into which should
 *                        be stored image data.
 * 
 * @param        BufSize  Number of requested pixels (size of buffer).
 *
 * @return       STERR_OK upon success.                            
 *               STERR_ERROR upon failure.
 *
 * ---------------------------------------------------------------- */
sint32 TCDVDR_GrabImage(uint8 *ImgBuf, sint32 BufSize);

/**----------------------------------------------------------------
 *
 * TCDVDR_NVMWrite
 *
 * Description: This function writes to given offset of section
 *              in NVM which is reserved for TCI.
 *
 * @param   src     Buffer which contains data which needs to be
 *                  written to TCI section of NVM.
 *
 * @param   offset  Offset from beginning of TCI section of NVM
 *                  into which should be stored given data.
 *
 * @param   length  Number of bytes which should be written to
 *                  given offset of TCI section of NVM.
 *
 * @return  STERR_OK upon success.
 *              
 * ---------------------------------------------------------------- */
sint32 TCDVDR_NVMWrite(uint8 *src, uint32 offset, uint32 length);

/**----------------------------------------------------------------
 *
 * TCDVDR_NVMRead
 *
 * Description: This function reads part of NVM reserved for TCI.
 *
 * @param    dest     Buffer into which data which is read should 
 *                    be stored.
 *
 * @param    offset   Offset into section of NVM reserved for TCI.
 *
 * @param    length   Number of bytes to read from given offset
 *                    of section of NVM reserved for TCI.
 *
 * @return   STERR_OK upon success.  
 *              
 * ---------------------------------------------------------------- */
sint32 TCDVDR_NVMRead(uint8 *dest, uint32 offset, uint32 length);

/**----------------------------------------------------------------
 *
 * TCDVDR_Sleep
 *
 * Description: Delays for the specified number of milliseconds.
 *              Used to enforce sensor timing constraints.
 *
 * @param   ulDelay    
 *
 * ---------------------------------------------------------------- */
void TCDVDR_Sleep(__IO uint32 period);

/**----------------------------------------------------------------
 *
 * TCDVDR_Malloc
 *
 * Description: Allocates and returns a buffer of size nSize
 *
 * @param   nSize
 *
 * @return  Memory allocated buffer
 *              
 * ---------------------------------------------------------------- */
void* TCDVDR_Malloc(uint32 nSize);

/**----------------------------------------------------------------
 *
 * TCDVDR_Free
 *
 * Description: Frees allocated buffer memory
 *
 * @param   ptr
 *
 * ---------------------------------------------------------------- */
void TCDVDR_Free(void *ptr);


#endif
