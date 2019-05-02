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
#ifndef _IVIEWNG_CONNECTION_H
#define _IVIEWNG_CONNECTION_H

/**
 * \file iViewNG-Connection.h
 *
 * This file declares the iViewNG API functions and data types which are required for connecting to
 * an iViewNG server.
 */

#include <iViewNG-Device.h>

// --------------------------------------------------------------------------------------------------------------------

//! Type to identify IP address types.
typedef unsigned int iViewConnectionType;

static const iViewConnectionType IVIEW_SERVERADRRESS_INVALID = 0; 		//!< IP address types: invalid address.
static const iViewConnectionType IVIEW_SERVERADRRESS_IPV4 = 1; 			//!< IP address types: IPv4 address.
static const iViewConnectionType IVIEW_SERVERADRRESS_IPV6 = 2; 			//!< IP address types: IPv6 address.
static const iViewConnectionType IVIEW_SERVERADRRESS_SHAREDMEMORY = 3; 	//!< Shared memory communication to localhost.
//! Unix domain socket communication for inter-process communication. Not usable under MS Windows.
static const iViewConnectionType IVIEW_SERVERADRRESS_UNIX_DOMAIN_SOCKET = 4;

static const uint8_t HOSTADDRESSLENGTH_IPV4 = 16;
static const uint8_t HOSTADDRESSLENGTH_IPV6 = 24;

//! Datastructure which represents the location of a remote iViewNG server instance.
typedef struct {

		//! The server host address, either IPv4 or IPv6.
		union {
				char ipv4 [HOSTADDRESSLENGTH_IPV4]; 	//! The server host address, IPv4 version.
				char ipv6 [HOSTADDRESSLENGTH_IPV6]; 	//! The server host address, IPv6 version.
		} ipAddress;

		//! Port for establishing the connection.
		//! If set to zero, the default port (1100) is used. Do not use anything but zero unless you explicitly start
		//! the iViewNG server with a different port number!
		unsigned short port;

} iViewHostAddress;

// --------------------------------------------------------------------------------------------------------------------

//! Data structure which represents one iViewNG server instance which is willing to provide access to one SMI
//! device. If one iViewNG server is able to provide access to more than one SMI device, each device will be
//! listed as one iViewHost.
//!
//! @note	Generate via iView_LocateServers() to get all elements filled with proper values.
//!
//! @note	If not allocated manyally, do not release the iViewHost structure manually, use  iView_ReleaseiViewHost()
//!			instead.
typedef struct {

		//! An optional string which the server may use to identify itself.
		wchar_t hostId [128];

		//! State the type of the connection.
		iViewConnectionType connectionType;

		//! The IP address & UDP/TCP port of a iViewNG server instance, if iViewHost.type implies network
		//! communication.
		//! Irrelevant for IVIEW_SERVERADRRESS_SHAREDMEMORY communication type.
		iViewHostAddress hostAddress;

		//! Provided/requested software version on host.
		//! Not yet implemented.
		iViewVersion hostVersion;

		//! The SMI device which is provided/requested.
		//! Not yet implemented.
		iViewDeviceType device;

		//! A set of provided/requested capabilities for this device. Device-dependent.
		//! Not yet implemented.
		void * capability;

} iViewHost;

// --------------------------------------------------------------------------------------------------------------------

//! Data structure which contains a list of iViewNG server address structures.
//!
//! @note	Do not create the iViewHostList structure manually. Generate via iView_LocateServers().
//! @note	Do not release the iViewHostList structure manually, use iView_ReleaseiViewHostList() instead.
typedef struct __iViewHostListStruct iViewHostList;
struct __iViewHostListStruct {

		//! Server host structure.
		iViewHost host;

		//! If more than one server is returned, this member points to the next structure. Defaults to NULL.
		iViewHostList * next;
};

//! Bit mask for defining restrictions for iView_LocateServers().
typedef uint32_t iViewLocationScope;

static const iViewLocationScope IVIEW_LOCATE_LOCALHOST_ONLY = 0x1; 		//! Restrict location to localhost only. Do
																		//! not search the network.
static const iViewLocationScope IVIEW_LOCATE_NETWORK = 0x2;				//! Search the network for servers.
static const iViewLocationScope IVIEW_LOCATE_FIRST_SERVER_ONLY = 0x100;	//! Immediately return when one server matching
																		//! the requirements is found. First come, first
																		//! served. Do not continue to search.

// --------------------------------------------------------------------------------------------------------------------

/**
 * Connect to the specified iViewNG server.
 *
 * This function blocks the caller until a connection was established or until the operation timed out. The ticket
 * state remains #IVIEWTICKETSTATUS_IN_PROGRESS until the connection was lost. When this happens, the ticket state will
 * be set to #IVIEWTICKETSTATUS_CLOSED, the progress is set to 100 and the return code is set to #RC_CONNECTION_FAILED.
 *
 * Before terminating, an established connection shall be closed using  iView_Disconnect().
 *
 * @pre		The iView C API was initialized with iView_Init().
 *
 * This function returns synchronously and provides the result of the operation to the user callback function.
 * Use the ticket to associate the operation request and the return of the result.
 * On return, the ticket's elements are set:
 * - iViewTicket.state is #IVIEWTICKETSTATUS_IN_PROGRESS
 * - iViewTicket.progress is 100.
 * 	 - On success:
 *     - iViewTicket.returnCode is #RC_NO_ERROR,
 *     - iViewTicket.state is #IVIEWTICKETSTATUS_IN_PROGRESS as long as the connection
 * 	     is alive. When the connection was lost, this is delivered as error.
 * 	 - On failure:
 * 	   - iViewTicket.returnCode indicates the error code,
 *     - iViewTicket.result is NULL or a pointer to iViewServerError.
 *
 * @param[in,out]	ticket			The iViewTicket data structure, provided by the API user. iView uses the ticket
 * 									structure to identify a command request when delivering results asynchronously.
 * 									Use iView_CreateTicket() to initialize a ticket data structure. The ticket must
 * 									not be released manually! Use iView_ReleaseTicket() instead!
 * @param[in]		server			Specification of the server. Mandatory.
 * @param[in]		timeout			Do not try to connect to server for more than this time span [milliseconds].
 * 									It is recommended to wait not less than 1 seconds and retry 2 times.
 * @param[in]		attempts		Number of attempts to retry initiating a connection when the first attempt failed.
 * 									Each attempt will approximately consume the time span defined by 'timeout'. When
 * 									all attempts fail, the function will return indicating the failure.
 * 									It is recommended to wait not less than 1 seconds and retry 2 times.
 * @param[in]		client			Specification of the client. Optional for legacy communication, not used for iVNG
 * 									communication.
 *
 * @retval	#RC_NO_ERROR				on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_INVALID_TICKET		if the ticket data structure is invalid
 * @retval	#RC_INVALID_PARAMETER	if any of the parameters are invalid
 * @retval	#RC_INVALID_STATE		if the server was already connected
 * @retval	#RC_OPERATION_FAILED		if the connection attempt failed (e.g. because of timeout)
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_INVALID_LICENSE		if the provided licenses do not permit to execute this operation
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_Connect (iViewTicket * const ticket, iViewHost const * const server,
        const uint32_t timeout, const uint32_t attempts, iViewHost const * const client);

/**
 * Cut the connection to the iViewNG server.
 * Does not block pending events which may still arrive when this function completed.
 *
 * @pre								The iView C API was initialized with iView_Init().
 *
 * @pre								Connection to a server process was established with iView_Connect().
 *
 * @post							All pending tickets will be set to state #IVIEWTICKETSTATUS_CLOSED, the return code
 * 									is set to #RC_NO_ERROR.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_CONNECTION_FAILED	if the connection to the server is not alive
 * @retval	#RC_UNSPECIFIED			on any other error
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_Disconnect ();


/**
 * Retrieve the list of available hosts in the network.
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
 * @test
 */

DLL_DEC_PRE iViewRC DLL_DEC_POST iView_GetHostList (iViewTicket * const ticket);


/**
 * Define this process to be the time master.
 * It means this process will send its system time to the server process. The server will mark the GazeSamples with
 * this date and time (YYYY.MM.DD HH:MM:SS,000) to enable synchronization with other data recorded on this machine.
 *
 * This is useful if you are connected to a server which runs on a different computer and want to find your own
 * time in server's gaze samples.
 * After setting this, the server might need a few seconds to mitigate the network latency.
 * If the network connection is not stable (too much jitter), the server will not be able to synchronize optimally.
 *
 * Has an effect only if connecting via network to a server.
 *
 * @retval	#RC_NO_ERROR			on success
 * @retval	#RC_NO_INIT				if the iViewNG C API was not initialized
 * @retval	#RC_INVALID_TICKET		if ticket data structure is invalid
 * @retval	#RC_INTERNAL_ERROR		if an internal error occurred
 * @retval	#RC_UNSPECIFIED			on any other error
 *
 */
DLL_DEC_PRE iViewRC DLL_DEC_POST iView_SetProcessAsTimeMaster ();


// --------------------------------------------------------------------------------------------------------------------

#endif
