#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/syscall.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
	if (argc != 1) {
		printf("pingpong: need no parameter\n");
	}

	int pipe1[2], pipe2[2];
    {
        int stat = pipe(pipe1);
        if (stat != 0) {
            printf("pingpong: pipe() failed\n");
            exit(0);
        }
    }
    {
        int stat = pipe(pipe2);
        if (stat != 0) {
            printf("pingpong: pipe() failed\n");
            exit(0);
        }
    }

	int pid = fork();
	if (pid < 0) {
		printf("pingpong: fork() failed\n");
        exit(0);
	}
    
    if (pid == 0) {
		int pid_c = getpid();
		char buf[1024];
		read(pipe1[0], buf, sizeof(buf));
		if (buf[0] == 'a') {
			printf("%d: received ping\n", pid_c);
			write(pipe2[1], "b", 1);
		}
	} else {
		int pid_f = getpid();
		write(pipe1[1], "a", 1);
		char buf[1024];
		read(pipe2[0], buf, sizeof(buf));
		if (buf[0] == 'b') {
			printf("%d: received pong\n", pid_f);
		}
	}
	exit(0);
}