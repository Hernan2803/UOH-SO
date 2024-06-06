#include <stdio.h>
#include <stdlib.h>
#include "nSystem.h"

#define N 2 //numero de meseros
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

void repartir(int ent){
  nPrintf("Repartiendo pedido %d\n", ent);
  nSleep(rand() % 10);
}

int cocinero(void* arg) {
  for (i = 0; i<MAX_PRODUCED_ITEMS; i++){
    
  }

  nExitTask(0);
  return 0;
}

int mesero(void* arg){
  while(TRUE){ //n_ent < MAX_PRODUCED_ITEMS){

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
    mesero[j] = nEmitTask(mesero);
  }
  nWaitTask(cocina);
  for (int j = 0; j < N; j++) {
    nPrintf("%d", j);
    nWaitTask(mesero[j]);
  }

  return 0;
}