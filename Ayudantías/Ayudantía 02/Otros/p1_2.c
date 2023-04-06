#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define READERS 5
#define WRITERS 3

int shared_resource = 0;
int readers_count = 0;

nMonitor mon;
pthread_cond_t can_read = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_write = PTHREAD_COND_INITIALIZER;

void *reader(void *arg) {
    int id = *((int*)arg);
    while(1) {
        nEnter(mon);
        while(shared_resource == -1) {
            nWaitCondition(&can_read, &mutex);
        }
        readers_count++;
        nExit(mon);

        printf("Reader %d reads %d\n", id, shared_resource);

        nEnter(mon);
        readers_count--;
        if(readers_count == 0) {
            pthread_cond_signal(&can_write);
        }
        nExit(mon);

        sleep(1);
    }
    return NULL;
}

void *writer(void *arg) {
    int id = *((int*)arg);
    while(1) {
        nEnter(mon);
        while(shared_resource != -1) {
            nWaitCondition(&can_write, &mutex);
        }
        shared_resource = id;

        printf("Writer %d writes %d\n", id, shared_resource);

        pthread_cond_signal(&can_read);
        pthread_cond_signal(&can_write);
        nExit(mon);

        sleep(1);
    }
    return NULL;
}

int main() {
    mon = nMakeMonitor();
    pthread_t readers[READERS];
    pthread_t writers[WRITERS];

    int reader_ids[READERS];
    int writer_ids[WRITERS];

    // create readers
    for(int i = 0; i < READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // create writers
    for(int i = 0; i < WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // join readers
    for(int i = 0; i < READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    // join writers
    for(int i = 0; i < WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}
