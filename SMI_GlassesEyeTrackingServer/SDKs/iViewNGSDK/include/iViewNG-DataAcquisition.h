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
#ifndef _IVIEWNG_DATA_ACQUISITION_H
#define _IVIEWNG_DATA_ACQUISITION_H

/**
 * \file iViewNG-Connection.h
 *
 * This file declares the iViewNG API functions and data types which are required for handling data
 * acquisition as well as storing acquired data by the server.
 */

#include <iViewNG-Core.h>

// --------------------------------------------------------------------------------------------------------------------

/**
 * Trigger the start of data acquisition.
 *
 * The ticket progress will be set to 100% when the request was processed by the server.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								Device Parameters were set with iView_SetDeviceParameters().
 *
 * @pre								Eye tracking device is available (connected and recognized by iViewNG server).
 *
 * @pre								ET device license permits use of this function.
 *
 * @pre								A data stream provided by the sensor to start is acquired.
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * This function returns synchronously and provides the result of the operation to the user callback function.
 * Use the ticket to associate the operation request and the return of the result.
 * On return, the ticket's elements are set:
 * - iViewTicket.resultType is #IVIEWRESULT_DATA_ACQUISITION_START.
 * 									- On success:
 * 										- iViewTicket.returnCode is RC_NO_ERROR,
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_IN_PROGRESS until the data acquisition
 * 									      is stopped actively or due to an error.
 * 										- iViewTicket.progress is 50.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED
 * 										- iViewTicket.progress is 100.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @retval	#RC_NO_ERROR				on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_STATE		if a server-side precondition is not met
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_StartDataAcquisition (iViewTicket * ticket);

/**
 * Trigger the end of data acquisition.
 *
 * The ticket progress will be set to 100% when the request was processed by the server.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								Device Parameters were set with iView_SetDeviceParameters().
 *
 * @pre								Eye tracking device is available (connected and recognized by iViewNG server).
 *
 * @pre								ET device license permits use of this function.
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * This function returns synchronously and provides the result of the operation	to the user callback function.
 * Use the ticket to associate the operation request and the return of the result.
 * On return, the ticket's elements are set:
 * - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED
 * - iViewTicket.progress is 100.
 * 									- On success:
 *   - iViewTicket.returnCode is #RC_NO_ERROR,
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 *
 * @post							On success, this function will close the ticket which called iView_StartDataAcquisition();
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @retval	#RC_NO_ERROR				on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_StopDataAcquisition (iViewTicket * ticket);

/**
 * Ask the iViewNG server to start recording.
 *
 * The ticket progress will be set to 100% when the request was processed by the server.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								Device Parameters were set with iView_SetDeviceParameters().
 *
 * @pre								Eye tracking device is available (connected and recognized by iViewNG server).
 *
 * @pre								ET device license permits use of this function.
 *
 * @pre								Data acquisition is in progress.
 * 
 * @pre								After iView_StartDataAcquisition() the server had a cool down time of at least 2 seconds via iView_Sleep().
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * This function returns synchronously and provides the result of the operation to the user callback function.
 * Use the ticket to associate the operation request and the return of the result.
 * On return, the ticket's elements are set:
 * 									- On success:
 *   - iViewTicket.returnCode is #RC_NO_ERROR,
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_IN_PROGRESS until the recording is stopped actively or due to an error.
 * 										- iViewTicket.progress is 50.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED
 * 										- iViewTicket.progress is 100.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @retval	#RC_NO_ERROR				on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_StartRecording (iViewTicket * ticket);

/**
 * Ask the iViewNG server to stop recording.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								Device Parameters were set with iView_SetDeviceParameters().
 *
 * @pre								Eye tracking device is available (connected and recognized by iViewNG server).
 *
 * @pre								ET device license permits use of this function.
 *
 * @pre								Recording is in progress.
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * This function returns synchronously and provides the result of the operation to the user callback function.
 * Use the ticket to associate the operation request and the return of the result.
 * On return, the ticket's elements are set:
 * - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED
 * - iViewTicket.progress is 100.
 * 									- On success:
 *   - iViewTicket.returnCode is #RC_NO_ERROR,
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 *
 * @post							On success, this function will close the ticket which called iView_StartRecording();
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @retval	#RC_NO_ERROR				on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_STATE		if a server-side precondition is not met
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_StopRecording (iViewTicket * ticket);

// --------------------------------------------------------------------------------------------------------------------

//! Identifier for the iViewMessage data structure.
typedef uint32_t iViewMessageType;

// Possible data types for iViewResultTypes
static const iViewMessageType IVIEWMESSAGETYPE_INVALID = 0;		//!< Invalid type, do not use.
static const iViewMessageType IVIEWMESSAGETYPE_TEXT = 1; 		//!< The structure is a iViewMessageText structure.

// ATM only these three types are defined and allowed
//static const iViewMessageType IVIEWMESSAGETYPE_USER_0 = 100;
//static const iViewMessageType IVIEWMESSAGETYPE_USER_1 = 101;
//static const iViewMessageType IVIEWMESSAGETYPE_USER_2 = 102;

/**
 * Declaration of the basic message structure. There are several message structures available,
 * depending on the message type. The element iViewMessage.type indicates the data type of the
 * whole structure. All message structures share the first element: use static casting
 * to access the other elements.
 */
typedef struct {

		//! Identify the type of this structure.
		iViewMessageType type;

} iViewMessage;

/**
 * A message including a simple NULL-terminated text string. BeGaze will integrate the string as comment line into
 * the exported IDF.
 */
typedef struct {

		//! Data inherited from iViewMessage. Value must be #IVIEWMESSAGETYPE_TEXT.
		iViewMessageType type;

		//! The text to store. The user is responsible for releasing the data.
		wchar_t const * text;

} iViewMessageText;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Submit a message to the server for proper storage.
 *
 * Use this function to pass additional information concerning a participant to the iViewNG server for storage. The
 * data will be saved together with the other participant-related files. Use BeGaze's IDF data export function to
 * integrate the recorded messages into the IDF file. The appearance of the data depends on the message type and the
 * used eye tracking device.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								Eye tracking device is available (connected and recognized by iViewNG server).
 *
 * @pre								ET device license permits use of this function.
 *
 * @pre								Participant data was set.
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * This function returns synchronously and provides the result of the operation to the user callback function.
 * Use the ticket to associate the operation request and the return of the result.
 * On return, the ticket's elements are set:
 * - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED
 * - iViewTicket.progress is 100.
 * 									- On success:
 *   - iViewTicket.returnCode is #RC_NO_ERROR,
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 * @param[in]		message			Pointer to a message structure. May be released immediately after calling
 * 									iView_SaveMessage(). The user is responsible for releasing the data.
 *
 * @retval	#RC_NO_ERROR				on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_STATE		if a server-side precondition is not met
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */
DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SaveMessage (iViewTicket * ticket, iViewMessage const * message);

// --------------------------------------------------------------------------------------------------------------------

#endif
