#include "ipc.h"
#include "main.h"

void init_sem(int* empty, int* items, int* need, int* mutex) {
    *empty = semget(EMPTY, 1, IPC_CREAT | 0666);
    if (*empty == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ITEM_SIZE; i++) {
        items[i] = semget(ITEMS[i], 1, IPC_CREAT | 0666);
        if (items[i] == -1) {
            perror("semget");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < ITEM_SIZE; i++) {
        need[i] = semget(NEED[i], 1, IPC_CREAT | 0666);
        if (need[i] == -1) {
            perror("semget");
            exit(EXIT_FAILURE);
        }
    }

    *mutex = semget(MUTEX, 1, IPC_CREAT | 0666);
    if (*mutex == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char* argv[]) {
    int id = atoi(argv[1]);
    int empty, items[ITEM_SIZE], need[ITEM_SIZE], mutex;
    init_sem(&empty, items, need, &mutex);

    fprintf(stderr, "Producer %d: Initilized.\n", id);
    int i = 0;
    for (; ; ) {
        fprintf(stderr, "Producer %d: Waiting for need of %d-th items.\n", id, i + 1);
        P(need[i]);
        fprintf(stderr, "Producer %d: Got the need of %d-th items.\n", id, i + 1);
        fprintf(stderr, "Producer %d: Waiting for the space.\n", id);
        P(empty);
        fprintf(stderr, "Producer %d: Got the space.\n", id);
        P(mutex);
        sleep(5); // for better visualization
        fprintf(stderr, "Producer %d: Produced %d-th items.\n", id, i + 1);
        V(mutex);
        V(items[i]);
        i = (i + 1) % ITEM_SIZE;

        sleep(1); // for better visualization
    }
}