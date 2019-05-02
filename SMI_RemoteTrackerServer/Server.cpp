#include "Server.h"

Server::Server(char* ServerIPAddress, int ServerPort, char* RecieveAddress, int RecievePort, char* _crossbarAddress, std::string _retID)
{
	Running = true;

	wampSetup = false;
	wampServer = nullptr;
	crossbarAddress = _crossbarAddress;
	retID = _retID;

	int ret = Connect(ServerIPAddress, ServerPort, RecieveAddress, RecievePort);

	if (ret != 1)
	{
		Running = false;

		printf("Press any key to exit...\n");
		_getch();
		return;
	}
}

Server::~Server()
{

}

void Server::setupWAMP()
{
	wampServer = new WampServer();
	wampServerThread = thread(&WampServer::WebsocketStreamer, wampServer, crossbarAddress, retID);
	printf("Running RET WAMP session...\n");
}

int MonitorIndex = 0;
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
	HDC      hdcMonitor,
	LPRECT   lprcMonitor,
	LPARAM   dwData)
{
	MONITORINFOEX info;
	info.cbSize = sizeof(info);
	if (GetMonitorInfo(hMonitor, &info))
	{
		std::cout << "Monitor " << info.szDevice << ": " << std::abs(info.rcMonitor.left - info.rcMonitor.right)
			<< " y: " << std::abs(info.rcMonitor.top - info.rcMonitor.bottom)
			<< std::endl;
		MonitorIndex++;
	}
	return TRUE;  // continue enumerating
}

//method can be: 2(Fastest), 5(Balanced) or 9(Best)
//speed can be 0(normal) or 1(fast)
//pointAccepting can be 0(manual), 1(semi-auto) or 2(full-auto)
//device can be 0(main device) or 1(other device)
void Server::calibrateRemoteTracker(retData calibrationData) {
	wampServer->HasCalibrated();

	REDGeometryStruct GeoStruct;
	GeoStruct.redGeometry = calibrationData.geoEnum;
	GeoStruct.redStimDistDepth = calibrationData.Depth;
	GeoStruct.redStimDistHeight = calibrationData.Height;
	GeoStruct.stimX = calibrationData.ScreenWidth;
	GeoStruct.stimY = calibrationData.ScreenHeight;
	char name[] = "RemoteTrackerTool";
	strcpy(GeoStruct.setupName, name);
	GeoStruct.redInclAngle = calibrationData.REDAngle;

	int result = iV_SetREDGeometry(&GeoStruct);
	if (result != 1) {
		cerr << "Could not set RED Geometry " << result << endl;
	}

	iV_SetSpeedMode(60);

	CalibrationStruct calStruct;
	iV_GetCalibrationParameter(&calStruct);

	calStruct.method = calibrationData.method;
	calStruct.displayDevice = calibrationData.displayDevice;
	calStruct.backgroundBrightness = 120;
	calStruct.speed = calibrationData.speed;
	calStruct.autoAccept = calibrationData.autoAccept;

	iV_SetupCalibration(&calStruct);
	iV_Calibrate();


	//iV_Validate();

	AccuracyStruct accStruct;
	iV_GetAccuracy(&accStruct, 0);
	float accX = (accStruct.deviationRX+accStruct.deviationLX)/2;
	float accY = (accStruct.deviationRY + accStruct.deviationLY) / 2;
	wampServer->publishCalibrationResult(accX, accY);
}

void Server::Run()
{
	int ret;
	while (Running)
	{
		if (!wampSetup) {
			int delay = 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));

			setupWAMP();
			wampSetup = true;
		}
		else if (wampServer->IsStoppedUnexpectedly()) {
			wampServer->LeaveSession();
			wampServerThread.join();
			delete wampServer;
			wampServer = nullptr;
			wampSetup = false;
		}
		else if (wampServer->ShouldCalibrate()) {
			calibrateRemoteTracker(wampServer->getCalibrationData());
		}

		//Fill the sample with the latest avaliable data
		SampleStruct Sample;
		ret = iV_GetSample(&Sample);

		if (ret != RET_SUCCESS)
		{
			//eyeDataServer->mainloop();
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		TrackingStatusStruct TrackingStatus;
		ret = iV_GetTrackingStatus(&TrackingStatus);
		if (ret != RET_SUCCESS)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		DataContainer::getInstance()->addGazeSample(Sample);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	if (wampServer) {
		wampServer->LeaveSession();
		wampServerThread.join();
		delete wampServer;
		wampServer = nullptr;
	}
}

int Server::Calibrate(int CalibrationMethod, int AutoAccept)
{
	CalibrationStruct calibrationSetup;
	calibrationSetup.displayDevice = 1;
	calibrationSetup.autoAccept = AutoAccept;
	calibrationSetup.method = CalibrationMethod;
	calibrationSetup.visualization = 1;
	calibrationSetup.speed = 0;
	calibrationSetup.targetShape = 2;
	calibrationSetup.backgroundBrightness = 230;
	calibrationSetup.foregroundBrightness = 250;
	calibrationSetup.targetSize = 20;
	int calsetupret = iV_SetupCalibration(&calibrationSetup);
	int ret = iV_Calibrate();

	if (ret == 1)
	{
		printf("iV_Calibrate: calibration successfull\n");
	}
	else
	{
		printf("iV_Calibrate: calibration failed: %i \n", ret);
	}

	return ret;
}

int Server::Connect(char* ServerIPAddress, int ServerPort, char* RecieveAddress, int RecievePort)
{
	iV_SetConnectionTimeout(5);
	//int ret = iV_Connect(ServerIPAddress, ServerPort, RecieveAddress, RecievePort);
	int ret = iV_ConnectLocal();

	if (ret == 1)
	{
		printf("iV_Connect: connection established\n");
	}
	else
	{
		printf("iV_Connect: failed to  established: %i \n", ret);
	}

	return ret;
}

int Server::ConfigureRedGeometry()
{
	REDGeometryStruct RedStruct;
	RedStruct.redGeometry = REDGeometryEnum::standalone;
	RedStruct.stimX = 510;
	RedStruct.stimY = 290;
	RedStruct.redInclAngle = 20;
	RedStruct.redStimDistHeight = -6;
	RedStruct.redStimDistDepth = 23;

	int ret = iV_SetREDGeometry(&RedStruct);

	if (ret == 1)
	{
		printf("iV_SetREDGeometry: Successfull\n");
	}
	else
	{
		printf("iV_SetREDGeometry: failed to configure: %i \n", ret);
	}
	return ret;
}

void Server::SetSampleFrequency(int Frequency)
{
	int ret = iV_SetSpeedMode(Frequency);

	if (ret == 1)
	{
		printf("iV_SetSpeedMode: Successfully set to %iHz frequency\n", Frequency);
	}
	else
	{
		printf("iV_SetSpeedMode: Failed to set the frequency: %i", ret);
	}
}

bool Server::IsRunning()
{
	return Running;
}

void Server::StopServer()
{
	Running = false;
	//int ret = iV_Disconnect();

	//if (ret != 1)
	//{
	//	printf("Failed to disconnect from device\n");
	//}
	//else
	//{
		printf("Disconnected successfully\n");
	//}
}
