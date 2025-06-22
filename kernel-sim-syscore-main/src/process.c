#include "clk.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int remainingtime;
int schedulerpid;

int previous_time, current_time;
void sigcont_handler(int sig_num) {
    previous_time = get_clk();
} 

int main(int agrc, char *argv[]) {
    
    signal(SIGCONT, sigcont_handler);
    
    sync_clk();
    
    remainingtime = atoi(argv[1]);
    schedulerpid = atoi(argv[2]);



    previous_time = get_clk();
    while (remainingtime > 0) {
        current_time = get_clk();
        if (previous_time < current_time) {
            remainingtime--;
            previous_time = current_time;
        }
    }

  
    
    kill(schedulerpid, SIGUSR2);

    return 0;
}
