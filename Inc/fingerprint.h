#ifndef __fingerprint_H
#define __fingerprint_H

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "MyTypedef.h"

#include "SPI.h"
#include "PSIQual.h"
#include "ERL_API.h"
#include "ECL_API.h"
#include "EPP_API.h"
#include "EPP_ERRORS.h"

//
//#include "MyTypedef.h"
//#include "stm32f4xx_hal.h"
//#include "MidSystemClock.h"
//#include "MidFlash.h"

void FINT_DeviceInit(void);		//指纹设备初始化
bool FINT_SelfTest(void);
bool FINT_calibrate(void);
bool FINT_gather(void);




#endif /* __fingerprint_H */
