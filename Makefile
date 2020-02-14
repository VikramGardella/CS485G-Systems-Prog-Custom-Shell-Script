SOURCES = main.cpp
OBJECTS = main.o

proj4: main.cpp
	g++ -o proj4 main.cpp

CXXFLAGS = -std=c++11 -D_GNU_SOURCE -Wall

