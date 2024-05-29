#include <stdio.h>
#include <stdlib.h>
#include "nSystem.h"

#define N 2
#define MAX_BUFFER_SIZE 5
#define MAX_PRODUCED_ITEMS 40

int i;
int n_ent = 0;
nSem space;
nSem pedido;

void cocinar(){
  nPrintf("Cocinando\n");
  nSleep(rand() % 5);
}

void repartiendo(int ent){
  nPrintf("Repartiendo pedido %d\n", ent);
  nSleep(rand() % 10);
}

int cocinero(void* arg) {
  for (i = 0; i<MAX_PRODUCED_ITEMS; i++){
    cocinar();
    nWaitSem(space);
    nSignalSem(pedido);
    nPrintf("Cocinado %d\n", i);
  }

  nExitTask(0);
  return 0;
}

int repartir(void* arg){
  while(TRUE){ //n_ent < MAX_PRODUCED_ITEMS){
    nWaitSem(pedido);
    int local_ent = n_ent;
    n_ent ++;
    nSignalSem(space);
    repartiendo(local_ent);
    nPrintf("Pedido %d entregado\n", local_ent);
  }

  nExitTask(0);
  return 0;
}

int nMain() {
  space = nMakeSem(MAX_BUFFER_SIZE);
  pedido = nMakeSem(0);

  nTask cocina = nEmitTask(cocinero);

  nTask mesero[N];
  for (int j = 0; j < N; j++) {
    mesero[j] = nEmitTask(repartir);
  }
  nWaitTask(cocina);
  for (int j = 0; j < N; j++) {
    nPrintf("%d", j);
    nWaitTask(mesero[j]);
  }

  return 0;
}