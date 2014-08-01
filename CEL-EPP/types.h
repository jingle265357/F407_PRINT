/**
 * @file types.h
 *
 * This module defines types which if used ease 
 * porting effort significantly.
 *
 * Copyright Authentec Inc
 *
 */

#ifndef TYPES_H
#define TYPES_H

/*
 * type mappings 
 */
typedef signed char    sint8;
typedef unsigned char  uint8;
typedef signed short   sint16;
typedef unsigned short uint16;
typedef signed long    sint32;
typedef unsigned long  uint32;
typedef float          float32;
typedef double         float64;

#include "spi.h"

#include "mdefines.h"

/**
 * Boolean false value
 */
#ifndef FALSE
#define FALSE           0
#endif /* #ifndef FALSE */

/**
 * Boolean true value
 */
#ifndef TRUE
#define TRUE            1
#endif /* #ifndef TRUE */

/**
 * NULL value
 */
#ifndef NULL
#define NULL            (0)
#endif /* #ifndef NULL */

#endif
