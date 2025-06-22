#ifndef RR_ALGO
#define RR_ALGO

#include "../scheduler.h"
#include "../helper.h"

void RR_run();
void RR_processFinishedHandler(int signum);
void RR_Start();

#endif