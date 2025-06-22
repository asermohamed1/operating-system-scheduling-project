#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "helper.h"

#include "Algos/HPF.h"
#include "Algos/RR.h"
#include "Algos/SRTN.h"


extern int msgq_id;
extern int sem_id;
extern int MoreProcessesComing;
extern pQueue* priority_queue;
extern queue* normal_queue;
extern int quantum;
extern int TotalRT;
extern int procNum;
extern int TotalWT;
extern float sumWTA;
extern float sumWTASq;

void intializeMessageQueue();
void getProcess(int);
void add_to_HPF_queue(PCB* process);
void add_to_SRTN_queue(PCB* process);
void add_to_RR_queue(PCB* process);
void create_scheduler_log();
void create_scheduler_perf();
void print_process_info(const PCB* process, int state);
PCB* create_process(
    int id, int arrivalTime, int priority, int runTime,
    bool running, bool startedBefore, int enterQueue,
    int quitQueue, int executionTime, int waitingTime, int pid, int memsize);

#endif
