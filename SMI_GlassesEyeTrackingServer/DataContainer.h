#ifndef __DataContainer_h__
#define __DataContainer_h__

#include <iostream>
#include <iViewNG-Utility.h>
#include <vector>
#include "circularfifo_memory_sequential_consistent.hpp"
#include "CustomDatatypes.h"

using namespace memory_sequential_consistent;

using namespace std;

class DataContainer
{

private:
	static DataContainer* instance;
	static bool instanceFlag;
	DataContainer(){

	}

	//Circular gaze sample list
	CircularFifo<iViewDataStreamGazeSample, 10> cGazeSamples;

	//SceneCamera sample list
	CircularFifo<iViewDataStreamSceneImage, 2> cSceneImageSamples;

	//Encoded SceneCamera sample list
	CircularFifo<iViewDataStreamH264SceneImage, 2> cEncodedSceneImageSamples;

	//VP9 encoded SceneCamera sample list
	CircularFifo<EncodedVPXSample, 2> cVPCustomEncodedSceneCameraSamples;

public:

	static DataContainer* getInstance();
	~DataContainer()
	{
		instanceFlag = false;
	}

	//Add a gaze sample to current write list 
	void addGazeSample(iViewDataStreamGazeSample sample);
	//Read a sample from current read list
	bool getGazeSample(iViewDataStreamGazeSample &OutSample);
	//iViewDataStreamGazeSample DataContainer::getGazeSample();

	void addSceneCameraSample(iViewDataStreamSceneImage imageSample);
	bool getSceneCameraSample(iViewDataStreamSceneImage &OutImageSample);

	void addEncodedSceneCameraSample(iViewDataStreamH264SceneImage imageSample);
	bool getEncodedSceneCameraSample(iViewDataStreamH264SceneImage &OutImageSample);

	void addVPEncodedSceneCameraSample(EncodedVPXSample imageSample);
	bool getVPEncodedSceneCameraSample(EncodedVPXSample &OutImageSample);
};

#endif
