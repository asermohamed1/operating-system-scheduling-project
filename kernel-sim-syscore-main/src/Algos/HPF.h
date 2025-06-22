#ifndef HPF_ALGO
#define HPF_ALGO

#include "../scheduler.h"
#include "../helper.h"
#include "../utilites/Structs.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <stdio.h>
#include <unistd.h>




void runProcess();
void processFinishedHandler(int signum);
void HPF_Start();

#endif