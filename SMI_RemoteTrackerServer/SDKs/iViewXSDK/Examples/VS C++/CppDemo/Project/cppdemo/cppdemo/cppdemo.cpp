// -----------------------------------------------------------------------
//
// (c) Copyright 1997-2016, SensoMotoric Instruments GmbH
// 
// Permission  is  hereby granted,  free  of  charge,  to any  person  or
// organization  obtaining  a  copy  of  the  software  and  accompanying
// documentation  covered  by  this  license  (the  "Software")  to  use,
// reproduce,  display, distribute, execute,  and transmit  the Software,
// and  to  prepare derivative  works  of  the  Software, and  to  permit
// third-parties to whom the Software  is furnished to do so, all subject
// to the following:
// 
// The  copyright notices  in  the Software  and  this entire  statement,
// including the above license  grant, this restriction and the following
// disclaimer, must be  included in all copies of  the Software, in whole
// or  in part, and  all derivative  works of  the Software,  unless such
// copies   or   derivative   works   are   solely   in   the   form   of
// machine-executable  object   code  generated  by   a  source  language
// processor.
// 
// THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
// EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
// MERCHANTABILITY,   FITNESS  FOR  A   PARTICULAR  PURPOSE,   TITLE  AND
// NON-INFRINGEMENT. IN  NO EVENT SHALL  THE COPYRIGHT HOLDERS  OR ANYONE
// DISTRIBUTING  THE  SOFTWARE  BE   LIABLE  FOR  ANY  DAMAGES  OR  OTHER
// LIABILITY, WHETHER  IN CONTRACT, TORT OR OTHERWISE,  ARISING FROM, OUT
// OF OR IN CONNECTION WITH THE  SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// -----------------------------------------------------------------------

#include "stdafx.h"

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "iViewXAPI.h"


// this function will only be executed when a sample is available
int __stdcall SampleCallbackFunction(SampleStruct sampleData)
{
	std::cout << "From Sample Callback X: " << sampleData.leftEye.gazeX << " Y: " << sampleData.leftEye.gazeY << std::endl;
	return 1;
}

// this function will only be executed when a tracking monitor image is available
int __stdcall TrackingMonitorCallbackFunction(ImageStruct imageData)
{
	std::cout << "From TrackingMonitor Callback ImageSize: " << imageData.imageSize << " Width: " << imageData.imageWidth << " Height: " << imageData.imageHeight << std::endl;
	return 1;
}


int _tmain(int argc, _TCHAR* argv[])
{
	AccuracyStruct accuracyData; 
	SystemInfoStruct systemInfoData; 
	CalibrationStruct calibrationData;
	int ret_calibrate = 0, ret_validate = 0, ret_connect = 0; 
	char c = ' ';
	char repeat = ' ';

	std::cout << "Test iViewXAPI Demo" << std::endl;

	// connect to iViewX 
	ret_connect = iV_Connect("127.0.0.1", 4444, "127.0.0.1", 5555);

	switch(ret_connect)
	{
		case RET_SUCCESS:
			std::cout <<  "Connection was established successfully" << std::endl;

			// read out meta data from iViewX 
			std::cout << "GetSystemInfo: " << iV_GetSystemInfo(&systemInfoData) << std::endl;
			std::cout << "SystemInfo ETSystem: " << systemInfoData.iV_ETDevice << std::endl;
			std::cout << "SystemInfo iV_Version: " << systemInfoData.iV_MajorVersion << "." << systemInfoData.iV_MinorVersion << "." << systemInfoData.iV_Buildnumber << std::endl;
			std::cout << "SystemInfo API_Version: " << systemInfoData.API_MajorVersion << "." << systemInfoData.API_MinorVersion << "." << systemInfoData.API_Buildnumber << std::endl;
			std::cout << "SystemInfo samplerate: " << systemInfoData.samplerate << std::endl;

			break;
		case ERR_COULD_NOT_CONNECT:
			std::cout <<  "Connection could not be established" << std::endl;
			break;
		case ERR_WRONG_PARAMETER:
			std::cout <<  "Wrong Parameter used" << std::endl;
			break;
		default:
			std::cout <<  "Any other error appeared" << std::endl;
			return 0;
	}

	if(ret_connect == RET_SUCCESS)
	{
		// shall we perform a calibration?
 		std::cout <<  "Do you want to calibrate? (y)es | (n)o" << std::endl;
		c = getchar();

		//flush the buffer
		getchar(); 
 
 		if( c == 'y')
		{
			// set up calibration
			calibrationData.method = 2;
			calibrationData.speed = 1;
			calibrationData.displayDevice = 0;
			calibrationData.targetShape = 2;
			calibrationData.foregroundBrightness = 250;
			calibrationData.backgroundBrightness = 230;
			calibrationData.autoAccept = 2;
			calibrationData.targetSize = 20;
			calibrationData.visualization = 1;
			strcpy_s(calibrationData.targetFilename, 256, "");

			iV_SetupCalibration(&calibrationData);

			// start calibration
			ret_calibrate = iV_Calibrate();

			switch(ret_calibrate)
			{
				case RET_SUCCESS:
					std::cout <<  "Calibration done successfully" << std::endl;
					
					// start validation
					ret_validate = iV_Validate();

					// show accuracy only if validation was successful
					if(ret_validate == RET_SUCCESS)
					{
						std::cout << "iV_GetAccuracy: " << iV_GetAccuracy(&accuracyData, 0) << std::endl;
						std::cout << "AccuracyData DevX: " << accuracyData.deviationLX << " DevY: " << accuracyData.deviationLY << std::endl;
						getchar();
					}
					break;
				case ERR_NOT_CONNECTED:
					std::cout <<  "iViewX is not reachable" << std::endl;
					break;
				case ERR_WRONG_PARAMETER:
					std::cout <<  "Wrong Parameter used" << std::endl;
					break;
				case ERR_WRONG_DEVICE:
					std::cout <<  "Not possible to calibrate connected Eye Tracking System" << std::endl;
					break;
				default:
					std::cout <<  "An unknown error appeared" << std::endl;
					break;
			}
		}


		// show some windows
		iV_ShowEyeImageMonitor();
		iV_ShowTrackingMonitor();
		//iV_ShowSceneVideoMonitor();

		// start data output via callback function
		// define a callback function for receiving samples 
		iV_SetSampleCallback(SampleCallbackFunction);
		iV_SetTrackingMonitorCallback(TrackingMonitorCallbackFunction);

		getchar();
		
		// disable callbacks
		iV_SetSampleCallback(NULL);
		iV_SetTrackingMonitorCallback(NULL);

		// disconnect
		std::cout << "iV_Disconnect: " << iV_Disconnect() << std::endl;

		getchar();

	}

	return 0;
}

