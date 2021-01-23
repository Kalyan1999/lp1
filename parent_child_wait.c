// Creates a child processes and the parent waits until its children exit.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CHILDS 5

int main() {
    for(int i = 0; i < MAX_CHILDS; i++)
        if(!fork()) {
            printf("Parent(pid) %d -> Child(pid) %d:\t", getppid(), getpid());
            srand(getpid());

            int n = 100;
            long sum = 0;
            for(int i = 0; i < n; i++) {
                sum += rand();
            }
            printf("Sum of %d random numbers = %ld\n", n, sum);
            
            exit(0);
        }

    for(int i = 0; i < MAX_CHILDS; i++) {
        int wstatus;

        int cid = wait(&wstatus);

        if(WIFEXITED(wstatus))
            printf("\033[0;32m> child(pid): %d of parent (pid): %d exited normally with status: "
                    "%d\033[0m\n", cid, getpid(), WEXITSTATUS(wstatus));
        else
            printf("\033[0;31m> child(pid): %d of parent (pid): %d exited abnormally\033[0m\n", 
                    cid, getpid());
    }
    
    return 0;
}