#include "faceBase.h"

int fileCnt;

void processAndSave(Mat &img, const char *dstDir){
	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);
	double scale = max((SIZE+1.0)/img.rows, (SIZE+1.0)/img.cols);
	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);

	//cout<<size.height<<' '<<size.width<<endl;

	Rect rect(0, (int)(size.height/67.0*10), SIZE, SIZE);
	img(rect).copyTo(roi);
	imwrite( (string(dstDir) + '/' + createPicName(++fileCnt)).c_str(), roi);  
}


void cutImage(Mat &img, const char* dstDir){
	Mat roi;
	cvtColor(img,img,CV_BGR2GRAY);

	while (img.rows>=SIZE && img.cols>=SIZE){
		for (int i=0; i<(img.cols- SIZE) / DELTA; ++i)
			for (int j=0; j<(img.rows- SIZE) / DELTA; ++j){
				if (!(rand()%100000<=20)) continue;
				Rect rect(i*DELTA, j*DELTA, SIZE, SIZE);
				img(rect).copyTo(roi);
				imwrite( (string(dstDir)
						   	+ '/' + createPicName(++fileCnt)).c_str(), roi);  
			}
		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}
}


bool processPicsInDir(const char *srcDir, 
		const char *dstDir, void (*processFunc)(Mat &img, const char *dst)){
	vector<string> picName;
	fileCnt = 0;
	if (!getFileNameFromDir(srcDir, picName)) return false;
	for (int i=0; i<picName.size(); ++i){
		Mat src = imread(picName[i].c_str());
		(*processFunc)(src, dstDir);
	}
	return true;
}

int main(){
	srand(time(NULL));
	if (!processPicsInDir("origin_pic", "negative_pic", *cutImage)) return 1;
	if (!processPicsInDir("pain_crops", "positive_pic", *processAndSave)) return 1;

	return 0;
}
