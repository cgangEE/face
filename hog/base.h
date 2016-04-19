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
extern int X;
extern int Y;
extern int CELL;
extern int BLOCK;
extern int STRIDE;
extern int BIN;
extern double EPS;

extern int DELTA;
extern double SCALE;

#define floatType double

bool getFileNameFromDir(const char *dir, vector<string> &ret);
string createPicName(int no);
void getGradient(Mat &src, Mat &MX, Mat &MY);

static floatType ***hist;

class FeatureExtract{
public:
	vector<floatType> getFeature(Mat src);
	FeatureExtract();
	~FeatureExtract();
};

#endif
