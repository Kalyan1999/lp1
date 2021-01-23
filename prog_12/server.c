// Creates a Message Queue and acts as a Server providing "isPrime" & "isEven" services.

#include <math.h>
#include "message.h"

void sigint(int);
int isEven(int);
int isPrime(int);

key_t key = -1;
int msg_id = -1;

int main() {
    signal(SIGINT, sigint);

    printf("Server Program.\nPress Ctrl+C to exit.\n\n");

    if((key = ftok("message.h", 69)) < 0) {
        printf("Error creating key using ftok(). Exiting...\n");
        return -1;
    }

    if((msg_id = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Error getting message queue using msgget(). Exiting...\n");
        return -1;
    }

    request req;
    response res;

    while(true) {
        msgrcv(msg_id, &req, REQ_SIZE, MSG_TYPE_REQ, 0);

        if(req.req_service == REQ_SRV_EVEN) {
            printf("Received Request: isEven(%d)... ", req.req_data);
            
            res.res_type = MSG_TYPE_RES_EVEN;
            res.res_data = isEven(req.req_data);
            msgsnd(msg_id, &res, RES_SIZE, 0);
            
            printf("Processed.\n");
        } else if(req.req_service == REQ_SRV_PRIME) {
            printf("Received Request: isPrime(%d)... ", req.req_data);
            
            res.res_type = MSG_TYPE_RES_PRIME;
            res.res_data = isPrime(req.req_data);
            msgsnd(msg_id, &res, RES_SIZE, 0);
            
            printf("Processed.\n");
        } else {
            printf("Received Request: Unknown... Discarding.\n");
        }
    }

    return 0;
}

void sigint(int signum) {
    printf("\nDeleting message queue and Stopping Server.....\n");
    if(msg_id != -1) msgctl(msg_id, IPC_RMID, NULL);
    exit(0);
}

int isEven(int n) {
    return n % 2 == 0;
}

int isPrime(int n) {
    if(n < 2) return false;

    int max = (int)sqrt(n);
    for(int i = 2; i <= max; i++) {
        if(n%i == 0) return false;
    }
    
    return true;
}