// ----------------------------------------------------------------------------
//
// (c) Copyright 1997-2015, SensoMotoric Instruments GmbH
//
// All rights reserved. This work contains unpublished proprietary information 
// of SensoMotoric Instruments GmbH and is copy protected by law. It may not 
// be disclosed to third parties or copied or duplicated in any form, in whole
// or in part, without the specific written permission of SensoMotoric 
// Instruments GmbH
//
// ----------------------------------------------------------------------------
#ifndef _IVIEWNG_BASIC_DATATYPES_H
#define _IVIEWNG_BASIC_DATATYPES_H

/**
 * \file iViewNG-BasicDatatypes.h
 *
 * These are the basic datatypes and limits the iView SDK uses. Most compilers should have these datatypes defined
 * properly. If required, please modify this file locally to assert that your compiler uses the types as intended.
 *
 * For MinGW (gcc on Windows), the datatypes are defined in stdint.h, limits in limits.h.
 */
#ifdef __GNUC__

// For GNU compilers, stdint.h defines those data types.
#include <stdint.h>

#else

// If the compiler doesn't, please define the data types here:

//! 8 bit datatypes
typedef signed char int8_t;
typedef unsigned char uint8_t;

//! 16 bit datatypes
typedef short int16_t;
typedef unsigned short uint16_t;

//! 32 bit datatypes
typedef int int32_t;
typedef unsigned uint32_t;

//! 64 bit datatypes
typedef long long int64_t;
typedef unsigned long long uint64_t;

#endif // __GNUC__

// --------------------------------------------------------------------------------------------------------------------

// For most compilers, limits.h define the required limits.
#include <limits.h>

// Define the limits if not included in limits.h:
#ifndef UINT_MAX
#define UINT_MAX 0xffffffff
#endif

// --------------------------------------------------------------------------------------------------------------------

// This defines size_t and others.
#include <stddef.h>

// If your compiler doesn't know size_t, define it here:
// typedef unsigned int size_t;

#endif // _IVIEWNG_BASIC_DATATYPES_H
