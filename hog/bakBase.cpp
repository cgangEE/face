#include "base.h"

int X;
int Y;
int MAXX;
int MAXY;
int CELL;
int BLOCK;
int STRIDE;
int BIN;
double EPS;

double SCALE;

bool getFileNameFromDir(const char *dir, vector<string> &ret){
	DIR *dp;
	struct dirent *dirp;

	if ((dp = opendir(dir)) == NULL){
		printf("read dir<%s> error!\n", dir);
		return false;
	} 

	while ((dirp = readdir(dp)) != NULL){
		if ( (dirp->d_type == 8 || dirp->d_type == 10) 
			&& dirp->d_name[0]!='.'){
			ret.push_back(string(dir) + '/' + dirp->d_name);
		}
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
	if (src.channels() != 1) 
		cvtColor(src, src, CV_BGR2GRAY);

	
	Mat kernelX = (Mat_<char>(1, 3) << 1, 0, -1);
	Mat kernelY = (Mat_<char>(3, 1) << 1, 0, -1);
	Mat MX, MY;

	filter2D (src, MX, CV_16S, kernelX);  
	filter2D (src, MY, CV_16S, kernelY);  


	for (int i=0; i<MX.rows; ++i){
		MX.at<short>(i,0) = 
			(short) src.at<uchar>(i,0) - src.at<uchar>(i, 1);

		MX.at<short>(i, MX.cols-1) =
			(short) src.at<uchar>(i, MX.cols-2) 
			- src.at<uchar>(i, MX.cols-1);
	}



	for (int j=0; j<MY.cols; ++j){
		MY.at<short>(0,j) = 
			(short) src.at<uchar>(0,j) - src.at<uchar>(1,j);
		MY.at<short>(MY.rows-1,j) = 
			(short) src.at<uchar>(MY.rows-2,j) 
			- src.at<uchar>(MY.rows-1,j);
	}

	Mat tmp[9];

	for (int i=0; i<BIN; ++i){
		tmp[i]  = Mat(MX.rows / CELL, MX.cols / CELL, 
				CV_32F, 0.0);
		hist[i]  = Mat(MX.rows / CELL * BLOCK, MX.cols / CELL * BLOCK, 
				CV_32F, 0.0);
	}

	for (int i=0; i<MX.rows / CELL; ++i)
		for (int j=0; j<MX.cols / CELL; ++j){

			for (int s=0; s<CELL; ++s){
				for (int t=0; t<CELL; ++t){
					short x = MX.at<short>(i*CELL+s, j*CELL+t);
					short y = MY.at<short>(i*CELL+s, j*CELL+t);

					double angle = x==0? PI/2 : atan(y*1.0/x);
					if (angle < 0.0) angle += PI;
					double magnitude = sqrt(SQR(x*1.0) + SQR(y*1.0));
					
					int idx = (int) floor(angle / PI * BIN);
					tmp[idx].at<float>(i,j) += magnitude;

				}
			}
		}

	for (int i=0; i < MX.rows/CELL - BLOCK + 1; i+=STRIDE)
		for (int j=0; j < MX.cols/CELL - BLOCK + 1; j+=STRIDE){

			floatType len = EPS;
			for (int s=0; s<BLOCK; ++s)
				for (int t=0; t<BLOCK; ++t)
					for (int r=0; r<BIN; ++r)
						len += SQR(hist[i+s][j+t][r]);

			len = sqrt(len);
			for (int s=0; s<BLOCK; ++s)
				for (int t=0; t<BLOCK; ++t){
					for (int r=0; r<BIN; ++r){
						feature.push_back(hist[i+s][j+t][r]/len);
					}
				}
		}
	
}

vector<floatType> FeatureExtract::getFeature(Mat src){

	getHist(src);

	vector<floatType> feature;

	for (int i=0; i <= X/CELL - BLOCK; i+=STRIDE)
		for (int j=0; j <= Y/CELL - BLOCK; j+=STRIDE){
			
			floatType len = EPS;
			for (int s=0; s<BLOCK; ++s)
				for (int t=0; t<BLOCK; ++t)
					for (int r=0; r<BIN; ++r)
						len += SQR(hist[i+s][j+t][r]);

			len = sqrt(len);
			for (int s=0; s<BLOCK; ++s)
				for (int t=0; t<BLOCK; ++t){
					for (int r=0; r<BIN; ++r){
						feature.push_back(hist[i+s][j+t][r]/len);
					}
				}

		}

	return feature;
}

void readINI(){
	INIReader reader("./base.ini");
	if (reader.ParseError() < 0) {
		fprintf(stderr, "can't load base.ini\n");
		exit(1);
    }

	X = reader.GetInteger("base", "X", 0);
	Y = reader.GetInteger("base", "Y", 0);

	MAXX = reader.GetInteger("base", "MAXX", 0);
	MAXY = reader.GetInteger("base", "MAXY", 0);

	CELL  = reader.GetInteger("base", "CELL", 0);
	BLOCK  = reader.GetInteger("base", "BLOCK", 0);
	STRIDE  = reader.GetInteger("base", "STRIDE", 0);
	BIN  = reader.GetInteger("base", "BIN", 0);
	EPS  = reader.GetReal("base", "EPS", 0.0);

	SCALE = reader.GetReal("base", "SCALE", 0.0);
}

FeatureExtract::FeatureExtract(){
	readINI();
}

//----------Quick Feature Extract ------------------------------


void QuickFeatureExtract::init(Mat src){
	getHist(src);
}


vector<floatType> QuickFeatureExtract::getFeature(int si, int sj){

	vector<floatType> feature;

	for (int i=si; i <= si + X/CELL - BLOCK; i+=STRIDE)
		for (int j=sj; j <= sj + Y/CELL - BLOCK; j+=STRIDE){
			
			floatType len = EPS;
			for (int s=0; s<BLOCK; ++s)
				for (int t=0; t<BLOCK; ++t)
					for (int r=0; r<BIN; ++r)
						len += SQR(hist[i+s][j+t][r]);

			len = sqrt(len);
			for (int s=0; s<BLOCK; ++s)
				for (int t=0; t<BLOCK; ++t){
					for (int r=0; r<BIN; ++r){
						feature.push_back(hist[i+s][j+t][r]/len);
					}
				}

		}

	return feature;
}


QuickFeatureExtract::QuickFeatureExtract(){
	readINI();
}



