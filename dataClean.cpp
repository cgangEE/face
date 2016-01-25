#include "faceTool.h"

int fileCnt;


string createPicName(int no){
	string ret;
	while (no){
		ret = (char)('0'+no%10) + ret;
		no /= 10;
	}
	return ret + ".jpg";
}


void processAndSave(Mat &img, const char *dstDir){
	Mat roi;
	cvtColor(img, img, CV_BGR2GRAY);
	double scale = max((SIZE+1.0)/img.rows, (SIZE+1.0)/img.cols);
	Size size(img.cols * scale, img.rows * scale);
	resize(img, img, size);
	Rect rect(0, 8, SIZE, SIZE);
	img(rect).copyTo(roi);
	imwrite( (string(dstDir) + '/' + createPicName(++fileCnt)).c_str(), roi);  
}


void cutImage(Mat &img, const char* dstDir){
	Mat roi;
	cvtColor(img,img,CV_BGR2GRAY);

	while (img.rows>=SIZE && img.cols>=SIZE){
		for (int i=0; i<(img.cols- SIZE) / DELTA; ++i)
			for (int j=0; j<(img.rows- SIZE) / DELTA; ++j){
				if (!(rand()%100000<=50)) continue;
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
	if (!processPicsInDir("yale", "positive_pic", *processAndSave)) return 1;

	return 0;
}
