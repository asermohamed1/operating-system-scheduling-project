#include "HPF.h"

PCB* currentRunningProcess = NULL;

void runProcess() {
    currentRunningProcess->waitingTime = get_clk() - currentRunningProcess->arrivalTime;
    TotalWT += currentRunningProcess->waitingTime;
    kill(currentRunningProcess->pid,SIGCONT);
    print_process_info(currentRunningProcess, 0);
}

void processFinishedHandler(int signum) {
    currentRunningProcess->executionTime = currentRunningProcess->runTime;
    print_process_info(currentRunningProcess, 3);
    free(currentRunningProcess);
    currentRunningProcess = NULL;
    signal(SIGUSR2, processFinishedHandler);
}

void HPF_Start() {
    signal(SIGUSR2, processFinishedHandler);
    while (!isEmpty(priority_queue) || MoreProcessesComing) {
        if (!currentRunningProcess && !isEmpty(priority_queue)) {
            currentRunningProcess = peek(priority_queue);
            runProcess();
            pop(priority_queue);
        }
    }
    while (currentRunningProcess);
}