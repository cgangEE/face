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

#define adaBoostFinal "addBoostFinal.txt"

#define DELTA 5
#define SIZE 50
#define SCALE 0.9
#define adaBoostT 20

bool getFileNameFromDir(const char *dir, vector<string> &ret);

class FeatureExtract{
	int pixInt[SIZE+10][SIZE+10];
public:
	FeatureExtract();
	int pixRectInt(int i, int j, int x, int y);
	vector<int> getFeature(Mat &src);
};
#endif
