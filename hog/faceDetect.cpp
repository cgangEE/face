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
			min(330.0 / img.cols, 330.0 /img.rows));

	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);


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


	vector< pair<double, pair< pair<double, int> , pair<int,int> > > >ret;

	while (img.rows>=SIZE && img.cols>=SIZE){
		for (int i=0; i<=(img.cols - SIZE) / DELTA; ++i)
			for (int j=0; j<=(img.rows - SIZE) / DELTA; ++j){

				Rect rect(i*DELTA, j*DELTA, SIZE, SIZE);
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

				double *prob;
				double type = svm_predict_probability(model, f, prob);

				if (type != -1){
					scale = cols * 1.0 / img.cols;
					sz = (int) ceil(SIZE * scale);
					x = (int) ceil(i * DELTA * scale);
					y = (int) ceil(j * DELTA * scale);

					ret.push_back(
							make_pair(max(prob[0], prob[1]),
							make_pair(
								make_pair(scale, sz), 
								make_pair(x, y)
								)));
				}

			}

		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}

	sort(ret.rbegin(), ret.rend());
	cout<<ret.size()<<endl;

	for (int i=0; i<min(5, (int)ret.size()); ++i){
		double scale = ret[i].second.first.first;
		int sz = ret[i].second.first.second;
		int x= ret[i].second.second.first;
		int y= ret[i].second.second.second;

		rectangle(frame, 
				Point(x, y), Point(x+sz, y+sz), Scalar(0,0,255), 3);
	}
	imwrite("detectedBoy.jpg", frame);

	free(f);
}


int main(){
	svm_model *model = svm_load_model(svmModelName);
//	camera(model);
	const char *fileName = "boy.jpg";
	testLocal(model, fileName);
	return 0;
}
