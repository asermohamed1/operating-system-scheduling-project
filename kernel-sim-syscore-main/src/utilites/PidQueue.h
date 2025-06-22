#ifndef PID_QUEUE_H
#define PID_QUEUE_H

#include <stdlib.h>
#include "Structs.h"

PidQueue* createPidQueue();
void push_pid(PidQueue* queue, int pid,int id);
int pop_pid(PidQueue* queue);
int peek_pid(PidQueue* queue);
int isEmpty_pid(PidQueue* queue);

#endif
