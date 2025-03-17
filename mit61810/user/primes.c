#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/syscall.h"
#include "user/user.h"

const int MAX = 35;

void sieve(int read_p) {
    int p[2];
    pipe(p);

    int prime;
    if (read(read_p, &prime, sizeof(prime)) == 0) {
        close(p[1]);
        close(p[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    int pid = fork();
    if (pid == 0) {
        close(p[1]);
        sieve(p[0]);
    } else {
        close(p[0]);
        while (1) {
            int n;
            if (read(read_p, &n, sizeof(n)) == 0) {
                break;
            }
            if (n % prime != 0) {
                write(p[1], &n, sizeof(n));
            }
        }
        close(read_p);
        close(p[1]);
    }
}

int main(int argc, char *argv[]) {
	if (argc != 1) {
		printf("prime: need no parameter\n");
	}

    int p[2];
    pipe(p);

    int pid = fork();
    if (pid == 0) {
        close(p[1]);
        sieve(p[0]);
    } else {
        close(p[0]);
        printf("prime %d\n", 2);
        for (int i = 2; i <= MAX; i++) {
            if (i % 2 == 0) {
                continue;
            }
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
    }
    wait(0);
	exit(0);
}