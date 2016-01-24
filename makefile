dataClean: dataClean.cpp
	g++ -I/usr/local/include -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc dataClean.cpp -o dataClean

