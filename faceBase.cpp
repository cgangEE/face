#include "faceBase.h"

bool getFileNameFromDir(const char *dir, vector<string> &ret){
	DIR *dp;
	struct dirent *dirp;

	if ((dp = opendir(dir)) == NULL){
		printf("read dir<%s> error!\n", dir);
		return false;
	} 

	while ((dirp = readdir(dp)) != NULL)
		if (dirp->d_type == 8 && dirp->d_name[0]!='.'){
			ret.push_back(string(dir) + '/' + dirp->d_name);
		}

	return true;
}


string createPicName(int no){
	string ret;
	while (no){
		ret = (char)('0'+no%10) + ret;
		no /= 10;
	}
	return ret + ".jpg";
}


FeatureExtract::FeatureExtract(){
	memset(pixInt, 0, sizeof(pixInt));
}

FeatureExtract::FeatureExtract(int *featureSelected){
	FeatureExtract();

	set<int> fSelected;
	for (int i=0; i<adaBoostT; ++i)
		fSelected.insert(featureSelected[i]);

	int n = SIZE;
	int m = SIZE;
	int cnt = 0;

	for (int i=1; i<=n; ++i)
		for (int j=1; j<=m; ++j)
			for (int x=2; x<=n-i+1; x++)
				for (int y=2; y<=m-j+1; y++){
					if (y%2==0){
						if (fSelected.find(cnt++) != fSelected.end())
							featureIndex.push_back(
									Feature(i, j, x, y, 0));
					}
					if (x%2==0){
						if (fSelected.find(cnt++) != fSelected.end())
							featureIndex.push_back(
									Feature(i, j, x, y, 1));
					}
					if (y%3==0){
						if (fSelected.find(cnt++) != fSelected.end())
							featureIndex.push_back(
									Feature(i, j, x, y, 2));
					}
					if (x%2==0 && y%2==0){
						if (fSelected.find(cnt++) != fSelected.end())
							featureIndex.push_back(
									Feature(i, j, x, y, 3));
					}
				}
}

void FeatureExtract::pixIntegral(Mat &src){
	int n = src.rows;
	int m = src.cols;
	for (int i=0; i<n; ++i){
		for (int j=0; j<m; ++j){
			pixInt[i+1][j+1] = pixInt[i][j+1] - pixInt[i][j] 
				+ pixInt[i+1][j] + src.at<uchar>(i,j);
		}
	}
}

int FeatureExtract::pixRectInt(int i, int j, int x, int y){
	return pixInt[x][y] - pixInt[i-1][y] + pixInt[i-1][j-1] - pixInt[x][j-1];
}

int FeatureExtract::Feature::getFeature(FeatureExtract *fe){
	switch (t){
		case 0:
			return fe->pixRectInt(i, j, x, y/2) 
					- fe->pixRectInt(i, j+y/2, x, y/2);
		case 1:
			return fe->pixRectInt(i, j, x/2, y) 
					- fe->pixRectInt(i+x/2, j, x/2, y);
		case 2:
			return fe->pixRectInt(i, j, x, y/3) 
					- fe->pixRectInt(i, j+y/3, x, y/3)
					+ fe->pixRectInt(i, j+y/3*2, x, y/3);
		case 3:
			return fe->pixRectInt(i, j, x/2, y/2) 
					- fe->pixRectInt(i, j+y/2, x/2, y/2) 
					- fe->pixRectInt(i + x/2, j, x/2, y/2) 
					+ fe->pixRectInt(i+x/2, j+y/2, x/2, y/2);
	}
	printf("featureExtract.feature.getFeature error!\n");
	return 0;
}

vector<int> FeatureExtract::getAdaBoostFeature(Mat &src){
	vector<int> feature;
	pixIntegral(src);

	for (int i=0; i<adaBoostT; ++i)
		feature.push_back(featureIndex[i].getFeature(this));

	return feature;
}


vector<int> FeatureExtract::getFeature(Mat &src){
	vector<int> feature;
	pixIntegral(src);

	int n = src.rows;
	int m = src.cols;

	for (int i=1; i<=n; ++i)
		for (int j=1; j<=m; ++j)
			for (int x=2; x<=n-i+1; x++)
				for (int y=2; y<=m-j+1; y++){
					if (y%2==0){
						feature.push_back(
								Feature(i, j, x, y, 0).getFeature(this));
					}
					if (x%2==0){
						feature.push_back(
								Feature(i, j, x, y, 1).getFeature(this));
					}
					if (y%3==0){
						feature.push_back(
								Feature(i, j, x, y, 2).getFeature(this));
					}
					if (x%2==0 && y%2==0){
						feature.push_back(
								Feature(i, j, x, y, 3).getFeature(this));
					}
				}

	return feature;
}

