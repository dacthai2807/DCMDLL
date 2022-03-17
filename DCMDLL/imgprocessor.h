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

class ImgProcessor {

public: 

	ImgProcessor() {}

	bool rotateImage(const char *inputFilename, const char *outputFilename, const ROTATE_TYPE type);

	bool flipImage(const char *inputFilename, const char *outputFilename, const FLIP_TYPE type);

	bool scaleImageByFactor(const char *inputFilename, const char *outputFilename, const SCALE_TYPE type, const float factor);

	bool clipAndScaleImageByFactor(const char *inputFilename, const char *outputFilename, const int left, const int top, const int width, const int height, const SCALE_TYPE type, const float factor);

	bool changeContrastOfImage(const char *inputFilename, const char *outputFilename);

	~ImgProcessor() {}
};

#endif
