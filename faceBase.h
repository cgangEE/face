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

#define trainFileNameX "trainX.txt"
#define trainFileNameY "trainY.txt"

#define adaBoostFinal "adaBoostFinal.txt"

#define DELTA 4
#define SIZE 20
#define SCALE 0.9
#define adaBoostT 15

bool getFileNameFromDir(const char *dir, vector<string> &ret);
string createPicName(int no);

class FeatureExtract{

	class Feature{
		int i, j, x, y, t;
	public:
		Feature(){}
		Feature(int i, int j, int x, int y, int t):
			i(i),j(j),x(x),y(y),t(t){}
		int getFeature(FeatureExtract *fe);
	};

public:
	vector<Feature> featureIndex;
	int pixInt[SIZE+10][SIZE+10];
	void pixIntegral(Mat &src);
	int pixRectInt(int i, int j, int x, int y);


	FeatureExtract();
	FeatureExtract(int *featureSelected);
	vector<int> getFeature(Mat &src);
	vector<int> getAdaBoostFeature(Mat &src);
};
#endif
