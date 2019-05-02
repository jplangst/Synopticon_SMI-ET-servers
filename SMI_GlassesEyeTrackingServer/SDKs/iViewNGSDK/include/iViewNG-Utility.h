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
#ifndef _IVIEWNG_UTILITY_H
#define _IVIEWNG_UTILITY_H

/**
 * \file iViewNG-Utility.h
 *
 * This file declares some useful functions which may be useful while using the API.
 */

#include <iViewNG-Device.h>
#include <iViewNG-DataStreams.h>

// --------------------------------------------------------------------------------------------------------------------

/**
 * Put the process into idle mode for at least / approximately the specified amount of time. The purpose of this
 * function is to provide a platform-independent sleep function. The precision of sleeping exactly the specified
 * amount of time depends on the used operating system.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in]		milliSeconds	the amount of time to wait
 */

DLL_DEC_PRE void DLL_DEC_POST iView_Sleep (const uint32_t milliSeconds);

/**
 * Get a human-readable string for a given return code.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in,out]	dst				Memory where to store the Result. If this parameter is NULL or
 * @param[in,out]	maxSize			on call: size of the dst allocation in bytes
 * 									on return: bytes of the dst allocation used or bytes needed
 * @param[in]		rc		The return code to convert.
 *
 * @retval	#RC_NO_ERROR				on success. maxSize contains the used bytes of dst (incl. the terminating 0-char)
 * @retval	#RC_OUT_OF_MEMORY		dst ist not large enough to store the result. maxSize contains the
 * 									minimum required size in bytes. (incl. the terminating 0-char)
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_iViewRcToString (wchar_t * dst, size_t * maxSize, const iViewRC rc);

/**
 * Print the ticket contents to a char array. Useful for analyzing tickets manually.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in,out]	dst				Address of pointer to hold the result. The result is allocated with malloc() and must
 * 									be released by the caller. The result pointer must contain NULL.
 * @param[in,out]	maxSize			on call: size of the dst allocation in bytes
 * 									on return: bytes of the dst allocation used or bytes needed
 * @param[in]		ticket		Ticket to convert.
 *
 * @retval	#RC_NO_ERROR				on success. maxSize contains the used bytes of dst (incl. the terminating 0-char)
 * @retval	#RC_INVALID_PARAMETER	if ticket is NULL or if the contents of ticket is not NULL
 * @retval	#RC_OUT_OF_MEMORY		dst ist not large enough to store the result. maxSize contains the
 * 									minimum required size in bytes. (incl. the terminating 0-char)
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_iViewTicketToString (wchar_t * dst, size_t * maxSize,
        iViewTicket const * const ticket);

/**
 * Print the DeviceParameters parameter block contents to a char array. Useful for analyzing parameters manually.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in,out]	dst				Address of pointer to hold the result. The result is allocated with malloc() and must
 * 									be released by the caller. The result pointer must contain NULL.
 * @param[in,out]	maxSize			on call: size of the dst allocation in bytes
 * 									on return: bytes of the dst allocation used or bytes needed
 * @param[in]		params		Parameter block to convert.
 *
 * @retval	#RC_NO_ERROR				on success. maxSize contains the used bytes of dst (incl. the terminating 0-char)
 * @retval	#RC_INVALID_PARAMETER	if ticket is NULL or if the contents of ticket is not NULL
 * @retval	#RC_OUT_OF_MEMORY		dst ist not large enough to store the result. maxSize contains the
 * 									minimum required size in bytes. (incl. the terminating 0-char)
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_iViewDeviceParametersToString (wchar_t * dst, size_t * maxSize,
        iViewDeviceParameters const * const params);

/**
 * Print the iViewDataStreamType enum to a char array. Useful for analyzing parameters manually.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in,out]	dst				Address of pointer to hold the result. The result is allocated with malloc() and must
 * 									be released by the caller. The result pointer must contain NULL.
 * @param[in,out]	maxSize			on call: size of the dst allocation in bytes
 * 									on return: bytes of the dst allocation used or bytes needed
 * @param[in]		type		Parameter to convert.
 *
 * @retval	#RC_NO_ERROR				on success. maxSize contains the used bytes of dst (incl. the terminating 0-char)
 * @retval	#RC_INVALID_PARAMETER	if ticket is NULL or if the contents of ticket is not NULL
 * @retval	#RC_OUT_OF_MEMORY		dst ist not large enough to store the result. maxSize contains the
 * 									minimum required size in bytes. (incl. the terminating 0-char)
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_iViewDataStreamTypeToString (wchar_t * dst, size_t * maxSize,
        const iViewDataStreamType type);

// --------------------------------------------------------------------------------------------------------------------

/**
 * Return the value for NAN (not-a-number).
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @note	Please mind that it's not possible to compare NAN values using the '==' operator! Use iView_IsNan()
 * 			instead!
 *
 * @retval	NAN as the iViewNG library encodes it.
 */

DLL_DEC_PRE double DLL_DEC_POST iView_GetNanValue ();

/**
 * Return the value for +INF (positive infinity).
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @note	Please mind that it's not possible to compare INF values using the '==' operator! Use iView_IsInf()
 * 			instead!
 *
 * @retval	+INF as the iViewNG library encodes it.
 */

DLL_DEC_PRE double DLL_DEC_POST iView_GetInfValue ();

/**
 * Test if a given variable contains NAN.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in]	num		the variable to test
 *
 * @retval	0	if 'num' does not contain NAN
 * @retval	1	if 'num' contains NAN
 */

//DLL_DEC_PRE char DLL_DEC_POST iView_IsNan (const float num);
/**
 * Test if a given variable contains NAN.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in]	num		the variable to test
 *
 * @retval	0	if 'num' does not contain NAN
 * @retval	1	if 'num' contains NAN
 */

DLL_DEC_PRE char DLL_DEC_POST iView_IsNan (const double num);

/**
 * Test if a given variable contains +INF.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in]	num		the variable to test
 *
 * @retval	0	if 'num' does not contain +INF
 * @retval	1	if 'num' contains +INF
 */

//DLL_DEC_PRE char DLL_DEC_POST iView_IsInf (const float num);

/**
 * Test if a given variable contains INF.
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in]	num		the variable to test
 *
 * @retval	0	if 'num' does not contain +/- INF
 * @retval	1	if 'num' contains +INF
 * @retval	-1	if 'num' contains -INF
 */

DLL_DEC_PRE char DLL_DEC_POST iView_IsInf (const double num);

// --------------------------------------------------------------------------------------------------------------------

#endif
