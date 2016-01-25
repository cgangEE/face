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

bool getFileNameFromDir(const char *dir, vector<string> &ret);

#endif
