#include "Semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int InitializeSemaphore(key_t key) {
    int Semaphore_id = semget(key, 1, 0666 | IPC_CREAT);
    if (Semaphore_id == -1) {
        perror("Error creating semaphore");
        exit(-1);
    }

    union Semun semun;
    semun.val = 0;
    if (semctl(Semaphore_id, 0, SETVAL, semun) == -1) {
        perror("Error initializing semaphore");
        exit(-1);
    }

    return Semaphore_id;
}

void down(int Semaphore_id) {
    struct sembuf op = {0, -1, 0};
    if (semop(Semaphore_id, &op, 1) == -1) {
        perror("Semaphore down failed");
        exit(-1);
    }
}

void up(int Semaphore_id) {
    struct sembuf op = {0, 1, 0};
    if (semop(Semaphore_id, &op, 1) == -1) {
        perror("Semaphore up failed");
        exit(-1);
    }
}

void clearSemaphores(int Semaphore_id) {
    if (semctl(Semaphore_id, 0, IPC_RMID) == -1) {
        perror("Error removing semaphore");
        exit(-1);
    }
}

