CXX = g++
CXXFLAGS =
OUTPUT_NAME = fourmis.so

default: fourmis.so

fourmis.so: fourmis.cpp memoire.c ram.c graph.cpp thread_queue.h
	$(CXX) -o fourmis.so -shared -fPIC fourmis.cpp graph.cpp thread_queue.h

clean:
	$(RM) *.o
	$(RM) fourmis.so
