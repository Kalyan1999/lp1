// Creates a child process and registers a signal handler to get exit status of child process
// asynchronously

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define true (1)
#define false (0)

void signalHandler(int);

int stop = false;

int main() {
    srand(time(0));

    if(!fork()) {
        sleep(rand() % 15 + 1);
        exit((rand() % 100));
    }

    signal(SIGCHLD, signalHandler);
    
    int i = 1;
    while (!stop) {
        printf("\rWaiting for Child Exit Signal..... %d", i);
        fflush(stdout);
        sleep(1);
        i++;
    }

    return 0;
}

void signalHandler(int signum) {
    printf("\nChild Exit Signal Received.\n");
    
    int wstatus;
    int cid = wait(&wstatus);
    if(WIFEXITED(wstatus))
        printf("\033[0;32m> child(pid): %d of parent (pid): %d exited normally with status: "
                "%d\033[0m\n", cid, getpid(), WEXITSTATUS(wstatus));
    else
        printf("\033[0;31m> child(pid): %d of parent (pid): %d exited abnormally\033[0m\n", 
                cid, getpid());
    stop = true;
}
