### dataClean

从origin-pic中读取的照片，分别平移、切割和缩放，并从中随机抽取，再筛选165个50X50的图片作为反例，放入到negative-pic中。

从yale中，把图像缩放和剪切为50X50的图片，共165张，作为正例存入到positive-pic中去。

### featureExtract.o

对 FeatureExtract class 的 getFeature 传入一幅 mat 图像，该函数返回一组 vector &lt;int&gt;特征。
