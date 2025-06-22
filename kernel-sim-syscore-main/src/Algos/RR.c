#include "RR.h"
#include <stdbool.h>

PCB* RR_currentRunningProcess = NULL;


void RR_processFinishedHandler(int signum) {
    RR_currentRunningProcess->executionTime = RR_currentRunningProcess->runTime;
    TotalWT += RR_currentRunningProcess->waitingTime;
    print_process_info(RR_currentRunningProcess, 3);
    deQueue(normal_queue);
    free(RR_currentRunningProcess);
    RR_currentRunningProcess = NULL;
    signal(SIGUSR2, RR_processFinishedHandler);
}


void RR_run() {
    int Currenttime = get_clk();
   
    if(RR_currentRunningProcess->startedBefore == false) 
        print_process_info(RR_currentRunningProcess, 0);
    else    
        print_process_info(RR_currentRunningProcess, 2);

    
    RR_currentRunningProcess->waitingTime += Currenttime - RR_currentRunningProcess->enterQueue;
    RR_currentRunningProcess->enterQueue = Currenttime;
    RR_currentRunningProcess->startedBefore = true;

    bool wait = (RR_currentRunningProcess->runTime ==  RR_currentRunningProcess->executionTime + quantum);

    int StartTime = get_clk();
    kill(RR_currentRunningProcess->pid,SIGCONT);
    
    while (get_clk() != StartTime + quantum && RR_currentRunningProcess) {
        sleep(.2);
    }
    if (wait) while(RR_currentRunningProcess);
    if (!RR_currentRunningProcess) return;
    kill(RR_currentRunningProcess->pid,SIGSTOP);


    if (!RR_currentRunningProcess)  return;
    RR_currentRunningProcess->executionTime += get_clk() - RR_currentRunningProcess->enterQueue;
    print_process_info(RR_currentRunningProcess, 1);
    deQueue(normal_queue);
    enQueue(normal_queue,RR_currentRunningProcess);
    RR_currentRunningProcess = NULL;
}


void RR_Start() {
    signal(SIGUSR2, RR_processFinishedHandler);
    while (!isEmptyN(normal_queue) || MoreProcessesComing) {
        if (!RR_currentRunningProcess && !isEmptyN(normal_queue)) {
            RR_currentRunningProcess = peekN(normal_queue);
            RR_run();
        }
    }
    while (RR_currentRunningProcess);
}
