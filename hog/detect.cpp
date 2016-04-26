#include "base.h"
#define svmModelName "train.txt.model"

int fileCnt = 0;

QuickFeatureExtract quickFeatureExtract;

void testLocal(const char *fileName, char c,
		double *w, double b, int featureSize){

	fileCnt = 0;

	Mat img = imread(fileName);
	Mat frame;
	img.copyTo(frame);

	struct timeval start,stop,diff; 
	gettimeofday(&start,0); 

	cout<<img.cols<<' '<<img.rows<<endl;

	int cols = img.cols;
	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);

	floatType scale = min(1.0,
			min(MAXX * 1.0 / img.rows, MAXY * 1.0 / img.cols));

	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);

	int x, y, szX, szY;

	double maxDec = -1e100;


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

	cout<< c<<' '<<maxDec <<endl;

	rectangle(frame, 
			Point(y, x), Point(y+szY, x+szX), 
			Scalar(255,255,0), 3);

	gettimeofday(&stop, 0);
	timeval_subtract(&diff, &start, &stop);
	printf("%d ms\n", diff.tv_usec / 1000 );

	string outputFileName = "aDetected.jpg";
	outputFileName[0]  = c;
	imwrite(outputFileName.c_str(), frame);
}


int main(){
	int featureSize = 0;
	double *w, b;
	getDecPlane(w, b, featureSize);

	for (char c = 'a'; c<='z'; ++c){


		char fileName[] = "a.png";
		fileName[0] = c;
		testLocal(fileName, c, w, b, featureSize);

	}

	return 0;
}
