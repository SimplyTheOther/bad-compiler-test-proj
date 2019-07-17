objects = main.o functions.o stupidclasses.o
CXXFLAGS = -O0 -g -Wall -Werror
#LDFLAGS = 
CXX = g++

test: $(objects)
	$(CXX) -o test $(objects) $(LDFLAGS)

# specifies dependencies for creation of each object file 
# rules not needed because make has .cpp to .o rule known by default
main.o : functions.h stupidclasses.h
functions.o : functions.h
stupidclasses.o : stupidclasses.h

# used to delete generated files with "make clean"
.PHONY: clean
clean :
	rm test $(objects)