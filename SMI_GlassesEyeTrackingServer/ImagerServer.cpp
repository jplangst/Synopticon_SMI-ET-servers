// VRPN Imager Server example program.

#include "ImagerServer.h"               

ImagerServer::ImagerServer() 
{
	compressedBufferData = (uint8_t*) malloc(gCompressedBufferSize);
	ShouldLeave = false;
}

ImagerServer::~ImagerServer() {
	free(compressedBufferData);
}

uint32_t ImagerServer::encodeImage(iViewImage &image, uint8_t ** &OutCompressedBuffer) {
	float quality = 1;
	uint32_t compressedBufferSize;

	ImageWidth = image.width;
	ImageHeight = image.height;
	Stride = image.widthStep;

	compressedBufferSize = WebPEncodeBGRA((uint8_t*)image.imageData, image.width, image.height, image.widthStep, quality, OutCompressedBuffer);

	return compressedBufferSize;
}

void ImagerServer::updateServer()
{
	while (!ShouldLeave) {
		if (!imageReady) {
			iViewDataStreamSceneImage imageSample;
			bool hasSample = DataContainer::getInstance()->getSceneCameraSample(imageSample);

			if (hasSample)
			{
				uint8_t * tmpBuffer = nullptr;
				uint8_t ** CompressedBuffer = &tmpBuffer;

				compressedBufferSize = encodeImage(*imageSample.imageData, CompressedBuffer);

				if (compressedBufferSize > gCompressedBufferSize) {
					std::cerr << "Compressed Image too big. Size was: " << compressedBufferSize << std::endl;
					std::cerr << "Max size is: " << gCompressedBufferSize << std::endl;
				}

				if (compressedBufferSize <= 0)
				{
					WebPFree(tmpBuffer);
				}
				else
				{
					memcpy(compressedBufferData, tmpBuffer, compressedBufferSize);
					WebPFree(tmpBuffer);
					imageReady = true;
				}
			}
		}
	}
}

bool ImagerServer::isImageReady() {
	return imageReady;
}

std::vector<uint8_t> ImagerServer::getImage() {
	std::vector<uint8_t> image;
	image.assign(compressedBufferData, compressedBufferData + compressedBufferSize);
	return image;
}

uint8_t* ImagerServer::getImage(uint32_t &outBufferSize) {
	outBufferSize = compressedBufferSize;
	return compressedBufferData;
}

void ImagerServer::prepareNextImage() {
	imageReady = false;
}

void ImagerServer::getImageProperties(int &OutWidth, int &OutHeight, int &OutStride){
	OutWidth = ImageWidth;
	OutHeight = ImageHeight;
	OutStride = Stride;
}

void ImagerServer::StopServer() {
	ShouldLeave = true;
}