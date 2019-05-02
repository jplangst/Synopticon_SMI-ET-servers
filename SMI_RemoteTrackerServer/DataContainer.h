#ifndef __DataContainer_h__
#define __DataContainer_h__

#include <iostream>
#include <vector>
#include "CircularFIFOQueue.hpp"

#include "iViewXAPI.h"

using namespace memory_sequential_consistent;

using namespace std;

const double FIXATION_VELOCITY_THRESHOLD = 50; //distance/sec 
const double SACCADE_VELOCITY_THRESHOLD = 100; //distance/sec
const double FIXATION_DISTANCE_THRESHOLD = 25;
const double FIXATION_DURATION_THRESHOLD = 100; //ms
//Paper1 - shortest duration for fixation: 100ms
//Paper2 - duration: 180 - 200ms for visual search, 225-250ms for reading, 260-330ms for scene viewing

struct FixationPoint
{
	double x;
	double y;
	double duration;
	FixationPoint(double _x, double _y, double _duration)
	{
		x = _x;
		y = _y;
		duration = _duration;
	}
};

class FixationGroup 
{
private:
	vector<FixationPoint> points;
	long long timestamp; //timestamp of the fisrt sample
public:
	FixationGroup(long long _timestamp);
	~FixationGroup();
	pair<double, double> getCentroid();
	double getDuration();
	long long getTimestamp() { return timestamp; }
	void addPoint(FixationPoint p);
};

class DataContainer
{

private:
	static DataContainer* instance;
	static bool instanceFlag;
	DataContainer() {
		currentFixationGroup = nullptr;
		firstSample = true;
	}

	//Circular gaze sample list
	CircularFifo<SampleStruct, 10> cGazeSamples;
	CircularFifo<FixationGroup*, 10> cGazeFixations;
	double pX; //previous x
	double pY; //previous y
	double pZ; //previous z
	long long ptimestamp; //previous timestamp

	double distance(double x1, double y1, double x2, double y2);
	double visualDegree(double x1, double y1, double x2, double y2, double z);
	void calculateFixations(SampleStruct sample);
	bool firstSample;
	FixationGroup* currentFixationGroup;
public:

	static DataContainer* getInstance();
	~DataContainer()
	{
		if (!currentFixationGroup)
		{
			delete currentFixationGroup;
			currentFixationGroup = nullptr;
		}
		instanceFlag = false;
	}

	//Add a gaze sample to current write list 
	void addGazeSample(SampleStruct sample);
	//Read a sample from current read list
	bool getGazeSample(SampleStruct &OutSample);
	bool getGazeFixation(FixationGroup* &OutFixation);
};

#endif
