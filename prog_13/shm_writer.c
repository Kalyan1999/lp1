// Gets a Shared Memory (creates if doesn't exists) and writes MAX_COUNT (defined in 
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

    printf("Writing %d Random Numbers.\n", MAX_COUNT);
    for(int i = 0; i < MAX_COUNT; i++) {
        sbuf.sem_num = SEM_READ;
        sbuf.sem_op = SEMOP_WAIT;
        semop(sem_id, &sbuf, 1);

        *buf = rand();
        printf("\033[0;32m Wrote: %3d\033[0m\n", *buf);

        sbuf.sem_num = SEM_WRITE;
        sbuf.sem_op = SEMOP_RELEASE;
        semop(sem_id, &sbuf, 1);
    }
    printf("Done.....\n");

    return 0;
}

void cleanup() {
    if(buf != NULL) shmdt(buf);
    if(shm_id != -1) shmctl(shm_id, IPC_RMID, NULL);
    if(sem_id != -1) semctl(sem_id, 0, IPC_RMID);
}