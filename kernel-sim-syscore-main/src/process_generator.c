#include "helper.h"
#include "memory_manager.h"




Tree* Memory;
queue* WaitingQueue;


#define SEM_KEY 300
#define MSG_KEY 100

pid_t schedulerpid;
queue* Processes;
Selected_Algo config;
PidQueue* ForkedProcesses_ids;
int procNum = 0;
int TotalRT = 0;
int sem_id;
int msgq_id;

msgbuff message;



void clear_resources(int signum) {
    msgctl(msgq_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    destroy_clk(1);
    exit(0);
}


void intializeMessageQueue() {
    msgq_id = msgget(MSG_KEY, 0666 | IPC_CREAT);

    if (msgq_id == -1) {
        perror("Error in create message queue");
        exit(-1);
    }
}

void sendProcess(PCB *process) {
    message.mtype = 7;
    message.process = *process;
    int send_val = msgsnd(msgq_id, &message, sizeof(message.process), !IPC_NOWAIT);
 
    if (send_val == -1)
        perror("Error in send");
}

void ForkProcess(PCB *process) {
    pid_t forked_process = fork();
    if (forked_process == -1)  {
        perror("Error in fork");
        clear_resources(0);
        exit(-1);
    }
    if (forked_process == 0) {
        char Remaningtime_str[16],schedulerpid_str[16];
        snprintf(Remaningtime_str, sizeof(Remaningtime_str), "%d", process->runTime);
        snprintf(schedulerpid_str, sizeof(schedulerpid_str), "%d", schedulerpid);

        char *args[] = {"./process", Remaningtime_str, schedulerpid_str, NULL};
        execv(args[0], args);
    }
    else {
        process->pid = forked_process;
        sendProcess(process);
        kill(schedulerpid, SIGUSR1);
        printf("Signal sent\n");
        kill(forked_process, SIGSTOP);
        down(sem_id);
    }
}


int Read_Processes(const char *filename,queue* ReadyQueue) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }


    char line[1024];

    fgets(line, sizeof(line), file); // for skiping #

    while (fgets(line, sizeof(line), file)) {
        PCB *p = (PCB*) malloc(sizeof(PCB));
        if (sscanf(line, "%d\t%d\t%d\t%d\t%d", &p->id, &p->arrivalTime, &p->runTime, &p->priority,&p->memsize) == 5) {
            p->running = false;
            p->startedBefore = false;
            p->enterQueue = p->arrivalTime;
            p->quitQueue = 0;
            p->executionTime = 0;
            p->waitingTime = 0;
            p->pid = -1;    
            procNum++;
            TotalRT += p->runTime;
            enQueue(ReadyQueue,p);
        }
    }

    fclose(file);
    return 0;
}


void sendStopProcess() {
    PCB process = {.id = -1};
    sendProcess(&process);  
}


void parse_arguments(int argc, char *argv[], Selected_Algo *config) {
    if (argc < 5) {
        fprintf(stderr, "Usage: ./os-sim -s <rr|hpf|srtn> [-q quantum] -f <inputfile>\n");
        exit(EXIT_FAILURE);
    }

    config->algorithm = INVALID_ALGO;
    config->quantum = -1;
    config->input_file[0] = '\0';

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            if (strcmp(argv[i + 1], "rr") == 0)
                config->algorithm = RR;
            else if (strcmp(argv[i + 1], "hpf") == 0)
                config->algorithm = HPF;
            else if (strcmp(argv[i + 1], "srtn") == 0)
                config->algorithm = SRTN;
            else {
                fprintf(stderr, "Invalid scheduling algorithm: %s\n", argv[i + 1]);
                exit(EXIT_FAILURE);
            }
            i++;
        }
        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            config->quantum = atoi(argv[i + 1]);
            if (config->quantum <= 0) {
                fprintf(stderr, "Invalid quantum: %s\n", argv[i + 1]);
                exit(EXIT_FAILURE);
            }
            i++;
        }
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            strncpy(config->input_file, argv[i + 1], sizeof(config->input_file) - 1);
            config->input_file[sizeof(config->input_file) - 1] = '\0';
            i++;
        }
    }

    if (config->algorithm == INVALID_ALGO) {
        fprintf(stderr, "Missing or invalid scheduling algorithm.\n");
        exit(EXIT_FAILURE);
    }

    if (config->algorithm == RR && config->quantum == -1) {
        fprintf(stderr, "Round Robin requires -q <quantum> argument.\n");
        exit(EXIT_FAILURE);
    }

    if (config->algorithm != RR && config->quantum != -1) {
        fprintf(stderr, "Quantum specified for non-RR algorithm.\n");
        exit(EXIT_FAILURE);
    }

    if (config->input_file[0] == '\0') {
        fprintf(stderr, "Input file not specified.\n");
        exit(EXIT_FAILURE);
    }
}




int main(int argc, char * argv[])
{

    pid_t clkpid = fork();
    if (clkpid == 0) {
        char *args[] = {"./clk", NULL};
        execv(args[0], args);
    }

    MakeMemoryLog();

    ForkedProcesses_ids = createPidQueue();

    signal(SIGINT, clear_resources);
    
    


    parse_arguments(argc,argv,&config);

    WaitingQueue = createQueue();
    Memory = createTree();
    MakeMemoryTree(&Memory->root,1024,0,1023);

    Processes = createQueue();
    if (Read_Processes(config.input_file, Processes) == -1)
        exit(EXIT_FAILURE);

    sem_id = InitializeSemaphore(SEM_KEY);
    intializeMessageQueue();

   

    schedulerpid = fork();
    if (schedulerpid == 0) {
        char Quantum[16], Algorithm[16], ProcNum[16], TotalRTstr[16];
        snprintf(ProcNum, sizeof(ProcNum), "%d", procNum);
        snprintf(TotalRTstr, sizeof(TotalRTstr), "%d", TotalRT);
        snprintf(Quantum, sizeof(Quantum), "%d", config.quantum);
        snprintf(Algorithm, sizeof(Algorithm), "%d", config.algorithm);

        char *args[] = {"./scheduler", Algorithm, ProcNum, TotalRTstr, Quantum, NULL};
        execv(args[0], args);
    }
    if (schedulerpid == -1) 
        exit(EXIT_FAILURE);
 

    sync_clk();
    PrintQueue(Processes);
    while (!isEmptyN(Processes)) {
        PCB* process = peekN(Processes);
        
        if (waitpid(schedulerpid, NULL, WNOHANG) > 0) {
            perror("Scheduler Terminated with an error");
            exit(1);
        }
            
        
        PidQueueNode* temp = ForkedProcesses_ids->head;
        PidQueueNode* prev = NULL;
        while (temp != NULL) {
            if (waitpid(temp->pid, NULL, WNOHANG) > 0) {
                FreeMemory(temp->id, config.algorithm,ForkProcess);
                if (prev == NULL) {
                    ForkedProcesses_ids->head = temp->next;
                } else {
                    prev->next = temp->next;
                }
            }
            prev = temp;
            temp = temp->next;
        }

        if (get_clk() >= process->arrivalTime) {
            printf("Sending and forking try\n");
            if (AllocateMemory(process,config.algorithm,0)) {
                printf("Sending and forking start\n");
                ForkProcess(process);
                printf("Forked and sent\n");
                push_pid(ForkedProcesses_ids,process->pid,process->id);
                deQueue(Processes);
                free(process);
            } else {
                deQueue(Processes);
            }
        }
    }
    


    printf("iam here 1\n");
    
    while (!isEmpty_pid(ForkedProcesses_ids) || !isEmptyN(WaitingQueue)) { 
        if (waitpid(schedulerpid, NULL, WNOHANG) > 0) {
            perror("Scheduler Terminated with an error");
            exit(1);
        }
            
        if (!isEmpty_pid(ForkedProcesses_ids)) {
           // printf("iam here 2\n");
            PidQueueNode* temp = ForkedProcesses_ids->head;
            PidQueueNode* prev = NULL;
            while (temp != NULL) {
                //printf("iam here 3\n");
                if (waitpid(temp->pid, NULL, WNOHANG) > 0) {
                    FreeMemory(temp->id, config.algorithm,ForkProcess);
                    if (prev == NULL) {
                        ForkedProcesses_ids->head = temp->next;
                    } else {
                        prev->next = temp->next;
                    }
                    break;
                }
                prev = temp;
                temp = temp->next;
            }
        } else {
            printf("Forked Processes is empty\n");
        }
    }
    sendStopProcess();
    kill(schedulerpid, SIGUSR1);
    waitpid(schedulerpid, NULL, 0);
    clear_resources(1);
    return 0;
}


