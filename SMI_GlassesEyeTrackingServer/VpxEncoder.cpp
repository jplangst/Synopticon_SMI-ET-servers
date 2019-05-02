#include "VpxEncoder.h"

VpxEncoder::VpxEncoder()
{
	initialized = false;

	fps = 1;
}

VpxEncoder::~VpxEncoder()
{
	if (initialized) {
		// Flush encoder.
		/*while (encode_frame(codec, NULL, -1, 0)) {
		}*/

		printf("Processed %d frames.\n", frame_count);

		delete[] outputBuffer;

		if (codec && vpx_codec_destroy(codec)) {
			//die_codec(&codec, "Failed to destroy codec.");
		}
	}
}

bool VpxEncoder::initEncoder(int _imageWidth, int _imageHeight) {
	bool initSuccessfull = false;

	imageWidth = _imageWidth;
	imageHeight = _imageHeight;

	const vpx_codec_iface_t* iface = VPX_INTERFACE;
	if (!iface) {
		//Could not initialize interface
	}

	vpx_codec_enc_cfg_t config;
	vpx_codec_err_t res = vpx_codec_enc_config_default(iface, &config, 0);
	if (res) {
		//Could not configure codec
	}

	config.rc_target_bitrate = imageWidth * imageHeight *
		config.rc_target_bitrate / config.g_w / config.g_h;

	config.g_w = imageWidth;
	config.g_h = imageHeight;
	config.g_threads = 1;
	config.rc_target_bitrate = 2000;


	codec = new vpx_codec_ctx_t;

	res = vpx_codec_enc_init(codec, iface, &config, 0);
	if (res)
	{
		delete codec;
		codec = nullptr;
		//Failed to initialise codec
		//THROW("failed initialize codec: %s", vpx_codec_err_to_string(res));
	}

	frame_count = 0;	
	frames_encoded = 0;

	keyframe_interval = 15;
	

	outputBuffer = new uint8_t[200000];

	initSuccessfull = true;
	initialized = initSuccessfull;

	return initSuccessfull;
}

int VpxEncoder::encode_frame(vpx_codec_ctx_t *codec, vpx_image_t *img, int frame_index, int flags) { 
																			
	const vpx_codec_err_t res = vpx_codec_encode(codec, img, frame_index, 1, flags, VPX_DL_REALTIME);
	if (res != VPX_CODEC_OK) {
		//die_codec(codec, "Failed to encode frame");
		cerr << "Failed to encode frame";
	}
	
	int got_pkts = 0;
	vpx_codec_iter_t iter = NULL;
	const vpx_codec_cx_pkt_t *pkt = NULL; //Encoder output

	size_t bytes_written = 0;
	int keyframe;
	while ((pkt = vpx_codec_get_cx_data(codec, &iter)) != NULL) {
		if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
			got_pkts = 1;
			keyframe = (pkt->data.frame.flags & VPX_FRAME_IS_KEY) != 0;
			//printf(keyframe ? "K" : ".");

			memcpy(outputBuffer, pkt->data.frame.buf, pkt->data.frame.sz);

			outputBuffer += pkt->data.frame.sz;
			bytes_written += pkt->data.frame.sz;				
		}
	}

	if (got_pkts) {
		std::vector<uint8_t> frameBuffer;

		outputBuffer -= bytes_written;
		frameBuffer.assign(outputBuffer, outputBuffer + bytes_written);

		EncodedVPXSample newSample;
		newSample.imageData = frameBuffer;
		newSample.imageWidth = imageWidth;
		newSample.imageHeight = imageHeight;
		newSample.isKeyframe = keyframe; //Not needed anymore I think

		DataContainer::getInstance()->addVPEncodedSceneCameraSample(newSample);
	}

	return got_pkts;
}

void VpxEncoder::encodeFrame() {
	shouldStop = false;

	uint8_t* outYuvImage = nullptr;

	int framesRecieved = 0;

	while (!shouldStop) {	
		iViewDataStreamSceneImage imageSample;
		bool hasSample = DataContainer::getInstance()->getSceneCameraSample(imageSample);

		if (hasSample)
		{
			/*if (framesRecieved % fps == 0)
			{*/		
				if (!initialized) {
					initEncoder(imageSample.imageData->width, imageSample.imageData->height);

					//Should be deleted after use is over
					outYuvImage = new uint8_t[imageWidth * imageHeight * 3 / 2]; 
				}
			
				int flags = 0;
				if (keyframe_interval > 0 && frame_count % keyframe_interval == 0) {
					flags = VPX_EFLAG_FORCE_KF;
				}

				uint8_t* yPlane = outYuvImage;
				uint8_t* uPlane = yPlane + imageWidth * imageHeight;
				uint8_t* vPlane = uPlane + (imageWidth * imageHeight / 4);

				int result = 0;
				//RAWToI420
				//RGB24ToI420
				result = libyuv::RGB24ToI420((uint8_t*)imageSample.imageData->imageData, imageSample.imageData->widthStep,
					yPlane, imageWidth, uPlane, imageWidth / 2, vPlane, imageWidth / 2,
					imageWidth, imageHeight); //-height to vertically flip image?

				//vpx_img_wrap(raw, VPX_IMG_FMT_I420, imageWidth, imageHeight, 1, outYuvImage);
				vpx_image_t img_wrapper;
				//VPX_IMG_FMT_UYVY
				//VPX_IMG_FMT_I420
				vpx_img_wrap(&img_wrapper, VPX_IMG_FMT_I420, imageWidth, imageHeight, 1, outYuvImage);
			
				if (encode_frame(codec, &img_wrapper, frame_count++, flags)) {
					frames_encoded++;
				}		

				//delete outYuvImage;

				//break;
			//}

			framesRecieved++;
		}

		SleepEx(50, false);
	}

	//Cleanup
	delete outYuvImage;
}

void VpxEncoder::stopEncoding() {
	shouldStop = true;
}