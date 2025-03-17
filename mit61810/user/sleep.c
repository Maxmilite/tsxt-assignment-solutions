#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
        printf("sleep: missing parametes\n");
        printf("Usage: sleep [time]\n");
		exit(0);
	} else if (argc > 2) {
		printf("sleep: too many parameters\n");
        printf("Usage: sleep [time]\n");
		exit(0);
	}

	int time = atoi(argv[1]);
	sleep(time);
	exit(0);
}