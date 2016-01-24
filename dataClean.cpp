#include <iostream>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <dirent.h>
#include <unistd.h>

#include "opencv/cv.h"
#include "opencv/highgui.h"
using namespace std;
using namespace cv;

#define picDirName  "origin_pic"

string createPicName(int no){
	string ret;
	while (no){
		ret = (char)('0'+no%10) + ret;
		no /= 10;
	}
	return "../result_pic/"+ret + ".jpg";
}

bool changeFileName(int &fileCnt){
	DIR *dp;
	struct dirent *dirp, *fp;

	if ((dp = opendir(picDirName)) == NULL){
		puts("picDirName error!");
		return false;
	} 

	int debug = 0;
	chdir(picDirName);
	while((dirp = readdir(dp)) != NULL)
		if (dirp->d_type == 8 && dirp->d_name[0]!='.'){
			fileCnt++;
			Mat src = imread(dirp->d_name);
			cout<<createPicName(fileCnt).c_str()<<endl;
			imwrite(createPicName(fileCnt).c_str(), src);  
		}
	chdir("..");
	closedir(dp);
	return true;
}

int main(){
	int fileCnt = 0;
	if (!changeFileName(fileCnt))
		return 1;

	return 0;
}
