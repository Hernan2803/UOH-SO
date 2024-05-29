#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "nSystem.h"

#define N 3

static nMonitor m;

int sillasOcupadas = 0;

int cliente(void *arg) {
    nEnter(m);
    if(sillasOcupadas>=N){
        nExit(m);
        nExitTask(0);
    }
    sillasOcupadas++;
    nNotifyAll(m);
    nExit(m);
    nExitTask(0);
    return 0;
}

int peluquero(void *arg) {
    while (TRUE){
        nEnter(m);
        while(sillasOcupadas<=0){
            nWait(m);
        }
        sillasOcupadas--;
        nExit(m);

        nPrintf("Se desocupa una silla (pasa un cliente), %d sillas ocupadas de %d\n", sillasOcupadas, N);
        nPrintf("Peluquero atendiendo a un cliente\n");
        nSleep(rand() % 10); //atendiendo
        nPrintf("Peluquero termino la atencion\n");
    }
}

int nMain() {
    m = nMakeMonitor();
    
    nTask pel = nEmitTask(peluquero);
    nTask cli[20];
    for (int i = 0; i < 20; i++) {
        cli[i] = nEmitTask(cliente, NULL);
        nSleep(rand() % 5);
    }
    nWaitTask(pel);
    for (int i = 0; i < 20; i++) {
        nWaitTask(cli[i]);
    }
    
    return 0;
}