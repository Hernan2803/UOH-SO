#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <time.h>
#include "nSystem.h"

#define N 100

nMonitor mon;
int occuped = 0;

void imprimir(){
  /*int limit = rand()%1000;
  int o1=0;
  int o2=1;
  for (int i = 2; i<=limit; i++){
    int o3 = o1+o2;
    o1=o2;
    o2=o3;
  }*/
  nSleep(rand()%1001);
}

int print_while() {
  while (1) {
    // pedir la impresora
    nEnter(mon);
    while(occuped != 0){
      nWait(mon);
    }
    occuped++;
    nExit(mon);
    nPrintf("Master a tomado la impresora\n");
    
    // imprimiendo
    imprimir();

    // liberar la impresora
    nEnter(mon);
    while(occuped == 0){
      nWait(mon);
    }
    occuped--;
    nNotifyAll(mon);
    nExit(mon);

    // buscando que imprimir despues
    nSleep(100);
  }
  nExitTask(0);
  return 0;
}

int printer() {
  // pedir la impresora
  nEnter(mon);
  while(occuped != 0){
    nWait(mon);
  }
  occuped++;
  nExit(mon);
  nPrintf("Un minion a tomado la impresora\n");
  
  //imprimiendo
  imprimir();

  //liberar la impresora
  nEnter(mon);
  while(occuped == 0){
    nWait(mon);
  }
  occuped--;
  nNotifyAll(mon);
  nExit(mon);

  nExitTask(0);
  return 0;
}


int nMain() {
  srand(time(NULL));
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