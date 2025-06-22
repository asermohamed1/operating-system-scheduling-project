#include "memory_manager.h"




bool AllocateMemory(PCB* process,SchedulingAlgo algo,int from)
{
    bool isAllocated = insertProcess(Memory->root,process->id,pow(2,ceil(log2(process->memsize))));
    if(!isAllocated && from == 0) {
        enQueue(WaitingQueue,process);
    } 
    return isAllocated;
}

void FreeMemory(int id,SchedulingAlgo algo,void(*func)(PCB*))
{
    bool isFreed = false;
    SearchnDelete(Memory->root,id,&isFreed);
    NotifyProcess(algo,func);
}

void NotifyProcess(SchedulingAlgo algo,void(*func)(PCB*))
{
    printf("Notfing Processes\n");
    PrintQueue(WaitingQueue);
    PCB* process = TraverseQueue(WaitingQueue,Memory->root,algo,AllocateMemory);
    while (process) {
        PrintQueue(WaitingQueue);
        printf("Notifying process %d\n",process->id);
        func(process);
        printf("Notified process %d\n",process->id);
        push_pid(ForkedProcesses_ids,process->pid,process->id);
        printf("Pushed process %d to ForkedProcesses_ids\n",process->id);
        free(process);
        process = NULL;
        printf("Freed process\n");
        process = TraverseQueue(WaitingQueue,Memory->root,algo,AllocateMemory);
        printf("Traversed Queue\n");
    }     
    if (isEmptyN(WaitingQueue))
        printf("Waiting Queue is empty\n");
    printf("returning from NotifyProcess\n");
}

void MakeMemoryLog()
{
    FILE *file = fopen("memory.log", "w");
    if (file != NULL) {
        fprintf(file, "#At time x allocated y bytes for process z from i to j\n");  
        fclose(file);
    }
}




