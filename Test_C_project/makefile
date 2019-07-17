objects = main.o functions.o noise_gen.o
CFLAGS = -O0 -g -Wall -Werror
#LDFLAGS = ""
CC = gcc

test: $(objects)
	$(CC) -o test $(objects) $(LDFLAGS)

# specifies dependencies for creation of each object file 
# rules not needed because make has .c to .o rule known by default
main.o : functions.h noise_gen.h
functions.o : functions.h
noise_gen.o : noise_gen.h

# used for "make clean" - deletes generated files
.PHONY : clean
clean :
	rm test $(objects)
	rm out.log error.log