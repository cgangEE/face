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

#include <sys/time.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "inih/cpp/INIReader.h"
#include "libsvm/svm.h"

using namespace std;
using namespace cv;

#define negDir "negative_pic"
#define posDir "positive_pic"

#define trainFileName "train.txt"
#define svmModelName "train.txt.model"

#define SQR(x) ((x)*(x))
#define PI (acos(-1.0))

#define SIZE 80
extern int X;
extern int Y;
extern int MAXX;
extern int MAXY;
extern int CELL;
extern int BLOCK;
extern int STRIDE;
extern int BIN;
extern double EPS;

extern double SCALE;

#define floatType double

bool getFileNameFromDir(const char *dir, vector<string> &ret);
string createPicName(int no);
int timeval_subtract(struct timeval* result, struct timeval* x, struct timeval* y);
void getDecPlane(double *& w, double &b, int &featureSize);

static floatType ***hist;

class FeatureExtract{

public:
	vector<floatType> getFeature(Mat src);
	FeatureExtract();
	~FeatureExtract();
};

class QuickFeatureExtract{
public:
	void init(Mat src);
	vector<floatType> getFeature(int si, int sj);
	QuickFeatureExtract();
	~QuickFeatureExtract();
};

#endif
