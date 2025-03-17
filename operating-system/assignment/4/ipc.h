#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <error.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int ITEM_SIZE = 3;
const int size_of_shared_memory = 1;

key_t SHEKEY = 0x5555;
key_t EMPTY = 0x6666;
key_t ITEMS[] = { 0x7777, 0x8888, 0x9999 };
key_t NEED[] = { 0x777F, 0x888F, 0x999F };
key_t MUTEX = 0xAAAA;


union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
	struct seminfo *_buf;
};

static int sem_setval(int semid, int val) {
	union semun sem_union;
	sem_union.val = val;
	if (semctl(semid, 0, SETVAL, sem_union) == -1) {
		perror("semctl");
		return -1;
	}
	return 1;
}

static int sem_getval(int semid) {
	int ret;
	union semun sem_union;
	ret = semctl(semid, 0, GETVAL, sem_union);
	if (ret == -1) {
		perror("semctl");
		return -1;
	}
	return ret;
}

static int sem_delete(int semkey, int semid) {
	union semun sem_union;
	if (semget(semkey, 1, IPC_EXCL) == -1) {
		perror("semget");
		printf("semid: %x already erased!\n", semkey);
		return -1;
	}
	if (semctl(semid, 0, IPC_RMID, sem_union) == -1) {
		perror("semctl");
		return -1;
	}
	return 1;
}

static int P(int semid) {
	struct sembuf sem_buf;
	sem_buf.sem_num = 0;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = SEM_UNDO;
	if (semop(semid, &sem_buf, 1) == -1) {
		perror("semop");
		return -1;
	}
	return 1;
}

static int V(int semid) {
	struct sembuf sem_buf;
	sem_buf.sem_num = 0;
	sem_buf.sem_op = 1;
	sem_buf.sem_flg = SEM_UNDO;
	if (semop(semid, &sem_buf, 1) == -1) {
		perror("semop");
		return -1;
	}
	return 1;
}

// void shm_detche(char *shmptr) {
// 	int ret;
// 	ret = shmdt(shmptr);
// 	if (ret == -1) {
// 		perror("shmdt");
// 		exit(EXIT_FAILURE);
// 	}
// }
// 
// void shm_delete(int shmid) {
// 	int ret;
// 	ret = shmget(SHEKEY, 1, IPC_EXCL);
// 	if (ret == -1) {
// 		perror("shmdt");
// 		exit(EXIT_FAILURE);
// 	} else {
// 		ret = shmctl(shmid, IPC_RMID, 0);
// 		if (ret == 1) {
//			perror("shmget");
//			exit(EXIT_FAILURE);
//		}
// 	}
// }
