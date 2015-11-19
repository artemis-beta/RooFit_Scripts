all:
	g++ -c -std=c++11 `root-config --cflags` First_RooFit_Toy.cpp
	g++ -o First_RooFit_Toy `root-config --glibs` -lRooFit -lRooFitCore First_RooFit_Toy.o 
