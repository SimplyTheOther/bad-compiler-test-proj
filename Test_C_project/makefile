objects = main.o functions.o noise_gen.o
CFLAGS = "-g"
LDFLAGS = ""
CC = gcc

test: $(objects)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o test $(objects)

main.o : functions.h noise_gen.h
functions.o : functions.h
noise_gen.o : noise_gen.h

.PHONY : clean
clean :
	rm edit $(objects)