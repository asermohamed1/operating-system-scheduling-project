#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "Structs.h"
#include <stdio.h>


queue* createQueue();
void enQueue(queue* q, PCB* d);
PCB* peekN(queue* q);
void deQueue(queue* q);
bool isEmptyN(queue* q);
PCB* TraverseQueue(queue* q,MemoryNode* root,SchedulingAlgo algo,bool(*func)(PCB* ,SchedulingAlgo,int));
void PrintQueue(queue* q);

#endif

