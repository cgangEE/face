#include "faceBase.h"

bool featureExtractFromDir(const char *dir, 
		vector<vector<floatType> > &x, vector<int> &y, int type){

	vector<string> fileName;
	if (!getFileNameFromDir(dir, fileName)) return false;

	FeatureExtract featureExtract;

	for (int i=0; i<fileName.size(); ++i){

		Mat src = imread(fileName[i].c_str());
		cvtColor(src, src, CV_BGR2GRAY);

		for (int i=0; i<2; ++i){
			if (i) 
				flip(src, src, 1);
			vector<floatType> feature = featureExtract.getFeature(src);

			x.push_back(feature);
			y.push_back(type);
		}
	}

	return true;
}

bool writeFeatureToFile(vector<vector<floatType> > &x, vector<int> &y){
	FILE *fTrainX = fopen(trainFileNameX, "wb");
	FILE *fTrainY = fopen(trainFileNameY, "wb");

	if (!fTrainX){
		printf("file<%s> open failed!\n", trainFileNameX);
		return false;
	}
	if (!fTrainY){
		printf("file<%s> open failed!\n", trainFileNameY);
		return false;
	}

	int n = x.size();
	int m = x[0].size();

	floatType *buf = (floatType *)malloc(m * sizeof(floatType));
	int *bufInt = (int *)malloc(n * sizeof(int));
	
	fwrite(&n, sizeof(int), 1, fTrainX);
	fwrite(&m, sizeof(int), 1, fTrainX);

	for (int i=0; i<n; ++i){
		for (int j=0; j<m; ++j)
			buf[j] = x[i][j];
		fwrite(buf, sizeof(floatType), m, fTrainX);
	}

	for (int i=0; i<n; ++i)
		bufInt[i] = y[i];
	fwrite(bufInt, sizeof(int), n, fTrainY);

	free(buf);
	free(bufInt);

	fclose(fTrainX);
	fclose(fTrainY);
	return true;
}

int main(){
	vector<vector<floatType> > x;
	vector<int> y;

	if (!featureExtractFromDir(posDir, x, y, 1)) return 1;
	if (!featureExtractFromDir(negDir, x, y, -1)) return 1;

	if (!writeFeatureToFile(x, y)) return 1;

	return 0;
}
