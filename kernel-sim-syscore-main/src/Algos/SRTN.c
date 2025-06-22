#include "SRTN.h"

PCB* SRTN_currentRunningProcess = NULL;

int prev_exec_time = 0;
int prev_enter_queue = 0;
int run_time = 0;


void SRTN_processFinishedHandler(int signum) {
    SRTN_currentRunningProcess->executionTime = SRTN_currentRunningProcess->runTime;
    TotalWT += SRTN_currentRunningProcess->waitingTime;
    print_process_info(SRTN_currentRunningProcess, 3);
    pop(priority_queue);
    free(SRTN_currentRunningProcess);
    SRTN_currentRunningProcess = NULL;
    signal(SIGUSR2, SRTN_processFinishedHandler);
}

void SRTN_run() {
    printf("SRTN Running\n");
    if (!SRTN_currentRunningProcess) return;
    int Currenttime = get_clk();
  
    prev_exec_time = SRTN_currentRunningProcess->executionTime;
  
    if(SRTN_currentRunningProcess->startedBefore == false) 
        print_process_info(SRTN_currentRunningProcess, 0);
    else    
        print_process_info(SRTN_currentRunningProcess, 2);

    SRTN_currentRunningProcess->running = true;
    SRTN_currentRunningProcess->waitingTime += Currenttime - SRTN_currentRunningProcess->enterQueue;
    SRTN_currentRunningProcess->enterQueue = Currenttime;
    SRTN_currentRunningProcess->startedBefore = true;


    prev_enter_queue =  SRTN_currentRunningProcess->enterQueue;

    kill(SRTN_currentRunningProcess->pid,SIGCONT);
    
    while (SRTN_currentRunningProcess && SRTN_currentRunningProcess == peek(priority_queue)) {
        sleep(.4);
    }
   
    if (prev_exec_time + get_clk() - prev_enter_queue == run_time && !isEmpty(priority_queue)) while (SRTN_currentRunningProcess);
    
    for(int i = 0;i < 100;i++) if (!SRTN_currentRunningProcess) return;
    
    kill(SRTN_currentRunningProcess->pid,SIGSTOP);


    if (!SRTN_currentRunningProcess)  return;
    
    SRTN_currentRunningProcess->running = false;
    
    SRTN_currentRunningProcess->executionTime += get_clk() - SRTN_currentRunningProcess->enterQueue;
  
    print_process_info(SRTN_currentRunningProcess, 1);
    
    SRTN_currentRunningProcess = NULL;
}

void SRTN_Start() {
    signal(SIGUSR2, SRTN_processFinishedHandler);
    while (!isEmpty(priority_queue) || MoreProcessesComing) {
        if (!SRTN_currentRunningProcess && !isEmpty(priority_queue)) {
            SRTN_currentRunningProcess = peek(priority_queue);
            prev_enter_queue = SRTN_currentRunningProcess->enterQueue;
            prev_exec_time = 0;
            run_time = SRTN_currentRunningProcess->runTime;
            SRTN_run();
        }
    }
    while (SRTN_currentRunningProcess);
}
