#include "base.h"
#include "libsvm/svm.h"
#define svmModelName "train.txt.model"

int fileCnt = 0;

QuickFeatureExtract featureExtract;

void testLocal(double *w, double b, int featureSize,
		const char *fileName,
		const char *dstDir){

	Mat img = imread(fileName);
	cvtColor(img, img, CV_BGR2GRAY);
	Mat roi;

	while (img.cols>=X && img.rows>=Y){
		featureExtract.init(img);

		for (int i=0; i<= img.rows/CELL - X/CELL; ++i)
			for (int j=0; j<= img.cols/CELL - Y/CELL; ++j){

				vector<floatType> feature = featureExtract.getFeature(i, j);

				double tmp = b;
				for (int l=0; l<featureSize; ++l)
					tmp += w[l] * feature[l];

				if (tmp > 1){
					++fileCnt;
					if (fileCnt%1000==0) cout<<fileCnt<<endl;

					Rect rect(j*CELL, i*CELL, Y, X);
					img(rect).copyTo(roi);
					imwrite( (string(dstDir)
								+ '/' + createPicName(fileCnt)).c_str(), roi);  
				}

			}

		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}

}

bool getHardExample(
		double *w, double b, int featureSize,
		const char *srcDir, const char *dstDir){

	vector<string> picName;
	if (!getFileNameFromDir(srcDir, picName)) return false;

	fileCnt = 12180;

	int xxx = 0;
	for (int i=0; i<picName.size(); ++i){
		testLocal(w, b, featureSize, picName[i].c_str(), dstDir);
		printf("%d\n", i);
	}
	return true;
}

int main(){
	int  featureSize = 0;
	double *w, b;
	getDecPlane(w, b, featureSize);
	if (!getHardExample(w, b, featureSize,
				"./INRIAPerson/train_64x128_H96/neg", "negative_pic"))
		return 1;

	return 0;
}

