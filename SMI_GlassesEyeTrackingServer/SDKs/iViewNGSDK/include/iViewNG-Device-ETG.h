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
#ifndef _IVIEWNG_DEVICE_ETG_H
#define _IVIEWNG_DEVICE_ETG_H

/**
 * \file iViewNG-Device-ETG.h
 *
 * This file declares the iViewNG API functions and data types which are specific to the iView ETG
 * eye tracker.
 *
 * The ETG currently supports the following eye camera modes:
 *
 * Camera operation 			    | Eye camera resolution 	   | Eye camera sample rate
 * -------------------------------- | ---------------------------- | -------------------------------
 * #IVIEWTRACKINGMODE_ETG_BINOCULAR	| #IVIEWRESOLUTION_ETG_640x480 | #IVIEWSAMPLERATE_ETG_30
 * #IVIEWTRACKINGMODE_ETG_BINOCULAR	| #IVIEWRESOLUTION_ETG_320x240 | #IVIEWSAMPLERATE_ETG_30
 * #IVIEWTRACKINGMODE_ETG_BINOCULAR	| #IVIEWRESOLUTION_ETG_320x240 | #IVIEWSAMPLERATE_ETG_60 (*)
 *
 * (*)	60Hz mode requires an ETG-2 device!
 *
 * The ETG currently supports the following scene camera modes:
 *
 * Scene camera resolution 			| Scene camera sample rate
 * -------------------------------- | ---------------------------
 * #IVIEWRESOLUTION_ETG_1280x960	| #IVIEWSAMPLERATE_ETG_24
 * #IVIEWRESOLUTION_ETG_960x720		| #IVIEWSAMPLERATE_ETG_30 (*)
 *
 * (*)	30 Hz mode requires an ETG-2 device!
 */

// --------------------------------------------------------------------------------------------------------------------

#include <iViewNG-Device.h>
#include <iViewNG-DataStreams.h>
#include <iViewNG-Calibration.h>

//!	Identifier for encoding the UI status of a client
typedef uint32_t iViewStatusType;

//! Identifier for different device types / operation modes.
static const iViewDeviceType IVIEWDEVICE_ETG = 0x00010000; 					//!< bit mask for all iView ETG device modes
static const iViewDeviceType IVIEWDEVICE_ETG_CAMERAPLAYBACK = 0x00010010;	//!< use ETG Controller & iViewNG Server
static const iViewDeviceType IVIEWDEVICE_ETG_BOOT = 0x00EB0000;				//!< to detect iViewETG single BootLoader Driver for SMI CPLD CypressFirmware Upload

//! Identifier for different eye camera operation modes.
static const iViewTrackingMode IVIEWTRACKINGMODE_ETG_MONOCULAR_LEFT = 0x01;			//!< use only left eye camera for tracking, not yet supported
static const iViewTrackingMode IVIEWTRACKINGMODE_ETG_MONOCULAR_RIGHT = 0x02;		//!< use only right eye camera for tracking, not yet supported
static const iViewTrackingMode IVIEWTRACKINGMODE_ETG_BINOCULAR = 0x04;				//!< use both eye cameras for tracking

//! Identifier for different calibration types.
static const iViewCalibrationType IVIEWCALIBRATIONTYPE_ETG_1POINT = 0x02;			//!< not yet supported
static const iViewCalibrationType IVIEWCALIBRATIONTYPE_ETG_1POINTAVERAGING = 0x04;	//!< not yet supported
static const iViewCalibrationType IVIEWCALIBRATIONTYPE_ETG_1POINTSPHERICAL = 0x08;	//!< Standard 1-pt calibration.
static const iViewCalibrationType IVIEWCALIBRATIONTYPE_ETG_2POINT = 0x10;			//!< not yet supported
static const iViewCalibrationType IVIEWCALIBRATIONTYPE_ETG_3POINT = 0x20;			//!< Standard 3-pt calibration.
static const iViewCalibrationType IVIEWCALIBRATIONTYPE_ETG_5POINT = 0x40;			//!< not yet supported

//! Identifier for different camera resolutions.
static const iViewCameraResolution IVIEWRESOLUTION_ETG_320x240 = 0x01;		//!< camera resolution of 320 pixel horizontal, 240 pixel vertical
static const iViewCameraResolution IVIEWRESOLUTION_ETG_400x300 = 0x02;		//!< camera resolution of 400 pixel horizontal, 300 pixel vertical
static const iViewCameraResolution IVIEWRESOLUTION_ETG_640x480 = 0x04;		//!< camera resolution of 640 pixel horizontal, 480 pixel vertical
static const iViewCameraResolution IVIEWRESOLUTION_ETG_960x720 = 0x08;		//!< camera resolution of 980 pixel horizontal, 720 pixel vertical
static const iViewCameraResolution IVIEWRESOLUTION_ETG_1280x720 = 0x10;		//!< camera resolution of 1280 pixel horizontal, 720 pixel vertical
static const iViewCameraResolution IVIEWRESOLUTION_ETG_1280x960 = 0x20;		//!< camera resolution of 1280 pixel horizontal, 960 pixel vertical

//! Identifier for different sampling rates.
static const iViewSamplingRate IVIEWSAMPLERATE_ETG_24 = 0x01;		//!< 24 samples per second
static const iViewSamplingRate IVIEWSAMPLERATE_ETG_30 = 0x02;		//!< 30 samples per second
static const iViewSamplingRate IVIEWSAMPLERATE_ETG_60 = 0x04;		//!< 60 samples per second
static const iViewSamplingRate IVIEWSAMPLERATE_ETG_120 = 0x08;		//!< 120 samples per second
static const iViewSamplingRate IVIEWSAMPLERATE_ETG_240 = 0x10;		//!< 240 samples per second

//! Do not change the current setting.
const iViewWhiteBalanceProgram IVIEWWHITEBALANCE_ETG_AUTO = 0x01; 			//!< Automatically adjust white balance setting.
const iViewWhiteBalanceProgram IVIEWWHITEBALANCE_ETG_DAYLIGHT = 0x02; 		//!< For using outside, clear days.
const iViewWhiteBalanceProgram IVIEWWHITEBALANCE_ETG_BASIC = 0x04; 			//!< For using outside, cloudy or in shade.
const iViewWhiteBalanceProgram IVIEWWHITEBALANCE_ETG_TUNGSTEN = 0x08; 		//!< For using in tungsten lightning.
const iViewWhiteBalanceProgram IVIEWWHITEBALANCE_ETG_FLUORESCENT = 0x10; 	//!< For using in white fluorescent lightning.
const iViewWhiteBalanceProgram IVIEWWHITEBALANCE_ETG_FLUORESCENT_H = 0x20; 	//!< For using daylight fluorescent lightning.


//!	Identifier for encoding the status of a license
typedef int32_t iViewLicenseStatusType;
const iViewLicenseStatusType IVIEW_LICENSE_INVALID_MANIPULATED = -0x03;		//!< Time restricted license expired due to manipulation
const iViewLicenseStatusType IVIEW_LICENSE_INVALID_EXPIRED = -0x02;			//!< Time restricted license expired
const iViewLicenseStatusType IVIEW_LICENSE_INVALID = -0x01;					//!< license invalid for any reason
const iViewLicenseStatusType IVIEW_LICENSE_UNKNOWN = 0x0; 					//!< Unknown license status
const iViewLicenseStatusType IVIEW_LICENSE_VALID_UNLIMITED = 0x01; 			//!< license valid without restriction
const iViewLicenseStatusType IVIEW_LICENSE_VALID_TIMERESTRICTED = 0x02; 	//!< Time restricted license still valid


//!	Identifier for encoding the product edition of ETG
typedef uint32_t iViewLicenseProductEditionEtg;
const iViewLicenseProductEditionEtg IVIEW_PRODUCT_EDITIONS_ETG_UNKNOWN = 0; 			//!<
const iViewLicenseProductEditionEtg IVIEW_PRODUCT_EDITIONS_ETG_SE_18 = 1; 				//!<
const iViewLicenseProductEditionEtg IVIEW_PRODUCT_EDITIONS_ETG_SE_19_20 = 2; 			//!<
const iViewLicenseProductEditionEtg IVIEW_PRODUCT_EDITIONS_ETG_21_OBSERVER = 3; 		//!<
const iViewLicenseProductEditionEtg IVIEW_PRODUCT_EDITIONS_ETG_21_WIRELESS_OBSERVER = 4;//!<
const iViewLicenseProductEditionEtg IVIEW_PRODUCT_EDITIONS_ETG_21_ANALYSIS = 5; 		//!<
const iViewLicenseProductEditionEtg IVIEW_PRODUCT_EDITIONS_ETG_21_APPLICATION = 6;		//!<
const iViewLicenseProductEditionEtg IVIEW_PRODUCT_EDITIONS_ETG_120_ANALYSIS = 7; 		//!<
const iViewLicenseProductEditionEtg IVIEW_NUMBER_OF_PRODUCT_EDITIONS_ETG = 8;			//!<



// --------------------------------------------------------------------------------------------------------------------

//! The maximal core count supported by server status.
static const int SERVERSTATUS_MAX_CORECOUNT = 64;

// --------------------------------------------------------------------------------------------------------------------

//! The maximal number of characters of a serial number, including '\0'.
static const unsigned int DRIVER_ETG_EYECAM_LENGTH = 32;

/**
 * Structure to describe the version of the ETG device components.
 */
typedef struct {

		//! Scene camera software version, format YYYYMMDD or 0 if unknown.
		uint32_t softwareVersionSceneCam;

		//! Eye camera firmware version number or 0 if unknown.
		uint32_t firmwareVersionEyeCam;

		//! Eye camera driver version or empty if unknown.
		wchar_t driverVersionEyeCam[DRIVER_ETG_EYECAM_LENGTH];

} iViewDeviceVersionEtg;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Structure which describes an ETG license.
 */
typedef struct {

	iViewLicense * next; //!< Pointer to another license structure, if there are multiple licenses.

	iViewLicenseStatusType licenseStatus;

	int32_t daysUntilExpiration;

	iViewLicenseProductEditionEtg productEdition;

} iViewLicenseEtg;

// --------------------------------------------------------------------------------------------------------------------

//! The maximal number of characters of a serial number, including '\0'.
static const unsigned int SERIAL_NUMBER_ETG_LENGTH = 32;

/**
 * Structure which describes an ETG serial number.
 */
typedef struct {

		wchar_t chassis[SERIAL_NUMBER_ETG_LENGTH];

} iViewSerialNumberEtg;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Structure to represent the recording device for an ETG system.
 */
typedef struct {

		//! The remaining battery lifetime in number of minutes.
		//! A value of INT_MAX indicates that the recording device does not operate on battery but on AC power.
		//! A value of -1 indicates that the battery state could not be determined.
		int32_t batteryLifetimeMinutes;

		//! The remaining battery lifetime in percent.
		//! A value of -1 indicates that the battery state could not be determined.
		int32_t batteryLifetimePercent;

		//! The currently free storage space on recording unit data tank, given in recording minutes. This
		//! describes the device which contains the data repository. A value of UINT_MAX indicates that the capacity
		//! exceeds UINT_MAX-1 minutes by an unknown size.
		uint32_t freeStorageMinutes;

		//! The status of the scene camera. If 1, the camera is operational and may be started. If 0, the camera
		//! is not (yet) ready.
		int32_t sceneCameraAvailable;

		//! The status of the eye camera. If 1, the camera is operational and may be started. If 0, the camera
		//! is not (yet) ready.
		int32_t eyeCameraAvailable;

		//! The current system wide cpu usage over all cpu cores (the load of the computer).
		//! In [0,100]. Can be -1 on error.
		double systemWideCpuUsage;

		//! The length of the recording in progress or (if recording is not active) the length of the last
		//! acquired recording [in milliseconds].
		uint32_t recordingTime;

		//! Count of CPU cores.
		int32_t countOfCpuCores;

		//! The measured core frequencies in MHz, only countOfCpuCores is used.
		//! Sorted ascending (core1, core2, .., coreMAX)
		int32_t cpuCoreFrequencies[SERVERSTATUS_MAX_CORECOUNT];

		//! The measured core temperatures in Celsius, only countOfCpuCores is used.
		//! Sorted ascending (core1, core2, .., coreMAX)
		int32_t cpuCoreTemperatures[SERVERSTATUS_MAX_CORECOUNT];

		//! The measured acquisition frame rate of the left eye, in Hz.
		//! The value is refreshed roughly every 3,33s for ETG 30Hz.
		//! The value is refreshed roughly every 1,66s for ETG 60Hz.
		//! Is NaN if no acquisition is running.
		double measuredAcquisitionFpsOfLeftEye;

		//! The measured acquisition frame rate of the left eye, in Hz.
		//! The value is refreshed roughly every 3,33s for ETG 30Hz.
		//! The value is refreshed roughly every 1,66s for ETG 60Hz.
		//! Is NaN if no acquisition is running.
		double measuredAcquisitionFpsOfRightEye;

		//! Is true when scene camera is acquiring
		uint32_t sceneAcquiring;

		//! Is true when eye cameras are acquiring
		uint32_t eyeAcquiring;

		//! Is true when recording (e.g. betweeen start and stop recording calls).
		//! Then the recording duration is computed.
		uint32_t isRecording;

		//! The IP address of the sender of this control structure.
		//! Default is 0.0.0.0 (meaning there is no sender yet).
		wchar_t const * ipAddressOfSender;

		//! The state into which client gui state was switched into.
		iViewStatusType clientState;

		//! Specifies where the experiment repository directory is located.
		//! A repository typically contains a set of experiments, each represented
		//! by an own directory, which in turns contains many sub-directories,
		//! one for each participant and its recordings.
		wchar_t const * repositoryDirectory;

		//! Contains the XML content of the currently set experiment definition.
		wchar_t const * experimentDefinition;

		//! Contains the XML content of the currently set participant definition.
		wchar_t const * participantDefinition;

		//! Contains the XML content of the currently set annotation definition.
		wchar_t const * annotationDefinition;

		//! The operating system string. Currently windows or android.
		wchar_t const * operatingSystemString;

		//! The currently used white balance program of the scene camera (iViewWhiteBalanceProgram).
		int32_t whiteBalanceProgram;

		//! Reserved for future use.
		int32_t reserved1;

		//! Reserved for future use.
		int32_t reserved2;

		//! Reserved for future use.
		int32_t reserved3;

} iViewDataStreamEtgServerStatus;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Used in iViewDeviceParameters if iViewDeviceType is #IVIEWDEVICE_ETG_CAMERAPLAYBACK.
 * Includes the parameters for using realtime playback of an ETG device (aka Mii, Smiggles, GazeWear, HED5).
 * If all parameters are set to zero, the parameters will not be changed but remain in their current
 * state.
 *
 * The data may be released immediately after submitting the structure to iViewNG.
 *
 * @attention	Always initialize the structure with zero before using it!
 */
typedef struct {
		//! The directory for storing recorded files, NOT ending with the path separator.
		//! A value of NULL means 'no change' and orders the server to not change the last submitted path.
		wchar_t const * outputDirectory;

		//! The filename to append to outputDirectory, NOT including
		//! any suffix, NOT including any path separator.
		//!
		//! A value of NULL means 'no change' and orders the server to not change the last submitted path.
		wchar_t const * baseFilename;

		//! The operation mode(s) of the eye tracker.
		//!
		//! A value of #IVIEWTRACKINGMODE_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewTrackingMode operationMode;

		//! The resolution of the submitted scene images [pixels]
		//!
		//! A value of #IVIEWRESOLUTION_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewCameraResolution cameraResolutionScene;

		//! The resolution of the submitted eye images [pixels]
		//!
		//! A value of #IVIEWRESOLUTION_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewCameraResolution cameraResolutionEye;

		//! Average scene camera frame rate [frames per second]
		//!
		//! A value of #IVIEWSAMPLINGRATE_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewSamplingRate samplingRateSceneCam;

		//! Average scene camera frame rate [frames per second]
		//!
		//! A value of #IVIEWSAMPLINGRATE_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewSamplingRate samplingRateEyeCam;

		//! Scene camera autowhitebalance program.
		//!
		//! A value of #IVIEWWHITEBALANCE_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewWhiteBalanceProgram whiteBalanceProgram;

		//! Audio activation state.
		//!
		//! A value of #IVIEWTRACKINGMODE_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewAudioActivationState audioState;

		//! Gaze overlay mode.
		//!
		//! A value of #IVIEWGAZEOVERLAYMODE_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewGazeOverlayMode gazeOverlayMode;

		//! Eye camera exposure mode.
		//!
		//! A value of #IVIEWEYECAMEXPOSUREMODE_CURRENT
		//! means 'no change' and orders the server to not change the last submitted setting.
		iViewEyeCamExposureMode eyeCamExposureMode;

		//! @note not yet supported
		iViewOverlaySpecList * overlaySpecList;

} iViewDeviceParametersEtgCameraPlayback;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Data structure including all required parameters for calibrating an ETG device.
 */
typedef struct {

		//! The calibration distance [m] which will be used. A value of 0 indicates that the calibration
		//! distance is unknown / not used.
		double calibrationDistance;

} iViewCalibrationParametersEtg;

// --------------------------------------------------------------------------------------------------------------------

/**
 *
 * For manual calibration of the gaze, the graphical client is expected to submit the 'real' gaze position
 * in scene video coordinates. This means that at this frame, the subject was fixating the point (in 3D)
 * described by the submitted coordinates (in 2D).
 */


/**
 *
 * This function signals the server that the current application takes control. If another client previously controlled
 * the server, it will be informed that we are now the controlling client and switch into a passive status.
 *
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								ServerStatus should be subscribed
 *
 * @pre								the local server is already in recording screen
 *
 *
 * @post							When accepted and forwarded to the iViewNG server, the ticket progress will change
 * 									from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * 									#IVIEWTICKETSTATUS_ACCEPTED.
 *
 * The ticket progress will be set to 100% and the ticket state will be set to IVIEWTICKETSTATUS_CLOSED
 * when another client application took control
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
 * @retval  #RC_OPERATION_FAILED	the ServerStatus is not subscribed
 * @retval 	#RC_INVALID_STATE		the local client is not in recording screen
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_TakeControl (iViewTicket * const ticket);

// --------------------------------------------------------------------------------------------------------------------


/**
 *
 * This function is a wrapper to enable BeGaze compatible recordings with the SDK.
 *
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								ServerStatus should be subscribed
 *
 * @pre								the local server is already in recording screen
 *
 * @pre								our application gained control through iView_TakeControl
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a request, to indicate the state of processing the request
 * 									and to provide the result data. The user is responsible for not releasing this
 * 									memory unless iView_ReleaseTicket() is called to release the result data structure.
 *
 * @param[in] name					the name of the participant, used for the naming of the appropriate
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval  #RC_OPERATION_FAILED	the ServerStatus is not subscribed
 * @retval 	#RC_INVALID_STATE		the local client is not in recording screen
 * @retval	#RC_UNSPECIFIED			on any other error
 * @retval  #RC_NOT_IN_CONTROL		our application does not control the server
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SendParticipantName (iViewTicket * const, const wchar_t* name);

// --------------------------------------------------------------------------------------------------------------------


/**
 *
 * This function is a wrapper to enable BeGaze compatible recordings with the SDK without an ETG-client.
 *
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								ServerStatus should be subscribed
 *
 * @pre								the local server is already in recording screen
 *
 * @pre								our application gained control through iView_TakeControl
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a request, to indicate the state of processing the request
 * 									and to provide the result data. The user is responsible for not releasing this
 * 									memory unless iView_ReleaseTicket() is called to release the result data structure.
 *
 * @param[in] name					the name of the participant, used for the naming of the appropriate
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval  #RC_OPERATION_FAILED	the ServerStatus is not subscribed
 * @retval 	#RC_INVALID_STATE		the local client is not in recording screen
 * @retval	#RC_UNSPECIFIED			on any other error
 * @retval  #RC_NOT_IN_CONTROL		our application does not control the server
 */
DLL_DEC_PRE iViewRC DLL_DEC_POST iView_PrepareRecording (iViewTicket * const,
		const wchar_t* dataFolder, const wchar_t* experimentName, const wchar_t* participantName,
		iViewCameraResolution eyeResolution, iViewSamplingRate eyeSampleRate,
		iViewCameraResolution sceneResolution, iViewSamplingRate sceneSampleRate);

// --------------------------------------------------------------------------------------------------------------------

/**
 * This function gets the status of the currently used ETG license
 *
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a request, to indicate the state of processing the request
 * 									and to provide the result data. The user is responsible for not releasing this
 * 									memory unless iView_ReleaseTicket() is called to release the result data structure.
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
 * @retval  #RC_OPERATION_FAILED	the ServerStatus is not subscribed
 * @retval 	#RC_INVALID_STATE		the local client is not in recording screen
 * @retval	#RC_UNSPECIFIED			on any other error
 * @retval  #RC_NOT_IN_CONTROL		our application does not control the server
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_GetLicenseStatus (iViewTicket * const ticket);

// --------------------------------------------------------------------------------------------------------------------


#endif
