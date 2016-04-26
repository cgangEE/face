#include "base.h"
#define svmModelName "train.txt.model"


int totalPos = 0;
int totalNeg = 0;
int falseNeg = 0;
int falsePos = 0;

QuickFeatureExtract featureExtract;


void testPos(Mat &img, double *w, double b, int featureSize){
	Rect rect(3, 3, Y, X);
	img(rect).copyTo(img);
	cvtColor(img, img, CV_BGR2GRAY);
	featureExtract.init(img);

	vector<floatType> feature = featureExtract.getFeature(0, 0);

	double tmp = b;
	for (int l=0; l<featureSize; ++l)
		tmp += w[l] * feature[l];

	++totalPos;
	if (tmp < 0.0) ++ falseNeg;
}


void testNeg(Mat &img, double *w, double b, int featureSize){
	cvtColor(img, img, CV_BGR2GRAY);
	Mat roi;

	floatType scale = min(1.0,
			min(300 * 1.0 / img.rows, 300 * 1.0 / img.cols));
	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);

	while (img.cols>=X && img.rows>=Y){
		featureExtract.init(img);

		for (int i=0; i<= img.rows/CELL - X/CELL; ++i)
			for (int j=0; j<= img.cols/CELL - Y/CELL; ++j){

				++totalNeg;
				vector<floatType> feature = featureExtract.getFeature(i, j);

				double tmp = b;
				for (int l=0; l<featureSize; ++l)
					tmp += w[l] * feature[l];

				if (tmp>0.0) ++ falsePos;
			}

		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}

}


bool processPicsInDir(const char *srcDir, 
		void (*processFunc)(Mat &img, double *w, double b, int featureSize),
		double *w, double b, int featureSize){

	vector<string> picName;
	if (!getFileNameFromDir(srcDir, picName)) return false;

	for (int i=0; i<picName.size(); ++i){
		if (i%100==0) cout<<i<<endl;
		Mat src = imread(picName[i].c_str(), -1);
		(*processFunc)(src, w, b, featureSize);
	}
	return true;
}


int main(){
	int featureSize = 0;
	double *w, b;
	getDecPlane(w, b, featureSize);

	if (!processPicsInDir("./INRIAPerson/test_64x128_H96/pos", 
				*testPos, w, b, featureSize)) return 1;

	cout<< totalPos - falseNeg<<' '<<falseNeg<<endl;

	if (!processPicsInDir("./INRIAPerson/test_64x128_H96/neg", 
				*testNeg, w, b, featureSize)) return 1;

	cout<< totalNeg - falsePos<<' '<<falsePos<<endl;
	return 0;
}
