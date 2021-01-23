// Gets a Shared Memory (creates if doesn't exists) and reads MAX_COUNT (defined in 
// "shm_helper.h") random numbers and uses Semaphores for synchronization of shm access

#include "shm_helper.h"

void cleanup();

int *buf = NULL;

int main() {
    struct sembuf sbuf;
    sbuf.sem_flg = SEM_UNDO;

    atexit(cleanup);
    signal(SIGINT, exit);
    initialize();

    if((buf = (int*) shmat(shm_id, NULL, 0)) == NULL) {
        printf("Error attaching shm using shmat(): %s\n", strerror(errno));
        return -1;
    }

    printf("Reading %d Random Numbers.\n", MAX_COUNT);
    for(int i = 0; i < MAX_COUNT; i++) {
        sbuf.sem_num = SEM_READ;
        sbuf.sem_op = SEMOP_RELEASE;
        semop(sem_id, &sbuf, 1);

        sbuf.sem_num = SEM_WRITE;
        sbuf.sem_op = SEMOP_WAIT;
        semop(sem_id, &sbuf, 1);
        
        printf("\033[0;31m Read: %3d\033[0m\n", *buf);
    }
    printf("Done.....\n");

    return 0;
}

void cleanup() {
    if(buf != NULL) shmdt(buf);
}