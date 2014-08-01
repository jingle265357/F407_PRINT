/**
 * @file PSIQual.h
 *
 * PSI Qualification
 *
 * Copyright UPEK 2007
 *
 * Version: 1.0
 *
 */

#ifndef _PSI_QUAL_H_
#define _PSI_QUAL_H_

sint32 PSIQualRegisterDefaults(sint32 SensorVersion);

sint32 PSIQualRegisterOperation(void);

sint32 PSIQualNVMOperation(void);

sint32 PSIQualTestFIFO(uint32 *pZeroCount);

#endif /*_PSI_QUAL_H_*/
