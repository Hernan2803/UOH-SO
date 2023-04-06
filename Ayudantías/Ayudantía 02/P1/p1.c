#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nSystem.h"

#define N 100
#define N_PRINTERS 3

nMonitor mon;
int occuped = 0;

void imprimir(){
  nSleep(rand()%1001);
}

int print_while() {
  while (1) {
    // pedir la impresora
    nEnter(mon);
    while(occuped>=N_PRINTERS){
      nWait(mon);
    }
    occuped++;
    nPrintf("Master a tomado la impresora\n");
    nExit(mon);

    // imprimiendo
    imprimir();

    // liberar la impresora
    nEnter(mon);
    occuped--;
    nNotifyAll(mon);
    nExit(mon);

    // buscando que imprimir despues
    //nSleep(100);
  }
  nExitTask(0);
  return 0;
}

int printer() {
  // pedir la impresora
  nEnter(mon);
  while(occuped>=N_PRINTERS){
    nWait(mon);
  }
  occuped++;
  nPrintf("Un minion a tomado la impresora\n");
  nExit(mon);

  //imprimiendo
  imprimir();
  
  //liberar la impresora
  nEnter(mon);
  occuped--;
  nNotifyAll(mon);
  nExit(mon);
  nExitTask(0);
  return 0;
}


int nMain() {
  
  mon = nMakeMonitor();
  
  nTask master, minions[N];
  master = nEmitTask(print_while);
  
  for (int i = 0; i < N; i++) {
    minions[i] = nEmitTask(printer);
  }
  

  for (int i = 0; i < 3; i++) {
    nWaitTask(minions[i]);
  }
  nWaitTask(master);

  return 0;
}