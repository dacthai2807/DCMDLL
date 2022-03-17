#ifndef __IMGPROCESSOR__
#define __IMGPROCESSOR__

enum ROTATE_TYPE {
	ROTATE_LEFT,
	ROTATE_RIGHT,
	ROTATE_TOP_DOWN
};

enum FLIP_TYPE {
	FLIP_HORIZONTALLY,
	FLIP_VERTICALLY,
	FLIP_BOTH_AXES
};

enum SCALE_TYPE {
	SCALE_X,
	SCALE_Y
};

enum VOI_LUT_FUNCTION {
	LINEAR,
	SIGMOID,
};

class ImgProcessor {

public: 

	ImgProcessor() {}

	bool rotateImage(const char *inputFilename, const char *outputFilename, const ROTATE_TYPE type);

	bool rotateImage(const char *inputFilename, const char *outputFilename, const signed int degree);

	bool flipImage(const char *inputFilename, const char *outputFilename, const FLIP_TYPE type);

	bool scaleImageByFactor(const char *inputFilename, const char *outputFilename, const SCALE_TYPE type, const float factor);

	bool clipAndScaleImageByFactor(const char *inputFilename, const char *outputFilename, const int left, const int top, const int width, const int height, const SCALE_TYPE type, const float factor);

	bool changeContrastOfImage(const char *inputFilename, const char *outputFilename);

	bool convertToBMPFile(const char *inputFilename, const char *outputFilename);

	bool applyExistingVOIWindows(const char *inputFilename, const char *outputFilename, const signed int numOfWindows, const VOI_LUT_FUNCTION voiLutFunc = LINEAR);

	bool applyMinMaxVOIWindows(const char *inputFilename, const char *outputFilename, const VOI_LUT_FUNCTION voiLutFunc = LINEAR);

	bool applyHistogramVOIWindows(const char *inputFilename, const char *outputFilename, const signed int ignoringPercent, const VOI_LUT_FUNCTION voiLutFunc = LINEAR);

	bool applySettingVOIWindows(const char *inputFilename, const char *outputFilename, const float center, const float width, const VOI_LUT_FUNCTION voiLutFunc = LINEAR);

	~ImgProcessor() {}
};

#endif
