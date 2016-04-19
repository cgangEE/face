#include "base.h"

int fileCnt;

void processAndSave(Mat &img, const char *dstDir){
	Mat roi;
	Rect rect(16, 16, Y, X);
	img(rect).copyTo(img);


	imwrite( (string(dstDir) + '/' + createPicName(++fileCnt)).c_str(), img);  
}


void cutImage(Mat &img, const char* dstDir){
	Mat roi;

	for (int i=0; i<10; ++i){

		double scale = max(X * 1.0 / img.rows, Y * 1.0 / img.cols);

		scale += ( rand() % (int) (1000000 * (1 - scale)) ) / 1000000.0;
		scale = min(1.0, scale);

		Size size(img.cols * scale, img.rows * scale);
		resize(img, roi, size);


		int x, y;

		if (roi.rows == X || roi.cols == Y)
			x = y = 0;
		else {
			x = rand() % (roi.rows - X);
			y = rand() % (roi.cols - Y);
		}


		Rect rect(y, x, Y, X);

		roi(rect).copyTo(roi);
		imwrite( (string(dstDir)
			   	+ '/' + createPicName(++fileCnt)).c_str(), roi);  
	}

}


bool processPicsInDir(const char *srcDir, 
		const char *dstDir, void (*processFunc)(Mat &img, const char *dst)){

	vector<string> picName;
	if (!getFileNameFromDir(srcDir, picName)) return false;

	cout<<picName.size()<<endl;

	for (int i=0; i<picName.size(); ++i){
		Mat src = imread(picName[i].c_str(), -1);
		(*processFunc)(src, dstDir);
	}
	return true;
}


int main(){
	srand(time(NULL));
	FeatureExtract f;

	fileCnt = 0;
	if (!processPicsInDir("./INRIAPerson/train_64x128_H96/pos", 
				"positive_pic", *processAndSave)) return 1;

	fileCnt = 0;
	if (!processPicsInDir("./INRIAPerson/train_64x128_H96/neg", "negative_pic", *cutImage)) return 1;

	return 0;
}
