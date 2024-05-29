#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "nSystem.h"

nMonitor m;

int view = 0;
int barista_mod = 20;

int cliente(int id) {
    nEnter(m);


    nExit(m);
    return 0;
}

int barista(void *arg) {
    while (barista_mod--){
        nEnter(m);

        nExit(m);
    }
    return 0;
}

int nMain() {
    m = nMakeMonitor();
    
    nTask baristas = nEmitTask(barista);

    nTask clientes[20];
    for (int i = 0; i < 20; i++) {
        clientes[i] = nEmitTask(cliente, i);
        nSleep(rand() % 10);
    }

    nWaitTask(baristas);
    for (int i = 0; i < 20; i++) {
        nWaitTask(clientes[i]);
    }
    
    return 0;
}