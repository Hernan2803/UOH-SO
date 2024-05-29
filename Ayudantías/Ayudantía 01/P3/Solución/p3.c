#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "nSystem.h"

#define N 3

static nMonitor m;

int count = 0;

int cliente(void *arg) {
    nEnter(m);
    while (count >= N) {
        nExit(m);
        nExitTask(0);
    }
    count++;
    nPrintf("\n%d sillas ocupadas de %d\n\n", count, N);
    nNotifyAll(m);
    nExit(m);
    nExitTask(0);
}

int peluquero(void *arg) {
    while (TRUE){
        nEnter(m);
        while (count <= 0) {
            nPrintf("Peluquero durmiendo\n");
            nWait(m);
        }
        count--;
        nExit(m);
        nPrintf("Se desocupa una silla (pasa un cliente), %d sillas ocupadas de %d\n", count, N);
        nPrintf("Peluquero atendiendo a un cliente\n");
        nSleep(rand() % 5);
        nPrintf("Peluquero termino la atencion\n");
    }
}

int nMain() {
    m = nMakeMonitor();
    
    nTask pel = nEmitTask(peluquero);
    nTask cli[20];
    for (int i = 0; i < 20; i++) {
        cli[i] = nEmitTask(cliente, NULL);
        nSleep(rand() % 10);
    }
    nWaitTask(pel);
    
    
    return 0;
}