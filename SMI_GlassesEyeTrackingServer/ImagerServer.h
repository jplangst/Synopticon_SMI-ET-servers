#pragma once

#include <stdio.h>       // for fprintf, NULL, stderr
#include <vector>
#include <iViewNG-Core.h>
#include <iViewNG-DataStreams.h>
#include "DataContainer.h"

//Image compression
#include "webp/encode.h"
#include <stdlib.h>
#include <vector>

class ImagerServer {
private:	
	const uint32_t gCompressedBufferSize = 800000;
	bool imageReady;
	uint32_t compressedBufferSize;
	uint8_t* compressedBufferData;
	int ImageHeight;
	int ImageWidth;
	int Stride;

	bool ShouldLeave;

	uint32_t encodeImage(iViewImage &image, uint8_t ** &OutCompressedBuffer);
public:
	ImagerServer();
	~ImagerServer();

	void getImageProperties(int &OutWidth, int &OutHeight, int &OutStride);

	std::vector<uint8_t> getImage();
	uint8_t* getImage(uint32_t &outBufferSize);
	void prepareNextImage();
	bool isImageReady();
	void updateServer();

	void StopServer();
};

