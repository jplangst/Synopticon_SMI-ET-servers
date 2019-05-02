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
#ifndef _IVIEWNG_CALIBRATION_H
#define _IVIEWNG_CALIBRATION_H

/**
 * \file iViewNG-Calibration.h
 *
 * This file declares the iViewNG API functions and data types which are used to realize the calibration
 * of an eye tracking device. This includes only the device-independent part. Device-specific definitions
 * are located in the according device header file.
 */

#include <iViewNG-Core.h>

// --------------------------------------------------------------------------------------------------------------------

//! Identifier for different calibration method (bit mask, may be combined to enumerate types).
typedef uint32_t iViewCalibrationType;

//! Invalid calibration scheme to detect unset data structure elements.
static const iViewCalibrationType IVIEWCALIBRATIONTYPE_INVALID = 0;

//! Default calibration scheme: no calibration. On some systems, this might mean "0-point-calibration".
static const iViewCalibrationType IVIEWCALIBRATIONTYPE_NONE = 1;

/**
 * When calling iView_SetupCalibration(), the user must provide information on the requested calibration type and
 * parameters.
 */
typedef struct {

		//! The calibration method to use.
		iViewCalibrationType type;

		//! Only meaningful for offline calibration:
		//! The scene frame index at which the calibration shall be applied. Use a value of -1 if the frame index is
		//! not known / not required and the server shall guess the proper value.
		int32_t applyAtFrame;

		//! Calibration scheme dependent data structure. Cast this according to the type, for details see the file
		//! iViewNG-Device-*.h.
		void * parameters;

} iViewCalibrationParameters;

// --------------------------------------------------------------------------------------------------------------------

//! The unit of the coordinates in a calibration data
typedef uint32_t iViewCalibrationUnit;
static const iViewCalibrationUnit IVIEWCALIBRATIONUNIT_INVALID = 0;		//!< do not use
static const iViewCalibrationUnit IVIEWCALIBRATIONUNIT_PIXEL = 1;		//!< Calibration coordinates are given in pixels.

//! The type of the calibration data; not all types are supported by all devices
typedef uint32_t iViewCalibrationDataType;
static const iViewCalibrationDataType IVIEWCALIBRATIONDATATYPE_INVALID = 0;
static const iViewCalibrationDataType IVIEWCALIBRATIONDATATYPE_1POINT = 1; //!< a fixed point

//! Identifier for different usages of a calibration point (bit mask, may be combined to enumerate types).
typedef uint32_t iViewCalibrationPointUsage;

//! Each calibration point has a unique id of this type.
typedef uint32_t iViewCalibrationPointId;

typedef struct {

		//! The type of the calibration data
		iViewCalibrationDataType type;

		//! The number of the scene frame (taken from SmiImage structure) at which the calibration was performed.
		//! If set to -1, the frame number is unknown and the server may make an educated guess on its own, e.g.
		//! use the frame number of the last acquired image.
		//!
		//! @see iView_SubscribeDataStream		Subscribing to #IVIEWDATASTREAM_SCENEIMAGES_WITH_GAZE or
		//!										#IVIEWDATASTREAM_SCENEIMAGES delivers iViewDataStreamSceneImage
		//!										structures which contain the frame number to specify here.
		//!
		//! If type is IVIEWCALIBRATIONDATATYPE_2POINT this value is ignored when passed to the Server.
		//! When returned, this value is the frame at which the calibration happened.
		int32_t sceneFrameNumber;

		//! The unit in which the coordinates are given.
		//! @note irrelevant for ETG
		iViewCalibrationUnit unit;

		//! If the usage is _INVALID and the id is valid this point should be removed from the calibration.
		//! @note irrelevant for ETG
		iViewCalibrationPointUsage usage;

		//! @note irrelevant for ETG
		iViewCalibrationPointId id;

		//!
		void * parameters;

} iViewCalibrationData;


typedef struct {
		//! The horizontal position of the mouse click, in image pixel size, zero is leftmost column.
		// ! A value of -1 is used to indicate that the coordinate is invalid / unset.
		int32_t coordinateX;

		//! The vertical position of the mouse click, in image pixel size, zero is topmost row
		// ! A value of -1 is used to indicate that the coordinate is invalid / unset.
		int32_t coordinateY;

} iViewCalibrationData1Point;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------

/**
 * Initialize calibration procedure but do not start it.
 *
 * Use this function to submit the calibration parameters to the iViewNG server. This function call specifies
 *
 * - which calibration type to use (required)
 * - and when to apply the calibration (optional, only meaningful for offline calibration).
 *
 * This function may be called at any time when a calibration procedure was not already started with
 * iView_StartCalibration() and not yet terminated by calling iView_SetCalibrationData() or
 * iView_CancelCalibration().
 *
 * The ticket progress will be set to 100 when the request was processed by the server.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								Eye tracking device is available (connected and recognized by iViewNG server).
 *
 * @pre								ET device license permits use of this function.
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * @post							This function returns synchronously and provides the result of the operation
 * 									to the user callback function. Use the ticket to associate the operation request
 * 									and the return of the result. On return, the ticket's elements are set:
 * 									- On success:
 * 										- iViewTicket.returnCode is #RC_NO_ERROR,
 * 										- iViewTicket.result is NULL.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 * 									- iViewTicket.progress is 100.
 * 									- iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED since there will be no subsequent
 * 									  call with this ticket. The ticket should be released.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 * @param[in]		setup			Pointer to parameter data structure.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if iView was not initialized by iView_Init()
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if the ticket data structure is invalid / corrupted
 * @retval	#RC_INVALID_PARAMETER	if any parameter is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SetupCalibration (iViewTicket * ticket,
        iViewCalibrationParameters const * const setup);

/**
 * Start the calibration procedure.
 *
 * Call this function to indicate that a calibration procedure is in progress. This means, the function
 * iView_SetCalibrationData() will be called soon after.
 *
 * When iView_StartCalibration() is called, the server might start to buffer calculated gaze samples. To save memory
 * and CPU time, please finish a started calibration procedure by providing the proper amount of calibration data
 * points (the number of required points depends on the calibration method submitted with iView_SetupCalibration())
 * via iView_SetCalibrationData() or by canceling the procedure via iView_CancelCalibration().
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								Eye tracking device is available (connected and recognized by iViewNG server).
 *
 * @pre								ET device license permits use of this function.
 *
 * @pre								The calibration parameters were supplied by iView_SetupCalibration().
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * @post							This function returns synchronously and provides the result of the operation
 * 									to the user callback function. Use the ticket to associate the operation request
 * 									and the return of the result. On return, the ticket's elements are set:
 * 									- On success:
 * 										- iViewTicket.returnCode is #RC_NO_ERROR,
 * 										- iViewTicket.result is NULL.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 * 									- iViewTicket.progress is 100.
 * 									- iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED since there will be no subsequent
 * 									  call with this ticket. The ticket should be released.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @retval	#RC_NO_ERROR				on success
 * @retval	#RC_NO_INIT				if iView was not initialized by iView_Init()
 * @retval	#RC_INVALID_STATE		if iView_SetupCalibration() was not called in advance
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if the ticket data structure is invalid / corrupted
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_StartCalibration (iViewTicket * ticket);

/**
 * Send calibration data for a single calibration point to the server.
 *
 * The proper number of calibration data sets depends on the calibration type requested by iView_SetupCalibration().
 * A 1-point-calibration requires one data set, a 3-point-calibration means the calibration is finished after
 * calling iView_SetCalibrationData() three times etc. The client application must assert that either the proper
 * number of calibration data structures is delivered by iView_SetCalibrationData() or the calibration procedure
 * is canceled by iView_CancelCalibration().
 *
 * Providing calibration data for a multi-point-calibration may have an immediate impact on the calculated gaze
 * vector, even if the proper number of calibration data sets was not yet provided. The iView server may apply a
 * calibration scheme which optimizes the gaze vector with the amount of calibration data structures given. Subsequent
 * calls of iView_SetCalibrationData() will then further improve the calibration impact.
 *
 * Sending invalid data may cancel a calibration procedure and acknowledge the failure to the API user.
 *
 *
 * @pre								The iView C API was initialized with iView_Init().
 * @pre								Connection to a server process was established with iView_Connect().
 * @pre								The calibration parameters were supplied by iView_SetupCalibration().
 * @pre								The calibration procedure was started by calling iView_StartCalibration().
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * @post							This function returns synchronously and provides the result of the operation
 * 									to the user callback function. Use the ticket to associate the operation request
 * 									and the return of the result. On return, the ticket's elements are set:
 * 									- On success:
 * 										- iViewTicket.returnCode is #RC_NO_ERROR,
 * 										- iViewTicket.result is NULL.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 * 									- iViewTicket.progress is 100.
 * 									- iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED since there will be no subsequent
 * 									  call with this ticket. The ticket should be released.
 *
 * @post							The successful delivery of calibration data will set the ticket associated with
 * 									iView_StartCalibration() into the state #IVIEWTICKETSTATUS_CLOSED and its return
 * 									code to #RC_NO_ERROR.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 * @param[in]		data			Pointer to the parameter datastructure.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if iView was not initialized by iView_Init()
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if the ticket data structure is invalid / corrupted
 * @retval	#RC_INVALID_PARAMETER	if any parameter is invalid
 * @retval	#RC_INVALID_STATE		if iView_SetupCalibration() was not called in advance
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SetCalibrationData (iViewTicket * ticket,
		iViewCalibrationData const * const data);

/**
 * Cancel an active calibration procedure.
 *
 * The ticket progress will be set to 100 when the request was processed by the server.
 *
 * @pre								The iView C API was initialized with iView_Init().
 * @pre								Connection to a server process was established with iView_Connect().
 * @pre								The calibration parameters were supplied by iView_SetupCalibration().
 * @pre								The calibration procedure was started by calling iView_StartCalibration().
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * @post							This function returns synchronously and provides the result of the operation
 * 									to the user callback function. Use the ticket to associate the operation request
 * 									and the return of the result. On return, the ticket's elements are set:
 * 									- On success:
 * 										- iViewTicket.returnCode is #RC_NO_ERROR,
 * 										- iViewTicket.result is NULL.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 * 									- iViewTicket.progress is 100.
 * 									- iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED since there will be no subsequent
 * 									  call with this ticket. The ticket should be released.
 *
 * @post							The successful delivery of a cancel request will set the ticket associated with
 * 									iView_StartCalibration() into the state #IVIEWTICKETSTATUS_CLOSED and its return
 * 									code to #RC_OPERATION_FAILED.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @retval	#RC_NO_ERROR				on success
 * @retval	#RC_NO_INIT				if iView was not initialized by iView_Init()
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if the ticket data structure is invalid / corrupted
 * @retval	#RC_INVALID_STATE		if iView_SetupCalibration() was not called in advance
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_CancelCalibration (iViewTicket * ticket);

#endif

