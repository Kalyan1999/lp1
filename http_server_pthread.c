// TCP Server that accepts requests and creates a thread using pthread to handle requests.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define true (1)
#define false (0)
#define BACKLOG 16
#define REQ_BUF_SIZE 1024
#define RES_BUF_SIZE 2048

void* handleRequest(void*);

int main() {
    int tcp_socket = -1, conn_fd = -1, on = 1;

    printf("Very Simple HTTP Web Server.\nPress Ctrl+C to exit.\n\n");

    if((tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("Unable to get IPv4 TCP Socket using socket(): %s\n", strerror(errno));
        return -1;
    }

    setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

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
            continue;
        }

        pthread_t tid;
        int *new_conn_fd = (int*)malloc(sizeof(conn_fd));
        *new_conn_fd = conn_fd;
        if(pthread_create(&tid, NULL, handleRequest, (void*) new_conn_fd) != 0) {
            printf("Unable to create new thread: %s\n", strerror(errno));
            close(conn_fd);
            free(new_conn_fd);
            continue;
        }

        if(pthread_detach(tid) != 0) {
            printf("Unable to detach new thread: %s\n", strerror(errno));
            continue;
        }
    }

    return 0;
}

void* handleRequest(void *new_conn_fd) {
    int conn_fd = *((int*)new_conn_fd);
    char req[REQ_BUF_SIZE], res[RES_BUF_SIZE];

    if(recv(conn_fd, req, REQ_BUF_SIZE, 0) < 0) {
        printf("Unable to read message: %s\n", strerror(errno));
        close(conn_fd);
        free(new_conn_fd);
        pthread_exit(-1);
    }

    printf("Received Request... ");

    sprintf(res, "HTTP/1.1 200 OK\nContent-Type: text/html\r\n\nHello, World.<br/><br/>\n"
            "Request Handled by a Thread (tid): %ld created by Parent (pid): %d.<br/><br/>\n"
            "Request: %s", pthread_self(), getpid(), req);

    if(send(conn_fd, res, strlen(res), 0) < 0) {
        printf("Unable to send message: %s\n", strerror(errno));
        close(conn_fd);
        free(new_conn_fd);
        pthread_exit(-1);
    }

    close(conn_fd);
    printf("Request Served. (tid: %ld)\n", pthread_self());
    free(new_conn_fd);
    pthread_exit(0);
}