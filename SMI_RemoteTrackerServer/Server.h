#pragma once
#include "iViewXAPI.h"

#include <string>
#include <conio.h>
#include <stdio.h>

//Thread sleep
#include <chrono>
#include <thread> 

#include "DataContainer.h"
#include "WampServer.h"

class Server
{
	bool wampSetup;
	WampServer* wampServer;
	std::thread wampServerThread;
	char* crossbarAddress;
	std::string retID;

	bool Running;
	int Connect(char* ServerIPAddress, int ServerPort, char* RecieveAddress, int RecievePort);
	int ConfigureRedGeometry();
	int Calibrate(int CalibrationMethod, int AutoAccept);

	void setupWAMP();

public:
	Server(char* ServerIPAddress, int ServerPort, char* RecieveAddress, int RecievePort, char* crossbarAddress, std::string retID);
	~Server();

	void StopServer();
	void SetSampleFrequency(int Frequency);
	bool IsRunning();

	//Called by the thread
	void Run();

	void calibrateRemoteTracker(retData calibrationData);
};

