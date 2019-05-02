#pragma once
#include <vector>

typedef struct {
	std::vector<uint8_t> imageData;
	int isKeyframe;
	int imageWidth;
	int imageHeight;
} EncodedVPXSample;