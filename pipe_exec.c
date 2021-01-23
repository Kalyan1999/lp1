// Implements `ls | wc -l -w -c` using pipe() and exec()

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];

    if(pipe(pipefd) < 0) {
        printf("Unable to create pipe. Exiting...");
        return -1;
    }

    if(!fork()) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execl("/usr/bin/ls", "ls", ".", NULL);
        _exit(1);
    }

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);

    execl("/usr/bin/wc", "wc", "-l", "-c", "-w", NULL);
    wait(NULL);

    return 0;
}