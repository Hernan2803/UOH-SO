#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "nSystem.h"

#define M 10
#define N 3

nMonitor mon;
nCondition cond_vendedor, cond_cliente;

int current_ticket = 0;
int ticket = 0;

int vendedor(int id){
  while(TRUE){
    nEnter(mon);
    nPrintf("Ticket %d\n", ticket);
    while(ticket==0 || ticket==current_ticket){
      nWaitCondition(cond_vendedor);
    }
    current_ticket++;
    nPrintf("Vendedor %d llama al cliente con ticket %d\n", id, current_ticket);
    nSignalCondition(cond_cliente);
    nExit(mon);

    nSleep(rand() % 10);

    nEnter(mon);
    if (ticket == current_ticket && ticket == M) {
      nExit(mon);
      break;
    }
    nExit(mon);
  }

  nExitTask(0);
  return 0;
}

int client(int id){
  int my_ticket;

  nEnter(mon);
  my_ticket = ++ticket;
  nPrintf("Cliente %d obtiene el ticket %d\n", id, my_ticket);
  nSignalCondition(cond_vendedor);
  nExit(mon);

  nEnter(mon);
  while(my_ticket != current_ticket){
    nWaitCondition(cond_cliente);
  }
  
  nExit(mon);
  nPrintf("Cliente %d es atendido\n", id);

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