// TCP Server using Linux sockets API

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

#define true (1)
#define false (0)
#define BACKLOG 16
#define BUF_SIZE 1024

int main() {
    int tcp_socket = -1, conn_fd = -1;
    char msg[BUF_SIZE];

    printf("Server Program. (Simple Message Echo Server)\nPress Ctrl+C to exit.\n\n");

    if((tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("Unable to get IPv4 TCP Socket using socket(): %s\n", strerror(errno));
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int server_addr_size = sizeof(server_addr);

    if(bind(tcp_socket, (struct sockaddr*) &server_addr, server_addr_size) < 0) {
        printf("Unable to bind socket to server address: %s\n", strerror(errno));
        return -1;
    }

    if(listen(tcp_socket, BACKLOG) < 0) {
        printf("Unable to listen to socket: %s\n", strerror(errno));
        return -1;
    }

    while(true) {
        if((conn_fd = accept(tcp_socket, (struct sockaddr*) &server_addr, 
                                &server_addr_size)) < 0) {
            printf("Unable to accept new connection: %s\n", strerror(errno));
            break;
        }

        if(recv(conn_fd, msg, BUF_SIZE, 0) < 0) {
            printf("Unable to read message: %s\n", strerror(errno));
            break;
        }

        printf("Received: %s; Echoing back message... ", msg);
        if(send(conn_fd, msg, BUF_SIZE, 0) < 0) {
            printf("Unable to send message: %s\n", strerror(errno));
            break;
        }
        
        printf("Message Sent.\n");
        fflush(stdout);

        if(conn_fd != -1) close(conn_fd);
        conn_fd = -1;
    }

    return 0;
}