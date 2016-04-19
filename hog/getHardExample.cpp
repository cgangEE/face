#include "base.h"
#include "libsvm/svm.h"
#define svmModelName "train.txt.model"

int fileCnt = 0;

void testLocal(svm_model *model, const char *fileName,
		const char *dstDir){
	Mat img = imread(fileName);
	cvtColor(img, img, CV_BGR2GRAY);

	Mat roi;

	FeatureExtract featureExtract;
	svm_node *f  = NULL;


	while (img.cols>=X && img.rows>=Y){
		Mat MX, MY;
		getGradient(img, MX, MY);


		for (int i=0; i<=(img.cols - X) / DELTA; ++i)
			for (int j=0; j<=(img.rows - Y) / DELTA; ++j){

				Rect rect(i*DELTA, j*DELTA, X, Y);
				img(rect).copyTo(roi);


				vector<floatType> feature = featureExtract.getFeature(roi);
				if (!f) 
					f = (svm_node*) 
						malloc(sizeof(svm_node) * (feature.size()+1));


				for (int i=0; i<feature.size(); ++i){
					f[i].index = i+1;
					f[i].value = feature[i];
				}
				f[feature.size()].index = -1;

				double type = svm_predict(model, f);

				if (type != -1){
					++fileCnt;
					if (fileCnt%1000==0) cout<<fileCnt<<endl;

					imwrite( (string(dstDir)
								+ '/' + createPicName(fileCnt)).c_str(), roi);  
				}

			}

		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}

	free(f);
}

bool getHardExample(
		svm_model *model,
		const char *srcDir, const char *dstDir){

	vector<string> picName;
	if (!getFileNameFromDir(srcDir, picName)) return false;

	fileCnt = 12180;

	int xxx = 0;
	for (int i=0; i<picName.size(); ++i){
		testLocal(model, picName[i].c_str(), dstDir);
		if (++xxx == 10) break;
	}
	return true;
}

int main(){
	svm_model *model = svm_load_model(svmModelName);
	if (!getHardExample(model, 
				"./INRIAPerson/train_64x128_H96/neg", "negative_pic"))
		return 1;

	return 0;
}

