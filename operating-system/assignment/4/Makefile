program = main

consumer = smoker
producer = producer
manager = manager

headers = $(program).h ipc.h
srcs = $(program).c
objects = $(program).o
opts = -g -c -lpthread
dist = $(program)

consumer_src = $(consumer).c
producer_src = $(producer).c
manager_src = $(manager).c

consumer_obj = $(consumer).o
producer_obj = $(producer).o
manager_obj = $(manager).o

consumer_dist = $(consumer)
producer_dist = $(producer)
manager_dist = $(manager)

all: consumer producer manager

consumer: $(consumer_obj)
	gcc $(consumer_obj) -o $(consumer_dist)
consumer.o: $(consumer_src) $(head)
	gcc $(opts) $(consumer_src)

producer: $(producer_obj)
	gcc $(producer_obj) -o $(producer_dist)
producer.o: $(producer_src) $(head)
	gcc $(opts) $(producer_src)

manager: $(manager_obj)
	gcc $(manager_obj) -o $(manager_dist)
manager.o: $(manager_src) $(head)
	gcc $(opts) $(manager_src)

main: $(objects)
	gcc $(objects) -o $(dist)
main.o: $(srcs) $(head)
	gcc $(opts) $(srcs)
clean:
	rm $(consumer) $(producer) $(manager) $(program) *.o