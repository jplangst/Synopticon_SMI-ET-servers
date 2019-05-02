/**
	File: Utils.h
	Author: Jens-Patrick Langstrand
*/

#ifndef __Utils_h__
#define __Utils_h__

#include <string.h>
#include <sstream>
#include <time.h> 
#include "iViewNG-DataStreams.h"
#include "iViewNG-Utility.h"

using namespace std;

class Utils
{
	
public:

	/*
		Get system time as string
		Return YYYMMDD_HHMMSS
	*/
	static string getSystemTimeAsString();

	/**
		Get the contents of an iViewVector3D as a string
		@param 
			vector - the iViewVector3D object to convert
		@return
			the contents of the vector as a string
	*/
	static string getVectorAsString(iViewVector3D vector);

	/**
		Get the contents of an iViewVector2D as a string
		@param 
			point - the iViewVector2D object to convert
		@return
			the contents of the point as a string
	*/
	static string getPointAsString(iViewVector2D point);

	/**
		Get the contents of an iViewEyeSample as a string
		@param 
			eye - the iViewEyeSample object to convert
		@return
			the contents of the eye object as a string
	*/
	static string getEyeDataAsString(iViewEyeSample eye);

	/*
		Get log file name with timestamp
		@param 
			logfilename - log file name (e.g., positiontracker.log)
		@return
			log file name with timestamp (e.g., positiontracker_20150722_094916.log)
	*/
	static string getLogFileNameWithTimeStamp(string logFileName);

	/**
		Get the contents of an iViewDataStreamGazeSample as a string
		@param 
			sample - the iViewDataStreamGazeSample object to convert
		@return
			the contents of the gaze sample object as a string
	*/
	static string getSampleAsString(iViewDataStreamGazeSample sample);

	/**
		Check the value of the number to see if its Nan or Inf
		@param 
			number - the number to check
		@return
			Nan if the number is Nan, Inf if it is in Inf and the number as a string if it is ok
	*/
	static string checkNumber(double number);

	//Check if the sample contains NAN. Returns true if a value in the sample is NAN
	static bool isSampleValid(iViewEyeSample sample);
};

#endif //Utils_h