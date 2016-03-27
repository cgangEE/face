### faceBase

getFileNameFromDir 返回文件夹中的所有文件的路径。

FeatureExtract.getFeature(Mat src) 从图形 src 中获取一组 hog 特征。

### dataClean

把 peal 中的图片缩放到 90X90 大小的灰度图片，保存到 positive-pic中。
把 origic-pic 中的图片随机剪裁为 90X90 大小的灰度图片，保存到 negative-pic中。

### featureExtract

从 positive-pic 和 negative-pic 中抽取 HOG 特征，到train.txt中。

