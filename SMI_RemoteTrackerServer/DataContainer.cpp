#include "DataContainer.h"


bool DataContainer::instanceFlag = false;
DataContainer* DataContainer::instance = NULL;

DataContainer* DataContainer::getInstance()
{
	if (!instanceFlag)
	{
		instance = new DataContainer();
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

void DataContainer::addGazeSample(SampleStruct sample)
{
	bool result = cGazeSamples.push(sample);
	if (!result) { //Failed to add sample because container is full
		//TODO should the sample be deleted?
	}
	calculateFixations(sample);
}

bool DataContainer::getGazeSample(SampleStruct &OutSample)
{
	return cGazeSamples.pop(OutSample);
}

bool DataContainer::getGazeFixation(FixationGroup* &OutSample)
{
	return cGazeFixations.pop(OutSample);
}

void DataContainer::calculateFixations(SampleStruct sample)
{
	double x = 0;
	double y = 0;
	double z = 0;
	double gx = 0;
	double gy = 0;

	if (!(abs(sample.leftEye.diam) < DBL_MIN) && !(abs(sample.rightEye.diam) < DBL_MIN)) {
		//both eyes are valid
		x = (sample.leftEye.eyePositionX + sample.rightEye.eyePositionX) / 2;
		y = (sample.leftEye.eyePositionY + sample.rightEye.eyePositionY) / 2;
		gx = (sample.leftEye.gazeX + sample.rightEye.gazeX) / 2;
		gy = (sample.leftEye.gazeY + sample.rightEye.gazeY) / 2;
		z = (sample.leftEye.eyePositionZ + sample.rightEye.eyePositionZ) / 2;
	}
	else if (!(abs(sample.leftEye.diam) < DBL_MIN)) {
		//only left eye is valid
		x = sample.leftEye.eyePositionX;
		y = sample.leftEye.eyePositionY;
		gx = sample.leftEye.gazeX;
		gy = sample.leftEye.gazeY;
		z = sample.leftEye.eyePositionZ;
	}
	else if (!(abs(sample.rightEye.diam) < DBL_MIN)) {
		//only right eye is valid
		x = sample.rightEye.eyePositionX;
		y = sample.rightEye.eyePositionY;
		gx = sample.rightEye.gazeX;
		gy = sample.rightEye.gazeY;
		z = sample.rightEye.eyePositionZ;
	}
	else {
		//both eyes are invalid
		return;
	}
	if (!firstSample) {
		double deltaTime = (sample.timestamp - ptimestamp)/1000;
		double velocity = distance(gx, gy, pX, pY) / (deltaTime);
		//double velocity = visualDegree(x, y, pX, pY, z) / (deltaTime / 1000);
		
		if (velocity < FIXATION_VELOCITY_THRESHOLD) {
			if (!currentFixationGroup)
			{
				currentFixationGroup = new FixationGroup(sample.timestamp);
			}
			/*std::cerr << "Add fixation point " << velocity << std::endl;*/
			if (distance(gx, gy, pX, pY) > FIXATION_DISTANCE_THRESHOLD)
			{
				if (currentFixationGroup->getDuration() >= FIXATION_DURATION_THRESHOLD)
				{
					//std::cerr << "Add fixation group " << currentFixationGroup->getDuration() << std::endl;
					cGazeFixations.push(currentFixationGroup);
				}
				else
				{
					delete currentFixationGroup; //disregard noise
				}
				currentFixationGroup = new FixationGroup(sample.timestamp);
			}
			currentFixationGroup->addPoint(FixationPoint(gx, gy, deltaTime));
		}
		else if (velocity > SACCADE_VELOCITY_THRESHOLD) {
			if (currentFixationGroup) {
				if (currentFixationGroup->getDuration() >= FIXATION_DURATION_THRESHOLD)
				{
					//std::cerr << "Add fixation group saccade " << currentFixationGroup->getDuration() << std::endl;
					cGazeFixations.push(currentFixationGroup);
				}
				else
				{
					/*std::cerr << "Delete fixation group " << currentFixationGroup->getDuration() << std::endl;*/
					delete currentFixationGroup; //disregard noise
				}
				currentFixationGroup = nullptr;
			}
		}
	}
	else {
		firstSample = false;
	}
	//pX = x;
	//pY = y;
	pX = gx;
	pY = gy;
	ptimestamp = sample.timestamp;
}

double DataContainer::distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double DataContainer::visualDegree(double x1, double y1, double x2, double y2, double z)
{
	/*std::cerr << "s " << distance(x1, y1, x2, y2) << std::endl;
	std::cerr << "d " << z << std::endl;
	std::cerr << "a " << (2 * atan(distance(x1, y1, x2, y2) / z)) * 180 / 3.14159265358979323846 << std::endl;*/
	return (2 * atan(distance(x1, y1, x2, y2) / (2*z)))*180/3.14159265358979323846;
}

FixationGroup::FixationGroup(long long _timestamp)
{
	timestamp = _timestamp;
}

FixationGroup::~FixationGroup()
{

}

pair<double, double> FixationGroup::getCentroid()
{
	double c_x = 0;
	double c_y = 0;
	double n = points.size();
	for (FixationPoint p : points)
	{
		//c_x += p.x*p.duration;
		//c_y += p.y*p.duration;
		//mass += p.duration;
		c_x += p.x;
		c_y += p.y;
	}
	if (n != 0)
	{
		return make_pair<double, double>(c_x / n, c_y / n);
	}
	return make_pair<double, double>(-1, -1);
}

double FixationGroup::getDuration()
{
	double duration = 0;
	for (int i = 1; i < points.size(); i++)
	//for (FixationPoint p : points)
	{
		FixationPoint p = points[i];
		duration += p.duration;
	}
	return duration;
}

void FixationGroup::addPoint(FixationPoint p)
{
	points.push_back(p);
}