#include "faceBase.h"

bool readAdaBoostFromFile(
		double *error, double *theta, 
		int *featureSelected, int *sign){

	FILE *fAdaBoostFinal = fopen(adaBoostFinal, "rb");
	if (!fAdaBoostFinal){
		printf("open file<%s> error!\n", adaBoostFinal);
		return false;
	}

	fread(error, sizeof(error[0]), adaBoostT, fAdaBoostFinal);
	fread(theta, sizeof(theta[0]), adaBoostT, fAdaBoostFinal);
	fread(featureSelected, sizeof(featureSelected[0]), adaBoostT, fAdaBoostFinal);
	fread(sign, sizeof(sign[0]), adaBoostT, fAdaBoostFinal);

	fclose(fAdaBoostFinal);
	return true;
}

double adaBoostFinalH(vector<int> &feature,
		double *error, double *theta, 
		int *featureSelected, int *sign){

	double type = 0.0;
	for (int i=0; i<adaBoostT; ++i)
			type += log((1-error[i])/error[i])
				* ( 
				(sign[i] * feature[i] < sign[i] * theta[i]) ?
				1 : -1);

	return type;
}

void faceDetect(Mat &img, 
		double *error, double *theta, 
		int *featureSelected, int *sign){

	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);

	Mat face;
	double max = -100e100;

	double scale = min(300.0 / img.cols, 300.0 /img.rows);
	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);


	FeatureExtract featureExtract(featureSelected);
	int fileCnt = 0;

	while (img.rows>=SIZE && img.cols>=SIZE){
		for (int i=0; i<(img.cols- SIZE) / DELTA; ++i)
			for (int j=0; j<(img.rows- SIZE) / DELTA; ++j){
				Rect rect(i*DELTA, j*DELTA, SIZE, SIZE);
				img(rect).copyTo(roi);

				vector<int> feature = featureExtract.getAdaBoostFeature(roi);
				
				double type = adaBoostFinalH(feature, 
						error, theta, featureSelected, sign);

				if (type >= 0){
					max = type;
					face = roi;
					cout<<type<<endl;
					imwrite( (string("result_pic") 
							+ "/" + createPicName(++fileCnt)).c_str(), roi);
				}

			}
		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}
}

int main(){
	double *error = (double*) malloc(sizeof(double) * adaBoostT);
	double *theta = (double*) malloc(sizeof(double) * adaBoostT);
	int *featureSelected = (int*) malloc(sizeof(int) * adaBoostT);
	int *sign = (int*) malloc(sizeof(int) * adaBoostT);


	if (!readAdaBoostFromFile(error, theta, featureSelected, sign)) return 1;

	Mat img = imread("1.jpg");
	faceDetect(img, error, theta, featureSelected, sign);

	free(error);
	free(theta);
	free(featureSelected);
	free(sign);
	return 0;
}
