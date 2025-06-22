#include "clk.h"
#include "utilites/Semaphore.h"
#include "utilites/Queue.h"
#include "utilites/PQueue.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>    // for fork, execl
#include <sys/types.h> // for pid_t
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "utilites/PidQueue.h"






