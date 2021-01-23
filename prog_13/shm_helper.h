#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define MAX_COUNT 25

#define SEM_COUNT 2
#define SEM_READ 0
#define SEM_WRITE 1
#define SEMOP_WAIT -1
#define SEMOP_RELEASE 1

void initialize();

key_t key = -1;
int shm_id = -1, sem_id = -1;

void initialize() {
    if((key = ftok("shm_helper.c", 69)) < 0) {
        printf("Error creating key using ftok(): %s\n", strerror(errno));
        exit(-1);
    }

    if((shm_id = shmget(key, sizeof(int), 0600 | IPC_CREAT)) < 0) {
        printf("Error getting shm_id using shmget(): %s\n", strerror(errno));
        exit(-1);
    }

    if((sem_id = semget(key, SEM_COUNT, 0600 | IPC_CREAT)) < 0) {
        printf("Error getting sem_id using semget(): %s\n", strerror(errno));
        exit(-1);
    }
}