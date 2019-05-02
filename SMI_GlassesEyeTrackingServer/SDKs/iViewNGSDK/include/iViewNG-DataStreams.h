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
#ifndef _IVIEWNG_DATA_STREAMS_H
#define _IVIEWNG_DATA_STREAMS_H

/**
 * \file iViewNG-DataStreams.h
 *
 * This file declares the iViewNG API functions and data types which are required for handling data
 * streams. Please refer to SDK documentation sections @ref sec_synchronous and @ref tutorial_c_data_acquisition
 * the for details.
 */

#include <iViewNG-Core.h>

// --------------------------------------------------------------------------------------------------------------------

/* **************************************************************************************** */
/* **************************************************************************************** */
/* **************************************************************************************** */

static const int32_t IVIEW_DEPTH_1U = 1;							//!< constant for iViewImage.depth
static const int32_t IVIEW_DEPTH_8U = 8;							//!< constant for iViewImage.depth
static const int32_t IVIEW_DEPTH_16U = 16;							//!< constant for iViewImage.depth
static const int32_t IVIEW_DEPTH_32F = 32;							//!< constant for iViewImage.depth
static const int32_t IVIEW_DEPTH_SIGN = 0x80000000;					//!< constant for iViewImage.depth
static const int32_t IVIEW_DEPTH_8S = (IVIEW_DEPTH_SIGN | 8);		//!< constant for iViewImage.depth
static const int32_t IVIEW_DEPTH_16S = (IVIEW_DEPTH_SIGN | 16);		//!< constant for iViewImage.depth
static const int32_t IVIEW_DEPTH_32S = (IVIEW_DEPTH_SIGN | 32);		//!< constant for iViewImage.depth

/**
 * The image structure iViewImage is used by iViewNG for providing non-compressed
 * images. This structure is supposed to be compatible with the OpenCV image structure so
 * that an iViewImage pointer can be cast to an IplImage pointer. OpenCV is published under the License Agreement
 * For Open Source Computer Vision Library:
 *
 *  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
 *
 *  By downloading, copying, installing or using the software you agree to this license.
 *  If you do not agree to this license, do not download, install,
 *  copy or use the software.
 *
 *                          License Agreement
 *                For Open Source Computer Vision Library
 *
 * Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
 * Copyright (C) 2009, Willow Garage Inc., all rights reserved.
 * Third party copyrights are property of their respective owners.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   * Redistribution's of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   * Redistribution's in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * The name of the copyright holders may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * This software is provided by the copyright holders and contributors "as is" and
 * any express or implied warranties, including, but not limited to, the implied
 * warranties of merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the Intel Corporation or contributors be liable for any direct,
 * indirect, incidental, special, exemplary, or consequential damages
 * (including, but not limited to, procurement of substitute goods or services;
 * loss of use, data, or profits; or business interruption) however caused
 * and on any theory of liability, whether in contract, strict liability,
 * or tort (including negligence or otherwise) arising in any way out of
 * the use of this software, even if advised of the possibility of such damage.
 */
typedef struct {

		int32_t dataSize;          		//!< sizeof(iViewImage)
		int32_t id;                		//!< version (=0)
		int32_t numberOfChannels;  		//!< number of colour channels
		int32_t alphaChannel;      		//!< not used
		int32_t depth; 					//!< pixel depth in bits: #IVIEW_DEPTH_8U, #IVIEW_DEPTH_8S, #IVIEW_DEPTH_16S,
		                                //!< #IVIEW_DEPTH_32S,#IVIEW_DEPTH_32F and are supported
		int8_t colorModel [4]; 			//!< not used
		int8_t channelSeq [4]; 			//!< not used
		int32_t dataOrder; 				//!< 0 - interleaved color channels, 1 - separate color channels
		int32_t origin; 				//!< 0 - top-left origin, 1 - bottom-left origin (Windows bitmaps style)
		int32_t align; 					//!< not used
		int32_t width; 					//!< image width in pixels
		int32_t height; 				//!< image height in pixels
		void * roi; 					//!< not used, always NULL
		void * maskROI;					//!< not used, always NULL
		void * imageId; 				//!< not used, always NULL
		void * tileInfo;				//!< not used, always NULL
		int32_t imageSize; 				//!< image data size in bytes, (== image->height * image->widthStep in case of
		                                //!< interleaved data)
		int8_t * imageData; 			//!< pointer to aligned image data
		int32_t widthStep; 				//!< size of aligned image row in bytes
		int32_t BorderMode [4];			//!< not used
		int32_t BorderConst [4];		//!< not used
		int8_t * imageDataOrigin;		//!< pointer to very origin of image data (not necessarily aligned) -
		                                //!< needed for correct deallocation
} iViewImage;

/* **************************************************************************************** */
/* **************************************************************************************** */
/* **************************************************************************************** */

// --------------------------------------------------------------------------------------------------------------------

/**
 * When subscribing to #IVIEWDATASTREAM_SCENEIMAGES, #IVIEWDATASTREAM_SCENEIMAGES_WITH_GAZE or
 * #IVIEWDATASTREAM_SCENEIMAGES_H264_DECODED, the subscription ticket returns instances of this
 * data structure. It contains the full image scene image.
 */
typedef struct {

		uint32_t sceneFrameNumber;					//!< The number of the frame.
		iViewImage * imageData;		 				//!< The image structure.

} iViewDataStreamSceneImage;

/**
 * When subscribing to #IVIEWDATASTREAM_SCENEIMAGES_H264, the subscription ticket returns instances of this
 * data structure. It contains the scene image encoded with the H.264 compression algorithm.
 */
typedef struct {

		uint32_t sceneFrameNumber;					//!< The number of the frame.
		uint32_t isKeyFrame;						//!< 1 if this frame is a key frame, 0 otherwise.
		uint32_t dataSize;							//!< the size of H.264 data in bytes
		void * data;								//!< The raw H.264 data.

} iViewDataStreamH264SceneImage;

// --------------------------------------------------------------------------------------------------------------------

//! Identifier to determine the eye.
typedef uint32_t iViewEyeIdentifier;

static const iViewEyeIdentifier EYE_UNKNOWN = 0; 	//!< the eye is unknown
static const iViewEyeIdentifier EYE_LEFT = 1;		//!< identifies a human's left eye
static const iViewEyeIdentifier EYE_RIGHT = 2;		//!< identifies a human's right eye
static const iViewEyeIdentifier EYE_BINOCULAR = 3;	//!< declares that both eyes are included

/**
 * When subscribing to #IVIEWDATASTREAM_EYEIMAGES_LEFT or #IVIEWDATASTREAM_EYEIMAGES_RIGHT, the subscription
 * ticket returns instances of this data structure. It contains the eye image.
 */
typedef struct {

		iViewEyeIdentifier eye; 					//! Which eye is shown?
		uint32_t eyeFrameNumber; 					//! The number of the frame.
		iViewImage * imageData;		 				//! The image structure.

} iViewDataStreamEyeImage;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Data
 */
typedef struct {
		//! An indicator for the tracking quality. It indicates the confidence in the calculated gaze.
		//! A value in [0,100] where 0 means no confidence and 100 means highest confidence.
		//! If the trackingQuality is not calculated or otherwise invalid its value is set to -1.
		int32_t trackingQuality;

		//! This value represents the projection of the eye on the camera sensor.
		//! Valid values are:
		//! NAN	(location unknown), 0 (centered), 1 (upper/right/far maximum), -1 (lower/left/near maximum),
		//!	+INF (if detected outside, too high), -INF (if detected outside, too low).
		//! The shift between upper and lower boundary is a linear movement
		//! @see iView_IsNan() iView_IsInf()
		double relativePositionX; 					//!< vertical position
		double relativePositionY; 					//!< horizontal position
		double relativePositionZ; 					//!< depth/distance position

		// The position rating tells the user if the eyes are out of the optimal head box area.
		//! Valid values are:
		//! 0 (optimal position), 1 (almost untrackable: too right/too low/too far), -1 almost untrackable: too
		//! left/too high/too near. All values between are possible. This value does NOT behave linear!
		double positionRatingX; 					//!< vertical position
		double positionRatingY; 					//!< horizontal position
		double positionRatingZ; 					//!< depth/distance position

} iViewTrackingStatus;

/**
 * When subscribing to IVIEWDATASTREAM_TRACKINGSTATUS, the subscription ticket returns instances of this
 * data structure. It contains all information concerning the current tracking quality.
 */
typedef struct {

		//! Tracking status of the left eye (if available).
		iViewTrackingStatus leftEye;

		//! Tracking status of the right eye (if available).
		iViewTrackingStatus rightEye;

		//! Tracking status of both eyes, merged (if available).
		iViewTrackingStatus total;

		//! The tracking ratio (number of frames which resulted in gaze data / number of frames) in the range
		//! of [0..100]. The total tracking ratio describes the tracking ratio for the current recording.
		//! Please note that this value is only set when recording is active.
		//! A value of -1 indicates that there is no tracking ratio available at the moment.
		int32_t trackingRatioTotal;

		//! The tracking ratio (number of frames which resulted in gaze data / number of frames) in the range
		//! of [0..100]. The recent tracking ratio describes the tracking ratio for a certain number of recent
		//! frames.
		//! Please note that this value is only set when recording is active.
		//! A value of -1 indicates that there is no tracking ratio available at the moment.
		int32_t trackingRatioRecent;

		//! Frame index this trackability belongs to. A value of UINT_MAX indicates an invalid / unknown
		//! frame index.
		uint32_t eyeFrameIndex;

} iViewDataStreamTrackingStatus;

// --------------------------------------------------------------------------------------------------------------------

//! A structure for 2-dimensional position and direction vectors.
//! If all components are NaN, the vector is invalid.
//! @see iView_IsNan()
typedef struct {

		double x;
		double y;

} iViewVector2D;

//! A structure for 3-dimensional position and direction vectors.
//! If all components are NaN, the vector is invalid.
//! @see iView_IsNan()
typedef struct {

		double x;
		double y;
		double z;

} iViewVector3D;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Description of the detected parameters of a single eye.
 */
typedef struct {

		//! Diameter of the left Pupil [eye camera pixels].
		//! Values of NAN means that the diameter was not calculated.
		//! @see iView_IsNan()
		iViewVector2D pupilDiameter;

		//! Pupil radius [mm].
		//! A value of NAN means that the radius was not calculated.
		//! @see iView_IsNan()
		double pupilRadius;

		//! Contains floating point values between 0 and 1 representing the likelihood of having correctly recognized
		//! the pupil.
		//! A value of NAN means that the confidence was not calculated.
		//! @see iView_IsNan()
		double pupilConfidence;

		//! The point of regard, in screen coordinates.
		//! A value of NAN means that the coordinate is invalid and the vector could not be calculated.
		//! @see iView_IsNan()
		iViewVector2D pointOfRegard;

		//! The point from which the respective gaze vector originates [mm] which may depend on the used eye
		//! tracking system. For the ETG it is the eyeball center.
		//! A value of NAN means that the coordinate is invalid and the vector could not be calculated.
		//! @see iView_IsNan()
		iViewVector3D gazeBasePoint;

		//! 3d gaze vector based in 3d gaze base point
		//! A value of NAN means that the coordinate is invalid and the vector could not be calculated.
		//! @see iView_IsNan()
		iViewVector3D gazeDirection;

		//! This value represents the projection of the eye on the camera sensor.
		//! Valid values are:
		//! NAN	(location unknown), 0 (centered), 1 (upper/right/far maximum), -1 (lower/left/near maximum),
		//!	+INF (if detected outside, too high), -INF (if detected outside, too low).
		//! The shift between upper and lower boundary is a linear movement
		//! @see iView_IsNan() iView_IsInf()
		//double relativePositionY;

		//! Measure of uncertainty of eyeball estimator. Lower is better.
		//! For ETG: -1.5 is very good, 1.0 is mediore, 4.0 is bad.
		double eyeballUncertainty;

} iViewEyeSample;

/**
 * When subscribing to #IVIEWDATASTREAM_GAZE_INFORMATION, the subscription ticket returns instances of this
 * data structure. It contains all extracted information concerning a person's gaze at the specified moment
 * in time.
 */
typedef struct {
		//! The number of the scene frame this POR corresponds to.
		uint32_t sceneFrameNumber;

		//! The number of the eye frame this POR corresponds to.
		uint32_t eyeFrameNumber;

		//! Timestamp of the GazeSample in nanoseconds (server time since process start)
		uint64_t timestamp;

		//! If iView_SetProcessAsTimeMaster() was called then, this gaze samples reflects the time of
		//! this process, otherwise it is the local time of the server.
		//! This is the year component of this synchronized time.
		//! e.g. 2016.
		uint16_t year;
		//! If iView_SetProcessAsTimeMaster() was called then, this gaze samples reflects the time of
		//! this process, otherwise it is the local time of the server.
		//! This is the month component of this synchronized time.
		//! 1 to 12.
		uint16_t month;
		//! If iView_SetProcessAsTimeMaster() was called then, this gaze samples reflects the time of
		//! this process, otherwise it is the local time of the server.
		//! This is the day component of this synchronized time.
		//! 1 to 31.
		uint16_t day;
		//! If iView_SetProcessAsTimeMaster() was called then, this gaze samples reflects the time of
		//! this process, otherwise it is the local time of the server.
		//! This is the hour component of this synchronized time.
		//! 0 to 23.
		uint16_t hour;
		//! If iView_SetProcessAsTimeMaster() was called then, this gaze samples reflects the time of
		//! this process, otherwise it is the local time of the server.
		//! This is the minute component of this synchronized time.
		//! 0 to 59.
		uint16_t minute;
		//! If iView_SetProcessAsTimeMaster() was called then, this gaze samples reflects the time of
		//! this process, otherwise it is the local time of the server.
		//! This is the second component of this synchronized time.
		//! 0 to 59.
		uint16_t second;
		//! If iView_SetProcessAsTimeMaster() was called then, this gaze samples reflects the time of
		//! this process, otherwise it is the local time of the server.
		//! This is the millisecond component of this synchronized time.
		//! 0 to 999.
		uint16_t millisecond;

		//! The point of regard (POR), in screen coordinates.
		//! A value of NAN means that the coordinate is invalid and the POR could not be calculated.
		//! @see iView_IsNan()
		iViewVector2D pointOfRegard;

		//! The left eye data.
		iViewEyeSample leftEye;

		//! The right eye data.
		iViewEyeSample rightEye;

} iViewDataStreamGazeSample;

// --------------------------------------------------------------------------------------------------------------------

/**
 * If the user wants to asynchronously receive data, the user must subscribe to the data stream in advance. The
 * iViewDataStreamType type is used to specify the kind of data the user wants to receive. Please note that not
 * all eye tracking devices support all data streams!
 *
 * Data Stream	                                        | ETG    	| Bandwidth
 * ---------------------------------------------------- | --------- | ----------
 * #IVIEWDATASTREAM_SCENEIMAGES_WITH_GAZE				| yes (1)	| 675 Mbit/s
 * #IVIEWDATASTREAM_SCENEIMAGES							| yes (1)	| 675 Mbit/s
 * #IVIEWDATASTREAM_SCENEIMAGES_H264					| yes		| approx. 12 Mbit/s
 * #IVIEWDATASTREAM_SCENEIMAGES_H264_DECODED			| yes (2)	| approx. 12 Mbit/s
 * #IVIEWDATASTREAM_SCENEIMAGES_H264_DECODED_WITH_GAZE	| yes (2)	| approx. 12 Mbit/s
 * #IVIEWDATASTREAM_TRACKINGSTATUS						| yes		| 3 kbit/s
 * #IVIEWDATASTREAM_EYEIMAGES							| no		| -
 * #IVIEWDATASTREAM_EYEIMAGES_LEFT						| yes		| 2.2 - 8.8 Mbit/s
 * #IVIEWDATASTREAM_EYEIMAGES_RIGHT						| yes		| 2.2 - 8.8 Mbit/s
 * #IVIEWDATASTREAM_SERVERSTATUS						| yes		| up to 1 kbit/s
 * #IVIEWDATASTREAM_GAZE_INFORMATION					| yes		| approx. 300 kbit/s
 * #IVIEWDATASTREAM_CALIBRATION_DATA					| no		| -
 * #IVIEWDATASTREAM_EYE_EVENTS							| no		| -
 * #IVIEWDATASTREAM_TRACKING_MONITOR					| no		| -
 *
 * (1,2): these data streams are mutually exclusive.
 *
 * @see FAQ: @ref faq_streams.
 */
typedef uint32_t iViewDataStreamType;

//! Initial value, don't use.
static const iViewDataStreamType IVIEWDATASTREAM_INVALID = 0;

//! Receive fullframe scene images with gaze overlay.
//! This stream delivers data packets of the type iViewDataStreamSceneImage.
//!
//! Please note that this creates an enormous amount of data, depending on the image resolution and sample
//! rate. For example, with an ETG device operating with 1280*960 pixels at 24 frames per second, a bandwidth of
//! 675 Mbit/s is required.
static const iViewDataStreamType IVIEWDATASTREAM_SCENEIMAGES_WITH_GAZE = 1;

//! Receive fullframe scene images without gaze overlay.
//! This stream delivers data packets of the type iViewDataStreamSceneImage.
//!
//! Please note that this creates an enormous amount of data, depending on the image resolution and sample
//! rate. For example, with an ETG device operating with 1280*960 pixels at 24 frames per second, a bandwidth of
//! 675 Mbit/s is required.
static const iViewDataStreamType IVIEWDATASTREAM_SCENEIMAGES = 2;

//! Receive the H.264 encoded scene image and audio data stream.
//! This stream delivers data packets of the type iViewDataStreamH264SceneImage.
//!
//! Use this data stream if you intend to decode the H.264 data on your own.
//! The audio stream is not configurable: mono, 16bit, 16k (results in 256kbit/s) PCM encoded.
static const iViewDataStreamType IVIEWDATASTREAM_SCENEIMAGES_H264 = 3;

//! Receive fullframe scene images, but instead of sending the uncompressed images, send them compressed and let the
//! client side decode the images.
//! This stream delivers data packets of the type iViewDataStreamSceneImage.
//!
//! Use this data stream if you intend to receive scene images via low bandwidth networks (WLAN, 100 Mbit Ethernet etc).
static const iViewDataStreamType IVIEWDATASTREAM_SCENEIMAGES_H264_DECODED = 4;

//! Receive fullframe scene images with gaze overlay, but instead of sending the uncompressed images, send them compressed and let the
//! client side decode the images.
//! This stream delivers data packets of the type iViewDataStreamSceneImage.
//!
//! Use this data stream if you intend to receive scene images via low bandwidth networks (WLAN, 100 Mbit Ethernet etc).
static const iViewDataStreamType IVIEWDATASTREAM_SCENEIMAGES_H264_DECODED_WITH_GAZE = 5;

//! Receive trackability information.
//! This stream delivers data packets of the type iViewDataStreamTrackingStatus and
//! creates a communication load of approximately 3 kbit/s.
static const iViewDataStreamType IVIEWDATASTREAM_TRACKINGSTATUS = 6;

//! Receive eye images for devices which do not provide left / right eye images.
//! This stream delivers data packets of the type iViewDataStreamEyeImage.
//!
//! Please note that this creates a huge amount of data, depending on the image resolution and sample
//! rate.
static const iViewDataStreamType IVIEWDATASTREAM_EYEIMAGES = 7;

//! Receive left eye images. This stream delivers data packets of the type iViewDataStreamEyeImage.
//!
//! Please note that this creates a huge amount of data, depending on the image resolution and sample
//! rate. For example, with VGA resolution of 640*480 pixels at 30 frames per second, a bandwidth of
//! 8.8 Mbit/s is required.
static const iViewDataStreamType IVIEWDATASTREAM_EYEIMAGES_LEFT = 8;

//! Receive right eye images. This stream delivers data packets of the type iViewDataStreamEyeImage.
//!
//! Please note that this creates a huge amount of data, depending on the image resolution and sample
//! rate. For example, with VGA resolution of 640*480 pixels at 30 frames per second, a bandwidth of
//! 8.8 Mbit/s is required.
static const iViewDataStreamType IVIEWDATASTREAM_EYEIMAGES_RIGHT = 9;

//! Receive server status. This stream delivers data packets of the type iViewDataStreamEtgServerStatus and
//! creates a communication load of approximately 1 kbit/s.
static const iViewDataStreamType IVIEWDATASTREAM_SERVERSTATUS = 10;

//!< Receive the calculated gaze and related data. This stream delivers data packets of the type iViewDataStreamGazeSample
//! and creates a communication load of approximately 300 kbit/s.
static const iViewDataStreamType IVIEWDATASTREAM_GAZE_INFORMATION = 11;


static const iViewDataStreamType IVIEWDATASTREAM_CALIBRATION_DATA = 12;		//!< not yet implemented
static const iViewDataStreamType IVIEWDATASTREAM_EYE_EVENTS = 13;			//!< not yet implemented
static const iViewDataStreamType IVIEWDATASTREAM_TRACKING_MONITOR = 14;		//!< not yet implemented

//! Receive messages from 3rd party devices (LPT - port)
//! creates a communication load of approximately 1 kbit/s.
static const iViewDataStreamType IVIEWDATASTREAM_SAVE_MESSAGE = 15;		//!< not yet implemented

//! Receive audio data fromETG
//! creates a communication load of approximately 100 kbit/s.
static const iViewDataStreamType IVIEWDATASTREAM_PCM_AUDIO = 16;		//!< not yet implemented

// Declare the highest index of datastream types (used for initialization of some arrays).
static const iViewDataStreamType NUMBER_OF_IVIEWDATASTREAMS = 17;

// --------------------------------------------------------------------------------------------------------------------

//! Identifier for the iViewDataStreamSpec data structure.
typedef uint32_t iViewDataStreamSpecType;

static const iViewDataStreamSpecType IVIEWDATASTREAMSPEC_NONE = 0;					//!< no specifier, the structure type is iViewDataStreamSpec
static const iViewDataStreamSpecType IVIEWDATASTREAMSPEC_SAMPLE_RATE = 1;			//!< define upper limit for sample rate, the structure type is iViewDataStreamSpecSampleRate
static const iViewDataStreamSpecType IVIEWDATASTREAMSPEC_GAZE_FILTER = 2;			//!< apply a filter to the gaze cursor, the structure type is iViewDataStreamSpecGazeFilter
static const iViewDataStreamSpecType IVIEWDATASTREAMSPEC_QUALITY_OF_SERVICE = 3;	//!< control network communication protocol, the structure type is iViewDataStreamSpecQualityOfService

/**
 * Declaration of an optional addition to a stream subscription. There are several stream specifications available,
 * depending on the device and the stream type. The element iViewDataStreamSpec.type indicates the data type of the
 * whole stream specification structure. All stream specification share the first two elements, use static casting
 * to access the other elements.
 */
typedef struct iViewDataStreamSpecStruct iViewDataStreamSpec;

//! The basic layout of all stream specifiers.
struct iViewDataStreamSpecStruct {

		iViewDataStreamSpecType type;	//!< The type of this structure. Use this element for casting the whole structure.
		iViewDataStreamSpec * next;		//!< If there is another stream specifier, the address is contained here.

};

// --------------------------------------------------------------------------------------------------------------------

/**
 * Stream specifier to define a maximum framerate. Please note that this is an upper limit. You can reduce the sample
 * rate of a sensor or data stream, but not exceed its natural sample rate. It is safe to specify a sufficiently high
 * sample rate to assert that no sample is lost. The decision if a sample is to be transferred is calculated for each
 * sample individually. There will be no artifacts due to sample rate interference.
 */
typedef struct {

		//! Data inherited from iViewDataStreamSpec. Value must be #IVIEWDATASTREAMSPEC_SAMPLE_RATE.
		iViewDataStreamSpecType type;

		//! Data inherited from iViewDataStreamSpec.
		iViewDataStreamSpec * next;

		//! The desired receiving sample rate. Using this parameter it is possible to reduce the sample rate.
		//! Not all data streams understand this parameter (e.g. #IVIEWDATASTREAM_SCENEIMAGES_H264).
		//!
		//! - Use 0 to indicate no sample rate (no data will be sent at all).
		//! - Use INF to specify no sample rate limitation (send data as fast as possible).
		//! - Use any value between 0 and INF to throttle the sample rate as specified.
		//!
		//! Using a negative value will have the same effect as using INFINITY (no sample rate
		//! limitation).
		//! @see iView_IsInf() iView_GetInfValue()
		double sampleRate;

} iViewDataStreamSpecSampleRate;

// --------------------------------------------------------------------------------------------------------------------

//! Identifier for the gazeFilterType in the iViewDataStreamSpecGazeFilter data structure.
typedef uint32_t iViewDataStreamSpecGazeFilterType;

static const iViewDataStreamSpecGazeFilterType IVIEWDATASTREAMSPEC_GAZE_FILTER_TYPE_NO_FILTER = 0;	//!< disable filter
static const iViewDataStreamSpecGazeFilterType IVIEWDATASTREAMSPEC_GAZE_FILTER_TYPE_ETG_IIR	= 1;	//!< not implemented yet
static const iViewDataStreamSpecGazeFilterType IVIEWDATASTREAMSPEC_GAZE_FILTER_TYPE_ETG_FIR	= 2;	//!< not implemented yet

/**
 * Stream specifier to apply a gaze filter. Value must be #IVIEWDATASTREAMSPEC_GAZE_FILTER.
 *
 * @note not implemented yet
 */
typedef struct {

		//! Data inherited from iViewDataStreamSpec.
		iViewDataStreamSpecType type;

		//! Data inherited from iViewDataStreamSpec.
		iViewDataStreamSpec * next;

		//! Apply a gaze filter, see IVIEWDATASTREAMSPEC_GAZE_FILTER_TYPE_*
		iViewDataStreamSpecGazeFilterType gazeFilterType;

} iViewDataStreamSpecGazeFilter;

// --------------------------------------------------------------------------------------------------------------------

//! Identifier of different comunication qualities.
typedef uint32_t iViewQualityOfService;

static const iViewQualityOfService IVIEWQOS_UNKNOWN = -1;			//!< loss of data is acceptable
static const iViewQualityOfService IVIEWQOS_LOSSY = 0;			//!< loss of data is acceptable
static const iViewQualityOfService IVIEWQOS_LOSSLESS = 1;		//!< lossless communication required

/**
 * Stream specifier to specify the required quality of service for remote communication. This has an impact
 * on the used IP network communication protocol and thus on the data transmission rate and latency. It is
 * only useful for network communication. For shared memory communication, the quality of service has no
 * influence.
 *
 * Lossless communication is realized by using TCP communication by adding some protocol overhead. As
 * result, data transmitted over TCP is not lost unless the connection is not lost at all, but there may
 * be a lag as result of possible retransmission of data.
 * Lossy communication is realized by using UDP communication which is lean and has only little overhead.
 * As result, there is only a minimal lag from network communication, but data may be lost.
 *
 * By default, all API commands are transmitted using TCP, all data streams are transmitted using UDP.
 * To increase the safety of a data stream, use this stream specifier.
 */
typedef struct {

		//! Data inherited from iViewDataStreamSpec. Value must be #IVIEWDATASTREAMSPEC_QUALITY_OF_SERVICE.
		iViewDataStreamSpecType type;

		//! Data inherited from iViewDataStreamSpec.
		iViewDataStreamSpec * next;

		//! Specify the required quality of service for remote communication.
		iViewQualityOfService quality;

} iViewDataStreamSpecQualityOfService;

// --------------------------------------------------------------------------------------------------------------------

/**
 * Datastructure to specify a data stream subscription. The contents may vary according to the device type.
 *
 * @note	If it is some day required to add stuff for other devices, change this to a union, depending on the device.
 */
typedef struct {

		//! The type of the datastream.
		iViewDataStreamType streamType;

		//! Start of a linked list of (possibly device-dependent) stream specifiers. Defaults to NULL. Use
		//! iViewDataStreamSpec data structures and link them together.
		iViewDataStreamSpec * streamSpec;

} iViewStreamSubscription;

/**
 * With iView_SubscribeDataStream(), the user may subscribe to data streams. In return, the user will receive
 * iViewResult structures containing one or more iViewDataStream entities. When the end of a data stream is reached,
 * e.g. because the data stream was provided by a file player or a sensor terminated submission of data on its own,
 * a iViewDataStream structure is delivered which indicates that the end of the stream was reached. The data pointer
 * of this structure then points to a iViewDataStreamEndOfStream structure.
 */
typedef struct {
		//! The number of samples which have been created since the last call of startDataAcquisition(), excluding
		//! the EndOfStream data stream entity. Use this information to assert that no samples were lost in
		//! transmission.
		uint32_t numberOfSamples;

} iViewDataStreamEndOfStream;

// --------------------------------------------------------------------------------------------------------------------

//! Identifier to differ between alive streams (which will continue to send data) and terminated streams (which
//! will not submit any further data).
typedef uint32_t iViewDataStreamState;

static const iViewDataStreamState IVIEWDATASTREAM_END_UNKNOWN = 0;	//!< Stream state is unknown.
static const iViewDataStreamState IVIEWDATASTREAM_NO_END = 1;		//!< There will be more events.
static const iViewDataStreamState IVIEWDATASTREAM_END = 2;			//!< This stream reached its end.

/**
 * With iView_SubscribeDataStream(), the user may subscribe to data streams. In return, the user will receive
 * iViewResult structures containing one or more iViewDataStream entities.
 */
typedef struct {
		iViewDataStreamType type;

		//! A counter which contains a running number. The index enumerates the number of valid stream entities.
		//! With each provided data entity, this number is increased by 1, starting at 0 only when the application
		//! starts.
		uint32_t index;

		//! A flag to indicate whether this is the last stream entity (lastStreamEntity := #IVIEWDATASTREAM_END) or
		//! not and if more stream entities will be provided subsequently (lastStreamEntity := #IVIEWDATASTREAM_NO_END).
		//! Set to #IVIEWDATASTREAM_END_UNKNOWN if unknown. If set to #IVIEWDATASTREAM_END, the data pointer refers to
		//! a iViewDataStreamEndOfStream structure.
		iViewDataStreamState lastStreamEntity;

		//! Pointer to the data (may be NULL, e.g. if lastStreamEntity == 2). Use iViewDataStream.type to determine
		//! the type of this structure.
		void * data;

} iViewDataStream;

// --------------------------------------------------------------------------------------------------------------------

//! Definition of overlays to apply to images (bit mask, may be combined to list overlays).
//! @warning Not yet implemented.
typedef uint32_t iViewOverlayType;

//! Indicates an invalid data structure. Do not use.
static const iViewOverlayType IVIEWOVERLAY_INVALID = 0;

//! Display the SMI logo.
//! @warning Not yet implemented.
static const iViewOverlayType IVIEWOVERLAY_SMILOGO = 0x01;

//! Display the recording time.
//! @warning Not yet implemented.
static const iViewOverlayType IVIEWOVERLAY_RECORDINGTIME = 0x02;

//! Display the system time.
//! @warning Not yet implemented.
static const iViewOverlayType IVIEWOVERLAY_SYSTEMTIME = 0x04;

//! Display the system date and time.
//! @warning Not yet implemented.
static const iViewOverlayType IVIEWOVERLAY_SYSTEMDATETIME = 0x08;

//! Display the participant name.
//! @warning Not yet implemented.
static const iViewOverlayType IVIEWOVERLAY_PARTICIPANTNAME = 0x10;

//! Define the position of an overlay (bit mask, may be combined to list overlays).
typedef uint32_t iViewOverlayPosition;

static const iViewOverlayPosition IVIEWOVERLAYPOSITION_INVALID = 0;
static const iViewOverlayPosition IVIEWOVERLAYPOSITION_TOPLEFT = 0x0001;
static const iViewOverlayPosition IVIEWOVERLAYPOSITION_CENTER = 0x0002;
static const iViewOverlayPosition IVIEWOVERLAYPOSITION_BOTTOMRIGHT = 0x0004;

//! Additional parameters for the overlay. The meaning of the option depends on the overlay type.
//! @attention	Always initialize the structure with zero before using it!
typedef uint32_t iViewOverlayOption;

typedef struct {
		iViewOverlayType type;
		iViewOverlayPosition positionHorizontal;
		iViewOverlayPosition positionVertical;
		iViewOverlayOption option;

} iViewOverlaySpec;

typedef struct __iViewOverlaySpecListStruct iViewOverlaySpecList;
struct iViewOverlaySpecListStruct {
		iViewOverlaySpec content;
		iViewOverlaySpecList *next;
};

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------

/**
 * Subscribe a datastream. As consequence, the API will receive the subscribed data asynchronously
 * and eventually call the callback function to submit the data to the API user.
 *
 * Do never subscribe twice to the same data stream or expect the result tickets to be mixed.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								For some data streams: Eye tracking device is available (connected and recognized
 * 									by iViewNG server).
 *
 * @pre								For some data streams: ET device license permits use of this function.
 *
 * When accepted and forwarded to the iViewNG server, the ticket progress will change
 * from 0% to 10%, the ticket state will change from #IVIEWTICKETSTATUS_UNPROCESSED to
 * #IVIEWTICKETSTATUS_ACCEPTED.
 *
 * This function returns synchronously and provides the result of the operation to the user callback function.
 * Use the ticket to associate the operation request and the return of the result.
 * On return, the ticket's elements are set:
 * 									- On success:
 *   - iViewTicket.returnCode is #RC_NO_ERROR,
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_IN_PROGRESS until the subscription is stopped actively or due
 *     to an error.
 * 										- iViewTicket.progress is 50.
 * 									- On failure:
 * 										- iViewTicket.returnCode indicates the error code,
 * 										- iViewTicket.result is NULL or a pointer to iViewServerError.
 *   - iViewTicket.state is #IVIEWTICKETSTATUS_CLOSED
 * 										- iViewTicket.progress is 100.
 *
 * @note							Please mind that a subscribed data stream consumes communication bandwidth. See
 * 									@ref iViewDataStreamType for details.
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 *
 * @param[in]		subscription	type and constraints for the subscription, may be released immediately after the
 * 									function returns
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_PARAMETER	if any parameter is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SubscribeDataStream (iViewTicket * ticket,
        iViewStreamSubscription const * const subscription);

/**
 * Unsubscribe from a datastream of the specified type.
 * As consequence, the API will no longer receive the subscribed data asynchronously.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @pre								The specified stream was subscribed.
 *
 * When accepted and forwarded to the iViewNG server, the ticket progress will change from 0% to 10%, the ticket state
 * will change from #IVIEWTICKETSTATUS_UNPROCESSED to #IVIEWTICKETSTATUS_ACCEPTED.
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
 * @post							On success, this function will close the ticket which called iView_SubscribeDataStream();
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 * @param[in]		streamType	the desired stream type
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INVALID_PARAMETER	if any parameter is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_UnsubscribeDataStream (iViewTicket *ticket,
        const iViewDataStreamType streamType);

// --------------------------------------------------------------------------------------------------------------------

#endif
