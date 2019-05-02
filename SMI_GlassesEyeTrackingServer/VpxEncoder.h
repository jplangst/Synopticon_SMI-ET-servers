#pragma once

#include <iViewNG-Core.h>
#include <iViewNG-DataStreams.h>
#include "DataContainer.h"

#define VPX_CODEC_DISABLE_COMPAT 1
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#define VPX_INTERFACE (vpx_codec_vp8_cx())

#include "CustomDatatypes.h"

#include <Windows.h>

#include "libyuv.h"


class VpxEncoder
{
	uint8_t* outputBuffer;

	bool initialized;
	bool shouldStop;

	vpx_codec_ctx_t* codec;

	//vpx_image_t* raw;
	vpx_codec_err_t res;
	int frame_count;
	int fps;
	int bitrate;
	int keyframe_interval;
	int frames_encoded;

	int imageWidth;
	int imageHeight;

	bool initEncoder(int imageWidth, int imageHeight);
	int encode_frame(vpx_codec_ctx_t *codec, vpx_image_t *img, int frame_index, int flags);

public:
	VpxEncoder();
	~VpxEncoder();

	void encodeFrame();
	void stopEncoding();

	int32 getImageWidth() { return imageWidth; }
	int32 getImageHeight() { return imageHeight; }
};

