#include "faceBase.h"

#define DEBUG 1

bool readAdaBoostFromFile(
		floatType *error, floatType *theta, 
		int *featureSelected, int *sign,
		floatType *&threshold,
		int &n, int *&featureC){

	FILE *fCascade= fopen(cascadeFinal, "rb");
	if (!fCascade){
		printf("open file<%s> error!\n", cascadeFinal);
		return false;
	}

	fread(error, sizeof(error[0]), adaBoostT, fCascade);
	fread(theta, sizeof(theta[0]), adaBoostT, fCascade);
	fread(featureSelected, sizeof(featureSelected[0]), adaBoostT, fCascade);
	fread(sign, sizeof(sign[0]), adaBoostT, fCascade);


	fread(&n, sizeof(n), 1, fCascade);

	featureC = (int*) malloc(sizeof(int) * n);
	threshold = (floatType*) malloc(sizeof(floatType) * n);

	fread(featureC, sizeof(featureC[0]), n, fCascade);
	fread(threshold, sizeof(threshold[0]), n, fCascade);

	fclose(fCascade);
	return true;
}

floatType adaBoostFinalH(vector<floatType> &feature,
		floatType *error, floatType *theta, 
		int *featureSelected, int *sign){

	floatType ret = 0.0;
	for (int i=0; i<adaBoostT; ++i){
		ret += log((1-error[i])/error[i])
			* ( 
					(sign[i] * feature[i] < sign[i] * theta[i]) ?
					1 : -1);
	}

	return ret;
}

int fileCnt = 3000;

void faceDetect(Mat &img, 
		floatType *error, floatType *theta, 
		int *featureSelected, int *sign){

	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);

	Mat face;

	floatType scale = min(1.0, 
			min(400.0 / img.cols, 400.0 /img.rows));

	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);

	FeatureExtract featureExtract(featureSelected, adaBoostT);


	floatType maxScore = INT_MIN;

	while (img.rows>=SIZE && img.cols>=SIZE){
		for (int i=0; i<=(img.cols - SIZE) / DELTA; ++i)
			for (int j=0; j<=(img.rows - SIZE) / DELTA; ++j){
				Rect rect(i*DELTA, j*DELTA, SIZE, SIZE);
				img(rect).copyTo(roi);

				vector<floatType> feature = featureExtract.getAdaBoostFeature(roi);

				floatType score = adaBoostFinalH(feature, 
						error, theta, featureSelected, sign);

				if (score > 100.0){
					maxScore = score;
					cout<<score<<' '<<++fileCnt<<endl;
				//	face = roi;
					imwrite( (string("result_pic") 
							+ "/" + createPicName(fileCnt)).c_str(), roi);
				}

			}
		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}
	/*
	size = Size(300, 300);
	resize(face, face, size);
	imshow("x", face);
	*/
}


bool qualityTest(const char *testDir, int testSign, double testTheta,
		floatType *error, floatType *theta,
		int *featureSelected, int *sign){

	vector<string> fileNameVec;
	if (!getFileNameFromDir(testDir, fileNameVec)) return false;

	FeatureExtract featureExtract(featureSelected, adaBoostT);

	int correctCnt = 0;

	for (int i=0; i<fileNameVec.size(); ++i){
		Mat src = imread(fileNameVec[i].c_str());
		vector<floatType> feature = featureExtract.getAdaBoostFeature(src);
		floatType score = adaBoostFinalH(feature, 
				error, theta, featureSelected, sign);


		if (score * testSign >= testTheta) 
			correctCnt ++;
	}

	cout<<correctCnt<<' '<<fileNameVec.size()<<endl;


	return true;
}


void cameraFace(
		floatType *error, floatType *theta,
		int *featureSelected, int *sign){

	VideoCapture cap(0);
	if(!cap.isOpened())
		return;
	Mat src;
	int cnt = 0;
	for(;;)
	{
		Mat frame;
		cap>>frame;
		if (++cnt < 100) continue;


		imwrite(createPicName(cnt).c_str(), frame);
		faceDetect(frame, error, theta, featureSelected, sign);
//		imshow("x", frame);
	}

}

int main(){
	floatType *error = (floatType*) malloc(sizeof(floatType) * adaBoostT);
	floatType *theta = (floatType*) malloc(sizeof(floatType) * adaBoostT);
	int *featureSelected = (int*) malloc(sizeof(int) * adaBoostT);
	int *sign = (int*) malloc(sizeof(int) * adaBoostT);
	int n;
	floatType *threshold;
	int *featureC;


	if (!readAdaBoostFromFile(error, theta, featureSelected, sign
				threshold, n, featureC)) return 1;


	cameraFace(error, theta, featureSelected, sign);


	free(error);
	free(theta);
	free(featureSelected);
	free(sign);
	free(threshold);
	free(featureC);

	return 0;
}
