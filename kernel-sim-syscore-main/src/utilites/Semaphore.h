#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union Semun {
    int val;
};

int InitializeSemaphore(key_t key);
void down(int Semaphore_id);
void up(int Semaphore_id);
void clearSemaphores(int Semaphore_id);

#endif

