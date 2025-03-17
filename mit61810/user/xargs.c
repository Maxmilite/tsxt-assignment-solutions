#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/syscall.h"
#include "user/user.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(STDERR, "xargs: need at least one argument\n");
        exit(1);
    }

    char* args[argc];
    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }
    args[argc - 1] = 0;

    char buf[512];
    int n;

    char queue[512];
    int len = 0;
    while ((n = read(STDIN, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; i++) {
            if (buf[i] == ' ' || buf[i] == '\n') {
                queue[len] = 0;
                args[argc - 1] = queue;

                int pid = fork();
                if (pid < 0) {
                    fprintf(STDERR, "xargs: fork failed\n");
                    exit(1);
                }
                if (pid == 0) {
                    exec(argv[1], args);
                    exit(0);
                }
                wait(0);

                len = 0;
            } else {
                queue[len++] = buf[i];
            }
        }
    }

    if (n < 0) {
        fprintf(2, "xargs: read error\n");
        exit(1);
    }

    exit(0);    
}