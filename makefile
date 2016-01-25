featureExtract: featureExtract.cpp faceTool.o dataClean
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc featureExtract.cpp faceTool.o -o featureExtract

dataClean: dataClean.cpp faceTool.o
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc dataClean.cpp faceTool.o -o dataClean

faceTool.o: faceTool.h faceTool.cpp
	g++ -c faceTool.cpp 

