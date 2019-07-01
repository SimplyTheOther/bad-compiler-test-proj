objects = main.o functions.o
CFLAGS = "-g"
LDFLAGS = ""
CC = gcc

test: $(objects)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o test $(objects)

main.o : functions.h
functions.o : functions.h

.PHONY : clean
clean :
	rm edit $(objects)