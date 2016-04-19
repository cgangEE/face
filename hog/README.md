### base 

getFileNameFromDir 返回文件夹中的所有文件的路径。

FeatureExtract.getFeature(Mat src) 从图形 src 中获取一组 hog 特征。

### dataClean

把 INRIAPerson/train-64x128-H96/ 中的图片剪切好放入 positive-pic 中

把 INRIAPerson/test-64x128-H96/ 中的图片随机剪切好放入 negative-pic 中

### featureExtract

从 positive-pic 和 negative-pic 中抽取 HOG 特征，到train.txt中。

### getHardExample

把 INRIAPerson/test-64x128-H96/ 中的图片随机剪切好，并选取出svm-predict为1的hard example 存放到 negative-pic 中。

