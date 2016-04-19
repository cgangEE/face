#include "base.h"
#include "libsvm/svm.h"
#define svmModelName "train.txt.model"

int fileCnt = 0;

void test(svm_model *model, Mat img,
		int &x, int &y, int &sz){

	int cols = img.cols;
	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);

	floatType scale = min(1.0, 
			min(400.0 / img.cols, 400.0 /img.rows));

	Size size(img.cols * scale, img.rows * scale);
//	resize(img, img, size);


	FeatureExtract featureExtract;
	svm_node *f  = NULL;

	while (img.rows>=SIZE && img.cols>=SIZE){
		for (int i=0; i<=(img.cols - SIZE) / DELTA; ++i)
			for (int j=0; j<=(img.rows - SIZE) / DELTA; ++j){

				Rect rect(i*DELTA, j*DELTA, SIZE, SIZE);
				img(rect).copyTo(roi);

				++fileCnt;

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
					scale = cols * 1.0 / img.cols;
					x = (int) ceil(i * DELTA * scale);
					y = (int) ceil(j * DELTA * scale);
					sz = (int) ceil(SIZE * scale);

					free(f);
					return;
				}

			}

		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}

	free(f);
}

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

	int x, y, sz;

	FeatureExtract featureExtract;
	svm_node *f  = NULL;

	int rectCnt = 0;

	while (img.rows>=X && img.cols>=Y){
		for (int i=0; i<=(img.rows - X) / DELTA; ++i)
			for (int j=0; j<=(img.cols - Y) / DELTA; ++j){

				Rect rect(j*DELTA, i*DELTA, Y, X);
				img(rect).copyTo(roi);

				++fileCnt;
				if (fileCnt%1000==0) cout<<fileCnt<<endl;

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
					scale = cols * 1.0 / img.cols;
					int szX = (int) ceil(X * scale);
					int szY = (int) ceil(Y * scale);
					x = (int) ceil(i * DELTA * scale);
					y = (int) ceil(j * DELTA * scale);

					rectangle(frame, 
							Point(y, x), Point(y+szY, x+szX), Scalar(255,255,0), 3);
					imwrite("detectedTest.jpg", frame);

					if (++rectCnt == 10) return;
				}

			}

		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}


	free(f);
}


int main(){
	svm_model *model = svm_load_model(svmModelName);
	//	camera(model);
	const char *fileName = "X.png";
	testLocal(model, fileName);
	return 0;
}
