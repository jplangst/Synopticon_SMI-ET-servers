// -----------------------------------------------------------------------
//
// (c) Copyright 1997-2014, SensoMotoric Instruments GmbH
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
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iViewNG-Core.h>
#include <iViewNG-Calibration.h>
#include <iViewNG-Connection.h>
#include <iViewNG-DataAcquisition.h>
#include <iViewNG-Device-ETG.h>

#include <iViewNG-Utility.h>

#include "iViewNG-Convenience.h"

#include "OpenCvVisualization.h"

#include <time.h>
#include <conio.h> // for kbhit()

#include <thread>     

/* **************************************************************************************** */
/* *************************************** PROTOTYPES ************************************* */
/* **************************************************************************************** */

iViewRC ParseCommandLine(int, char **);
iViewRC Setup(bool);
iViewRC SetupCalib();
iViewRC Subscribe();
iViewRC Start();
iViewRC Cleanup();
iViewRC Calibrate();
void WaitForUserInteraction();
void MyCallback(iViewTicket * const ticket);

/* **************************************************************************************** */
/* ***************************************   DATA     ************************************* */
/* **************************************************************************************** */

iViewTicket * gTicketStartAcquisition = NULL;
iViewTicket * gTicketConnect = NULL;
iViewTicket * gTicketAddLicense = NULL;
iViewTicket * gTicketDeviceParameters = NULL;
iViewTicket * gTicketSubscriptionGaze = NULL;
iViewTicket * gTicketSubscriptionLeftEye = NULL;
iViewTicket * gTicketSubscriptionRightEye = NULL;
iViewTicket * gTicketSubscriptionScene = NULL;
iViewTicket * gTicketSubscriptionSceneWithGaze = NULL;
iViewTicket * gTicketSubscriptionSceneH264 = NULL;
iViewTicket * gTicketSubscriptionSceneH264WithGaze = NULL;
iViewTicket * gTicketUnsubscription = NULL;
iViewTicket * gTicketStopAcquisition = NULL;
iViewTicket * gTicketCalibration1Pt = NULL;
iViewTicket * gTicketCalibration3Pt[3];
iViewTicket * gServerStatus = NULL;

iViewHost gServer;

// Flag which tells us that the remote iViewNG-Server is used
// in conjunction with iViewETG-Client, so this Tutorial-RemoteViewer
// will not set device parameters (as the parameters set by
// iViewETG-Client would then be overwritten).
char gRemoteIsIviewEtg = 0;

// Flag which specifies whether an explicit shutdown command
// is to be sent to the server when this tutorial ends.
char gShutdownServer = 0;

char gStreamH264EncodedImages = 0;

// User-provided flags
char gShowGaze = 0;
char gShowEyeImages = 0;
char gShowSceneImages = 0;
char gShowSceneH264 = 0;
char gShowSceneImagesWithGaze = 0;
char gShowSceneH264ImagesWithGaze = 0;
char gCalibrate1Pt = 0;
char gCalibrate3Pt = 0;
char gScene24 = 0;
char gScene30 = 0;
char gGazeOverlay = 0;
iViewSamplingRate gSamplingRate = IVIEWSAMPLINGRATE_CURRENT;
char gTimeMaster = 0;
iViewEyeCamExposureMode gEyeExposureMode = IVIEWEYECAMEXPOSUREMODE_CURRENT;

// Scale factors applied to the eye/scene image for downscaling.
float gScaleEyes = 1.;
float gScaleScene = 1.;
char gScaleSceneSet = 0;

char  gVRPNPortSet = 0;
char* crossbarAddress = "127.0.0.1:8080"; //Default eye data name
char* glassesID = "NOT SET";

const float gSamplerateEyes = 99.;
const float gSamplerateScene = 99.;

// Local data for calibration
char gCalibrationPointsToDo = 0;
unsigned int gCurrentFrameNumber = 0;

// Local data for gaze overlay
int gGazeX = 0;
int gGazeY = 0;

unsigned int gTimeOfSetupCalibMsec = 0;
unsigned int gSetupCalibCooldownMsec = 2000;

const unsigned int TICKET_WAIT_MS = 5000;


/* **************************************************************************************** */
/* ********************************   FOR RECONNECT      ********************************** */
/* **************************************************************************************** */

time_t TheLastTimestamp;
const unsigned int CONNECTION_TIMEOUT_SECONDS = 0;
const unsigned int CONNECTION_NUM_TRIES = 100;
int CreateConnectionToDevice(bool);
bool connected = false;
bool ConnectedToServer = false;
bool bRecievingCallback = false;

/* **************************************************************************************** */
/* *************************************** FUNCTIONS  ************************************* */
/* **************************************************************************************** */

std::thread sceneCameraWAMPThread;
bool imageData;
bool streamSceneCamera;
SceneCameraWAMP* sceneCameraWAMP;
bool WampSetup;
void SetUpWAMP();


bool ShuttingDown = false;
BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
  switch( fdwCtrlType ) 
  { 
    case CTRL_C_EVENT: 
		ShuttingDown = true;
		return( TRUE );
    case CTRL_CLOSE_EVENT: 
		ShuttingDown = true;
		return(FALSE);
    case CTRL_BREAK_EVENT: 
		ShuttingDown = true;
		return(FALSE);
    case CTRL_LOGOFF_EVENT: 
		ShuttingDown = true;
		return(FALSE);
    case CTRL_SHUTDOWN_EVENT: 
		ShuttingDown = true;
		return(FALSE);
    default: 
		ShuttingDown = true;
		return FALSE; 
  } 
} 

void SetUpWAMP()
{
	if (imageData && streamSceneCamera) {
		sceneCameraWAMP = new SceneCameraWAMP();
		sceneCameraWAMPThread = thread(&SceneCameraWAMP::WebsocketStreamer, sceneCameraWAMP, crossbarAddress, glassesID);
		printf("Running Scene Camera WAMP session...\n");
	}
}

int main (int argc, char ** argv) 
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

	//For testing purposes, quick way to toggle on off sources
	bool eyeData = false;
	imageData = true;
	gStreamH264EncodedImages = false;

	iViewRC rc;

	memset (&gServer, 0, sizeof(iViewHost));

	//if ((rc = ParseCommandLine(argc, argv)) || (rc = Setup()) || (rc = Subscribe()) || (rc = Start()) || (rc = SetupCalib()))
	if ((rc = ParseCommandLine(argc, argv)) || (rc = CreateConnectionToDevice(true)))
	{
		printf("Press any key to exit...\n");
		getch();
		return RC_OPERATION_FAILED;
	}
			
	printf ("Connected to Eye Tracking device...\n");
	connected = true;
	ConnectedToServer = true;

	streamSceneCamera = false;

	if (gShowSceneImages || gShowSceneH264 || gShowSceneImagesWithGaze || gShowSceneH264ImagesWithGaze || gStreamH264EncodedImages)
	{
		streamSceneCamera = true;
	}

	WampSetup = false;

	WaitForUserInteraction();

	if (sceneCameraWAMP) {
		sceneCameraWAMP->LeaveSession();
		sceneCameraWAMPThread.join();
		delete sceneCameraWAMP;
		sceneCameraWAMP = nullptr;
	}

	return 0;
}

int CreateConnectionToDevice(bool InitLib)
{
	iViewRC rc;
	if ((rc = Setup(InitLib)) || (rc = Subscribe()) || (rc = Start()) || (rc = SetupCalib()))
	{
		printf_s("Failed to connnect to device.\n");
		return rc;
	}

	TheLastTimestamp = time(NULL);
	return 0;
}

/* **************************************************************************************** */

void handleGazeSample(iViewDataStreamGazeSample * gazeSample) {

	gGazeX = (int)(gazeSample->pointOfRegard.x + 0.5);
	gGazeY = (int)(gazeSample->pointOfRegard.y + 0.5);

	//iViewDataStreamGazeSample clonedSample =*gazeSample;
	//Add the input gaze sample to the DataContainer
	DataContainer::getInstance()->addGazeSample(*gazeSample);
}

/* **************************************************************************************** */

/**
 * This function will be called from MyCallback() when a new eye image is available.
 */
void handleEyeImage(iViewDataStreamEyeImage * image) {

	switch (image->eye) {

	case EYE_LEFT:

		displayLeftEyeImage(image->imageData);
		break;

	case EYE_RIGHT:

		displayRightEyeImage(image->imageData);
		break;

	case EYE_UNKNOWN:
		fprintf(stderr, "ERROR in callback: unknown eye type.\n");
		break;
	}

	return;
}

/* **************************************************************************************** */

/**
 * This function will be called from MyCallback() when a new scene image is available.
 */
void handleSceneImageWithGaze(iViewDataStreamSceneImage * image) { 

	gCurrentFrameNumber = image->sceneFrameNumber;
	displaySceneImage(image->imageData);

	return;
}

/* **************************************************************************************** */

/**
* This function will be called from MyCallback() when a new scene image is available.
*/
void handleH264DecodedSceneImage(iViewDataStreamH264SceneImage * image) {

	//gCurrentFrameNumber = image->sceneFrameNumber;

	//// Draw gaze onto scene only if requested.
	//if (gShowSceneImagesWithGaze || gShowSceneH264ImagesWithGaze) {
	//	drawGazeOverlay(image->imageData, gGazeX, gGazeY);
	//}

	////displaySceneImage(image->imageData);

	////iViewDataStreamSceneImage clonedImage =*image;

	////Add the input image sample to the DataContainer	
	//DataContainer::getInstance()->addSceneCameraSample(*image);
}

/**
 * This function will be called from MyCallback() when a new scene image is available.
 */
void handleH264DecodedSceneImage(iViewDataStreamSceneImage * image) {
	gCurrentFrameNumber = image->sceneFrameNumber;

	// Draw gaze onto scene only if requested.
	if (gShowSceneImagesWithGaze || gShowSceneH264ImagesWithGaze) {
		drawGazeOverlay(image->imageData, gGazeX, gGazeY);
	}

	//displaySceneImage(image->imageData);

	//iViewDataStreamSceneImage clonedImage =*image;

	//Add the input image sample to the DataContainer	
	DataContainer::getInstance()->addSceneCameraSample(*image);
}

/**
* This function will be called from MyCallback() when a new encded scene image is available.
*/
void handleH264EncodedSceneImage(iViewDataStreamH264SceneImage* image) {	
	////Add the input image sample to the DataContainer	

	DataContainer::getInstance()->addEncodedSceneCameraSample(*image);
}

/* **************************************************************************************** */

/**
 * The callback analyses the result type and delegates operation to dedicated event handler.
 */
void MyCallback(iViewTicket * const ticket) 
{

	//printf("MyCallback ticket id: %d \n", ticket->ticketNumber);

	// this should never happen
	if (NULL == ticket)
		return;

	//For Reconnection
	TheLastTimestamp = time(NULL);

	// extract the result
	iViewResult const * const result = ticket->result;

	// Check if we got a 1pt calibration response
	if (gTicketCalibration1Pt == ticket) {
		printf("Sending 1pt calibration data %s successful.\n", RC_NO_ERROR == ticket->returnCode ? "was" : "was not");
		iView_ReleaseTicket(&gTicketCalibration1Pt);
		return;
	}

	// Check if we got a 3pt calibration response
	for (int i = 0; i<3; i++) {
		if (gTicketCalibration3Pt[i] == ticket) {
			printf("Sending 3pt calibration data %s successful.\n", RC_NO_ERROR == ticket->returnCode ? "was" : "was not");
			iView_ReleaseTicket(&gTicketCalibration3Pt[i]);
			return;
		}
	}

	// if the ticket carries no result or if it's not a data stream ticket, we're not interested
	if (NULL == result || IVIEWRESULT_SUBSCRIBE_DATASTREAM != result->type)
		return;

	// cast to the proper result type
	iViewDataStream const * const stream = (iViewDataStream const *)result->data;

	if (stream->lastStreamEntity == IVIEWDATASTREAM_END) {
		fprintf(stderr, "Got last stream Entity\n");
		return;
	}

	switch (stream->type) {
	case IVIEWDATASTREAM_GAZE_INFORMATION:

 		handleGazeSample((iViewDataStreamGazeSample*)stream->data);
		break;

	case IVIEWDATASTREAM_EYEIMAGES_LEFT:

		handleEyeImage((iViewDataStreamEyeImage*)stream->data);
		break;

	case IVIEWDATASTREAM_EYEIMAGES_RIGHT:

		handleEyeImage((iViewDataStreamEyeImage*)stream->data);
		break;

	case IVIEWDATASTREAM_SCENEIMAGES:
	case IVIEWDATASTREAM_SCENEIMAGES_WITH_GAZE:
	case IVIEWDATASTREAM_SCENEIMAGES_H264_DECODED_WITH_GAZE:

		handleSceneImageWithGaze((iViewDataStreamSceneImage*)stream->data);
		break;

	case IVIEWDATASTREAM_SCENEIMAGES_H264_DECODED:

		handleH264DecodedSceneImage((iViewDataStreamSceneImage*)stream->data);
		break;

	case IVIEWDATASTREAM_SCENEIMAGES_H264:
		handleH264EncodedSceneImage((iViewDataStreamH264SceneImage*) stream->data);
		break;
	}
	
	connected = true;

	if (!bRecievingCallback)
	{
		fprintf(stderr, "Reciveving Callbacks!\n");
		bRecievingCallback = true;
	}
	

	iView_ReleaseResult(ticket);
	return;
}

/* **************************************************************************************** */

void Usage (char const * const cmd) {

	fprintf (stderr, "\nUSAGE\n\t%s [OPTION]...\n\n", cmd ? cmd : "");
	fprintf(stderr, "OPTIONS\n\n");
	fprintf(stderr, "\t--help               to print this information\n");
	fprintf(stderr, "\t--show-eyes          to open two windows displaying the eye images\n");
	fprintf(stderr, "\t--scale-eyes f       rescale eye images by multiplying image size by f\n");
	fprintf(stderr, "\t--show-scene         to open a window displaying the scene images (the\n");
	fprintf(stderr, "\t                     scene video is decoded by the server and full image\n");
	fprintf(stderr, "\t                     will be transferred to the remote viewer)\n");
	fprintf(stderr, "\t--show-scene-h264    to open a window displaying the scene images (the\n");
	fprintf(stderr, "\t                     scene video is transferred as H.264 stream,\n");
	fprintf(stderr, "\t                     decoded by the remote viewer)\n");
	fprintf(stderr, "\t--show-scene-with-gaze	to open a window displaying the scene images (the\n");
	fprintf(stderr, "\t                     scene video is decoded by the server and already\n");
	fprintf(stderr, "\t                     contains the gaze cursor, full image is transferred to\n");
	fprintf(stderr, "\t                     the remote viewer)\n");
	fprintf(stderr, "\t--show-scene-h264-with-gaze    to open a window displaying the scene images\n");
	fprintf(stderr, "\t                     (the scene video is transferred as H.264 stream,\n");
	fprintf(stderr, "\t                     decoded by the client and contains a gaze cursor\n");
	fprintf(stderr, "\t                     overlay created by the client)\n");
	fprintf(stderr, "\t--scale-scene f      rescale scene images by multiplying image size by f\n");
	fprintf(stderr, "\t--gaze-data-vrpn-port f        Gaze data VRPN server port \n");
	fprintf(stderr, "\t--gaze-data-vrpn-name f        Gaze data VRPN server name \n");
	fprintf(stderr, "\t--scene-camera-vrpn-port f        Scene camera VRPN server port \n");
	fprintf(stderr, "\t--scene-camera-vrpn-name f        Scene camera VRPN server name \n");
	fprintf(stderr, "\t--server ip_address  connect to the specified remote iViewNG server,\n");
	fprintf(stderr, "\t                     the server can be setup (device parameters) by this\n");
	fprintf(stderr, "\t                     Tutorial-RemoteViewer\n");
	fprintf(stderr, "\t--iviewetg ip_address  connect to the specified remote iViewNG server\n");
	fprintf(stderr, "\t                     which had already been setup by an iViewETG-Client.\n");
	fprintf(stderr, "\t                     This Tutorial-RemoteViewer will not submit any new device\n");
	fprintf(stderr, "\t                     parameters, it will only try to subscribe and listen to\n");
	fprintf(stderr, "\t                     the data it wants. Whether the data can be received\n");
	fprintf(stderr, "\t                     depends on the parameters that iViewETG-Client has set\n");
	fprintf(stderr, "\t--calibrate1pt       to perform a 1-point calibration\n");
	fprintf(stderr, "\t--calibrate3pt       to perform a 3-point calibration\n");
	fprintf(stderr, "\t--samplingrate n     set eye tracking sampling rate to either 30 or 60Hz\n");
	fprintf(stderr, "\t--scene30            scene 30hz\n");
	fprintf(stderr, "\t--scene24            scene 24hz\n");
	fprintf(stderr, "\t                     explicit command; if not specified, server's behavior\n");
	fprintf(stderr, "\t                     defines how it will behave when this client leaves\n");

	exit (-1);
}

/* **************************************************************************************** */

iViewRC ParseCommandLine(int argc, char ** argv) {

	// if not directed to server, use local server
	gServer.connectionType = IVIEW_SERVERADRRESS_SHAREDMEMORY;

	// check each parameter
	for (int i = 1; i < argc; i++) {

		if (argv[i][0] == '-') {

			if (0 == strncmp(argv[i], "--help", MAX(strlen(argv[i]), strlen("--help")))) {
				Usage(argv[0]);
			}

			if (0 == strncmp(argv[i], "--server", MAX(strlen(argv[i]), strlen("--server")))) {

				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--server'");
					Usage(argv[0]);
				}

				gServer.connectionType = IVIEW_SERVERADRRESS_IPV4;
				unsigned int argLen = strlen(argv[i]);
				strncpy(gServer.hostAddress.ipAddress.ipv4, argv[i], MIN(argLen, HOSTADDRESSLENGTH_IPV4 - 1));
				gServer.hostAddress.port = 0;

				printf("Server host: '%s:%u'\n", gServer.hostAddress.ipAddress.ipv4, gServer.hostAddress.port);
				continue;
			}

			if (0 == strncmp(argv[i], "--iviewetg", MAX(strlen(argv[i]), strlen("--iviewetg")))) {

				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--iviewetg'");
					Usage(argv[0]);
				}

				gServer.connectionType = IVIEW_SERVERADRRESS_IPV4;
				unsigned int argLen = strlen(argv[i]);
				strncpy(gServer.hostAddress.ipAddress.ipv4, argv[i], MIN(argLen, HOSTADDRESSLENGTH_IPV4 - 1));
				gServer.hostAddress.port = 0;

				gRemoteIsIviewEtg = 1;

				printf("Server (with iViewETG) host: '%s:%u'\n", gServer.hostAddress.ipAddress.ipv4, gServer.hostAddress.port);
				continue;
			}

			if (0 == strncmp(argv[i], "--samplingrate", MAX(strlen(argv[i]), strlen("--samplingrate")))) {
				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--samplingrate'");
					Usage(argv[0]);
				}

				switch (atoi(argv[i])) {
				case 30:
					gSamplingRate = IVIEWSAMPLERATE_ETG_30;
					break;
				case 60:
					gSamplingRate = IVIEWSAMPLERATE_ETG_60;
					break;
				case 120:
					gSamplingRate = IVIEWSAMPLERATE_ETG_120;
					break;
				default:
					fprintf(stderr, "Invalid sampling rate: neither '30' nor '60' nor '120'.\n");
					Usage(argv[0]);
				}

				continue;
			}

			if (0 == strncmp(argv[i], "--show-gaze", MAX(strlen(argv[i]), strlen("--show-gaze")))) {
				gShowGaze = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--show-eyes", MAX(strlen(argv[i]), strlen("--show-eyes")))) {
				gShowEyeImages = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--show-scene", MAX(strlen(argv[i]), strlen("--show-scene")))) {
				gShowSceneImages = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--show-scene-h264", MAX(strlen(argv[i]), strlen("--show-scene-h264")))) {
				gShowSceneH264 = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--show-scene-with-gaze", MAX(strlen(argv[i]), strlen("--show-scene-with-gaze")))) {
				gShowSceneImagesWithGaze = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--show-scene-h264-with-gaze", MAX(strlen(argv[i]), strlen("--show-scene-h264-with-gaze")))) {
				gShowSceneH264ImagesWithGaze = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--calibrate1pt", MAX(strlen(argv[i]), strlen("--calibrate1pt")))) {
				gCalibrate1Pt = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--calibrate3pt", MAX(strlen(argv[i]), strlen("--calibrate3pt")))) {
				gCalibrate3Pt = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--scale-eyes", MAX(strlen(argv[i]), strlen("--scale-eyes")))) {

				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--scale-eyes'");
					Usage(argv[0]);
				}

				gScaleEyes = stof(argv[i]);
				continue;
			}

			if (0 == strncmp(argv[i], "--scale-scene", MAX(strlen(argv[i]), strlen("--scale-scene")))) {

				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--scale-scene'");
					Usage(argv[0]);
				}

				gScaleScene = stof(argv[i]);
				gScaleSceneSet = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--crossbarAddress", MAX(strlen(argv[i]), strlen("--crossbarAddress")))) {

				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--crossbarAddress'");
					Usage(argv[0]);
				}

				crossbarAddress = argv[i];
				continue;
			}

			if (0 == strncmp(argv[i], "--glassesID", MAX(strlen(argv[i]), strlen("--glassesID")))) {

				if (argc <= ++i) {
					fprintf(stderr, "ERROR: missing argument for parameter '--glassesID'");
					Usage(argv[0]);
				}

				glassesID = argv[i];
				continue;
			}

			if (0 == strncmp(argv[i], "--scene24", MAX(strlen(argv[i]), strlen("--scene24")))) {
				gScene24 = 1;
				continue;
			}
			if (0 == strncmp(argv[i], "--scene30", MAX(strlen(argv[i]), strlen("--scene30")))) {
				gScene30 = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--gazeoverlay", MAX(strlen(argv[i]), strlen("--gazeoverlay")))) {
				gGazeOverlay = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--shutdownserver", MAX(strlen(argv[i]), strlen("--shutdownserver")))) {
				gShutdownServer = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--timemaster", MAX(strlen(argv[i]), strlen("--timemaster")))) {
				gTimeMaster = 1;
				continue;
			}

			if (0 == strncmp(argv[i], "--ica", MAX(strlen(argv[i]), strlen("--ica")))) {

				if (IVIEWSAMPLERATE_ETG_120 == gSamplingRate) {
					fprintf(stderr, "ICA cannot run with 120Hz sampling.\n");
					Usage(argv[0]);
				}

				gEyeExposureMode = IVIEWEYECAMEXPOSUREMODE_ICA;
				continue;
			}

			if (0 == strncmp(argv[i], "--ica-off", MAX(strlen(argv[i]), strlen("--ica-off")))) {
				gEyeExposureMode = IVIEWEYECAMEXPOSUREMODE_NORMAL;
				continue;
			}

			fprintf(stderr, "ERROR: unknown parameter: %s", argv[i]);
			Usage(argv[0]);
		}
	}

	gShowGaze = 1;


	if (!gShowGaze && !gShowEyeImages && !gShowSceneImages && !gShowSceneH264 && !gShowSceneImagesWithGaze && !gShowSceneH264ImagesWithGaze) {
		fprintf(stderr, "ERROR: Refusing to run without showing any data stream!\n");
		Usage(argv[0]);
	}

	if (gShowSceneImages && gShowSceneH264 && gShowSceneImagesWithGaze && gShowSceneH264ImagesWithGaze) {
		fprintf(stderr, "ERROR: Please specify either --show-scene or --show-scene-h264 or --show-scene-with-gaze.\n");
		Usage(argv[0]);
	}

	if ((gCalibrate1Pt || gCalibrate3Pt)
		&& !gShowSceneH264 && !gShowGaze && !gShowSceneImagesWithGaze && !gShowSceneH264ImagesWithGaze) {
		fprintf(stderr, "ERROR: To calibrate, you must specify either --show-scene-with-gaze or --show-scene-h264-with-gaze.\n");
		Usage(argv[0]);
	}

	gServer.device = IVIEWDEVICE_ETG_CAMERAPLAYBACK;

	return RC_NO_ERROR;
}

/* **************************************************************************************** */

iViewRC Setup(bool InitLib) {

	fprintf(stderr, "Starting...");

	if(InitLib)
		CALL_API(iView_Init(IVIEWSDK_IVNG));

	// Start server only if shared memory is used
	if (IVIEW_SERVERADRRESS_SHAREDMEMORY == gServer.connectionType)
		CALL_API(iView_StartServer(NULL, L"--mode etg"));

	fprintf(stderr, "done...\n");

	CALL_API(iView_SetCallback(MyCallback));

	fprintf(stderr, "Connecting...");

	if (gTicketConnect) CALL_API(iView_ReleaseTicket(&gTicketConnect));
	gTicketConnect = NULL;

	CALL_API(iView_CreateTicket(&gTicketConnect));
	CALL_API(iView_Connect(gTicketConnect, &gServer, TICKET_WAIT_MS, 10, NULL));

	fprintf(stderr, "done.\n");

	/* ---------------------------------------------------------------------------- */
	// an example on how to add an own license key
	//
	// this would be the code to send a license key (provided you have a custom license key).
	// but normally, the license is contained in the ETG itself, so you do not need to add
	// a special license key
	//
	//	fprintf(stderr, "Sending license...");
	//
	//	CALL_API(iView_CreateTicket (&gTicketAddLicense));
	//	CALL_API(iView_StartCatchingTickets());
	//	CALL_API(iView_AddLicenseKey (gTicketAddLicense, ""));
	//
	//	if (0 == iView_WaitForTicket (gTicketAddLicense, TICKET_WAIT_MS)) {
	//		printf ("Failed to add license.\n");
	//		return RC_OPERATION_FAILED;
	//	}
	//
	//	fprintf(stderr, "done.\n");


	/* ---------------------------------------------------------------------------- */
	// set time master
	if (gTimeMaster) {
		fprintf(stderr, "Telling Server that this SDK instance is the time master..");
		CALL_API(iView_SetProcessAsTimeMaster());
		fprintf(stderr, "done\n");
	}

	/* ---------------------------------------------------------------------------- */
	// submit device parameters to server

	if (gRemoteIsIviewEtg) {
		fprintf(stderr, "Remote iViewNG-Server is running with iViewETG-Client, will not submit device parameters\n");
		fprintf(stderr, "done.\n");
		return RC_NO_ERROR;
	}

	fprintf(stderr, "Submitting device parameters...");

	iViewDeviceParametersEtgCameraPlayback specificParameters;

	specificParameters.outputDirectory = L".";
	specificParameters.baseFilename = L"Tutorial-RemoteViewer";
	specificParameters.operationMode = IVIEWTRACKINGMODE_CURRENT;

	specificParameters.cameraResolutionScene = IVIEWRESOLUTION_CURRENT;
	specificParameters.samplingRateSceneCam = IVIEWSAMPLINGRATE_CURRENT;
	if (gScene24) {
		specificParameters.cameraResolutionScene = IVIEWRESOLUTION_ETG_1280x960;
		specificParameters.samplingRateSceneCam = IVIEWSAMPLERATE_ETG_24;
	}
	if (gScene30) {
		specificParameters.cameraResolutionScene = IVIEWRESOLUTION_ETG_960x720;
		specificParameters.samplingRateSceneCam = IVIEWSAMPLERATE_ETG_30;
	}

	specificParameters.whiteBalanceProgram = IVIEWWHITEBALANCE_ETG_AUTO;
	specificParameters.audioState = IVIEWTRACKINGMODE_CURRENT;
	specificParameters.overlaySpecList = NULL;

	if (gSamplingRate == IVIEWSAMPLERATE_ETG_120) {
		// set to 120Hz eye tracking
		specificParameters.samplingRateEyeCam = IVIEWSAMPLERATE_ETG_120;
		specificParameters.cameraResolutionEye = IVIEWRESOLUTION_ETG_320x240;
	}
	else if (gSamplingRate == IVIEWSAMPLERATE_ETG_60) {
		// set to 60Hz eye tracking
		specificParameters.samplingRateEyeCam = IVIEWSAMPLERATE_ETG_60;
		specificParameters.cameraResolutionEye = IVIEWRESOLUTION_ETG_320x240;
	}
	else {
		// set to 30Hz eye tracking
		specificParameters.samplingRateEyeCam = IVIEWSAMPLERATE_ETG_30;
		specificParameters.cameraResolutionEye = IVIEWRESOLUTION_ETG_320x240;
	}


	if (gGazeOverlay) {
		fprintf(stderr, "Gaze Overlay mode specified, will set eye frequency to 30Hz, scene frequency to 24Hz\n");

		specificParameters.gazeOverlayMode = IVIEWGAZEOVERLAYMODE_ACTIVATED;

		specificParameters.samplingRateEyeCam = IVIEWSAMPLERATE_ETG_30;
		specificParameters.cameraResolutionEye = IVIEWRESOLUTION_ETG_320x240;

		specificParameters.cameraResolutionScene = IVIEWRESOLUTION_ETG_1280x960;
		specificParameters.samplingRateSceneCam = IVIEWSAMPLERATE_ETG_24;
	}

	specificParameters.eyeCamExposureMode = gEyeExposureMode;


	// because the license might take a while we sleep a bit here
	iView_Sleep(5000);

	iViewDeviceParameters deviceParameters;
	deviceParameters.deviceType = IVIEWDEVICE_ETG_CAMERAPLAYBACK;
	deviceParameters.parameters = &specificParameters;

	CALL_API(iView_CreateTicket(&gTicketDeviceParameters));
	CALL_API(iView_StartCatchingTickets());
	CALL_API(iView_SetDeviceParameters(gTicketDeviceParameters, &deviceParameters));


	if (0 == iView_WaitForTicket(gTicketDeviceParameters, TICKET_WAIT_MS)) {
		printf("Failed to set device parameters.\n");
		return RC_OPERATION_FAILED;
	}

	// Check if device parameters could be set
	if (gTicketDeviceParameters->returnCode != RC_NO_ERROR) {
		const size_t MAX_LEN = 1024;
		wchar_t ticketErrorStr[MAX_LEN];

		size_t len = MAX_LEN;
		iView_iViewRcToString(ticketErrorStr, &len, gTicketDeviceParameters->returnCode);

		wprintf(L"Failed to set device parameters, ticket return code '%s'\n",
			ticketErrorStr);


		if (gTicketDeviceParameters->returnCode == RC_INVALID_LICENSE) {
			fprintf(stderr, "got invalid license, will try with gazeoverlay mode\n");
			fprintf(stderr, "Gaze Overlay mode specified, will set eye frequency to 30Hz, scene frequency to 24Hz\n");

			specificParameters.gazeOverlayMode = IVIEWGAZEOVERLAYMODE_ACTIVATED;

			specificParameters.samplingRateEyeCam = IVIEWSAMPLERATE_ETG_30;
			specificParameters.cameraResolutionEye = IVIEWRESOLUTION_ETG_320x240;

			specificParameters.cameraResolutionScene = IVIEWRESOLUTION_ETG_1280x960;
			specificParameters.samplingRateSceneCam = IVIEWSAMPLERATE_ETG_24;


			iView_ReleaseTicket(&gTicketDeviceParameters);
			CALL_API(iView_CreateTicket(&gTicketDeviceParameters));
			CALL_API(iView_SetDeviceParameters(gTicketDeviceParameters, &deviceParameters));

			// wait some time for directgaze switch
			iView_Sleep(TICKET_WAIT_MS);

			// Check if device parameters could be set
			if (gTicketDeviceParameters->returnCode != RC_NO_ERROR) {
				const size_t MAX_LEN = 1024;
				wchar_t ticketErrorStr[MAX_LEN];

				size_t len = MAX_LEN;
				iView_iViewRcToString(ticketErrorStr, &len, gTicketDeviceParameters->returnCode);

				wprintf(L"Failed to set device parameters, ticket return code '%s'\n",
					ticketErrorStr);

				return gTicketDeviceParameters->returnCode;
			}
			else {
				fprintf(stderr, "done.\n");

				return RC_NO_ERROR;
			}

		}


		return gTicketDeviceParameters->returnCode;
	}

	fprintf(stderr, "done.\n");

	return RC_NO_ERROR;
}

/* **************************************************************************************** */

/**
 * Make sure SetupCalib() is called after Start() as calibration needs enough accumulated
 * scene images.
 */
iViewRC SetupCalib() {

	if (!gCalibrate1Pt && !gCalibrate3Pt)
		return RC_NO_ERROR;

	fprintf(stderr, "Submitting calibration parameters...");

	// We remember the time when we call setup calib, so that we do not propagate
	// calibration points too soon.
	iViewTicket * timeTicket = NULL;
	CALL_API(iView_CreateTicket(&timeTicket));
	CALL_API(iView_GetLocalTime(timeTicket));
	gTimeOfSetupCalibMsec = (int) (((iViewTime *)timeTicket->result->data)->time / 1000000);
	CALL_API(iView_ReleaseTicket(&timeTicket));

	// Sleep a bit to ensure that enough scene frames have been
	// accumulated by the server (necessary for calibration)
	// before sending the calibration setup.
	iView_Sleep(gSetupCalibCooldownMsec);

	// send calibration parameters
	iViewCalibrationParametersEtg specificParametersSetup;
	specificParametersSetup.calibrationDistance = 0;

	iViewCalibrationParameters parametersSetup;

	parametersSetup.type = IVIEWCALIBRATIONTYPE_ETG_1POINTSPHERICAL;
	if (gCalibrate3Pt) {
		parametersSetup.type = IVIEWCALIBRATIONTYPE_ETG_3POINT;
	}

	parametersSetup.applyAtFrame = -1;
	parametersSetup.parameters = &specificParametersSetup;

	iViewTicket * ticket = NULL;
	CALL_API(iView_CreateTicket(&ticket));
	CALL_API(iView_StartCatchingTickets());
	CALL_API(iView_SetupCalibration(ticket, &parametersSetup));

	if (0 == iView_WaitForTicket(ticket, TICKET_WAIT_MS)) {
		printf("Failed to set calibration parameters.\n");
		//return -1;
	}

	// and start calibration
	CALL_API(iView_ReleaseTicket(&ticket));
	CALL_API(iView_CreateTicket(&ticket));
	CALL_API(iView_StartCatchingTickets());
	CALL_API(iView_StartCalibration(ticket));

	if (0 == iView_WaitForTicket(ticket, TICKET_WAIT_MS)) {
		printf("Failed to start calibration.\n");
		//return -1;
	}

	fprintf(stderr, "done.\n");

	return RC_NO_ERROR;
}

/* **************************************************************************************** */

iViewRC Subscribe() {

	fprintf(stderr, "Subscribing data streams...\n");

	/* -------------------------------------------- */
	if (gShowGaze || gShowSceneH264ImagesWithGaze) {

		fprintf(stderr, "gaze information...");

		iViewStreamSubscription subscriptionGaze;
		memset(&subscriptionGaze, 0, sizeof(iViewStreamSubscription));
		subscriptionGaze.streamType = IVIEWDATASTREAM_GAZE_INFORMATION;

		if (gTicketSubscriptionGaze) CALL_API(iView_ReleaseTicket(&gTicketSubscriptionGaze));
		CALL_API(iView_CreateTicket(&gTicketSubscriptionGaze));
		CALL_API(iView_StartCatchingTickets());
		CALL_API(iView_SubscribeDataStream(gTicketSubscriptionGaze, &subscriptionGaze));

		if (0 == iView_WaitForTicket(gTicketSubscriptionGaze, TICKET_WAIT_MS)) {
			printf("Failed to set Subscribe gaze data stream.\n");
			//return -1;
		}

		printf("Subscribe ticket id: %d \n", gTicketSubscriptionGaze->ticketNumber);
	}

	/* -------------------------------------------- */
	if (gShowEyeImages) {

		fprintf(stderr, "eye images...");

		iViewDataStreamSpecSampleRate sampleRateEyes;
		sampleRateEyes.type = IVIEWDATASTREAMSPEC_SAMPLE_RATE;
		sampleRateEyes.next = NULL;
		sampleRateEyes.sampleRate = gSamplerateEyes;

		iViewStreamSubscription subscriptionLeftEye;
		subscriptionLeftEye.streamType = IVIEWDATASTREAM_EYEIMAGES_LEFT;
		subscriptionLeftEye.streamSpec = (iViewDataStreamSpec*)&sampleRateEyes;

		CALL_API(iView_CreateTicket(&gTicketSubscriptionLeftEye));
		CALL_API(iView_StartCatchingTickets());
		CALL_API(iView_SubscribeDataStream(gTicketSubscriptionLeftEye, &subscriptionLeftEye));

		if (0 == iView_WaitForTicket(gTicketSubscriptionLeftEye, TICKET_WAIT_MS)) {
			printf("Failed to set Subscribe left eye stream.\n");
			//return -1;
		}

		iViewStreamSubscription subscriptionRightEye;
		subscriptionRightEye.streamType = IVIEWDATASTREAM_EYEIMAGES_RIGHT;
		subscriptionRightEye.streamSpec = (iViewDataStreamSpec*)&sampleRateEyes;

		CALL_API(iView_CreateTicket(&gTicketSubscriptionRightEye));
		CALL_API(iView_StartCatchingTickets());
		CALL_API(iView_SubscribeDataStream(gTicketSubscriptionRightEye, &subscriptionRightEye));

		if (0 == iView_WaitForTicket(gTicketSubscriptionRightEye, TICKET_WAIT_MS)) {
			printf("Failed to set Subscribe right eye stream.\n");
			//return -1;
		}
	}

	/* -------------------------------------------- */
	if (gShowSceneImages) {

		fprintf(stderr, "scene image (decoded)...");

		iViewDataStreamSpecSampleRate sampleRateScene;
		sampleRateScene.type = IVIEWDATASTREAMSPEC_SAMPLE_RATE;
		sampleRateScene.next = NULL;
		sampleRateScene.sampleRate = gSamplerateScene;

		iViewStreamSubscription subscriptionScene;
		subscriptionScene.streamType = IVIEWDATASTREAM_SCENEIMAGES;
		subscriptionScene.streamSpec = (iViewDataStreamSpec*)&sampleRateScene;

		CALL_API(iView_CreateTicket(&gTicketSubscriptionScene));
		CALL_API(iView_StartCatchingTickets());
		CALL_API(iView_SubscribeDataStream(gTicketSubscriptionScene, &subscriptionScene));

		if (0 == iView_WaitForTicket(gTicketSubscriptionScene, TICKET_WAIT_MS)) {
			printf("Failed to set Subscribe scene stream.\n");
			//return -1;
		}
	}

	/* -------------------------------------------- */
	if (gShowSceneImagesWithGaze) {

		fprintf(stderr, "scene image (decoded) with gaze...");

		iViewDataStreamSpecSampleRate sampleRateScene;
		sampleRateScene.type = IVIEWDATASTREAMSPEC_SAMPLE_RATE;
		sampleRateScene.next = NULL;
		sampleRateScene.sampleRate = gSamplerateScene;

		iViewStreamSubscription subscriptionScene;
		subscriptionScene.streamType = IVIEWDATASTREAM_SCENEIMAGES_WITH_GAZE;
		subscriptionScene.streamSpec = (iViewDataStreamSpec*)&sampleRateScene;

		CALL_API(iView_CreateTicket(&gTicketSubscriptionSceneWithGaze));
		CALL_API(iView_StartCatchingTickets());
		CALL_API(iView_SubscribeDataStream(gTicketSubscriptionSceneWithGaze, &subscriptionScene));

		if (0 == iView_WaitForTicket(gTicketSubscriptionSceneWithGaze, TICKET_WAIT_MS)) {
			printf("Failed to set Subscribe scene stream.\n");
			return -1;
		}
	}

	/* -------------------------------------------- */
	if (gShowSceneH264 || gShowSceneH264ImagesWithGaze) {

		fprintf(stderr, "scene image (H.264)...");

		// For transfer through low quality networks, we better use TCP communication since a single lost
		// packet leaves the video stream unusable until the next I-frame is transmitted successfully.
		iViewDataStreamSpecQualityOfService communicationQuality;
		communicationQuality.type = IVIEWDATASTREAMSPEC_QUALITY_OF_SERVICE;
		communicationQuality.next = NULL;
		communicationQuality.quality = IVIEWQOS_LOSSLESS;

		iViewStreamSubscription subscriptionScene;
		subscriptionScene.streamType = IVIEWDATASTREAM_SCENEIMAGES_H264_DECODED;
		subscriptionScene.streamSpec = (iViewDataStreamSpec*)&communicationQuality;
	
		iView_ReleaseTicket(&gTicketSubscriptionSceneH264);

		CALL_API(iView_CreateTicket(&gTicketSubscriptionSceneH264));
		CALL_API(iView_StartCatchingTickets());
		CALL_API(iView_SubscribeDataStream(gTicketSubscriptionSceneH264, &subscriptionScene));

		if (0 == iView_WaitForTicket(gTicketSubscriptionSceneH264, TICKET_WAIT_MS)) {
			printf("Failed to set Subscribe scene stream.\n");
			return RC_OPERATION_FAILED;
		}
	}

	if (gStreamH264EncodedImages) {
		fprintf(stderr, "scene image (H.264)...");

		// For transfer through low quality networks, we better use TCP communication since a single lost
		// packet leaves the video stream unusable until the next I-frame is transmitted successfully.
		iViewDataStreamSpecQualityOfService communicationQuality;
		communicationQuality.type = IVIEWDATASTREAMSPEC_QUALITY_OF_SERVICE;
		communicationQuality.next = NULL;
		communicationQuality.quality = IVIEWQOS_LOSSLESS;

		iViewStreamSubscription subscriptionScene;
		subscriptionScene.streamType = IVIEWDATASTREAM_SCENEIMAGES_H264;
		subscriptionScene.streamSpec = (iViewDataStreamSpec*)&communicationQuality;

		iView_ReleaseTicket(&gTicketSubscriptionSceneH264);

		CALL_API(iView_CreateTicket(&gTicketSubscriptionSceneH264));
		CALL_API(iView_StartCatchingTickets());
		CALL_API(iView_SubscribeDataStream(gTicketSubscriptionSceneH264, &subscriptionScene));

		if (0 == iView_WaitForTicket(gTicketSubscriptionSceneH264, TICKET_WAIT_MS)) {
			printf("Failed to set Subscribe encoded scene stream.\n");
			return RC_OPERATION_FAILED;
		}
	}

	fprintf(stderr, "done.\n");

	return RC_NO_ERROR;
}

/* **************************************************************************************** */

iViewRC Start() {

	fprintf(stderr, "Starting data acquisition...");

	CALL_API(iView_CreateTicket(&gTicketStartAcquisition));
	CALL_API(iView_StartCatchingTickets());
	CALL_API(iView_StartDataAcquisition(gTicketStartAcquisition));

	if (0 == iView_WaitForTicket(gTicketStartAcquisition, TICKET_WAIT_MS)) {
		printf("Failed to Start data acquisition.\n");
		return RC_OPERATION_FAILED;
	}


	fprintf(stderr, "done.\n");

	if (gCalibrate1Pt) {
		printf("Please click once on the currently fixated point to calibrate.\n");
		gCalibrationPointsToDo = 1;
	}
	if (gCalibrate3Pt) {
		printf("Please click once on the currently fixated point to calibrate 1st point of a 3pt calibration.\n");
		gCalibrationPointsToDo = 3;
	}

	return RC_NO_ERROR;
}

/* **************************************************************************************** */

iViewRC Calibrate(const unsigned int coordinateX, const unsigned int coordinateY) {

	if (gCalibrationPointsToDo == 0)
		return RC_INVALID_STATE;

	// Setup, sending calibration type
	if (gCalibrate1Pt) {
		printf("Sending data for 1-point-calibration (%u/%u) at frame %u.\n", coordinateX, coordinateY,
			gCurrentFrameNumber);
	}
	else if (gCalibrate3Pt) {
		printf("Sending data point %u/3 for 3-point-calibration (%u/%u) at frame %u.\n",
			3 - gCalibrationPointsToDo + 1, coordinateX, coordinateY, gCurrentFrameNumber);
	}
	else {
		return RC_INVALID_STATE;
	}

	iViewCalibrationData1Point calibrationPoint;
	calibrationPoint.coordinateX = coordinateX;
	calibrationPoint.coordinateY = coordinateY;

	iViewCalibrationData data;
	data.type = IVIEWCALIBRATIONDATATYPE_1POINT;
	data.sceneFrameNumber = gCurrentFrameNumber;
	data.unit = IVIEWCALIBRATIONUNIT_PIXEL;
	data.usage = 0;
	data.id = 0;
	data.parameters = &calibrationPoint;

	if (gCalibrate1Pt) {
		CALL_API(iView_CreateTicket(&gTicketCalibration1Pt));
		CALL_API(iView_SetCalibrationData(gTicketCalibration1Pt, &data));
		// We don't want to block here, thus we receive the reply ticket via MyCallback().
	}

	if (gCalibrate3Pt) {
		CALL_API(iView_CreateTicket(&gTicketCalibration3Pt[3 - gCalibrationPointsToDo]));
		CALL_API(iView_SetCalibrationData(gTicketCalibration3Pt[3 - gCalibrationPointsToDo], &data));
		// We don't want to block here, thus we receive the reply ticket via MyCallback().
	}


	gCalibrationPointsToDo--;

	return RC_NO_ERROR;
}

/* **************************************************************************************** */

iViewRC Cleanup() {

	fprintf(stderr, "Unsubscribing data stream...");

	if (gTicketUnsubscription) CALL_API(iView_ReleaseTicket(&gTicketUnsubscription));
	CALL_API(iView_CreateTicket(&gTicketUnsubscription));
	CALL_API(iView_UnsubscribeDataStream(gTicketUnsubscription, IVIEWDATASTREAM_GAZE_INFORMATION));

	fprintf(stderr, "done.\n");

	/* ---------------------------------------------------------------------------- */
	/* stop data acquisition */

	// We will stop acquisition if the server is not being controlled by an iViewETG-Client
	if (gRemoteIsIviewEtg == 0) {
		fprintf(stderr, "Stopping data acquisition...");

		CALL_API(iView_CreateTicket(&gTicketStopAcquisition));
		CALL_API(iView_StopDataAcquisition(gTicketStopAcquisition));

		fprintf(stderr, "done.\n");

		iView_Sleep(1000);
	}

	/* ---------------------------------------------------------------------------- */
	/* shut down */
	fprintf(stderr, "Shutting down API...");
	
	if (gTicketConnect) CALL_API(iView_ReleaseTicket(&gTicketConnect));
	gTicketConnect = NULL;
	if (gTicketAddLicense) CALL_API(iView_ReleaseTicket(&gTicketAddLicense));
	gTicketAddLicense = NULL;
	if (gTicketDeviceParameters) CALL_API(iView_ReleaseTicket(&gTicketDeviceParameters));
	gTicketDeviceParameters = NULL;
	if (gTicketStartAcquisition) CALL_API(iView_ReleaseTicket(&gTicketStartAcquisition));
	gTicketStartAcquisition = NULL;
	if (gTicketSubscriptionGaze) CALL_API(iView_ReleaseTicket(&gTicketSubscriptionGaze));
	gTicketSubscriptionGaze = NULL;
	if (gTicketUnsubscription) CALL_API(iView_ReleaseTicket(&gTicketUnsubscription));
	gTicketUnsubscription = NULL;
	if (gTicketStopAcquisition) CALL_API(iView_ReleaseTicket(&gTicketStopAcquisition));
	gTicketStopAcquisition = NULL;


	// We will terminate the server only if explicit flag has been set.
	if (gShutdownServer) {
		// terminate the iViewNG server and give it some time to shut down
		fprintf(stderr, "\nShuttdown down Server...\n");
		CALL_API(iView_ShutdownServer(30000));
	}

	// shut down the API
	iView_Sleep(1000);

	//	disconnect from server
	CALL_API(iView_Disconnect());

	CALL_API(iView_Shutdown());

	fprintf(stderr, "done.\n");

	return RC_NO_ERROR;

}

#include <conio.h> // for kbhit()

int reconnectionAttempts = 0;

/**
 * Wait for user interaction as the data stream is being received via the callback function.
 */
void WaitForUserInteraction() {

	// Wait for data to arrive via callback
	printf("Press 'ESC' to exit. Press 's' to connect to a different iViewETG Server\n");

	int key = -1;

	bool connectionWarning = false;

	// while key is not 'Escape' and we are not shutting down
	while ((key != 0x1B && key != 'q' && key != 'x' && key != 'Q' && key != 'X') && !ShuttingDown) 
	{
		if (!kbhit()) 
		{
			if (gTicketConnect && gTicketConnect->state == IVIEWTICKETSTATUS_IN_PROGRESS)
			{
				connectionWarning = false;

				if (!WampSetup) {

					iView_Sleep(100 + 200 * reconnectionAttempts);
					reconnectionAttempts++;

					SetUpWAMP();
					WampSetup = true;
				}
				else if (sceneCameraWAMP->IsStoppedUnexpectedly()) {
					sceneCameraWAMP->LeaveSession();
					sceneCameraWAMPThread.join();
					delete sceneCameraWAMP;
					sceneCameraWAMP = nullptr;
					WampSetup = false;
				}

				continue;
			}
			else
			{
				iViewRC rc;
				if (!connectionWarning)
				{
					printf_s("\nConnection to device lost. Attempting to reconnect.... \n");
					iView_ReleaseTicket(&gTicketConnect);
					iView_ReleaseTicket(&gTicketStartAcquisition);
					connectionWarning = true;
					rc = CreateConnectionToDevice(false);
					TheLastTimestamp = time(NULL);
					bRecievingCallback = false;
					continue;
				}

				//For Reconnection Stuff
				double diff = difftime(time(NULL), TheLastTimestamp);
				if (diff > CONNECTION_TIMEOUT_SECONDS)
				{
					connected = false;

					printf_s("\nCan not connect to the device.\nPress r to retry, other keys to exit.... \n");

					key = getch();
					if (key != 'r' && key != 'R')
					{
						break;
					}
					else
					{
						connectionWarning = false;
						continue;
					}
				}



				// no key pressed, just wait.
				iView_Sleep(10);
				continue;
			}		
		}

		// key pressed!
		key = getch();
		if (key == 'q' || key == 'Q')
		{
			break;
		}
		else if (key == 's' || key == 'S')
		{
			cin.clear();
			printf_s("\nEnter the ip of the iViewETG Server.... \n");
			string ip_address = "";
			getline(cin, ip_address);
			gServer.connectionType = IVIEW_SERVERADRRESS_IPV4;
			unsigned int argLen = strlen(ip_address.c_str());
			strncpy(gServer.hostAddress.ipAddress.ipv4, ip_address.c_str(), MIN(argLen, HOSTADDRESSLENGTH_IPV4 - 1));
			gServer.hostAddress.port = 0;

			gRemoteIsIviewEtg = 1;

			Cleanup();
			iView_Sleep(1000);

			iViewRC rc;

			printf("Server (with iViewETG) host: '%s:%u'\n", gServer.hostAddress.ipAddress.ipv4, gServer.hostAddress.port);
			rc = CreateConnectionToDevice(true);
			TheLastTimestamp = time(NULL);
			bRecievingCallback = false;
			continue;
		}

		cin.clear();
		SleepEx(10, FALSE);
	}

	return;
}

/* **************************************************************************************** */
