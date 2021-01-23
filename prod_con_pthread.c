// Solves Producer - Consumer problem using pthreads and Mutex

#include <stdio.h>
#include <pthread.h>

#define BUF_SIZE 6
#define MAX_COUNT 42

int buf[BUF_SIZE];
int size = 0, front = 0, rear = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t m_produce = PTHREAD_COND_INITIALIZER;
pthread_cond_t m_consume = PTHREAD_COND_INITIALIZER;

void insert();
int delete();
void* producer(void*);
void* consumer(void*);

int main() {
    pthread_t producer_tid, consumer_pid;
    
    pthread_create(&producer_tid, NULL, producer, NULL);
    pthread_create(&consumer_pid, NULL, consumer, NULL);
    
    pthread_join(producer_tid, NULL);
    pthread_join(consumer_pid, NULL);
    
    return 0;
}

void insert(int data) {
    buf[rear] = data;
    rear = (rear + 1) % BUF_SIZE;
    size++;
}

int delete() {
    int data = buf[front];
    front = (front + 1) % BUF_SIZE;
    size--;
    return data;
}

void* producer(void* arg) {
    for(int i = 0; i < MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        while(size == BUF_SIZE) pthread_cond_wait(&m_produce, &mutex);
        insert(i);
        printf("\033[0;32m Produced: %3d\033[0m\n", i);
        pthread_cond_signal(&m_consume);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* consumer(void* arg) {
    for(int i = 0; i < MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        while(size == 0) pthread_cond_wait(&m_consume, &mutex);
        int data = delete();
        printf("\033[0;31m Consumed: %3d\033[0m\n", data);
        pthread_cond_signal(&m_produce);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}