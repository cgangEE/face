main: dataClean featureExtract detect camera getHardExample test
	echo 'x'

getHardExample: getHardExample.cpp base.o ini.o INIReader.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc getHardExample.cpp base.o ini.o INIReader.o libsvm/svm.o -o getHardExample

camera : camera.cpp base.o ini.o INIReader.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc camera.cpp base.o ini.o INIReader.o libsvm/svm.o -o camera

detect: detect.cpp base.o ini.o INIReader.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc detect.cpp base.o ini.o INIReader.o libsvm/svm.o -o detect

test: test.cpp base.o ini.o INIReader.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc test.cpp base.o ini.o INIReader.o libsvm/svm.o -o test

dataClean: dataClean.cpp base.o ini.o INIReader.o 
	g++  -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc dataClean.cpp base.o ini.o INIReader.o libsvm/svm.o -o dataClean

featureExtract: featureExtract.cpp  ini.o INIReader.o base.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc featureExtract.cpp ini.o INIReader.o base.o libsvm/svm.o -o featureExtract 

base.o: base.h base.cpp 
	g++ -c base.cpp 

INIReader.o: inih/cpp/INIReader.cpp inih/cpp/INIReader.h
	g++ -c inih/cpp/INIReader.cpp

ini.o: inih/ini.c
	gcc -c inih/ini.c 
