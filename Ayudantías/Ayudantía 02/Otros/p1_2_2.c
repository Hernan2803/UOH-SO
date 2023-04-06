#include <stdio.h>
#include <stdlib.h>
#include "nSystem.h"

#define READERS 5
#define WRITERS 3

int shared_resource = 0;
int readers_count = 0;

nMonitor mon;
nCondition can_read, can_write;

int reader(int id) {
    int local_count = 0;
    nPrintf("Lector %d creado\n", id);
    while(1) {
        nEnter(mon);
        while(shared_resource == -1) {
            nWaitCondition(can_read);
        }
        local_count = ++readers_count;
        nExit(mon);

        nPrintf("Reader %d reads %d\n", id, local_count);

        nEnter(mon);
        readers_count--;
        if(readers_count == 0) {
            nSignalCondition(can_write);
        }
        nExit(mon);

        nSleep(1);
    }
    nExitTask(0);

}

int writer(int id) {
    while(1) {
        nEnter(mon);
        while(shared_resource != 0) {
            nWaitCondition(can_write);
        }
        shared_resource = id;

        nPrintf("\nWriter %d writes %d\n", id, shared_resource);

        nSignalCondition(can_read);
        nSignalCondition(can_write);
        nExit(mon);

        nSleep(1);
    }
    nExitTask(0);
}

int nMain() {
    nTask readers[READERS], writers[WRITERS];
    mon = nMakeMonitor();
    can_read = nMakeCondition(mon);
    can_write = nMakeCondition(mon);

    //int reader_ids[READERS], writer_ids[WRITERS];

    // create readers
    for(int i = 0; i < READERS; i++) {
        readers[i] = nEmitTask(reader, i+1);
    }

    // create writers
    for(int i = 0; i < WRITERS; i++) {
        writers[i] = nEmitTask(writer, i+1);
    }

    // join readers
    for(int i = 0; i < READERS; i++) {
        nWaitTask(readers[i]);
    }

    // join writers
    for(int i = 0; i < WRITERS; i++) {
        nWaitTask(writers[i]);
    }

    return 0;
}
