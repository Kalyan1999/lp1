// Using Message Queue, created by server.c, seeks "isEven" service.

#include "message.h"

int main() {
    key_t key = -1;
    int msg_id = -1;

    printf("Client (isEven) Program.\nPress Ctrl+C to exit.\n\n");

    if((key = ftok("message.h", 69)) < 0) {
        printf("Error creating key using ftok(). Exiting...\n");
        return -1;
    }

    if((msg_id = msgget(key, 0666)) < 0) {
        printf("Error getting message queue using msgget().\nTry running the Server first.\n"
                    "Exiting...\n");
        return -1;
    }

    request req;
    response res;
    req.req_type = MSG_TYPE_REQ;

    while(true) {
        printf("Enter a Number: ");
        scanf("%d", &req.req_data);
        req.req_service = REQ_SRV_EVEN;
        if(msgsnd(msg_id, &req, REQ_SIZE, 0) < 0) {
            printf("Unable to send request to Server process.\n\n");
            continue;
        }

        if(msgrcv(msg_id, &res, RES_SIZE, MSG_TYPE_RES_EVEN, 0) < 0) {
            printf("Unable to receive response from Server process.\n\n");
            continue;
        }
        
        if(res.res_data)
            printf("%d is an even no.\n\n", req.req_data);
        else 
            printf("%d is an odd no.\n\n", req.req_data);
    }

    return 0;
}