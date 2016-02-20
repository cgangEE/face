#include "faceBase.h"

bool readTrainFromFile(vector<vector<floatType> > &x, vector<int> &y){
	FILE *fTrainX = fopen(trainFileNameX, "rb");
	FILE *fTrainY = fopen(trainFileNameY, "rb");

	if (!fTrainX){
		printf("file<%s> open failed\n", trainFileNameX);
		return false;
	}

	if (!fTrainY){
		printf("file<%s> open failed\n", trainFileNameY);
		return false;
	}

	int n, m, k;

	fread(&n, sizeof(int), 1, fTrainX);
	fread(&m, sizeof(int), 1, fTrainX);

	floatType *buf = (floatType*)malloc(m * sizeof(floatType));
	int *bufInt = (int *)malloc(n * sizeof(int));

	for (int i=0; i<n; ++i){
		fread(buf, sizeof(floatType), m, fTrainX);
		x.push_back(vector<floatType>());

		for (int j=0; j<m; ++j)
			x[i].push_back(buf[j]);
	}


	fread(bufInt, sizeof(int), n, fTrainY);
	for (int i=0; i<n; ++i)
		y.push_back(bufInt[i]);


	free(buf);
	free(bufInt);

	fclose(fTrainX);
	fclose(fTrainY);

	return true;
}


bool saveAdaBoostToFile(
		floatType *error, floatType *theta, 
		int *featureSelected, int *sign){

	FILE *fAdaBoostFinal = fopen(adaBoostFinal, "wb");
	if (!fAdaBoostFinal){
		printf("open file<%s> error!\n", adaBoostFinal);
		return false;
	}


	fwrite(error, sizeof(error[0]), adaBoostT, fAdaBoostFinal);
	fwrite(theta, sizeof(theta[0]), adaBoostT, fAdaBoostFinal);
	fwrite(featureSelected, sizeof(featureSelected[0]), adaBoostT, fAdaBoostFinal);
	fwrite(sign, sizeof(sign[0]), adaBoostT, fAdaBoostFinal);

	fclose(fAdaBoostFinal);
	return true;
}


bool trainAdaBoost(vector<vector<floatType> > &x, vector<int> &y){
	int n = x.size();
	int m = x[0].size();


	int posCnt = 0;
	for (int i=0; i<y.size(); ++i)
		if (y[i]==1) posCnt++;


	vector<floatType> w(posCnt, 1.0 / posCnt);
	for (int i=0; i<y.size() - posCnt; ++i)
		w.push_back(1.0 / (y.size() - posCnt));


	floatType *error = (floatType*) malloc(sizeof(floatType) * adaBoostT);
	floatType *theta = (floatType*) malloc(sizeof(floatType) * adaBoostT);
	int *featureSelected = (int*) malloc(sizeof(int) * adaBoostT);
	int *sign = (int*) malloc(sizeof(int) * adaBoostT);


	for (int t=0; t<adaBoostT; ++t){
		floatType wSum = 0.0;
		floatType Pos = 0.0;
		floatType Neg = 0.0;

		for (int i=0; i<w.size(); ++i) 
			wSum += w[i];

		for (int i=0; i<w.size(); ++i){
			w[i] /= wSum;
			if (y[i]==1) 
				Pos += w[i];
			else
				Neg += w[i];
		}

		floatType errorJ = 1.0;
		floatType thetaJ;
		int signJ;
		int featureSelectedJ;

		for (int j=0; j<m; ++j){
			vector<pair<floatType, pair<int, floatType> > > featureJ;
			for (int i=0; i<n; ++i)
				featureJ.push_back(
						make_pair(x[i][j], 
							make_pair(y[i], w[i]))
							);
			sort(featureJ.begin(), featureJ.end());

			floatType neg = 0.0;
			floatType pos = 0.0;
			for (int i=0; i<n; ++i){
				if (featureJ[i].second.first == 1)
					pos += featureJ[i].second.second;
				else 
					neg += featureJ[i].second.second;

				if (i!=n-1 && featureJ[i].first == featureJ[i+1].first)
					continue;

				if (min(pos+Neg-neg, neg+Pos-pos) < errorJ){
					errorJ = min(pos+Neg-neg, neg+Pos-pos);
					featureSelectedJ = j;
					signJ = (pos+Neg-neg < neg+Pos-pos) ? -1 : 1;
					thetaJ = (i==n-1) ? featureJ[i].first + 1
						: (featureJ[i].first + featureJ[i+1].first) / 2;
				}

			}
		}

		for (int i=0; i<w.size(); ++i)
			if ( !( (signJ * x[i][featureSelectedJ] < signJ * thetaJ)
					xor (y[i]==1)  )){
				w[i] *= errorJ/(1-errorJ);
			}


		printf("%.10lf\t%d\n", errorJ, featureSelectedJ);

		error[t] = errorJ;
		theta[t] = thetaJ;
		featureSelected[t] = featureSelectedJ;
		sign[t] = signJ;
	}


	int wrong = 0;
	for (int i=0; i<n; ++i){
		floatType type = 0.0;
		for (int j=0; j<adaBoostT; ++j)
			type += log((1-error[j])/error[j])
				* ( 
				(sign[j] * x[i][featureSelected[j]] < sign[j] * theta[j]) ?
				1 : -1);
		if (type * y[i] < 0) 
			++wrong;
	}


	cout<<wrong<<endl;


	bool ret = saveAdaBoostToFile(error, theta, featureSelected, sign);
	free(error);
	free(theta);
	free(featureSelected);
	free(sign);

	return ret;
}

int main(){
	vector<vector<floatType> > x;
	vector<int> y;

	if (!readTrainFromFile(x, y)) return 1;
	if (!trainAdaBoost(x, y)) return 1;

	return 0;
}
