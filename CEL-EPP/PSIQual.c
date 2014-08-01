/**
 * @file PSIQual.c
 *
 * PSI Qualification
 *
 * Copyright UPEK 2007-2010
 *
 * Version: 1.1
 *
 */

#include <string.h>
#include "spi.h"
//#include "types.h"
#include "EPP_API.h"
#include "EPP_ERRORS.h"
#include "PSIQual.h"

/**************************************************************************
 * How to Apply Power to Sensor:    TCDVDR_power_on_TC();
 *                                  TCDVDR_Sleep(2000);
 *                                  TCDVDR_Init(); 
 *
 * How to Remove Power from Sensor: TCDVDR_power_off_TC();
 **************************************************************************/

/**************************************************************************
 * Step #1: Review electrical integration on the TouchChip device side. 
 *          - How is the power management handled?
 *          - Are there 3V-5V level issues?
 **************************************************************************/

/**************************************************************************
 * Step #2: Verify the cold-wire controller interfacing the TouchChip generated 
 *          R and W cycles compliant with the TouchChip expected timing: 
 *          - R and W strobes >= 120ns
 *          - Each R or W must have a R or W strobe generated. It is not 
 *            acceptable that only the Address line is changed (as SRAM 
 *            devices would allow)
 *          - Interval between adjacent R or W operation >= 120ns
 **************************************************************************/

/**************************************************************************
 * Step #3: Re-verify Step #2 by reading all registers. At power-up the 
 *          registers of the TouchChip are set with default values. All the 
 *          default values are listed in the Interface With Embedded System 
 *          document, so these default values can be used for testing 
 *          purposes to check if the TouchChip can be accessed correctly. 
 *          The REGISTER READ functionality can be checked by reading 
 *          registers and using the default value table in the document to 
 *          check the values.
 *
 *          (sample source code provided in function PSIQualRegisterDefaults)
 **************************************************************************/

// Note: Power to sensor must be turned on before calling this function
sint32 PSIQualRegisterDefaults(sint32 VendorCode)
{
    uint32 data;
 
    //register 0x0F
    TCDVDR_ReadRegister(0x0F, &data);
    if(VendorCode == TCS2)
    {
        if((data & 0xFF) != 0x6C) 
			return STERR_ERROR;
    }
    else if(VendorCode == TCS1C)
    {
        if((data & 0xFF) != 0x98) 
			return STERR_ERROR;
    }
    else
    {
        if((data & 0xFF) != 0x94) 
			return STERR_ERROR;
    }

    //register 0x10
    TCDVDR_ReadRegister(0x10, &data);
    if((data & 0xFF) != 0x00) 
		return STERR_ERROR;

    //register 0x12
    TCDVDR_ReadRegister(0x12, &data);
    if(VendorCode == TCS2)
    {
        if((data & 0xFF) != 0xCF) 
			return STERR_ERROR;
    }
    else
    {
        if((data & 0xFF) != 0xFF) 
			return STERR_ERROR;
    }

    //register 0x14
    TCDVDR_ReadRegister(0x14, &data);
    if((data & 0xFF) != 0x01) 
		return STERR_ERROR;

    //register 0x15
    TCDVDR_ReadRegister(0x15, &data);
    if((data & 0xFF) != 0x00) 
		return STERR_ERROR;

    //register 0x17
    TCDVDR_ReadRegister(0x17, &data);
    if(VendorCode == TCS2)
    {
        if((data & 0xFF) != 0x1F) 
			return STERR_ERROR;
    }
    else
    {
        if((data & 0xFF) != 0x67) 
			return STERR_ERROR;
    }

    //register 0x19
    TCDVDR_ReadRegister(0x19, &data);
    if((data & 0xFF) != 0x01) 
		return STERR_ERROR;

    //register 0x1A
    TCDVDR_ReadRegister(0x1A, &data);
    if((data & 0xFF) != 0x09) 
		return STERR_ERROR;

    //register 0x1B
    TCDVDR_ReadRegister(0x1B, &data);
    if((data & 0xFF) != 0x09) 
		return STERR_ERROR;

    //register 0x1C
    TCDVDR_ReadRegister(0x1C, &data);
    if((data & 0xFF) != 0x0F) 
		return STERR_ERROR;

    //register 0x1D
    TCDVDR_ReadRegister(0x1D, &data);
    if((data & 0xFF) != 0x00) 
		return STERR_ERROR;

    //register 0x1F
    TCDVDR_ReadRegister(0x1F, &data);
    if(VendorCode == TCS2)
    {
        if((data & 0xFF) != 0x6C) 
			return STERR_ERROR;
    }
    else if(VendorCode == TCS1C)
    {
        if((data & 0xFF) != 0x98) 
			return STERR_ERROR;
    }
    else
    {
        if((data & 0xFF) != 0x94) 
			return STERR_ERROR;
    }


    return STERR_OK;
}

/**************************************************************************
 * Step #4: Re-verify Step #2 by writing & reading registers. This will 
 *          check the REGISTER WRITE functionality. Please note that some 
 *          registers when written with some specific values trigger some 
 *          other operation that can result in alteration of other register 
 *          values. 
 *
 *          Harmless Registers: 0x00, 0x07, 0x08, 0x10, 0x12, 0x14, 0x15, 
 *                              0x17, 0x19
 *
 *          (sample source code provided in function PSIQualRegisterOperation)
 **************************************************************************/

// Note: Power to sensor must be turned on before calling this function
sint32 PSIQualRegisterOperation(void)
{
    uint32 data, save_data, test_data = 0xA5;
    uint8 i, harmlessRegs[9] = {0x00, 0x07, 0x08, 0x10, 0x12, 0x14, 0x15, 0x17, 0x19};

    // test all harmless registers for readablity/writablity
    for(i=0; i<9; i++)
    {
        TCDVDR_ReadRegister(harmlessRegs[i], &save_data);
        TCDVDR_WriteRegister(harmlessRegs[i], test_data);
        TCDVDR_ReadRegister(harmlessRegs[i], &data);
        TCDVDR_WriteRegister(harmlessRegs[i], save_data);
        if((uint8)data != (uint8)test_data) 
			return STERR_ERROR;
    }
    return STERR_OK;
}

/**************************************************************************
 * Step #5: Verify the NVM WRITE and NVM READ functionality.
 *
 *          (sample source code provided in function PSIQualNVMOperation)
 **************************************************************************/

// Note: Power to sensor must be turned on before calling this function
sint32 PSIQualNVMOperation(void)
{
    uint8 test_buffer[20], read_buffer[20];
    uint32 i, offset, length;
    sint32 ret;

    offset = 0;
    length = 20;

    // write test NVM data
    for(i=0; i<length; i++)
    {
        test_buffer[i] = (uint8)i;
    }
    ret = TCDVDR_NVMWrite(test_buffer, offset, length);
    if(ret != STERR_OK)
        return ret;

    // read test NVM data
    ret = TCDVDR_NVMRead(read_buffer, offset, length);
    if(ret != STERR_OK)
        return ret;

    // check if test buffer written and read correctly
    if(memcmp(test_buffer, read_buffer, length))
    {
        return STERR_ERROR;
    }

    return STERR_OK;
}

/**************************************************************************
 * Step #6: Verify correct implementation of the TCDVDR_Sleep function. What 
 *          we want to really verify is that a delay at the HOST side is 
 *          properly “transported?to the device end: just space R and W 
 *          operation by some interval of time and verify that the same 
 *          interval is observed on the device side.
 *
 *          This can be verified by toggling a GPIO with a delay in between 
 *          high and low states using the TCDVDR_Sleep function.  Upon 
 *          implementing this process below, measure GPIO with an oscilliscope
 *          and verify that the same or greater delay amount used in the 
 *          TCDVDR_Sleep function is mirrored by the measurements.  Test this 
 *          for various delay amounts to ensure proper working implementation 
 *          of TCDVDR_Sleep function.  We suggest testing using sleep amounts 
 *          > 5000ms.
 *
 *          Process: 1. Set GPIO low
 *                   2. TVDVDR_Sleep(X)
 *                   3. Set GPIO high
 *                   4. TVDVDR_Sleep(X)
 *                   5. Repeat 1-4
 **************************************************************************/

/**************************************************************************
 * Step #7: Verify that we can read the FIFO. This can be done by enabling 
 *          a FIFO test: FIFO is filled by the HOST by known values and the 
 *          FIFO is then read back with a TCDVDR_GrabImage operation.
 *
 *          Setup: 1. Write 0x4D to register 0x09 (enable internal clock)
 *                 2. Write 0x10 to register 0x0C (enable FIFO test mode)
 *                 3. Write "YOUR_CODE" to register 0x0D (data that will fill the FIFO)
 *                 4. Write 0x00 to register 0x10h (start readout operation)
 *
 *          Test:  1. Grab full sized image using TCDVDR_GrabImage (read register 0xFF)
 *                 2. Verify each pixel in grabbed image is "YOUR_CODE"
 *
 *          Notes: - There might be some initial 0x00 and this is expected 
 *                   when read operation is too close to last write register 
 *                   command (10uSec or more delay is recommended).
 *                 - If the data read is correct but is interleaved with 0x00, 
 *                   the read operation is too fast and results in occasional 
 *                   FIFO_empty (that are coded as 0x00).
 *                 - Max read speed should not exceed 700kB/sec to avoid getting 
 *                   FIFO_empty data.
 *
 *          (sample source code provided in function PSIQualTestFIFO)
 **************************************************************************/

// Note: Power to sensor must be turned on before calling this function
sint32 PSIQualTestFIFO(uint32 *pZeroCount)
{
    uint32 VendorCode, data, my_code = 0xA5;
    sint32 ret, i, ImgSize;
    uint8 *ImgBuf = (uint8*)TCDVDR_Malloc(256*360);;


    //enable internal clock
    TCDVDR_WriteRegister(0x09, 0x4D);

    //enable FIFO test mode
    TCDVDR_WriteRegister(0x0C, 0x10);

    //data that will fill the FIFO
    TCDVDR_WriteRegister(0x0D, my_code);

    //start readout operation
    TCDVDR_WriteRegister(0x10, 0x00);

    //grab an image (read register 0xFF)
    TCDVDR_ReadRegister(0x0F, &data);
    VendorCode = (data & 0xFC) >> 2;
    if(VendorCode == TCS2)
    {
        ImgSize = TCS2_WIDTH * TCS2_HEIGHT;
    }
    else if(VendorCode == TCS1C)
    {
        ImgSize = TCS1C_WIDTH * TCS1C_HEIGHT;
    }
    else
    {
        ImgSize = TCS1A_WIDTH * TCS1A_HEIGHT;
    }
    ret = TCDVDR_GrabImage(ImgBuf, ImgSize);

    //verify each pixel in grabbed image is "my_code" and count number of 0x00 pixels
    if(ret == STERR_OK)
    {
        *pZeroCount = 0;
        for(i=0; i<ImgSize; i++)
        {
            if(ImgBuf[i] == 0x00)
            {
                (*pZeroCount)++;
            }
            else 
			if(ImgBuf[i] != my_code)
            {
				TCDVDR_Free(ImgBuf);
                return STERR_ERROR;
            }
        }
    }

	TCDVDR_Free(ImgBuf);
    return STERR_OK;
}


