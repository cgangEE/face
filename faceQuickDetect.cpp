#include "faceBase.h"

#define DEBUG 0
#define FILEID 0

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


int fileCnt = 0;

floatType adaBoostFinalH(
		FeatureExtract *featureExtract, Mat &roi,
		floatType *error, floatType *theta,
		int *featureSelected, int *sign,
		floatType * threshold,
		int n, int *featureC){

	int idx = 0;
	floatType score = 0.0;

	FeatureExtract fe;
	fe.pixIntegral(roi);

	for (int i=0; i<n; ++i){
		vector<floatType> f = featureExtract[i].getQuickAdaBoostFeature(&fe);

		for (int j=0; j<featureC[i]; ++j){
			floatType &e = error[j+idx];
			floatType &t = theta[j+idx];
			int &s = sign[j+idx];

			score += log((1-e)/e)
				* ( (s * f[j] < s * t) ?
						1 : -1);
		}

		if (DEBUG && fileCnt == FILEID) 
			cout<<score<<' '<<threshold[i]<<endl;

		if (score <= threshold[i]) return -10;

		idx += featureC[i];
	}

	return score;
}
int timeval_subtract(struct timeval* result, struct timeval* x, struct timeval* y) 
{ 
	int nsec; 

	if ( x->tv_sec>y->tv_sec ) 
		return -1; 

	if ( (x->tv_sec==y->tv_sec) && (x->tv_usec>y->tv_usec) ) 
		return -1; 

	result->tv_sec = ( y->tv_sec-x->tv_sec ); 
	result->tv_usec = ( y->tv_usec-x->tv_usec ); 

	if (result->tv_usec<0) 
	{ 
		result->tv_sec--; 
		result->tv_usec+=1000000; 
	} 

	return 0; 
}


#include <sys/time.h> 
void test(floatType *error, floatType *theta,
		int *featureSelected, int *sign,
		floatType * threshold,
		int n, int *featureC){


	int **fSelected = (int **) malloc(sizeof(int*) * n);
	FeatureExtract *featureExtract = (FeatureExtract *) malloc(sizeof(FeatureExtract) * n);
	memset(featureExtract, 0, sizeof(FeatureExtract) * n);

	int idx = 0;

	for (int i=0; i<n; ++i){
		fSelected[i] = (int *) malloc(sizeof(int) * featureC[i]);
		for (int j=0; j<featureC[i]; ++j){
			fSelected[i][j] = featureSelected[j + idx];
		}

		featureExtract[i].init(fSelected[i], featureC[i]);
		idx += featureC[i];
	}


	struct timeval start,stop,diff; 
	gettimeofday(&start,0); 

	Mat roi;
	Mat img = imread("3.jpg");

	floatType scale = min(1.0, 
			min(400.0 / img.cols, 400.0 /img.rows));

	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);

	cvtColor(img, img, CV_BGR2GRAY);

	Mat ret;
	floatType maxScore = -1e50;


	while (img.rows>=SIZE && img.cols>=SIZE){
		for (int i=0; i<=(img.cols - SIZE) / DELTA; ++i)
			for (int j=0; j<=(img.rows - SIZE) / DELTA; ++j){
				Rect rect(i*DELTA, j*DELTA, SIZE, SIZE);
				img(rect).copyTo(roi);

				++fileCnt;

				floatType score = adaBoostFinalH(featureExtract, roi, 
						error, theta, featureSelected, sign,
						threshold, n, featureC);

				if (DEBUG && fileCnt == FILEID){
					cout<<score<<endl;
				}

				if (score > 0.0  && score > maxScore){
					maxScore = score;
					roi.copyTo(ret);
				}


			}
		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}

	imwrite( ((string("pos_tmp"))
				+ "/" + createPicName(fileCnt)).c_str(), ret);

	gettimeofday(&stop,0); 
	timeval_subtract(&diff,&start,&stop); 
	printf("总计用时:%d 微秒\n",diff.tv_usec); 

	for (int i=0; i<n; ++i)
		free(fSelected[i]);
	free(fSelected);
}

int main(){
	floatType *error = (floatType*) malloc(sizeof(floatType) * adaBoostT);
	floatType *theta = (floatType*) malloc(sizeof(floatType) * adaBoostT);
	int *featureSelected = (int*) malloc(sizeof(int) * adaBoostT);
	int *sign = (int*) malloc(sizeof(int) * adaBoostT);
	int n;
	floatType *threshold;
	int *featureC;


	if (!readAdaBoostFromFile(error, theta, featureSelected, sign,
				threshold, n, featureC)) return 1;


	test(error, theta, featureSelected, sign, threshold, n, featureC);

	//	cameraFace(error, theta, featureSelected, sign);

	free(error);
	free(theta);
	free(featureSelected);
	free(sign);
	free(threshold);
	free(featureC);

	return 0;
}
