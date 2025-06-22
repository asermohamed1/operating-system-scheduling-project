#ifndef PQUEUE_H
#define PQUEUE_H


#include <stdbool.h>
#include "Structs.h"



pQueue* createPriorityQueue();
pqNode* newNode(PCB* d, int p);
PCB* peek(pQueue* q);
void pop(pQueue* q);
void push(pQueue* q,PCB* d, int p);
bool isEmpty(pQueue* q);
void updateFrontPriority(pQueue* q,int newP);
int GetTopPriority(pQueue* q);

#endif

