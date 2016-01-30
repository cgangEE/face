#ifndef __FEATURE_EXTRACT_H__
#define __FEATURE_EXTRACT_H__

#include "faceBase.h"

class FeatureExtract{
	int pixInt[SIZE+10][SIZE+10];
public:
	FeatureExtract();
	int pixRectInt(int i, int j, int x, int y);
	vector<int> getFeature(Mat &src);
};

#endif
