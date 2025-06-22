#include "PidQueue.h"

PidQueue* createPidQueue() {
    PidQueue* queue = (PidQueue*)malloc(sizeof(PidQueue));
    queue->head = NULL;
    return queue;
}

void push_pid(PidQueue* queue, int pid,int id) {
    PidQueueNode* newNode = (PidQueueNode*)malloc(sizeof(PidQueueNode));
    newNode->pid = pid;
    newNode->id = id;
    newNode->next = queue->head;
    queue->head = newNode;
}

int pop_pid(PidQueue* queue) {
    if (queue->head == NULL) {
        return -1;
    }
    PidQueueNode* temp = queue->head;
    int pid = temp->pid;
    queue->head = temp->next;
    free(temp);
    return pid;
}


int peek_pid(PidQueue* queue) {
    if (queue->head == NULL) {
        return -1;
    }
    return queue->head->pid;
}   

int isEmpty_pid(PidQueue* queue) {
    return queue->head == NULL;
}


