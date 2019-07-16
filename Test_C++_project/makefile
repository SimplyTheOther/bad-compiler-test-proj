objects = main.o functions.o stupidclasses.o
CXXFLAGS = "-g -Wall -Werror"
LDFLAGS = ""
CXX = g++

test: $(objects)
	$(CXX) $(CXXFLAGS) -o test $(objects) 

main.o : functions.h stupidclasses.h
functions.o : functions.h
stupidclasses.o : stupidclasses.h

.PHONY : clean
clean :
	rm test $(objects)