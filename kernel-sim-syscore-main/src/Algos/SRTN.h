#ifndef SRTN_ALGO
#define SRTN_ALGO

#include "../scheduler.h"
#include "../helper.h"

extern PCB* SRTN_currentRunningProcess;
extern int prev_exec_time;
extern int prev_enter_queue;
extern int run_time;

void SRTN_Start();
void SRTN_run();
void SRTN_processFinishedHandler(int signum);

#endif