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
        nWaitSem(sem);
        nPrintf("Escribiendo: %d\n", editing);
        while (editing > 0){
            nSignalSem(sem);
            nSleep(10);
            nWaitSem(sem);
        }
        reading++;
        nSignalSem(sem);
        
        // read
        nSleep(rand() % 100);

        nWaitSem(sem);
        reading--;
        nSignalSem(sem);
    }

    nExitTask(0);
    return 0;
}

int writer(void* arg){
    int j;
    for (j = 0; j < MAX_MOD; j++){
        nWaitSem(sem);
        nPrintf("Leyendo: %d\n", reading);
        editing++;
        while (reading > 0){
            nPrintf("Leyendo: %d\n", reading);
            nSignalSem(sem);
            nSleep(1);
            nWaitSem(sem);
        }
        nSignalSem(sem);

        nPrintf("Modificando\n");
        // write
        nSleep(rand() % 5);

        nWaitSem(sem);
        editing--;
        nSignalSem(sem);

        nSleep(rand() % 10);
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