#include "faceBase.h"


bool readAdaBoostFromFile(
		floatType *error, floatType *theta, 
		int *featureSelected, int *sign){

	FILE *fAdaBoostFinal = fopen(adaBoostFinal, "rb");
	if (!fAdaBoostFinal){
		printf("open file<%s> error!\n", adaBoostFinal);
		return false;
	}

	fread(error, sizeof(error[0]), adaBoostT, fAdaBoostFinal);
	fread(theta, sizeof(theta[0]), adaBoostT, fAdaBoostFinal);
	fread(featureSelected, sizeof(featureSelected[0]), adaBoostT, fAdaBoostFinal);
	fread(sign, sizeof(sign[0]), adaBoostT, fAdaBoostFinal);

	fclose(fAdaBoostFinal);
	return true;
}


floatType adaBoostFinalH(FeatureExtract &featureExtract, Mat &src, 
		vector< pair< pair<floatType, floatType>, pair<int,int> > > &feature, 
		int idx, int cnt){

	vector<floatType> f = featureExtract.getAdaBoostFeature(src);

	floatType ret = 0.0;
	for (int i=0; i<cnt; ++i){
		floatType &error = feature[idx+i].first.first;
		floatType &theta = feature[idx+i].first.second;
		int &sign = feature[idx+i].second.second;
		ret += log((1-error)/error)
			* ( (sign * f[i] < sign * theta) ?
					1 : -1);
	}
	return ret;
}


bool trainCascade(floatType *error, floatType *theta,
				int *featureSelected, int *sign,
				floatType * &threshold, 
				int &n, int *&featureC){

	vector< pair< pair< floatType, floatType>, 
			pair< int, int> > > feature;
	
	for (int i=0; i<adaBoostT; ++i)
		feature.push_back(make_pair( 
					make_pair(error[i], theta[i]),
					make_pair(featureSelected[i], sign[i])
					));

	sort(feature.begin(), feature.end());

	for (int i=0; i<adaBoostT; ++i){
		error[i] = feature[i].first.first;
		theta[i] = feature[i].first.second;
		featureSelected[i] = feature[i].second.first;
		sign[i] = feature[i].second.second;
	}


	vector<int> featureCnt;
	featureCnt.push_back(10);

	int total = adaBoostT;
	for (int i=0; i<featureCnt.size(); ++i)
		total -= featureCnt[i];

	while (total){
		int fCntI = min(featureCnt.back() * 1, total);
		featureCnt.push_back(fCntI);
		total -= fCntI;
	}


	n = featureCnt.size();

	featureC = (int*) malloc(sizeof(int) * n);
	for (int i=0; i<n; ++i) 
		featureC[i] = featureCnt[i];

	vector<string> posTest;
	vector<string> negTest;

	if (!getFileNameFromDir(posTestDir, posTest)) return false;
	if (!getFileNameFromDir(negTestDir, negTest)) return false;

	floatType *score = (floatType*) 
			malloc(sizeof(floatType) * (posTest.size() + negTest.size()));
	threshold = (floatType *) malloc(sizeof(floatType) * featureCnt.size());


	int idx = 0;
	for (int i=0; i<featureCnt.size(); ++i){
		memset(score, 0, sizeof(floatType) * (posTest.size() + negTest.size()));

		int *fSelected = (int*) malloc(sizeof(int) * featureCnt[i]);
		for (int j=0; j<featureCnt[i]; ++j)
			fSelected[j] = feature[j+idx].second.first;


		FeatureExtract featureExtract(fSelected, featureCnt[i]);

		for (int j=0; j<posTest.size(); ++j){
			Mat src = imread(posTest[j].c_str());
			score[j] += adaBoostFinalH(featureExtract, src, 
												feature, idx, featureCnt[i]);
		}

		for (int j=0; j<negTest.size(); ++j){
			Mat src = imread(negTest[j].c_str());
			score[posTest.size() + j] += adaBoostFinalH(featureExtract, src, 
												feature, idx, featureCnt[i]);
		}

		floatType min1 = 1e50;
		floatType min2 = 1e50;

		for (int j=0; j<negTest.size(); ++j){
			if (score[posTest.size() + j] < min1){
				min2 = min1;
				min1 = score[posTest.size() + j];
			} else if (score[posTest.size() + j] != min1)
				min2 = min(min2, score[posTest.size() + j]);
		}
		
		threshold[i] = min2;

		free(fSelected);
		idx += featureCnt[i];
	}
	
	return true;
}

bool saveCascadeToFile(floatType *error, floatType *theta,
				int *featureSelected, int *sign,
				floatType * threshold,
				int n, int *featureC){

	FILE *fCascade= fopen(cascadeFinal, "wb");
	if (!fCascade){
		printf("open file<%s> error!\n", cascadeFinal);
		return false;
	}

	fwrite(error, sizeof(error[0]), adaBoostT, fCascade);
	fwrite(theta, sizeof(theta[0]), adaBoostT, fCascade);
	fwrite(featureSelected, sizeof(featureSelected[0]), adaBoostT, fCascade);
	fwrite(sign, sizeof(sign[0]), adaBoostT, fCascade);


	fwrite(&n, sizeof(n), 1, fCascade);
	fwrite(featureC, sizeof(featureC[0]), n, fCascade);
	fwrite(threshold, sizeof(threshold[0]), n, fCascade);

	fclose(fCascade);
	return true;
}

int main(){
	floatType *error = (floatType*) malloc(sizeof(floatType) * adaBoostT);
	floatType *theta = (floatType*) malloc(sizeof(floatType) * adaBoostT);
	int *featureSelected = (int*) malloc(sizeof(int) * adaBoostT);
	int *sign = (int*) malloc(sizeof(int) * adaBoostT);


	if (!readAdaBoostFromFile(error, theta, featureSelected, sign)) return 1;

	floatType *threshold;
	int *featureC;
	int n;

	if (!trainCascade(error, theta, featureSelected, sign, 
				threshold, n, featureC)) return 1;

	if (!saveCascadeToFile(error, theta, featureSelected, sign, 
				threshold, n, featureC)) return 1;

	free(error);
	free(theta);
	free(featureSelected);
	free(sign);
	free(threshold);
	free(featureC);

	return 0;
}
