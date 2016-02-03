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

FeatureExtract::FeatureExtract(){
	memset(pixInt, 0, sizeof(pixInt));
}

int FeatureExtract::pixRectInt(int i, int j, int x, int y){
	return pixInt[x][y] - pixInt[i-1][y] + pixInt[i-1][j-1] - pixInt[x][j-1];
}

vector<int> FeatureExtract::getFeature(Mat &src){
	vector<int> feature;

	int n = src.rows;
	int m = src.cols;
	for (int i=0; i<n; ++i){
		for (int j=0; j<m; ++j){
			pixInt[i+1][j+1] = pixInt[i][j+1] - pixInt[i][j] 
				+ pixInt[i+1][j] + src.at<uchar>(i,j);
		}
	}

	for (int i=1; i<=n; ++i)
		for (int j=1; j<=m; ++j)
			for (int x=2; x<=n-i+1; x+=3)
				for (int y=2; y<=m-j+1; y+=3){
					if (y%2==0){
						feature.push_back( 
								pixRectInt(i, j, x, y/2) 
								- pixRectInt(i, j+y/2, x, y/2));
					}
					if (x%2==0){
						feature.push_back( 
								pixRectInt(i, j, x/2, y) 
								- pixRectInt(i+x/2, j, x/2, y));
					}
					if (y%3==0){
						feature.push_back(
								pixRectInt(i, j, x, y/3) 
								- pixRectInt(i, j+y/3, x, y/3)
							 	+ pixRectInt(i, j+y/3*2, x, y/3));
					}
					if (x%2==0 && y%2==0){
						feature.push_back(
								pixRectInt(i, j, x/2, y/2) 
								- pixRectInt(i, j+y/2, x/2, y/2) 
								- pixRectInt(i + x/2, j, x/2, y/2) 
								+ pixRectInt(i+x/2, j+y/2, x/2, y/2));
					}
				}

	return feature;
}

