#include "faceBase.h"

bool getFileNameFromDir(const char *dir, vector<string> &ret){
	DIR *dp;
	struct dirent *dirp;

	if ((dp = opendir(dir)) == NULL){
		printf("read dir<%s> error!\n", dir);
		return false;
	} 

	while ((dirp = readdir(dp)) != NULL)
		if (dirp->d_type == 8 && dirp->d_name[0]!='.'){
			ret.push_back(string(dir) + '/' + dirp->d_name);
		}

	return true;
}


string createPicName(int no){
	string ret;
	while (no){
		ret = (char)('0'+no%10) + ret;
		no /= 10;
	}
	return ret + ".jpg";
}

void getHist(Mat &src){
	cvtColor(src, src, CV_BGR2GRAY);

	Mat kernelX = (Mat_<char>(1, 3) << 1, 0, -1);
	Mat kernelY = (Mat_<char>(3, 1) << 1, 0, -1);
	Mat X, Y;

	filter2D (src, X, CV_16S, kernelX);  
	filter2D (src, Y, CV_16S, kernelY);  

	for (int i=0; i<X.cols; ++i){
		X.at<short>(i,0) = 
			(short) src.at<uchar>(i,0) - src.at<uchar>(i, 1);
		X.at<short>(i,X.rows-1) =
			(short) src.at<uchar>(i, X.rows-2) - src.at<uchar>(i, X.rows-1);
	}

	for (int j=0; j<Y.rows; ++j){
		Y.at<short>(0,j) = 
			(short) src.at<uchar>(0,j) - src.at<uchar>(1,j);
		Y.at<short>(Y.cols-1,j) = 
			(short) src.at<uchar>(Y.cols-2,j) - src.at<uchar>(Y.cols-1,j);
	}

	memset(hist, 0, sizeof(hist));

	for (int i=0; i<X.cols / CELL; ++i)
		for (int j=0; j<X.rows / CELL; ++j){
			for (int s=0; s<CELL; ++s){
				for (int t=0; t<CELL; ++t){
					short x = X.at<short>(i*CELL+s, j*CELL+t);
					short y = Y.at<short>(i*CELL+s, j*CELL+t);

					double angle = x==0? PI/2 : atan(y*1.0/x);
					if (angle < 0.0) angle += PI;
					double magnitude = sqrt(SQR(x*1.0) + SQR(y*1.0));
					
					int idx = (int) floor(angle / PI * BIN);
					hist[i][j][idx] += magnitude;
				}
			}
		}
}

vector<floatType> FeatureExtract::getFeature(Mat src){
//	resize(src, src, Size(9,9));

	getHist(src);

	vector<floatType> feature;

	for (int i=0; i <= SIZE/CELL - BLOCK; i+=STRIDE)
		for (int j=0; j <= SIZE/CELL - BLOCK; j+=STRIDE){
			
			floatType len = EPS;
			for (int s=0; s<BLOCK; ++s)
				for (int t=0; t<BLOCK; ++t)
					for (int r=0; r<BIN; ++r)
						len += SQR(hist[i+s][j+t][r]);

			len = sqrt(len);
			for (int s=0; s<BLOCK; ++s)
				for (int t=0; t<BLOCK; ++t){
					for (int r=0; r<BIN; ++r)
						feature.push_back(hist[i+s][j+t][r]/len);
				}
		}

	return feature;
}
