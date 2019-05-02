#pragma once

#include <autobahn.hpp>
#include <wamp_websocketpp_websocket_transport.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <msgpack.hpp>

#include "VpxEncoder.h"

#include <math.h>
#include "DataContainer.h"

#include <thread>     
#include "ImagerServer.h"
#include <vector>

#include "Utils.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef msgpack::v1::object object;

class SceneCameraWAMP {
private:
	VpxEncoder vpxEncoder;
	ImagerServer imagerServer;
	bool closedManually;
	bool shouldLeave;

	bool StoppedUnexpectedly;

	std::thread vpxThread;
	std::thread imagerServerThread;

	void StreamSceneCamera(std::shared_ptr<autobahn::wamp_session> session);
	void StreamEncodedSceneCamera(std::shared_ptr<autobahn::wamp_session> session);
	void StreamVPEncodedSceneCamera(std::shared_ptr<autobahn::wamp_session> session);
	void StreamGazeDataSample(std::shared_ptr<autobahn::wamp_session> session);

	std::string glassesID;

	boost::asio::io_service io;
public:
	void WebsocketStreamer(char* crossbarAddress, char* glassesID);
	void LeaveSession();

	bool IsStoppedUnexpectedly();

	SceneCameraWAMP();
	~SceneCameraWAMP();
};
