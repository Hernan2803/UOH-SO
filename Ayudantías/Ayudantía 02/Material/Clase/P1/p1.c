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
    while(TRUE){
        nEnter(mon);
        while (ticket == current_ticket) {
          nWaitCondition(cond_vendedor);
        }
        current_ticket++;
        nSignalCondition(cond_cliente);
        nExit(mon);

        nPrintf("%d llamando\n", id);
        nSleep(rand() % 50);

    }

    nExitTask(0);
    return 0;
}

int client(int id){
    nEnter(mon);
    int local_ticket = ++ticket;
    nSignalCondition(cond_vendedor);
    nExit(mon);

    nPrintf("Cliente id: %d tiene el ticket: %d\n", id, local_ticket);

    nEnter(mon);
    while (current_ticket != local_ticket){
        nWaitCondition(cond_cliente);
    }
    nExit(mon);
    nPrintf("Atienden a: %d\n", local_ticket);
    
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