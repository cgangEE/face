main: bin/featureExtract bin/dataClean
	ls

bin/featureExtract: featureExtract.cpp faceBase.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc featureExtract.cpp faceBase.o -o bin/featureExtract

bin/dataClean: dataClean.cpp faceBase.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc dataClean.cpp faceBase.o -o bin/dataClean

faceBase.o: faceBase.h faceBase.cpp 
	g++ -c faceBase.cpp 

