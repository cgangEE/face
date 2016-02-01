#include "faceBase.h"

bool featureExtractFromDir(const char *dir, 
		vector<vector<int> > &x, vector<int> &y, int type){

	vector<string> fileName;
	if (!getFileNameFromDir(dir, fileName)) return false;

	FeatureExtract featureExtract;

	for (int i=0; i<fileName.size(); ++i){
		Mat src = imread(fileName[i].c_str());
		vector<int> feature = featureExtract.getFeature(src);

		x.push_back(feature);
		y.push_back(type);
	}

	return true;
}

int main(){
	vector<vector<int> > x;
	vector<int> y;

	if (!featureExtractFromDir(posDir, x, y, 1)) return 1;
	if (!featureExtractFromDir(negDir, x, y, -1)) return 1;

	FILE *fTrainX = fopen(trainFileNameX, "w");
	FILE *fTrainY = fopen(trainFileNameY, "w");
	
	if (!fTrainX){
		printf("file<%s> open failed!\n", trainFileNameX);
		return 1;
	}
	if (!fTrainY){
		printf("file<%s> open failed!\n", trainFileNameY);
		return 1;
	}

	fprintf(fTrainX, "%lu %lu\n", x.size(), x[0].size());

	for (int i=0; i<x.size(); ++i){
		for (int j=0; j<x[i].size(); ++j)
			fprintf(fTrainX, "%d ", x[i][j]);
		fprintf(fTrainX, "\n");
	}

	for (int i=0; i<y.size(); ++i){
		fprintf(fTrainY, "%d\n", y[i]);
	}

	fclose(fTrainX);
	fclose(fTrainY);
	return 0;
}
