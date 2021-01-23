// Demonstrates the difference between the fork and vfork system calls.

#include <stdio.h> 
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h>

void forktest();
void vforktest();

int main() {
    forktest();
    vforktest();

    return 0;
}

void forktest() {
    int a = 3, b = 2;

    if(!fork()) {
        a++; 
        b++;

        printf("fork child   -> a: %d; b: %d\n", a, b);
        exit(0);
    }

    int cid = wait(NULL);
    printf("fork parent  -> a: %d; b: %d\n", a, b);
}

void vforktest() {
    int a = 3, b = 2;

    if(!vfork()) {
        a++; 
        b++;

        printf("vfork child  -> a: %d; b: %d\n", a, b);
        // Make sure this is _exit() and not exit()
        _exit(0);
    }

    printf("vfork parent -> a: %d; b: %d\n", a, b);
}

