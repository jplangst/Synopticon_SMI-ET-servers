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
#ifndef _IVIEWNG_CORE_H
#define _IVIEWNG_CORE_H

/**
 * \file iViewNG-Core.h
 *
 * This file declares the iViewNG API functions and data types which are considered the 'core' of the
 * API: initializing and deinitializing the client and server, fundamental interaction with the
 * iViewNG API.
 */

#include <iViewNG.h>

// --------------------------------------------------------------------------------------------------------------------

//! Data structure to represent a software version.
typedef struct {

		uint32_t major; 		//!< The major version number.
		uint32_t minor; 		//!< The minor version number.
		uint32_t patch; 		//!< The patch version number.
		uint32_t build; 		//!< The build number.

} iViewVersion;

// --------------------------------------------------------------------------------------------------------------------

//! Data structure to represent a time scale.
typedef struct {

		uint64_t time; 			//!< Absolute time in nanoseconds.

} iViewTime;

// --------------------------------------------------------------------------------------------------------------------

//! Data structure to represent a date.
typedef struct {

		uint32_t day; 			//!< Number of the day of the month, element of the interval [1..31].
		uint32_t month; 		//!< Number of the month, element of the interval [1..12].
		uint32_t year; 			//!< Year A.D. with 4 digits, e.g. 2010.

} iViewDate;

// --------------------------------------------------------------------------------------------------------------------

//! Data structure for error messages which also include a description.
typedef struct {
		//! The server may generate a sophisticated error code to further declare what happened. Defaults to
		//! #RC_UNSPECIFIED.
		iViewRC errorCode;

		//! A null-terminated human-readable message.
		wchar_t * message;

} iViewServerError;

// --------------------------------------------------------------------------------------------------------------------

//! Declaration of the SDK communication scheme.
typedef uint32_t iViewSdkType;

static const iViewSdkType IVIEWSDK_INVALID = 0; 	//!< Invalid value, do not use!
static const iViewSdkType IVIEWSDK_IVNG = 1; 		//!< Indicate iViewNG API and communication scheme.
//static const iViewSdkType IVIEWSDK_LEGACY = 2; 	//!< Indicate iView X legacy API and communication scheme.

// --------------------------------------------------------------------------------------------------------------------

//! The type of the data structure which is addressed by iViewResult.data.
//! To be able to determine the result data type, each iViewResult contains the data type of the included
//! result. Use iViewResult.type to determine how to cast iViewResult.data.
typedef uint32_t iViewResultType;

// Possible data types for iViewResultTypes
static const iViewResultType IVIEWRESULT_INVALID = 0;					//!< Never used!
static const iViewResultType IVIEWRESULT_ERROR = 1; 					//!< Result for iView_Connect()
static const iViewResultType IVIEWRESULT_CONNECT = 1; 					//!< Result for iView_Connect()
static const iViewResultType IVIEWRESULT_DISCONNECT = 2; 				//!< Result for iView_Connect()
static const iViewResultType IVIEWRESULT_ADD_LICENSE_KEY = 3; 			//!< Result for iView_AddLicenseKey()
static const iViewResultType IVIEWRESULT_GET_DEVICE_INFO = 4; 			//!< Result for iView_QueryDeviceVersion()
static const iViewResultType IVIEWRESULT_SET_DEVICE_PARAMETERS = 5; 	//!< Result for iView_SetDeviceParameters()
static const iViewResultType IVIEWRESULT_GET_DEVICE_PARAMETERS = 6; 	//!< Result for iView_SetDeviceParameters()
static const iViewResultType IVIEWRESULT_SUBSCRIBE_DATASTREAM = 7; 		//!< Result for iView_SubscribeDataStream()
static const iViewResultType IVIEWRESULT_UNSUBSCRIBE_DATASTREAM = 8; 	//!< Result for iView_SubscribeDataStream()
static const iViewResultType IVIEWRESULT_RECORDING_START = 9; 			//!< Result for iView_StartRecording()
static const iViewResultType IVIEWRESULT_RECORDING_STOP = 10; 			//!< Result for iView_StartRecording()
static const iViewResultType IVIEWRESULT_DATA_ACQUISITION_START = 11; 	//!< Result for iView_StartDataAcquisition()
static const iViewResultType IVIEWRESULT_DATA_ACQUISITION_STOP = 12; 	//!< Result for iView_StopDataAcquisition()
static const iViewResultType IVIEWRESULT_CALIBRATION_SETUP = 13; 		//!< Result for iView_CalibrationSetup()
static const iViewResultType IVIEWRESULT_CALIBRATION_START = 14; 		//!< Result for iView_CalibrationStart()
static const iViewResultType IVIEWRESULT_CALIBRATION_SEND_DATA = 15; 	//!< Result for iView_SendCalibrationData()
static const iViewResultType IVIEWRESULT_CALIBRATION_CANCEL = 16; 		//!< Result for iView_CalibrationCancel()
static const iViewResultType IVIEWRESULT_CALIBRATION_END = 17; 			//!< Result for iView_CalibrationEnd()
static const iViewResultType IVIEWRESULT_CALIBRATION_LIST = 18; 		//!< Result for iView_CalibrationList()
static const iViewResultType IVIEWRESULT_CALIBRATION_LOAD = 19; 		//!< Result for iView_CalibrationLoad()
static const iViewResultType IVIEWRESULT_CALIBRATION_SAVE = 20; 		//!< Result for iView_CalibrationSave()
static const iViewResultType IVIEWRESULT_GET_SERVER_TIME = 21; 			//!< Result for iView_GetServerTime()
static const iViewResultType IVIEWRESULT_GET_LOCAL_TIME = 22; 			//!< Result for iView_GetLocalTime()
static const iViewResultType IVIEWRESULT_SET_PARTICIPANT_DATA = 23; 	//!< Result for iView_SetParticipantData()
static const iViewResultType IVIEWRESULT_GET_PARTICIPANT_DATA = 24; 	//!< Result for iView_GetParticipantData()
static const iViewResultType IVIEWRESULT_SET_EXPERIMENT_DATA = 25; 		//!< Result for iView_SetExperimentData()
static const iViewResultType IVIEWRESULT_GET_EXPERIMENT_DATA = 26; 		//!< Result for iView_GetExperimentData()
static const iViewResultType IVIEWRESULT_SHUTDOWN_SERVER = 27; 			//!< Result for iView_shutdownserver()
static const iViewResultType IVIEWRESULT_SET_GAZE_CORRECTION_DATA = 28; //!< Result for iView_SetGazeCorrectionData()
static const iViewResultType IVIEWRESULT_GET_GAZE_CORRECTION_DATA = 29; //!< Result for iView_GetGazeCorrectionData()
static const iViewResultType IVIEWRESULT_SERVER_ERROR = 30; 			//!< Result for API-generated server error message
static const iViewResultType IVIEWRESULT_SAVE_MESSAGE = 31; 			//!< Result for save message
static const iViewResultType IVIEWRESULT_SET_ANNOTATION_DEFINITION = 37; //!< Result for iView_SetAnnotationDefinition()
static const iViewResultType IVIEWRESULT_GET_ANNOTATION_DEFINITION = 38; //!< Result for iView_GetAnnotationDefinition()
static const iViewResultType IVIEWRESULT_SET_ANNOTATION_INSTANCE = 39; 	//!< Result for iView_SetAnnotationInstance()
static const iViewResultType IVIEWRESULT_GET_ANNOTATION_INSTANCE = 40; 	//!< Result for iView_GetAnnotationInstance()
static const iViewResultType IVIEWRESULT_SET_CONTROLLER_STATUS = 41; 	//!< Result for iView_SetControllerStatus()
static const iViewResultType IVIEWRESULT_GET_EXPERIMENTS = 42; 			//!< Result for iView_GetExperiments()
static const iViewResultType IVIEWRESULT_SET_EYEMODEL = 43; 			//!< Result for iView_SetEyeModel()
static const iViewResultType IVIEWRESULT_GET_EYEMODEL = 44; 			//!< Result for iView_GetEyeModel()
static const iViewResultType IVIEWRESULT_TAKE_CONTROL = 45; 			//!< Result for iView_TakeControl()
static const iViewResultType IVIEWRESULT_GET_HOSTLIST = 46; 			//!< Result for iView_GetHostList()
static const iViewResultType IVIEWRESULT_GET_LICENSEDATA = 47; 			//!< Result for iView_GetLicenseData()
static const iViewResultType IVIEWRESULT_GET_LICENSESTATUS = 48; 		//!< Result for iView_GetLicenseData()
static const iViewResultType IVIEWRESULT_INVALID_SPECIFIER = 49; 		//!< Result for iView_GetLicenseStatus()
static const iViewResultType NUMBER_OF_IVIEWRESULTS = 50; 				//!< Never use!

//! Every iViewTicket structure may contain a iViewResult data structure. This result structure is used to submit
//! data back to the user application. To be able to determine the result data type, each iViewResult contains the
//! data type of the included result. Use iViewResult.type to determine how to cast iViewResult.data.
typedef struct __iViewResultStruct iViewResult;
struct __iViewResultStruct {

		//! The type of the data structure data points to.
		iViewResultType type;

		//! The result data. Use the iViewResult.type to determine how to cast.
		void * data;

		//! If more than one result is submitted at once, this is a pointer to the next result structure.
		//! Defaults to NULL (no more data).
		iViewResult * next;
};

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------

/**
 * Every iViewTicket contains a unique ticket number. Ticket numbers are managed by the iView API and must not be
 * modified by the client.
 */
typedef uint32_t iViewTicketNumber;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Every iViewTicket contains a ticket state to indicate the current state of processing the request. The state
 * describes the state of the API request itself, not the state of possibly collecting data from the iView
 * server.
 *
 * @note	Most API functions adjust the ticket state only after performing some initial checks, e.g. the system
 * 			initialization. The purpose of the state is to indicate the request state based on asynchronous feedback
 * 			from the server.
 */
typedef uint8_t iViewTicketState;

//! The status of a ticket is 'unprocessed' until the API starts with processing the request.
//! This value is always the initial value.
static const iViewTicketState IVIEWTICKETSTATUS_UNPROCESSED = 0x00;

//! The status of a ticket is 'accepted' when the request was recognized as valid and the request is forwarded to
//! the iViewNG server. and until the processing of the
//! request was not yet finished.
static const iViewTicketState IVIEWTICKETSTATUS_ACCEPTED = 0x04;

//! The status of a ticket is 'rejected' if the request was illegal, e.g. because the ticket data structure was
//! invalid or because the license did not provide access to that request.
static const iViewTicketState IVIEWTICKETSTATUS_REJECTED = 0x06;

//! The status of a ticket is 'processed' when the request was processed but the triggered operation is still in
//! progress. This is especially the case with server connections and data stream subscriptions.
static const iViewTicketState IVIEWTICKETSTATUS_IN_PROGRESS = 0x0a;

//! The status of a ticket is 'completed' when the request was processed and the triggered operation finished.
//static const iViewTicketState IVIEWTICKETSTATUS_COMPLETED = 0x0b;

//! The status of a ticket is 'closed' if the processing of the request was finished. Most functions will call the
//! user callback function after setting the state to 'closed'. The user shall then gather the required data from
//! the ticket result data structure and eventually call iView_ReleaseTicket() to release allocated memory. However,
//! streaming tickets will remain in the 'accepted' state until the stream subscription is canceled.
static const iViewTicketState IVIEWTICKETSTATUS_CLOSED = 0x10;

//! The status of a ticket is 'abandoned' if the user called iView_ReleaseTicket() to release allocated memory. It
//! is then (not earlier!) safe to delete the ticket data structure. A released ticket's result pointer is NULL.
static const iViewTicketState IVIEWTICKETSTATUS_RELEASED = 0x14;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Every iViewTicket contains a progress indicator to provide the state of request progress with a percentage scale
 * from 0 to 100 (request finished). Not all data requests provide information about the progress, see the function
 * documentation.
 */
typedef uint8_t iViewTicketProgress;

// --------------------------------------------------------------------------------------------------------------------

//! Constant indicating the number of characters allocated for a string in a structure.
static const unsigned int IVIEW_SIZE_OF_FUNCTION_NAME = 64;

/**
 * Datastructure for managing responses to API requests.
 *
 * Never allocate an iViewTicket structure on your own, use iView_CreateTicket() instead.
 *
 * Whenever an API function is called, the caller provides a pointer to a (not necessarily initialized) iViewTicket
 * structure. The iViewNG API writes a unique ticket number into the data structure and tags the request according
 * to the processing state. The caller may at any time check if the request was already answered by examining the
 * state indicator. While this flag is not set to #IVIEWTICKETSTATUS_IN_PROGRESS, the caller must not release the
 * memory or modify its contents.
 *
 * Upon completion, iViewNG may call the callback function with the iViewTicket as parameter. At this time, the
 * requestPending flag is set to #IVIEWTICKETSTATUS_IN_PROGRESS. If the called API function was supposed to return
 * data, the member 'result' is set to the result data structure.
 *
 * The caller of the API functions is then responsible for explicitly indicating to release all data structures!
 * iViewNG will not release neither iViewTicket structures nor the result data structure without the caller
 * explicitly requesting to release the data! The user must call iView_ReleaseTicket() to let iViewNG release the
 * iViewTicket structure and all contained structures.
 *
 * @attention	Do not create the iViewTicket on your stack! Assert that the ticket memory is not explicitly or
 * 				implicitly released or manipulated. This would corrupt your memory! If no longer required,
 * 				call iView_release() to explicitly release the ticket data structure!
 */
typedef struct {

		//! The unique number of the request. Use this number to identify tickets. There will never be more than
		//! one ticket with this ticket number.
		iViewTicketNumber ticketNumber;

		//! The name of the function call which was used for this ticket. There may be more than one ticket with
		//! this function name. Use this only for debugging purposes.
		wchar_t functionName [IVIEW_SIZE_OF_FUNCTION_NAME];

		//! The processing state of the ticket. Enumeration.
		iViewTicketState state;

		//! The progress of data acquisition.
		//! Not all command functions provide information about the progress, see the function documentation.
		iViewTicketProgress progress;

		//! A return code to indicate the result of the associated operation. Defaults to #RC_NO_ERROR.
		//! A value different to #RC_NO_ERROR indicates that 'result' points to an iViewServerError data structure
		//! if result != NULL. Otherwise, 'result' points to a iViewResult data structure.
		iViewRC returnCode;

		//! Pointer to the data returned by iView. Cast to iViewResult* if the return code indicates that the operation
		//! succeeded. Otherwise, cast to iViewServerError* to find an explanation.
		iViewResult * result;

} iViewTicket;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------

/**
 * Retrieve the iViewNG library version numbers and write to provided structure.
 *
 * This function may be called any time, even before calling iView_Init().
 *
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in,out]	version			User-provided data structure to be filled.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_INVALID_PARAMETER 	if the parameter is invalid (e.g. does not point to an initialized datastructure)
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_GetLibraryVersion (iViewVersion * version);

/**
 * Initialize the iViewNG C API.
 *
 * This function sets up the iView C API and puts it into the state where it may try to connect to the iViewNG
 * server. It is safe but useless to try to initialize the API more than one time. If the API is no longer
 * needed, call iView_Shutdown().
 *
 * This function does not interact with the iViewNG server (if not connected to server).
 * This function does not produce asynchronous return values.
 *
 * @param[in]	sdkType				defines the communication scheme (iViewNG vs. iViewX)
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_INVALID_STATE		if the API was already initialized
 * @retval	#RC_NO_INIT				if there is an initialization error
 * @retval	#RC_INVALID_PARAMETER	if sdkType does not address a proper SDK type
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_Init (iViewSdkType sdkType);

/**
 * Deinitialize the API client and release the acquired memory. If still connected, the connection will be closed
 * in advance.
 *
 * This function does not interact with the iViewNG server (if not connected to server).
 * This function does not produce asynchronous return values.
 *
 * @pre		The iView C API was initialized with iView_Init().
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_Shutdown ();

/**
 * Start new iViewNG server with the provided command parameter set. The function blocks the caller until it is
 * safe to assume that the server is up and ready.
 *
 * This function does not produce asynchronous return values.
 *
 * @param[in]		path			reserved for future use (use NULL pointer)
 * @param[in]		parameters		reserved for future use (use NULL pointer)
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_INVALID_STATE		if an iViewNG server instance is already running (deprecated)
 * @retval	#RC_OPERATION_FAILED	if the server could not be started
 * @retval	#RC_UNSPECIFIED			on any other error
 *
 * @test	CaseItgI028710
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_StartServer (wchar_t const * const path, wchar_t const * const parameters);

/**
 * Terminate the iView server and release the acquired memory. This does not deinitialize the C API. Call
 * iView_Shutdown() explicitly afterwards.
 *
 * This function does not produce asynchronous return values.
 *
 * @pre		The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @param[in]		timeout			Do not wait for shutdown for more than this timeout [milliseconds].
 * 									It is recommended to wait for not less than 15 seconds.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if the ticket data structure is invalid / corrupted
 * @retval	#RC_OPERATION_FAILED	if the server process did not terminate in time
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_ShutdownServer (const uint32_t timeout);

/**
 * Register the user callback function. The user callback function will be called asynchronously
 * to provide any data to the user. A proper callback function is required for receiving data asynchronously.
 *
 * It is possible to set the callback to NULL to indicate that no asynchronous results shall be delivered anymore
 * and that all results shall be dismissed immediately.
 *
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in]		callback		Pointer to the callback function. Use NULL to indicate that no results shall
 * 									be delivered anymore and that all results shall be dismissed immediately.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_UNSPECIFIED			on any other error
 */
typedef void (*IVIEW_CALLBACK) (iViewTicket * const);

//DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SetCallback (DLLCALL void (*callback) (iViewTicket * const));
DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SetCallback (DLLCALL IVIEW_CALLBACK);

/**
 * Retrieve the address of the registered user callback function.
 *
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @return	Pointer to the callback function registered with iView_SetCallback(), or NULL if no callback function
 * 			was registered.
 */

DLL_DEC_PRE IVIEW_CALLBACK DLL_DEC_POST iView_GetCallback ();

/**
 * Dynamically generate a ticket data structure.
 *
 * iView uses the ticket structure to identify a request, to indicate the state of processing the request and to
 * provide the result data.  Use iView_ReleaseTicket() to release the data structure and its contents. Do not release
 * any contained data before calling iView_ReleaseTicket()!
 *
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @param[in,out]	ticket			Pointer to the iViewTicket data structure to initialize.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_INVALID_PARAMETER	if ticket is NULL or if the contents of ticket is not NULL
 * @retval	#RC_OUT_OF_MEMORY		if the data structure could not be allocated
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_CreateTicket (iViewTicket ** const ticket);

/**
 * Recursively free the ticket data structure, including the result. All referred data allocated by iViewNG
 * API is released. The ticket pointer will be set to NULL.
 * Before releasing memory, the structure contents will be set to zero.
 *
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a request, to indicate the state of processing the request
 * 									and to provide the result data. The user is responsible for not releasing this
 * 									memory unless iView_ReleaseTicket() is called to release the result data structure.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_OPERATION_FAILED	if ticket could not be released
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_ReleaseTicket (iViewTicket ** const ticket);

/**
 * Recursively free the ticket result. All referred data allocated by iViewNG API is released.
 *
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a request, to indicate the state of processing the request
 * 									and to provide the result data. The user is responsible for not releasing this
 * 									memory unless iView_ReleaseTicket() is called to release the result data structure.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid or contains no result
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_ReleaseResult (iViewTicket * const ticket);

/**
 * Provide user license key to iView NG. The license key is used to check the availability of functionality. It is
 * possible to provide more than one license key, all license keys will be stored. If there is no connection to the
 * server, the license(s) will be transferred implicitly when connecting to the server.
 *
 * @pre								The iView C API was initialized with iView_Init().
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
 * 										- iViewTicket.result is NULL.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 * 									- iViewTicket.progress is 100.
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED since there will be no subsequent
 * 									  call with this ticket. The ticket should be released.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @param[in]		license			The license string as provided to the user.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_PARAMETER	if any parameter is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 *
 * @test	CaseApiU011750
 * @test	CaseItgI028700
 *
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_AddLicenseKey (iViewTicket * const ticket, char const * const license);

/**
 * Retrieve the current time from the iViewNG server.
 * Use this function for user-level synchronization of server and client application time scale.
 * This is the time from the start of server process.
 *
 * Will try to offset the network latency.
 *
 * The ticket progress will be set to 100 when the request was processed by the server.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
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
 * 										- iViewTicket.result is a pointer to iViewResult,
 *   - iViewResult.type is #IVIEWRESULT_GET_SERVER_TIME,
 * 										- iViewResult.data is a pointer to iViewTime,
 * 										- iViewResult.next is NULL since there will be no subsequent results.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 * 									- iViewTicket.progress is 100.
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED since there will be no subsequent
 * 									  call with this ticket. The ticket should be released.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 *
 * @test	CaseItgI028700
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_GetServerTime (iViewTicket * const ticket);

/**
 * Retrieve the current time from the local host.
 * Use this function for fetching the local time scale and for user-level synchronization of server and client
 * application time scale.
 * This is the time from the start of API user process 
 * (or at least the time when iViewNG-SDK shared library was loaded).
 *
 * Convenience function.
 * This function does not interact with the iViewNG server.
 * This function does not produce asynchronous return values.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * This function returns synchronously and provides the result of the operation.
 * Use the ticket to retrieve the result.
 * On return, the ticket's elements are set:
 * 									- On success:
 *   - iViewTicket.returnCode is #RC_NO_ERROR,
 * 										- iViewTicket.result is a pointer to iViewResult,
 *   - iViewResult.type is #IVIEWRESULT_GET_LOCAL_TIME,
 * 										- iViewResult.data is a pointer to iViewTime,
 * 										- iViewResult.next is NULL since there will be no subsequent results.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 * 									- iViewTicket.progress is 100.
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED since there will be no subsequent
 * 									  call with this ticket. The ticket should be released.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 								structure to identify a request, to indicate the state of processing the request
 * 								and to provide the result data. The user is responsible for not releasing this
 * 								memory unless iView_ReleaseTicket() is called to release the result data structure.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 *
 * @test	CaseItgI028700
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_GetLocalTime (iViewTicket * const ticket);

// --------------------------------------------------------------------------------------------------------------------

#endif
