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


bool saveAdaBoostToFile(vector<double> &error, 
		vector<double> &theta,
		vector<int> &featureSelected, vector<int> &sign){

	FILE *fAdaBoostFinal = fopen(adaBoostFinal, "wb");
	if (!fAdaBoostFinal){
		printf("open file<%s> error!\n", adaBoostFinal);
		return false;
	}


	int n = error.size();

	fwrite(&error[0], sizeof(error[0]), n, fAdaBoostFinal);
	fwrite(&theta[0], sizeof(theta[0]), n, fAdaBoostFinal);
	fwrite(&featureSelected[0], sizeof(featureSelected[0]), n, fAdaBoostFinal);
	fwrite(&sign[0], sizeof(sign[0]), n, fAdaBoostFinal);

	fclose(fAdaBoostFinal);
	return true;
}


bool trainAdaBoost(vector<vector<int> > &x, vector<int> &y){
	int T = 20;
	int n = x.size();
	int m = x[0].size();

	vector<double> w(n, 1.0/n);

	vector<double> error;
	vector<double> theta;
	vector<int> sign;
	vector<int> featureSelected;


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

		error.push_back(errorJ);
		theta.push_back(thetaJ);
		featureSelected.push_back(featureSelectedJ);
		sign.push_back(signJ);
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

	return saveAdaBoostToFile(error, theta, featureSelected, sign);
}

int main(){
	vector<vector<int> > x;
	vector<int> y;

	if (!readTrainFromFile(x, y)) return 1;
	if (!trainAdaBoost(x, y)) return 1;

	return 0;
}