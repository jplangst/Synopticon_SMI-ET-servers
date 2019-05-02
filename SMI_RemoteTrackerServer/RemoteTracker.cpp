#include "RemoteTracker.h"


RemoteTracker::RemoteTracker(vrpn_Connection *c, char* name) : vrpn_Tracker(name, c)
{

}

RemoteTracker::~RemoteTracker()
{

}

void RemoteTracker::StreamSample(const SampleStruct &Sample, const TrackingStatusStruct &TrackingStatus)
{
	vrpn_gettimeofday(&_timestamp, NULL);

	long seconds = (long)(Sample.timestamp / 1000);
	long microseconds = Sample.timestamp % 1000;
	
	vrpn_Tracker::timestamp = timeval();
	vrpn_Tracker::timestamp.tv_sec = seconds;
	vrpn_Tracker::timestamp.tv_usec = microseconds;

	// We will just put a fake data in the position of our tracker
	static float angle = 0; angle += 0.001f;
	static int maxX = 0;
	static int maxY = 0;
	static int minX = 0;
	static int minY = 0;

	//Package the left eye data
	EyeDataStruct LeftEye = Sample.leftEye;
	pos[0] = LeftEye.eyePositionX;
	pos[1] = LeftEye.eyePositionY;
	pos[2] = LeftEye.eyePositionZ;

	d_quat[0] = LeftEye.gazeX;
	d_quat[1] = LeftEye.gazeY;
	d_quat[2] = 0;
	d_quat[3] = 0;

	char leftEyeMsgBuf[1000];

	d_sensor = 0;
	//Encode the messages
	int  len = vrpn_Tracker::encode_to(leftEyeMsgBuf);

	//Pack the messages
	if (d_connection->pack_message(len, timestamp, position_m_id, d_sender_id, leftEyeMsgBuf,
		vrpn_CONNECTION_LOW_LATENCY))
	{
		fprintf(stderr, "can't write message: tossing\n");
	}

	//Package the right eye data
	EyeDataStruct RightEye = Sample.rightEye;
	pos[0] = RightEye.eyePositionX;
	pos[1] = RightEye.eyePositionY;
	pos[2] = RightEye.eyePositionZ;

	d_quat[0] = RightEye.gazeX;
	d_quat[1] = RightEye.gazeY;
	d_quat[2] = 0;
	d_quat[3] = 0;

	d_sensor = 1;
	char rightEyeMsgBuf[1000];

	//Encode the messages
	len = vrpn_Tracker::encode_to(rightEyeMsgBuf);

	//Pack the messages
	if (d_connection->pack_message(len, timestamp, position_m_id, d_sender_id, rightEyeMsgBuf,
		vrpn_CONNECTION_LOW_LATENCY))
	{
		fprintf(stderr, "can't write message: tossing\n");
	}

	//Pack the pupil data
	d_quat[0] = TrackingStatus.leftEye.validity;
	d_quat[1] = Sample.leftEye.diam*0.5; //convert the diameter(mm) to radius(mm)
	d_quat[2] = TrackingStatus.rightEye.validity;
	d_quat[3] = Sample.rightEye.diam*0.5; //convert the diameter(mm) to radius(mm)

	d_sensor = 2;
	char PupilMsgBuf[1000];

	//Encode the messages
	len = vrpn_Tracker::encode_to(PupilMsgBuf);

	//Pack the messages
	if (d_connection->pack_message(len, timestamp, position_m_id, d_sender_id, PupilMsgBuf,
		vrpn_CONNECTION_LOW_LATENCY))
	{
		fprintf(stderr, "can't write message: tossing\n");
	}
}

void RemoteTracker::mainloop()
{
	server_mainloop();
}