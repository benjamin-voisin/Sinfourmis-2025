CXX = g++
CXXFLAGS =
OUTPUT_NAME = fourmis.so

default: fourmis.so

fourmis.so: fourmis.cpp memoire.c ram.c graph.cpp
	$(CXX) -o fourmis.so -shared -fPIC fourmis.cpp graph.cpp

clean:
	$(RM) *.o
	$(RM) fourmis.so
