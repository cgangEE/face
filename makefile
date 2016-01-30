main: featureExtract.o dataClean
	ls

featureExtract.o: featureExtract.h featureExtract.cpp faceBase.h
	g++ -c -I/usr/local/include featureExtract.cpp 

dataClean: dataClean.cpp faceBase.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc dataClean.cpp faceBase.o -o dataClean

faceBase.o: faceBase.h faceBase.cpp 
	g++ -c faceBase.cpp 

