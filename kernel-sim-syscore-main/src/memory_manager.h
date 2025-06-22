#ifndef MemoryManager_h
#define MemoryManager_h

#include "./utilites/Tree.h"
#include "./utilites/Queue.h"
#include "./utilites/PQueue.h"
#include "./utilites/Structs.h"
#include "./utilites/PidQueue.h"
#include <unistd.h>
#include <math.h>

extern Tree* Memory;
extern queue* WaitingQueue;
extern PidQueue* ForkedProcesses_ids;


bool AllocateMemory(PCB* process,SchedulingAlgo algo,int from);
void FreeMemory(int id,SchedulingAlgo algo,void(*func)(PCB*));
void NotifyProcess(SchedulingAlgo algo,void(*func)(PCB*));


#endif 