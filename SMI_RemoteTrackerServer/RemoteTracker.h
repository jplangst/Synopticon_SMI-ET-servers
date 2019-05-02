#pragma once
#include "iViewXAPI.h"
#include "vrpn_Tracker.h"

class RemoteTracker : public vrpn_Tracker
{
	struct timeval _timestamp;
public:
	RemoteTracker(vrpn_Connection *c, char* name = "RemoteTrackerServer");
	~RemoteTracker();

	//Stream the passed sample through vrpn
	void StreamSample(const SampleStruct &Sample, const TrackingStatusStruct &TrackingStatus);

	//Not used
	virtual void mainloop();
};

