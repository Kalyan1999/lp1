// Peer-to-peer UDP Server using Linux sockets API

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 1024

int udp_socket = -1;

void cleanup();

int main() {
    atexit(cleanup);
    signal(SIGINT, exit);

    int on = 1;
    char buf[BUF_SIZE];
    struct sockaddr_in server, client;

    printf("Server program.\nPress Ctrl+C to exit.\n\n");

    if((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Unable to get UDP Socket using socket(): %s\n", strerror(errno));
        return -1;
    }

    setsockopt(udp_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_aton("127.0.0.1", &server.sin_addr);
    int server_size = sizeof(server);
    int client_size = sizeof(client);

    if(bind(udp_socket, (struct sockaddr*) &server, server_size) < 0) {
        printf("Unable to bind socket to server address: %s\n", strerror(errno));
        return -1;
    }

    while(true) {
        int buf_size = recvfrom(udp_socket, buf, BUF_SIZE, 0, 
                                (struct sockaddr*) &client, &client_size);

        sendto(udp_socket, buf, buf_size, 0, (struct sockaddr*) &client, client_size);
        
        printf("[Echoed message from %s:%d] > %s\n", 
                inet_ntoa(client.sin_addr), (int) (client.sin_port), buf);
    }

    return 0;
}

void cleanup() {
    printf("\nShutting down server.....\n");
    close(udp_socket);
}