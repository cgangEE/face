#include "base.h"

int fileCnt;

void processAndSave(Mat &img, const char *dstDir){
	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);

	Size size(SIZE, SIZE);
	resize(img, img, size);

	imwrite( (string(dstDir) + '/' + createPicName(++fileCnt)).c_str(), img);  
}


void cutImage(Mat &img, const char* dstDir){
	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);


	for (int i=0; i<5; ++i){

		double scale = SIZE * 1.0 / min(img.cols, img.rows);

		scale += ( rand() % (int) (1000000 * (1 - scale)) ) / 1000000.0;
		scale = min(1.0, scale);

		Size size(img.cols * scale, img.rows * scale);
		resize(img, roi, size);

		int x, y;


		if (roi.cols == SIZE || roi.rows == SIZE)
			x = y = 0;
		else {
			x = rand() % (roi.cols - SIZE);
			y = rand() % (roi.rows - SIZE);
		}

		Rect rect(x, y, SIZE, SIZE);

		roi(rect).copyTo(roi);
		imwrite( (string(dstDir)
			   	+ '/' + createPicName(++fileCnt)).c_str(), roi);  
	}

}


bool processPicsInDir(const char *srcDir, 
		const char *dstDir, void (*processFunc)(Mat &img, const char *dst)){
	vector<string> picName;
	if (!getFileNameFromDir(srcDir, picName)) return false;
	for (int i=0; i<picName.size(); ++i){
		Mat src = imread(picName[i].c_str());
		(*processFunc)(src, dstDir);
	}
	return true;
}

int main(){
	srand(time(NULL));
	fileCnt = 0;
	if (!processPicsInDir("origin_pic", "negative_pic", *cutImage)) return 1;

	fileCnt = 0;
	if (!processPicsInDir("peal", "positive_pic", *processAndSave)) return 1;

	return 0;
}
