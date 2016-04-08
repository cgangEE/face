#ifndef __BASE_H__
#define __BASE_H__

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
#include "inih/cpp/INIReader.h"

using namespace std;
using namespace cv;

#define negDir "negative_pic"
#define posDir "positive_pic"

#define trainFileName "train.txt"

#define SQR(x) ((x)*(x))
#define PI (acos(-1.0))

#define SIZE 80
static int X;
static int Y;
static int CELL;
static int BLOCK;
static int STRIDE;
static int BIN;
static double EPS;

static int DELTA;
static int SCALE;

#define floatType double

bool getFileNameFromDir(const char *dir, vector<string> &ret);
string createPicName(int no);

static floatType ***hist;

class FeatureExtract{
public:
	vector<floatType> getFeature(Mat src);
	FeatureExtract();
	~FeatureExtract();
};

#endif
