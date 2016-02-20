#ifndef __FACE_TOOL_H__
#define __FACE_TOOL_H__

#include <iostream>
#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <dirent.h>
#include <unistd.h>

#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;
using namespace cv;

#define negDir "negative_pic"
#define posDir "positive_pic"

#define negTestDir "negative_test"
#define posTestDir "positive_test"

#define trainFileNameX "trainX.txt"
#define trainFileNameY "trainY.txt"

#define adaBoostFinal "adaBoostFinal.txt"

#define DELTA 2
#define SIZE 24
#define SCALE 0.95
#define adaBoostT 200

#define SQR(x) ((x)*(x))

typedef float floatType;

bool getFileNameFromDir(const char *dir, vector<string> &ret);
string createPicName(int no);

class FeatureExtract{

	class Feature{
	public:
		int i, j, x, y, t;
		Feature(){}
		Feature(int i, int j, int x, int y, int t):
			i(i),j(j),x(x),y(y),t(t){}
		floatType getFeature(FeatureExtract *fe);
	};

public:
	vector<Feature> featureIndex;
	floatType pixInt[SIZE+10][SIZE+10];
	int pixIntSquar[SIZE+10][SIZE+10];

	void pixIntegral(Mat &src);
	floatType pixRectInt(int i, int j, int x, int y);

	FeatureExtract();
	FeatureExtract(int *featureSelected);
	vector<floatType> getFeature(Mat &src);
	vector<floatType> getAdaBoostFeature(Mat &src);
};
#endif
