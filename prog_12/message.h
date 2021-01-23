#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

typedef struct {
    long req_type;
    int req_data;
    int req_service;
} request;

typedef struct {
    long res_type;
    int res_data;
} response;

#define REQ_SIZE (2 * sizeof(int))
#define RES_SIZE (sizeof(int))

#define MSG_TYPE_REQ 1
#define MSG_TYPE_RES_EVEN 2
#define MSG_TYPE_RES_PRIME 3

#define REQ_SRV_EVEN 1
#define REQ_SRV_PRIME 2