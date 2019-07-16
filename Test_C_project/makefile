objects = main.o functions.o noise_gen.o
CFLAGS = "-g"
LDFLAGS = ""
CC = gcc

test: $(objects)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o test $(objects)

main.o : functions.h noise_gen.h
functions.o : functions.h
noise_gen.o : noise_gen.h

# used for "make clean" - deletes generated files
.PHONY : clean
clean :
	rm test $(objects)
	rm out.log error.log