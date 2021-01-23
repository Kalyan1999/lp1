// Peer-to-peer UDP Client using Linux sockets API

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 1024

int main() {
    int udp_socket = -1;
    int on = 1;
    char buf_out[BUF_SIZE], buf_in[BUF_SIZE];
    struct sockaddr_in server, client;

    printf("Client program.\n\n");

    if((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Unable to get UDP Socket using socket(): %s\n", strerror(errno));
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_aton("127.0.0.1", &server.sin_addr);
    int server_size = sizeof(server);

    printf("> ");
    scanf("%1000[^\n]%*c", buf_out);

    sendto(udp_socket, buf_out, strlen(buf_out)+1, 0, (struct sockaddr*) &server, server_size);
    recvfrom(udp_socket, buf_in, BUF_SIZE, 0, NULL, NULL);
    
    printf("\nSent: %s\n", buf_out);
    printf("Echo: %s\n", buf_in);

    close(udp_socket);
    return 0;
}