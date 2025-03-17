#include "ipc.h"
#include "main.h"

void init_sem(int* empty, int* items, int* need, int* mutex) {
    *empty = semget(EMPTY, 1, IPC_CREAT | 0666);
    if (*empty == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    sem_setval(*empty, size_of_shared_memory);

    for (int i = 0; i < ITEM_SIZE; i++) {
        items[i] = semget(ITEMS[i], 1, IPC_CREAT | 0666);
        if (items[i] == -1) {
            perror("semget");
            exit(EXIT_FAILURE);
        }
        sem_setval(items[i], 0);
    }

    for (int i = 0; i < ITEM_SIZE; i++) {
        need[i] = semget(NEED[i], 1, IPC_CREAT | 0666);
        if (need[i] == -1) {
            perror("semget");
            exit(EXIT_FAILURE);
        }
        sem_setval(need[i], 0);
    }

    *mutex = semget(MUTEX, 1, IPC_CREAT | 0666);
    if (*mutex == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    sem_setval(*mutex, 1);
}

void sig_handler() {

    fprintf(stderr, "Manager: Received SIGINT. Stopping...\n");

    sem_delete(EMPTY, semget(EMPTY, 1, IPC_CREAT | 0666));
    for (int i = 0; i < ITEM_SIZE; i++) {
        sem_delete(ITEMS[i], semget(ITEMS[i], 1, IPC_CREAT | 0666));
    }
    for (int i = 0; i < ITEM_SIZE; i++) {
        sem_delete(NEED[i], semget(NEED[i], 1, IPC_CREAT | 0666));
    }
    sem_delete(MUTEX, semget(MUTEX, 1, IPC_CREAT | 0666));

    exit(0);
}

int main(int argc, char* argv[]) {
    int empty, items[ITEM_SIZE], need[ITEM_SIZE], mutex;
    init_sem(&empty, items, need, &mutex);
    fprintf(stderr, "Manager: Finished.\n");
    fprintf(stderr, "Smoker 1 will need tobacco and paper.\n");
    fprintf(stderr, "Smoker 2 will need tobacco and glue.\n");
    fprintf(stderr, "Smoker 3 will need paper and glue.\n");
    fprintf(stderr, "Producer will produce Smoker 1, 2, 3, 1, 2, 3, ...'s needs successively.\n");
    signal(SIGINT, sig_handler);
    for (; ; ) {
        sleep(1);
    }
}