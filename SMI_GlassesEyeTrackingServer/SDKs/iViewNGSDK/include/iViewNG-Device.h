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
#ifndef _IVIEWNG_DEVICE_H
#define _IVIEWNG_DEVICE_H

/**
 * \file iViewNG-Device.h
 *
 * This file declares the iViewNG API functions and data types which are required for handling eye tracking
 * device parameters. This includes only the device-independent part. Device-specific definitions
 * are located in the according device header file.
 */

#include <iViewNG-Core.h>

// --------------------------------------------------------------------------------------------------------------------

//! The iViewDevice structure contains an enumeration attribute which indicates which device is used. See
//! iViewNG-Device-*.h for the proper setting.
typedef uint32_t iViewDeviceType;

static const iViewDeviceType IVIEWDEVICE_INVALID = 0x00; 	//!< Invalid setting.
static const iViewDeviceType IVIEWDEVICE_UNKNOWN = 0x01; 	//!< Unknown device.

// --------------------------------------------------------------------------------------------------------------------

/**
 * Datatype to encode server capabilities. Device-dependent.
 */
//typedef unsigned char iViewDeviceCapabilities;

/**
 * The operation mode(s) of the eye tracker.
 */
typedef uint32_t iViewTrackingMode;

//! Do not change the tracking mode / tracking mode unspecified.
static const iViewTrackingMode IVIEWTRACKINGMODE_CURRENT = 0;

/**
 * The resolution of the submitted images.
 */
typedef uint32_t iViewCameraResolution;

//! Do not change the resolution / resolution unspecified.
static const iViewCameraResolution IVIEWRESOLUTION_CURRENT = 0;

/**
 * Average sampling rate.
 */
typedef uint32_t iViewSamplingRate;

//! Do not change the sampling rate / sampling rate unspecified.
static const iViewSamplingRate IVIEWSAMPLINGRATE_CURRENT = 0;

/**
 * Scene camera autowhitebalance program.
 */
typedef uint32_t iViewWhiteBalanceProgram;

//! Do not change the sampling rate / sampling rate unspecified.
const iViewWhiteBalanceProgram IVIEWWHITEBALANCE_CURRENT = 0;

/**
 * Audio activation state.
 */
typedef uint32_t iViewAudioActivationState;

static const iViewAudioActivationState IVIEWAUDIOSTATE_CURRENT = 0; 	//!< Neutral setting. Leave the audio activation
																		//!< state as it is.
static const iViewAudioActivationState IVIEWAUDIOSTATE_ACTIVATED = 1; 	//!< Use audio.
static const iViewAudioActivationState IVIEWAUDIOSTATE_DEACTIVATED = 2; //!< Do not use audio.

/**
 * Gaze overlay mode.
 */
typedef uint32_t iViewGazeOverlayMode;

static const iViewGazeOverlayMode IVIEWGAZEOVERLAYMODE_CURRENT = 0; 	//!< Neutral setting. Do not change gaze overlay mode
static const iViewGazeOverlayMode IVIEWGAZEOVERLAYMODE_ACTIVATED = 1; 	//!< Use gaze overlay video.
static const iViewGazeOverlayMode IVIEWGAZEOVERLAYMODE_DEACTIVATED = 2; 	//!< Do not use gaze overlay video.

/**
 * Eye camera exposure mode.
 */
typedef uint32_t iViewEyeCamExposureMode;

static const iViewEyeCamExposureMode IVIEWEYECAMEXPOSUREMODE_CURRENT = 0; 	//!< Neutral setting. Do not change exposure mode
static const iViewEyeCamExposureMode IVIEWEYECAMEXPOSUREMODE_NORMAL = 1; 	//!< Use normal eye camera exposure mode.
static const iViewEyeCamExposureMode IVIEWEYECAMEXPOSUREMODE_ICA = 2; 		//!< Use eye camera exposure mode optimized for ICA.


// --------------------------------------------------------------------------------------------------------------------

/**
 * Generic structure of a license. Cast pointers to this structure to the device-dependent structure, e.g.
 * iViewLicenseEtg to gain access to the device-dependent attributes.
 */
typedef struct __iViewLicenseStruct iViewLicense;
struct __iViewLicenseStruct {
		iViewLicense * next; //!< Pointer to another license structure, if there are multiple licenses.
};

/**
 * Generic structure of a serial number. Cast pointers to this structure to the device-dependent structure, e.g.
 * iViewLicenseEtg to gain access to the device-dependent attributes.
 */
//typedef struct __iViewSerialNumberStruct iViewSerialNumber;
//struct __iViewSerialNumberStruct {
//};

/**
 * Using iView_GetDeviceInfo(), the client application may inquire which devices are attached to a certain iViewNG
 * server process. This data structure is returned asynchronously as answer. There may be more than just one device.
 * In this case, the devices are linked via the 'next' element.
 */
typedef struct __iViewDeviceInfoStruct iViewDeviceInfo;
struct __iViewDeviceInfoStruct {

		iViewDeviceType type; 				//!< Specifies the attached device type.
		iViewDeviceInfo * next; 			//!< If there is another device available, this element points to the next
		                        			//!< iViewDeviceInfo data structure.
		void * version; 					//!< Device-dependent version structure.
											//!< Cast based on 'type' element, e.g. to iViewDeviceVersionEtg.
		void * capabilities; 				//!< Device-dependent capability structure.
											//!< Cast based on 'type' element, e.g. to iViewDeviceCapabilityEtg.
											//!< Not yet used.
		iViewLicense * license;				//!< Device-dependent license structure for this device.
											//!< Cast based on 'type' element, e.g. to iViewLicenseEtg.
											//!< Not yet used.
		void * serialNumber; 				//!< Cast based on 'type' element, e.g. to iViewSerialNumberEtg.
};

// --------------------------------------------------------------------------------------------------------------------

/**
 * Using iView_SetParameters(), the client application may define parameters for data recording. The available
 * parameters depend on the server configuration and the iViewDeviceType. Use the iViewDeviceType to determine how to
 * cast the void pointer.
 */
typedef struct {

		iViewDeviceType deviceType;	//!< Defines the attached device type.
		void * parameters; 			//!< The result data. Use the iViewDeviceType to determine how to cast.

} iViewDeviceParameters;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Set operation mode to use the DeviceType specified in DeviceParameters and provide device parameters to the
 * iViewNG server.
 *
 * Depending on the device type, a iViewResult may be submitted synchronously or asynchronously.
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
 * This function produces asynchronously delivered iViewResult data and calls the user callback function to provide
 * the result of the operation through the ticket:
 * - if iViewTicket.returnCode is #RC_NO_ERROR:
 * 										- iViewTicket.result is NULL
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED
 * 									- if iViewTicket.returnCode indicated an error:
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError
 * 										- iViewTicket.state remains at the former state
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a request, to indicate the state of processing the request
 * 									and to provide the result data. The user is responsible for not releasing this
 * 									memory unless iView_ReleaseTicket() is called to release the result data structure.
 * @param[in]		params			The parameter set, depending on the device. The data structure must be allocated
 * 									by the user and may be released immediately after calling this function.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_PARAMETER	if any parameter is invalid
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SetDeviceParameters (iViewTicket * const ticket,
        iViewDeviceParameters const * const params);

/**
 * Get the operation mode which is currently in use by the server.
 *
 * @warning Not yet implemented.
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
 * This function produces asynchronously delivered iViewResult data and calls the
 * user callback function to provide the result of the operation through the ticket:
 *
 * - if iViewTicket.returnCode is #RC_NO_ERROR:
 * 										- iViewTicket.result is NULL
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED
 * 									- if iViewTicket.returnCode indicated an error:
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError
 * 										- iViewTicket.state remains at the former state
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a request, to indicate the state of processing the request
 * 									and to provide the result data. The user is responsible for not releasing this
 * 									memory unless iView_ReleaseTicket() is called to release the result data structure.
 *
 * @retval	#RC_INVALID_REQUEST		always
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_GetDeviceParameters (iViewTicket * ticket);

/**
 * Query the iViewDeviceInfo structure.
 *
 * This function produces an asynchronously delivered iViewResult data structure of the type iViewDeviceInfo.
 *
 * If there is more than one device available, this data structure enumerates all available devices. If there are
 * more than one device available, the result may be returned via the same ticket subsequently, each call containing
 * one or more new iViewDeviceInfo structures for new device(s). Devices which are recognized after calling
 * iView_GetDeviceInfo() will not be returned as soon as they appear.
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
 * The ticket progress will be set to 100% when the request was processed by the server.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a request, to indicate the state of processing the request
 * 									and to provide the result data. The user is responsible for not releasing this
 * 									memory unless iView_ReleaseTicket() is called to release the result data structure.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_GetDeviceInfo (iViewTicket * const ticket);

// --------------------------------------------------------------------------------------------------------------------

#endif
