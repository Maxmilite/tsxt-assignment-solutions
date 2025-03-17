program = main

headers = $(program).h
srcs = $(program).c
objects = $(program).o
opts = -g -c
dist = $(program)

all: main
main: $(objects)
	gcc $(objects) -o $(dist)
main.o: $(srcs) $(head)
	gcc $(opts) $(srcs)
clean:
	rm $(program) *.o