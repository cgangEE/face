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

#define trainFileName "train.txt"

#define SQR(x) ((x)*(x))
#define PI (acos(-1.0))

#define SIZE 80
#define CELL 8
#define BLOCK 2
#define STRIDE 1
#define BIN 9
#define EPS 0.1

typedef double floatType;

bool getFileNameFromDir(const char *dir, vector<string> &ret);
string createPicName(int no);

static floatType hist[SIZE/CELL][SIZE/CELL][BIN];

class FeatureExtract{
public:
	vector<floatType> getFeature(Mat src);
};

#endif
