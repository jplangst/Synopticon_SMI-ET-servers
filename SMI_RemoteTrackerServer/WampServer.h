#pragma once
#include "wtypes.h"

#include "iViewXAPI.h"

#include "DataContainer.h"
#include <autobahn.hpp>
#include <wamp_websocketpp_websocket_transport.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

typedef websocketpp::client<websocketpp::config::asio_client> client;

struct retData {
	//RED Geometry Specific
	REDGeometryEnum geoEnum = REDGeometryEnum::standalone;
	int Depth;
	int Height;
	int ScreenWidth;
	int ScreenHeight;
	int REDAngle;

	//Calibration specific
	int speed;
	int method;
	int displayDevice;
	int autoAccept;
};

class WampServer
{
private:
	bool closedManually;
	bool shouldLeave;
	bool StoppedUnexpectedly;

	static bool shouldCalibrate;
	static retData calibrationData;

	std::string retID;

	void StreamGazeDataSample(std::shared_ptr<autobahn::wamp_session> session);
	void StreamGazeDataFixation(std::shared_ptr<autobahn::wamp_session> session);
	void StreamCalibrationResult(std::shared_ptr<autobahn::wamp_session> session);
	boost::asio::io_service io;
	static bool bPublishCalibrationResult;
	static double calX;
	static double calY;
	float DisplayWidth;
	float DisplayHeight;
	void UpdateCurrentDisplaySize();
public:
	WampServer();
	~WampServer();

	void WebsocketStreamer(char* crossbarAddress, std::string retID);
	void LeaveSession();

	bool IsStoppedUnexpectedly();

	bool ShouldCalibrate();
	retData getCalibrationData();
	void HasCalibrated();
	static void OnCalibration(const autobahn::wamp_event& event);

	void publishCalibrationResult(double calX, double calY);
};

