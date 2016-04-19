#include "base.h"

bool featureExtractFromDir(const char *dir, 
		vector<vector<floatType> > &x, vector<int> &y, int type){

	vector<string> fileName;
	if (!getFileNameFromDir(dir, fileName)) return false;

	FeatureExtract featureExtract;

	for (int i=0; i<fileName.size(); ++i){

		Mat src = imread(fileName[i].c_str());

		for (int i=0; i<1; ++i){
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
	FILE *fTrain = fopen(trainFileName, "w");

	if (!fTrain){
		printf("file<%s> open failed!\n", trainFileName);
		return false;
	}

	int n = x.size();
	int m = x[0].size();

	cout<<n<<' '<<m<<endl;

	
	for (int i=0; i<n; ++i){
		fprintf(fTrain, "%d", y[i]);
		for (int j=1; j<=m; ++j)
			fprintf(fTrain, " %d:%f", j, x[i][j-1]);
	
		fprintf(fTrain, "\n");
	}

	fclose(fTrain);
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
