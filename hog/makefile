main: dataClean featureExtract faceDetect
	echo 'x'

faceDetect: faceDetect.cpp base.o ini.o INIReader.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc faceDetect.cpp base.o ini.o INIReader.o libsvm/svm.o -o faceDetect

dataClean: dataClean.cpp base.o ini.o INIReader.o 
	g++  -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc dataClean.cpp base.o ini.o INIReader.o -o dataClean

featureExtract: featureExtract.cpp  ini.o INIReader.o base.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc featureExtract.cpp ini.o INIReader.o base.o  -o featureExtract 

base.o: base.h base.cpp 
	g++ -c base.cpp 

INIReader.o: inih/cpp/INIReader.cpp inih/cpp/INIReader.h
	g++ -c inih/cpp/INIReader.cpp

ini.o: inih/ini.c
	gcc -c inih/ini.c 
