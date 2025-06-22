#ifndef STUFF
#define STUFF
#include <stdbool.h>

typedef struct ProcessStruct
{
    //info from the file
    int id;
    int arrivalTime;
    int priority;
    int runTime;

    bool running;

    // if equals false then create new process
    // else wake the process up
    bool startedBefore;

    // if (enterQueue == 0) enterQueue = arrivalTime
    // else enterQueue = clk;
    int enterQueue;

    // quitQueue = clk;
    int quitQueue;

    // executionTime = executionTime + (clk - quitQueue)
    int executionTime;

    // waitingTime = clk - enterQueue;
    int waitingTime;

    // The real id of forked process
    int pid;

    // added for memory
    int memsize;

} PCB;

typedef struct QNode {
    struct ProcessStruct* data;
    struct QNode* next;
} qNode;

typedef struct Queue {
    struct QNode *front, *rear;
} queue;

typedef struct PQNode {
    struct ProcessStruct* data;
    int priority;
    struct PQNode* next;
} pqNode;

typedef struct PQueue {
    struct PQNode *head;
} pQueue;

typedef struct messagebuff
{
    long mtype;
    struct ProcessStruct process;

} msgbuff;

typedef enum { RR, HPF, SRTN, INVALID_ALGO } SchedulingAlgo;


typedef struct {
    SchedulingAlgo algorithm;    
    int quantum;                  // Only for RR
    char input_file[256];
} Selected_Algo;


typedef struct TreeMemoryNode {
    int id;
    int size;
    int start;
    int end;
    struct TreeMemoryNode* left;
    struct TreeMemoryNode* right;
} MemoryNode;

typedef struct MemoryTree {
    MemoryNode* root;
} Tree;


typedef struct Pid_Queue_Node {
    int pid;
    int id;
    struct Pid_Queue_Node* next;
} PidQueueNode;

typedef struct Pid_Queue {
    PidQueueNode* head;
} PidQueue;

#endif