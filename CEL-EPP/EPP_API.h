#ifndef _EPP_API_H
#define _EPP_API_H

#include "types.h"

// Sensor version and dimension
#define TCS1C              38    ///< TCS1C sensor version
#define TCS1A              37    ///< TCS1A sensor version
#define TCS2               27    ///< TCS2  sensor version

#define TCS1A_WIDTH        256   ///< TCS1A sensor width
#define TCS1A_HEIGHT       360   ///< TCS1A sensor height

#define TCS1C_WIDTH        256   ///< TCS1C sensor width
#define TCS1C_HEIGHT       360   ///< TCS1C sensor height

#define TCS2_WIDTH         208   ///< TCS2  sensor width
#define TCS2_HEIGHT        288   ///< TCS2  sensor height

/**
 * Sensor data structure to be used by STInitialize.
 * @li SensorVersion Sensor version.
 * @li ArrayWidth Sensor X dimension.
 * @li ArrayHeight Sensor Y dimension.
 */
typedef struct SENSOR_DATA {
  uint8  SensorVersion;
  uint16 ArrayWidth;
  uint16 ArrayHeight;
} SENSOR_DATA, *PSENSOR_DATA;

/**
 * Chip States to be used by STSetChipState.
 * @li 1 NOMINAL_MODE.
 * @li 2 STANDBY_MODE.
 * @li 3 SLEEP_MODE.
 * Note that these refer to the internal sensor status
 */
typedef enum
{
  NOMINAL_MODE = 1,
  STANDBY_MODE,
  SLEEP_MODE 
} TCPowerState;

// PerfectPrint functions

/**
 * Initializes the sensor and gets sensor data.
 * @param Data Sensor data.
 * @return STERR_OK Upon success, otherwise an error code.
 */
sint32 STInitialize(SENSOR_DATA *Data);

/**
 * Makes EPP ready to work.
 * @param VendorCode Sensor vendor code.
 * @return STERR_OK Upon success, otherwise an error code.
 */
sint32 STAuthentify(sint32 *VendorCode);

/**
 * Puts sensor into sleep mode.
 */
void STTerminate(void);

/**
 * Grabs a windowed image from the sensor.
 *
 * @param ImgBuff     Image buffer.  The size of 
 *                    this buffer has to be a 
 *                    multiple of 1024 bytes.
 *                   
 * @param rows        Number of rows to grab.
 *
 * @param row_offset  Row offset
 * 
 * @param delta_row   Delta row
 *
 * @param cols        Number of cols to grab.
 *
 * @param col_offset  Col offset
 *
 * @param delta_col   Delta col
 *
 * @return       STERR_OK Upon success, otherwise an error code.
 */
sint32 STGrab(uint8 *ImgBuff, sint32 rows, sint32 row_offset, sint32 delta_row,
              sint32 cols, sint32 col_offset, sint32 delta_col);

/**
 * Selects one of 32 settings.
 *              
 * @param set     Selected setting.
 *
 * @return       STERR_OK Upon success, otherwise an error code.
 */
sint32 STInitSensorPar(sint32 set);

/**
 * Gets TCI version and description.
 *              
 * @param i_pTCIRev   Pointer to the version.
 *
 * @param i_szDesc    Pointer to the description.
 *
 * @param DescBufSize buffer size for description.
 *
 * @return          STERR_OK Upon success, otherwise an error code.
 */
sint32 STGetTCIRevision(sint8 *i_pTCIRev, sint8 *i_szDesc, sint32 *DescBufSize);

/**
 * Tests the communication of the sensor.
 *              
 * @return   STERR_OK Upon success, otherwise an error code.
 */
sint32 STQueryDevice(void);

/**
 *
 * Gets the current setting of the sensor.
 *              
 * @return   Current setting (0-31)
 */
sint32 STGetCurrentSetting(void);

/**
 * Gets the total settings of the sensor.
 *              
 * @return  The total settings
 */
sint32 STGetTotalSettings(void);

/** 
 * Sets sensor power state.
 *              
 * @param State       Power state to be set.
 *
 * @return   STERR_OK Upon success, otherwise an error code.
 */
sint32 STSetChipState(TCPowerState State);

/**
 * Gets the current power state of the sensor.
 *              
 * @return  The current power state of the sensor.
 */
sint32 STGetChipState(void);

/**
 * Reads data from NVM.
 *              
 * @param NVMaddr     Starting address to be read.
 *
 * @param NVMaddr      Number of bytes to be read.
 *
 * @param Buffer     buffer to hold the data.
 *
 * @return   STERR_OK Upon success, otherwise an error code.
 */
sint32 STNVMRead(sint32 NVMaddr, sint32 NumBytes, uint8 *Buffer);	

/**
 * Writes data into NVM.
 *              
 * @param NVMaddr    Starting address to be written.
 *
 * @param NVMaddr     Number of bytes to be written.
 *
 * @param Buffer    Buffer to hold the data.
 *
 * @return   STERR_OK Upon success, otherwise an error code.
 */
sint32 STNVMWrite(sint32 NVMaddr, sint32 NumBytes, uint8 *Buffer);

#endif /* _EPP_API_H */
