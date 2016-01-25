#include "faceTool.h"

int pixInt[SIZE+10][SIZE+10];

void haarFeatureExtract(Mat &src, vector<int> &feature){
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
			for (int x=2; x<=n-i+1; ++x)
				for (int y=2; y<=m-j+1; ++y){
					if (y%2==0){
						feature.push_back( 
								sum(i, j, x, y/2) - sum(i, j+y/2, x, y/2));
					}
					if (x%2==0){
						feature.push_back(
								sum(i, j, x/2, y) - sum(i+x/2, j, x/2, y));
					}
					if (y%3==0){
						feature.push_back(
								sum(i, j, x, y/3) - sum(i, j+y/3, x, y/3)
							 	+ sum(i, j+y/3*2, x, y/3));
					}
					if (x%2==0 && y%2==0){
						feature.push_back(
								sum(i, j, x/2, y/2) - sum(i, j+y/2, x/2, y/2) 
								-sum(i + x/2, j, x/2, y/2) 
								+ sum(i+x/2, j+y/2, x/2, y/2));
					}
				}
}

bool featureExtract(const char *dir, 
		vector<vector<int> > &x, vector<int> y, int type){
	y.push_back(type);

	vector<string> fileName;
	if (!getFileNameFromDir(dir, fileName)) return false;

	for (int l=0; l<fileName.size(); ++l){
		Mat src = imread(fileName[l].c_str());

		vector<int> feature;
		haarFeatureExtract(src, feature);
		x.push_back(feature);
	}
	return true;
}

int main(){
	vector<vector<int> > x;
	vector<int> y;
	if (!featureExtract(posDir, x, y, 1)) return -1;
	if (!featureExtract(negDir, x, y, 0)) return -1;
}
