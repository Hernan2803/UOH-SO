#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "nSystem.h"

#define M 10
#define N 3

nMonitor mon;
nCondition cond_vendedor, cond_cliente;

int current_ticket = 0; // ticket llamado
int ticket = 0; // ticket cliente

int vendedor(int id){


    nExitTask(0);
    return 0;
}

int client(int id){

 
    nExitTask(0);
    return 0;
}

int nMain(){
  mon = nMakeMonitor();
  cond_vendedor = nMakeCondition(mon);
  cond_cliente = nMakeCondition(mon);

  nTask vendedores[N], clients[M];
  
  //esperar tareas para terminar
  for (int i = 0; i < N; i++){
    vendedores[i] = nEmitTask(vendedor, i+1);
  }
  for (int i = 0; i < M; i++){
    clients[i] = nEmitTask(client, i+1);
  }
  for (int i = 0; i < M; i++){
    nWaitTask(clients[i]);
  }
  for (int i = 0; i < N; i++){
    nWaitTask(vendedores[i]);
  }
  return 0;
}