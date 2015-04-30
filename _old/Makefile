RM = rm -f

CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
CPPFILES = src/Facet.cpp src/Fill.cpp src/Math.cpp src/Part.cpp src/STLParser.cpp src/Vector.cpp
INCLUDE = /usr/local/opt/boost/include
LIBRARIES = /usr/local/opt/boost/lib

all: main bin_to_ascii_stl

main: bin bin/main

bin_to_ascii_stl: bin bin/bin_to_ascii_stl

bin:
	mkdir -p bin

bin/main: src/main.cpp $(CPPFILES)
	$(CXX) $< $(CPPFILES) $(CXXFLAGS) -I $(INCLUDE) -L $(LIBRARIES) -o $@

bin/bin_to_ascii_stl: src/bin_to_ascii_stl.cpp $(CPPFILES)
	$(CXX) $< $(CPPFILES) $(CXXFLAGS) -I $(INCLUDE) -L $(LIBRARIES) -o $@

clean:
	$(RM) -r bin

.PHONY: all main bin_to_ascii_stl clean
