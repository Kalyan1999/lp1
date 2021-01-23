// Bidirectional communication between sender and receiver programs using multiple FIFOs.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define true (1)
#define false (0)
#define BUF_SIZE 1024

void cleanup(int);

int server_fd = -1, client_fd = -1;

int main() {
    signal(SIGINT, cleanup);

    char *serverPath = "/tmp/fifo_ex_server";
    char *clientPath = "/tmp/fifo_ex_client";
    
    char buffer_in[BUF_SIZE], buffer_out[BUF_SIZE];

    if(mkfifo(serverPath, 0666) < 0) {
        printf("Error creating FIFO file. Exiting...\n");
        return -1;
    }

    printf("Server Program.\n\n");
    while(true) {
        printf("> ");
        scanf("%1000[^\n]%*c", buffer_out);

        if((server_fd = open(serverPath, O_WRONLY)) == -1) {
            printf("Error opening FIFO file. Exiting...\n");
            break;
        }

        write(server_fd, buffer_out, strlen(buffer_out)+1);
        close(server_fd);
        server_fd = -1;

        if((client_fd = open(clientPath, O_RDONLY)) == -1) {
            printf("Error opening FIFO file. Exiting...\n");
            break;
        }
        
        read(client_fd, &buffer_in, BUF_SIZE);
        printf("Client: %s\n", buffer_in);
        close(client_fd);
        client_fd = -1;
    }
    
    return 0;
}

void cleanup(int signum) {
    printf("\nClosing FIFO Files and performing cleanup...\n");
    if(server_fd != -1) close(server_fd);
    if(client_fd != -1) close(client_fd);
    exit(0);
}