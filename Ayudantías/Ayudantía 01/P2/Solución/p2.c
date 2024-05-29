#include <stdio.h>
#include <stdlib.h>
#include "nSystem.h"

#define N 4
#define MAX_BUFFER_SIZE 10
#define MAX_PRODUCED_ITEMS 100

int i;
int n_ent = 0;
nSem space;
nSem pizza;

void cocinar(){
  nPrintf("Cocinando\n");
  nSleep(rand() % 5);
}

void repartiendo(int ent){
  nPrintf("%d repartiendo\n", ent);
  nSleep(rand() % 7);
}

int cocinero(void* arg) {
  for (i = 0; i<MAX_PRODUCED_ITEMS; i++){
    cocinar();
    nWaitSem(space);
    nSignalSem(pizza);
  }
  
  nExitTask(0);
  return 0;
}

int repartidor(void* arg){
  while(TRUE){
    nWaitSem(pizza);
    int local_ent = n_ent;
    n_ent ++;
    nSignalSem(space);
    repartiendo(local_ent);
    nPrintf("%d entregado\n", local_ent);
  }
  nExitTask(0);
  return 0;
}

int nMain() {
  space = nMakeSem(MAX_BUFFER_SIZE);
  pizza = nMakeSem(0);

  nTask cocina = nEmitTask(cocinero);
  nTask repartidores[N];
  for (int j = 0; j < N; j++) {
    repartidores[j] = nEmitTask(repartidor);
  }
  nWaitTask(cocina);

  return 0;
}