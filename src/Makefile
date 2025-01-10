CXX = g++
CXXFLAGS =
OUTPUT_NAME = fourmis.so

default: fourmis.so

fourmis.so: fourmis.cpp
	gcc -o fourmis.so -shared -fPIC fourmis.cpp

clean:
	$(RM) *.o
	$(RM) fourmis.so
