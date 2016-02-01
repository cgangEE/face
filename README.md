### faceBase

对 FeatureExtract class 的 getFeature 传入一幅 mat 图像，该函数返回一组 vector &lt;int&gt; 特征。

getFileNameFromDir 返回文件夹中的所有文件的路径。

### dataClean

从origin-pic中读取的照片，分别平移、切割和缩放，并从中随机抽取，再筛选165个50X50的图片作为反例，放入到negative-pic中。

从yale中，把图像缩放和剪切为50X50的图片，共165张，作为正例存入到positive-pic中去。

### featureExtract

从positive-pic的正样本和negative-pic的负样本中提取特征保存到trainX.txt中，样本的类别保存到trainY.txt中。

### addBoost

从 trainX.txt 和 trainY.txt 中读取特征和类别，利用AdaBoost算法训练，并把分类器的参数存入到 adaBoostFinal.txt 文件中。

### faceDetect



