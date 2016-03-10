#include "faceBase.h"

bool extractToTest(const char *src, const char *dst){
	vector<string> fileNameVec;
	if (!getFileNameFromDir(src, fileNameVec)) return false;

	for (int i=0; i<fileNameVec.size(); ++i)
		if (rand()%10==0){
			string cmd = "mv ";
			cmd = cmd + fileNameVec[i];

			string fileName = "";
			
			for (int j=fileNameVec[i].size()-1; 
					fileNameVec[i][j]!='/' && j>=0; --j)
				fileName = fileNameVec[i][j] + fileName;


			cmd = cmd + " " + dst + "/" + fileName;
			system(cmd.c_str());
		}

	return true;
}

int main(){
	srand(time(NULL));
	if (!extractToTest(posDir, posTestDir)) return 1;
	if (!extractToTest(negDir, negTestDir)) return 1;
	return 0;
}
