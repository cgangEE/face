main: dataClean testGen featureExtract adaBoost faceDetect cascade faceQuickDetect
	echo 'x'

faceDetect: faceDetect.cpp faceBase.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc faceDetect.cpp faceBase.o -o faceDetect

faceQuickDetect: faceQuickDetect.cpp faceBase.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc faceQuickDetect.cpp faceBase.o -o faceQuickDetect

adaBoost: adaBoost.cpp faceBase.h
	g++ adaBoost.cpp -o adaBoost

featureExtract: featureExtract.cpp faceBase.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc featureExtract.cpp faceBase.o -o featureExtract

testGen: testGen.cpp faceBase.o
	g++ testGen.cpp faceBase.o -o testGen

cascade: cascade.cpp faceBase.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc cascade.cpp faceBase.o -o cascade


dataClean: dataClean.cpp faceBase.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc dataClean.cpp faceBase.o -o dataClean


faceBase.o: faceBase.h faceBase.cpp 
	g++ -c faceBase.cpp 

