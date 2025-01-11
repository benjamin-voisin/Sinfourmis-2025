CXX = g++
CXXFLAGS =
OUTPUT_NAME = fourmis.so

default: fourmis.so

fourmis.so: fourmis.cpp memoire.c ram.c graph.cpp queen/thread_queue.h queen/queen.hpp queen/queen.cpp
	$(CXX) -o $< -shared -fPIC $^

clean:
	$(RM) *.o
	$(RM) fourmis.so
