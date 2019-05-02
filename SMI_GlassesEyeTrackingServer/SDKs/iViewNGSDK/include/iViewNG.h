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
#ifndef _IVIEWNG_H
#define _IVIEWNG_H

/**
 * \file iViewNG.h
 *
 * This file declares several C preprocessor macros which are used by the other include files. There is no
 * need to include this file directly, it is automatically included where needed.
 */

#ifdef _MSC_VER
#ifdef __cplusplus
#define DLL_DEC_PRE extern "C" __declspec(dllimport)
#else
#define DLL_DEC_PRE extern __declspec(dllimport)
#endif
#define DLL_DEC_POST __stdcall
#else

#ifdef WINDOWS
#define DLL_DEC_PRE extern "C" __stdcall
#else
#define DLL_DEC_PRE extern "C"
#endif

#define DLL_DEC_POST
#endif

// define this if you want to build a .dll / -lib / .def usable in Visual Studio
//#define CREATELIB

#ifdef CREATELIB
#define DLLCALL _stdcall
#else
#define DLLCALL
#endif

// --------------------------------------------------------------------------------------------------------------------

#ifndef DLL_DEFINITION
#ifdef WINDOWS
#define DLL_DEFINITION __stdcall
#else
#define DLL_DEFINITION
#endif
#endif

#include "iViewNG-BasicDatatypes.h"

// --------------------------------------------------------------------------------------------------------------------

//! The return code for submitting the immediate result of a function call. This return code does not always indicate
//! the result of the requested operation, but only the call to the function. Operation results are always returned
//! by call-by-reference parameters like the iViewTicket data structure, synchronously and asynchronously.
typedef uint32_t iViewRC;

static const iViewRC RC_NO_ERROR = 0; 				//!< No error, the function call was accepted.
static const iViewRC RC_NO_INIT = 1; 				//!< iView_Init() was not called!
static const iViewRC RC_INTERNAL_ERROR = 2;			//!< The function call could not be processed due to a serious internal error. Please contact SMI.
static const iViewRC RC_OUT_OF_MEMORY = 3; 			//!< The function call failed due to insufficient memory.
static const iViewRC RC_INVALID_TICKET = 4;			//!< The provided iViewTicket is invalid (most likely, it's NULL, the result pointer is NULL or the ticket state is invalid).
static const iViewRC RC_INVALID_PARAMETER = 5; 		//!< The provided parameters were not valid.
static const iViewRC RC_INVALID_STATE = 6; 			//!< The server is not in a valid state for the operation.
static const iViewRC RC_INVALID_REQUEST = 7;		//!< The API request is invalid, e.g. not supported.
static const iViewRC RC_CONNECTION_FAILED = 8; 		//!< The operation failed due to the lack of a connection to the server.
static const iViewRC RC_OPERATION_FAILED = 9; 		//!< The requested operation failed.
static const iViewRC RC_DEVICE_NOT_READY = 10; 		//!< The API requested a command which needs to access an (e.g. ETG) device which is not yet ready (e.g. still initializing).
static const iViewRC RC_DEVICE_BUSY = 11; 			//!< The API requested a command which needs to access an (e.g. ETG) device which is already performing an operation.
static const iViewRC RC_INVALID_LICENSE = 12;		//!< The API request failed because the currently activated license(s) do not permit this operation.
static const iViewRC RC_TIMEOUT = 13; 				//!< An API operation was not answered in time.
static const iViewRC RC_ALREADY_RUNNING = 14; 		//!< The server says the requested operation is already running.
static const iViewRC RC_ALREADY_STOPPED = 15; 		//!< The server says the requested operation has already stopped.
static const iViewRC RC_SERVERSTATUS_NOT_SUBSCRIBED = 16; //!< The called function needs to have ServerStatus subscribed (e.g. iView_TakeControl())
static const iViewRC RC_TARGET_SYSTEM_NOT_IN_RECORDING_SCREEN = 17; //<!< You have to select an experiment and enter recording screen on the target system first
static const iViewRC RC_NOT_IN_CONTROL = 18; 		//!< You have to take control of the target system first, before you can call this function (e.g. iView_SendParticipantName())
static const iViewRC RC_VERSION_MISMATCH = 19; 		//!< Version mismatch, usually returned by iView_Connect() if the SDK version does not match the Server version
static const iViewRC RC_SERVER_VERSION_TOO_OLD = 20;//!< Server version is too old, usually returned by iView_Connect() if the SDK version does not match the Server version
static const iViewRC RC_SDK_VERSION_TOO_OLD = 21; 	//!< SDK version is too old, usually returned by iView_Connect() if the SDK version does not match the Server version
static const iViewRC RC_WIFI_TOO_WEAK = 22; 		//!< Wifi connection is too weak so connection between server and (sdk) client is compromised, try to reconnect.
static const iViewRC NUMBER_OF_RETURNCODES = 23; 	//!< Besides RC_UNSPECIFIED, this is always the highest return code so that it's possible to loop over all return codes.
static const iViewRC RC_UNSPECIFIED = UINT_MAX; 	//!< Unspecified error, should never happen.

#endif
