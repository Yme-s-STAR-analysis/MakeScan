all: scan

scan: main.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++17 -O3