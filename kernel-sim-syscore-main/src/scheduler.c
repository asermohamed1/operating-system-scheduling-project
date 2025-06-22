#include "scheduler.h"
#include <math.h>

#define SEM_KEY 300
#define MSG_KEY 100

int msgq_id;
int sem_id;
int MoreProcessesComing = 1;
SchedulingAlgo selected_algo;  
msgbuff message;
pQueue* priority_queue;
queue* normal_queue;
int quantum;
int TotalRT;
int procNum;
int TotalWT = 0;
float sumWTA = 0.0;
float meanWTA; 
float sumWTASq = 0.0; 
float STD; 

void intializeMessageQueue() {
    msgq_id = msgget(MSG_KEY, 0666 | IPC_CREAT);

    if (msgq_id == -1) {
        perror("Error in create message queue");
        exit(-1);
    }
}

void add_to_SRTN_queue(PCB* process) {
    // Push to the queue and the priority is the runTime (The remaining time at the beginning)
    PCB *newProcess = create_process(process->id, process->arrivalTime, process->priority,
                                    process->runTime, process->running,
                                    process->startedBefore, process->enterQueue, process->quitQueue,
                                    process->executionTime,
                                    process->waitingTime, process->pid, process->memsize);
    
    if (run_time < (prev_exec_time + get_clk() - prev_enter_queue) && !isEmpty(priority_queue)) {
        PCB* topProcess = NULL;  
        if (!isEmpty(priority_queue)) 
            topProcess = peek(priority_queue);     
        if (topProcess)          
            if (topProcess->running)         
                updateFrontPriority(priority_queue,run_time - (prev_exec_time + get_clk() - prev_enter_queue)); 
    }
    else if (SRTN_currentRunningProcess) {
        updateFrontPriority(priority_queue,0); 
    };
    push(priority_queue, newProcess, newProcess->runTime);
}

void add_to_HPF_queue(PCB* process) {
    // Push to the queue and the priority is the priority (The priority of the process)
    PCB *newProcess = create_process(process->id, process->arrivalTime, process->priority,
                                    process->runTime, process->running,
                                    process->startedBefore, process->enterQueue, process->quitQueue,
                                    process->executionTime,
                                    process->waitingTime, process->pid, process->memsize);
    push(priority_queue, newProcess, newProcess->priority);
}

void add_to_RR_queue(PCB* process) {
    // enqueue (The remaining time at the beginning)
    PCB *newProcess = create_process(process->id, process->arrivalTime, process->priority,
                                    process->runTime, process->running,
                                    process->startedBefore, process->enterQueue, process->quitQueue,
                                    process->executionTime,
                                    process->waitingTime, process->pid, process->memsize);
    enQueue(normal_queue, newProcess);
}

PCB* create_process(
    int id, int arrivalTime, int priority, int runTime,
    bool running, bool startedBefore, int enterQueue,
    int quitQueue, int executionTime, int waitingTime, int pid, int memsize) {

    PCB* process = (PCB*) malloc(sizeof(PCB));
    process->id = id;
    process->arrivalTime = arrivalTime;
    process->priority = priority;
    process->runTime = runTime;
    process->running = running;
    process->startedBefore = startedBefore;
    process->enterQueue = enterQueue;
    process->quitQueue = quitQueue;
    process->executionTime = executionTime;
    process->waitingTime = waitingTime;
    process->pid = pid;
    process->memsize = memsize;
    return process;
}

void print_process_info(const struct ProcessStruct *const process, int state) {
    /*
     * DESCRIPTION : A function that prints the scheduler action
     *
     * INPUT       : Process to print its info
     *               State of process
     *               0 : started
     *               1 : Stopped
     *               2 : Resumed
     *               3 : Finished
     *
     * OUTPUT      : void
    * */

    FILE *outputFile;
    int current_time = get_clk();
    outputFile = fopen("scheduler.log", "a+");
    if (!outputFile) {
        perror("Unable to open file \"scheduler.log\" terminating ...\n");
        exit(-1);
    }
    fprintf(outputFile, "%s", "At time ");
    fprintf(outputFile, "%d ", current_time);
    fprintf(outputFile, "%s", "process ");
    fprintf(outputFile, "%d ", process->id);
    switch (state) {
        case 0:
            fprintf(outputFile, "%s", "started ");
            break;
        case 1:
            fprintf(outputFile, "%s", "stopped ");
            break;
        case 2:
            fprintf(outputFile, "%s", "resumed ");
            break;
        case 3:
            fprintf(outputFile, "%s", "finished ");
            break;
        default:
            perror("Wrong state terminating ...");
            exit(-1);
    }
    fprintf(outputFile, "%s", "arr ");
    fprintf(outputFile, "%d ", process->arrivalTime);
    fprintf(outputFile, "%s", "total ");
    fprintf(outputFile, "%d ", process->runTime);
    fprintf(outputFile, "%s", "remain ");
    fprintf(outputFile, "%d ", process->runTime - process->executionTime);
    fprintf(outputFile, "%s", "wait ");
    fprintf(outputFile, "%d ", process->waitingTime);
   

    if (state == 3) { // if Finished
        int TA = current_time - process->arrivalTime;
        float WTA;
        if (process->runTime == 0)
            WTA = 0;
        else WTA = (float) TA / process->runTime;
        //save this values for scheduler.perf file
        sumWTA += WTA;
        sumWTASq += pow(WTA, 2);

        fprintf(outputFile, "%s", "TA ");
        fprintf(outputFile, "%d ", TA);
        fprintf(outputFile, "%s", "WTA ");
        fprintf(outputFile, "%.2f ", WTA);
    }
    fprintf(outputFile, "%s", "\n");
    fclose(outputFile);
}

void create_scheduler_perf() {
    /*
     * A function that creates "scheduler.perf"
     * and fill it with some statistics
    * */
    FILE *outputFile;
    outputFile = fopen("scheduler.perf", "w");
    if (!outputFile) {
        perror("Error opening scheduler.perf");
        exit(EXIT_FAILURE);
    }

    //CPU utilization
    fprintf(outputFile, "%s", "CPU utilization = ");
    printf("%d / %d\n",TotalRT,get_clk());
    fprintf(outputFile, "%.2f%%\n", (TotalRT * 100.0 / get_clk()));

    //Avg WTA
    fprintf(outputFile, "%s", "Avg WTA = ");
    fprintf(outputFile, "%.2f\n", (sumWTA / procNum));

    //Avg Waiting
    fprintf(outputFile, "%s", "Avg Waiting = ");
    fprintf(outputFile, "%.2f\n", ((float) TotalWT / procNum));

    //Std WTA
    meanWTA = sumWTA / procNum;
    STD = sqrt((sumWTASq / procNum) - (meanWTA * meanWTA));


    fprintf(outputFile, "%s", "Std WTA = ");
    fprintf(outputFile, "%.2f\n", STD);

    fclose(outputFile);
}



void getProcess(int signum) {
    printf("Started Receving Process\n");
    //receive from the message queue and add to the ready queue
    int rec_val = msgrcv(msgq_id, &message, sizeof(message.process), 7, !IPC_NOWAIT);
    if (rec_val == -1) {
        perror("Error in receive");
    }

    printf("Rececving Process: %d\n",message.process.id );
    
    //check if that process was the terminating one (id = -1)
    if (message.process.id == -1) {
        MoreProcessesComing = 0;
        return;
    }

    switch (selected_algo) {
        case HPF:
            add_to_HPF_queue(&message.process);
            break;
        case SRTN:
            add_to_SRTN_queue(&message.process);
            break;
        case RR:
            add_to_RR_queue(&message.process);
            break;
        default:
            fprintf(stderr, "Unknown scheduling algorithm in signal handler.\n");
    }

    // Process has been pushed to the queue
    // Up the semaphore to allow process generator to continue
    up(sem_id);
    signal(SIGUSR1, getProcess);
}

void create_scheduler_log() {
    FILE *outputFile;
    outputFile = fopen("scheduler.log", "w");
    
    fprintf(outputFile, "%s", "#At time x process y state arr w total z remain y wait k\n");
    fclose(outputFile);
}



int main (int argc,char* argv[]) {

    if (argc < 5) {
        fprintf(stderr, "Usage: %s <algorithm> <procNum> <TotalRT> <quantum>\n", argv[0]);
        return 1;
    }


    selected_algo = atoi(argv[1]);
    procNum = atoi(argv[2]);
    TotalRT = atoi(argv[3]);
    quantum = atoi(argv[4]);

    sem_id = InitializeSemaphore(SEM_KEY);
    intializeMessageQueue();

    signal(SIGUSR1, getProcess);
    create_scheduler_log();
    sync_clk();

    switch (selected_algo) {
        case HPF:
            priority_queue = createPriorityQueue();
            HPF_Start(); // priority_queue
            break;
        case SRTN:
            priority_queue = createPriorityQueue();
            SRTN_Start(); // priority_queue          
            break;
        case RR:
            normal_queue = createQueue();
            RR_Start(); // quantum, queue
            break;
        default:
            fprintf(stderr, "Invalid algorithm passed to scheduler.\n");
            return 1;
    }

    create_scheduler_perf();
}

