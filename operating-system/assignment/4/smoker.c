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

    fprintf(stderr, "Smoker %d: Initilized.\n", id);
    V(need[id - 1]);
    for (; ; ) {
        fprintf(stderr, "Smoker %d: Waiting for the items.\n", id);
        P(items[id - 1]);
        P(mutex);
        fprintf(stderr, "Smoker %d: Got the items. Smoking...\n", id);
        sleep(5); // for better visualization
        V(mutex);
        V(need[id - 1]);
        V(empty);
        fprintf(stderr, "Smoker %d: Smoked.\n", id);

        sleep(1); // for better visualization
    }
}