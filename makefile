CXX=g++
CXXFLAGS=-c -O3 -D_GLIBCXX_USE_CXX11_ABI=0 # I have no clue what this does, but it makes gurobi do what it is supposed to do
LDFLAGS=
SOURCES=$(shell find . -name "*.cpp") # searches all subdirectories
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=exe

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o exe -I$(GUROBI_HOME)/include -L$(GUROBI_HOME)/lib -lgurobi_c++ -lgurobi70 -lm # link with correct gurobi libraries (...) 


.cpp.o:
	$(CXX) $(CXXFLAGS) $<  -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)


