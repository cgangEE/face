#include "faceBase.h"

bool readTrainFromFile(vector<vector<int> > &x, vector<int> &y){
	FILE *fTrainX = fopen(trainFileNameX, "r");
	FILE *fTrainY = fopen(trainFileNameY, "r");

	if (!fTrainX){
		printf("file<%s> open failed\n", trainFileNameX);
		return false;
	}

	if (!fTrainY){
		printf("file<%s> open failed\n", trainFileNameY);
		return false;
	}

	int n, m, k;
	fscanf(fTrainX, "%d%d", &n, &m);

	for (int i=0; i<n; ++i){
		vector<int> feature;
		for (int j=0; j<m; ++j){
			fscanf(fTrainX, "%d", &k);
			feature.push_back(k);
		}
		x.push_back(feature);
	}


	for (int i=0; i<n; ++i){
		fscanf(fTrainY, "%d", &k);
		y.push_back(k);
	}

	fclose(fTrainX);
	fclose(fTrainY);

	return true;
}


bool saveAdaBoostToFile(
		double *error, double *theta, 
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


bool trainAdaBoost(vector<vector<int> > &x, vector<int> &y){
	int T = 20;
	int n = x.size();
	int m = x[0].size();


	int posCnt = 0;
	for (int i=0; i<y.size(); ++i)
		if (y[i]==1) posCnt++;
	vector<double> w(posCnt, 1.0 / posCnt);
	for (int i=0; i<y.size() - posCnt; ++i)
		w.push_back(1.0 / (y.size() - posCnt));


	double *error = (double*) malloc(sizeof(double) * adaBoostT);
	double *theta = (double*) malloc(sizeof(double) * adaBoostT);
	int *featureSelected = (int*) malloc(sizeof(int) * adaBoostT);
	int *sign = (int*) malloc(sizeof(int) * adaBoostT);


	for (int t=0; t<adaBoostT; ++t){
		double wSum = 0.0;
		double Pos = 0.0;
		double Neg = 0.0;

		for (int i=0; i<w.size(); ++i) 
			wSum += w[i];

		for (int i=0; i<w.size(); ++i){
			w[i] /= wSum;
			if (y[i]==1) 
				Pos += w[i];
			else
				Neg += w[i];
		}

		double errorJ = 1.0;
		double thetaJ;
		int signJ;
		int featureSelectedJ;

		for (int j=0; j<m; ++j){
			vector<pair<int, pair<int, double> > > featureJ;
			for (int i=0; i<n; ++i)
				featureJ.push_back(
						make_pair(x[i][j], 
							make_pair(y[i], w[i]))
							);
			sort(featureJ.begin(), featureJ.end());

			double neg = 0;
			double pos = 0;
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
				

		cout<<errorJ<<' '<<featureSelectedJ<<endl;


		error[t] = errorJ;
		theta[t] = thetaJ;
		featureSelected[t] = featureSelectedJ;
		sign[t] = signJ;
	}

	/*
	int wrong = 0;
	for (int i=0; i<n; ++i){
		double type = 0.0;
		for (int j=0; j<adaBoostT; ++j)
			type += log((1-error[j])/error[j])
				* ( 
				(sign[j] * x[i][featureSelected[j]] < sign[j] * theta[j]) ?
				1 : -1);
		if (type * y[i] < 0) 
			++wrong;
	}
	cout<<wrong<<endl;
	*/

	bool ret = saveAdaBoostToFile(error, theta, featureSelected, sign);
	free(error);
	free(theta);
	free(featureSelected);
	free(sign);

	return ret;
}

int main(){
	vector<vector<int> > x;
	vector<int> y;

	if (!readTrainFromFile(x, y)) return 1;
	if (!trainAdaBoost(x, y)) return 1;

	return 0;
}
