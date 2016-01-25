#include "faceTool.h"

#define picDirName  "origin_pic"
#define DELTA 5
#define SIZE 50
#define SCALE 0.9

string createPicName(int no){
	string ret;
	while (no){
		ret = (char)('0'+no%10) + ret;
		no /= 10;
	}
	return "../result_pic/"+ret + ".jpg";
}

void cutImage(Mat &img, int &fileCnt){
	Mat roi;
	cvtColor(img,img,CV_BGR2GRAY);
	while (img.rows>=SIZE && img.cols>=SIZE){
		for (int i=0; i<(img.cols- SIZE) / DELTA; ++i)
			for (int j=0; j<(img.rows- SIZE) / DELTA; ++j){
				if (rand()%100000<99900) continue;
				Rect rect(i*DELTA, j*DELTA, SIZE, SIZE);
				img(rect).copyTo(roi);
				++fileCnt;
				imwrite(createPicName(fileCnt).c_str(), roi);  
			}
		Size size(img.cols * SCALE, img.rows * SCALE);
		resize(img, img, size);
	}
}

bool processPicsInDir(const char *srcDir, const char *dstDir){
	vector<string> picName;
	if (!getFileNameFromDir(srcDir, picName)) return false;
	for (int i=0; i<picName.size(); ++i){

	}
	return true;
}

int main(){
	srand(time(NULL));
	if (!processPicsInDir("origin_pic", "negative_pic")) return 1;

	return 0;
}
