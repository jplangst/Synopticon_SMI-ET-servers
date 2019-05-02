#include "Utils.h"

string Utils::checkNumber(double number)
{
	std::stringstream ss;

	if(iView_IsNan(number))
	{
		ss << "Nan";
	}
	else if(iView_IsInf(number))
	{
		ss << "Inf";
	}
	else
	{
		ss << number;
	}

	std::string ts = ss.str();
	return ts;
}

string Utils::getSampleAsString(iViewDataStreamGazeSample sample)
{
	std::stringstream ss;
	ss << sample.timestamp << " " << sample.eyeFrameNumber << " " << sample.sceneFrameNumber << " " << getPointAsString(sample.pointOfRegard) << " " 
		<< getEyeDataAsString(sample.leftEye) << " " << getEyeDataAsString(sample.rightEye);
	std::string ts = ss.str();
	return ts;
}

string Utils::getLogFileNameWithTimeStamp(string logFileName)
{
	size_t found = logFileName.find_last_of( ".");
	if(found != string::npos)
	{
		string fileName = logFileName.substr(0, found);
		string extension = logFileName.substr(found+1);
		return (fileName + "_" + getSystemTimeAsString() + "." + extension);
	}
	return "logs/" + logFileName + "_" + getSystemTimeAsString() + ".log";
}

string Utils::getSystemTimeAsString()
{
	time_t timer;
	time(&timer);
	std::stringstream ss;
	ss << timer;
	std::string ts = ss.str();
	return ts;
}

string Utils::getVectorAsString(iViewVector3D vector)
{
	std::stringstream ss;
	ss << checkNumber(vector.x) << "," << checkNumber(vector.y) << "," << checkNumber(vector.z);
	std::string ts = ss.str();
	return ts;
}

string Utils::getPointAsString(iViewVector2D point)
{
	std::stringstream ss;
	ss << checkNumber(point.x) << "," << checkNumber(point.y);
	std::string ts = ss.str();
	return ts;
}

string Utils::getEyeDataAsString(iViewEyeSample eye)
{
	std::stringstream ss;
	ss << getVectorAsString(eye.gazeBasePoint) << " " << getVectorAsString(eye.gazeDirection) << " " << getPointAsString(eye.pointOfRegard) << " "
		<< checkNumber(eye.pupilConfidence) << " " << getPointAsString(eye.pupilDiameter) << " " << checkNumber(eye.pupilRadius);
	std::string ts = ss.str();
	return ts;
}

bool Utils::isSampleValid(iViewEyeSample sample)
{
	iViewVector3D gazeBasePoint = sample.gazeBasePoint;

	if(iView_IsNan(gazeBasePoint.x))
	{
		return false;
	}
	else if (iView_IsNan(gazeBasePoint.y))
	{
		return false;
	}
	else if(iView_IsNan(gazeBasePoint.y))
	{
		return false;
	}


	iViewVector3D gazeDirection = sample.gazeDirection;

	if(iView_IsNan(gazeDirection.x))
	{
		return false;
	}
	else if (iView_IsNan(gazeDirection.y))
	{
		return false;
	}
	else if(iView_IsNan(gazeDirection.y))
	{
		return false;
	}
	

	return true;
}