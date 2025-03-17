gcc main.c -o main -lwiringPi -lpthread
gcc socket.c -o socket
gcc line_tracing.c -o line_tracing -lwiringPi -lpthread
gcc ultra_sonic.c -o ultra_sonic -lwiringPi -lpthread
