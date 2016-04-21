#include "base.h"
#include "libsvm/svm.h"
#define svmModelName "train.txt.model"

int fileCnt = 0;

/*
void camera(svm_model *model){
	VideoCapture cap(0);
	if (!cap.isOpened())
		return;

	int cnt = 0;
	for (;;){
		Mat frame;
		cap>>frame;

		if (++cnt < 10) continue; 

		int x, y, sz = 0;
		test(model, frame, x, y, sz);

		if (sz) {
			rectangle(frame, 
					Point(x, y), Point(x+sz, y+sz), Scalar(0,0,255), 3);
		}
		imshow("x", frame);
		waitKey(3);
	}

}
*/

FeatureExtract featureExtract;
char c;

void testLocal(svm_model *model, const char *fileName){

	Mat img = imread(fileName);
	Mat frame;
	img.copyTo(frame);

	cout<<img.cols<<' '<<img.rows<<endl;

	int cols = img.cols;
	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);

	floatType scale = min(1.0, 
			min(300.0 / img.cols, 300.0 /img.rows));

	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);

	int x, y, szX, szY;

	svm_node *f  = NULL;
	double maxDec = -1e100;

	while (img.rows>=X && img.cols>=Y){
		for (int i=0; i<=(img.rows - X) / DELTA; ++i)
			for (int j=0; j<=(img.cols - Y) / DELTA; ++j){

				Rect rect(j*DELTA, i*DELTA, Y, X);
				img(rect).copyTo(roi);

				++fileCnt;
				if (fileCnt%1000==0) cout<<fileCnt<<endl;

				vector<floatType> feature = featureExtract.getFeature(roi);
				if (f == NULL) 
					f = (svm_node*) 
						malloc(sizeof(svm_node) * (feature.size()+1));


				for (int i=0; i<feature.size(); ++i){
					f[i].index = i+1;
					f[i].value = feature[i];
				}
				f[feature.size()].index = -1;


				double *dec_values = (double*) malloc(sizeof(double));
				double type = svm_predict_values(model, f, dec_values);

				if (type != -1 && dec_values[0]>maxDec){
					maxDec = dec_values[0];
					cout<<type<<' '<<dec_values[0]<<endl;

					scale = cols * 1.0 / img.cols;
					szX = (int) ceil(X * scale);
					szY = (int) ceil(Y * scale);
					x = (int) ceil(i * DELTA * scale);
					y = (int) ceil(j * DELTA * scale);

				}

				free(dec_values);

			}

		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}

	rectangle(frame, 
			Point(y, x), Point(y+szY, x+szX), 
			Scalar(255,255,0), 3);

	string outputFileName = "aDetected.jpg";
	outputFileName[0]  = c;
	imwrite(outputFileName.c_str(), frame);

	free(f);
}


int main(){
	svm_model *model = svm_load_model(svmModelName);
	
	for (c = 'a'; c<='f'; ++c){
		char fileName[] = "e.png";
		fileName[0] = c;
		testLocal(model, fileName);
	}
	return 0;
}
