#include <stdio.h>
#include <stdlib.h>
#include "nSystem.h"

#define MAX_MOD 5
#define MAX_READ 20

nSem sem;
int reading = 0;
int editing = 0;

int reader(void* arg){
    int i;
    for (i = 0; i < MAX_READ; i++){

        // read
        nSleep(rand() % 100);

    }

    nExitTask(0);
    return 0;
}

int writer(void* arg){
    int j;
    for (j = 0; j < MAX_MOD; j++){

        // write
        nSleep(rand() % 5);
        
    }

    nExitTask(0);
    return 0;
}

int nMain() {
    sem = nMakeSem(1);

    nTask writer_task = nEmitTask(writer);
    nTask reader_task[3];
    for (int j = 0; j < 3; j++) {
        reader_task[j] = nEmitTask(reader);
    }

    nWaitTask(writer_task);

    for (int j = 0; j < 3; j++) {
        nWaitTask(reader_task[j]);
    }

    return 0;
}