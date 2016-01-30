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

