// TCP Client using Linux sockets API

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

#define true (1)
#define false (0)
#define BACKLOG 16
#define BUF_SIZE 1024

int main() {
    int tcp_socket = -1;
    char msg[BUF_SIZE];

    printf("Client Program.\nPress Ctrl+C to exit.\n\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int server_addr_size = sizeof(server_addr);

    while(true) {
        printf("> ");
        scanf("%1000[^\n]%*c", msg);
        
        if((tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
            printf("Unable to get IPv4 TCP Socket using socket(): %s\n", strerror(errno));
            return -1;
        }

        if(connect(tcp_socket, (struct sockaddr*) &server_addr, server_addr_size) < 0) {
            printf("Unable to get new connection: %s\n", strerror(errno));
            break;
        }

        if(send(tcp_socket, msg, BUF_SIZE, 0) < 0) {
            printf("Unable to send message.: %s\n", strerror(errno));
            break;
        }

        if(recv(tcp_socket, msg, BUF_SIZE, 0) < 0) {
            printf("Unable to read message.: %s\n", strerror(errno));
            break;
        }
        
        printf("Server: %s\n", msg);

        if(tcp_socket != -1) close(tcp_socket);
        tcp_socket = -1;
    }
    
    if(tcp_socket != -1) close(tcp_socket);
    return 0;
}