CXX=g++
CXXFLAGS=-c -Wall -Wextra -ggdb3 -fsanitize=leak -lasan
SOURCES=main.cpp stackUtils.cpp configStack.cpp universalUtils.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clear:
	rm -rf *.o $(EXECUTABLE)

lclear:
	rm -rf logOutput.txt