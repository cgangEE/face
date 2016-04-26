#include "base.h"
#include "libsvm/svm.h"
#define svmModelName "train.txt.model"

int fileCnt = 0;


QuickFeatureExtract quickFeatureExtract;

void testLocal(Mat img, double *w, double b, int featureSize, 
				int &szX, int &szY, int &x, int &y, double &maxDec){

	fileCnt = 0;

	int cols = img.cols;
	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);

	floatType scale = min(1.0,
			min(MAXX * 1.0 / img.rows, MAXY * 1.0 / img.cols));

	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);


	while (img.rows>=X && img.cols>=Y){
		quickFeatureExtract.init(img);


		for (int i=0; i<= img.rows/CELL - X/CELL; ++i)
			for (int j=0; j<= img.cols/CELL - Y/CELL; ++j){


				vector<floatType> feature = 
					quickFeatureExtract.getFeature(i, j);

				double tmp = b;
				for (int l=0; l<featureSize; ++l)
					tmp += w[l] * feature[l];


				if (tmp > maxDec){
					maxDec = tmp;

					scale = cols * 1.0 / img.cols;
					szX = (int) ceil(X * scale);
					szY = (int) ceil(Y * scale);
					x = (int) ceil(i * CELL * scale);
					y = (int) ceil(j * CELL * scale);
				}

			}

		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}
}


void camera(double *w, double b, int featureSize){
	VideoCapture cap(0);
	if (!cap.isOpened())
		return;

	int cnt = 0;
	for (;;){
		Mat frame;
		cap>>frame;

		if (++cnt < 10) continue; 


		int x, y, szX, szY;
		double maxDec = -1e100;
		testLocal(frame, w, b, featureSize, 
				szX, szY, x, y, maxDec);

//		cout<<maxDec<<endl;

		if (maxDec > 1)
			rectangle(frame, 
					Point(y, x), Point(y+szY, x+szX), 
					Scalar(255,255,0), 3);

		imshow("x", frame);
		waitKey(3);
	}

}


int main(){
	svm_model *model = svm_load_model(svmModelName);
	int featureSize = (X / CELL - BLOCK + 1) * (Y / CELL - BLOCK + 1) 
		* BIN * BLOCK * BLOCK;

	double *w = (double*) malloc(sizeof(double) * featureSize);
	double b = -(model->rho[0]);

	memset(w, 0, sizeof(double) * featureSize);
		
	for (int i=0; i<model->l; ++i){
		for (int j=0; j<featureSize; ++j)
			w[j] = w[j] + model->sv_coef[0][i] * model->SV[i][j].value;
	}
	
	if (model->label[0] == -1){
		b *= -1.0;
		for (int j=0; j<featureSize; ++j)
			w[j] *= -1.0;
	}

	camera(w, b, featureSize);

	return 0;
}

