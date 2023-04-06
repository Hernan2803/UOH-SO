#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "nSystem.h"

#define NUM_CLIENTS 10
#define NUM_SELLERS 1

nMonitor mon;
nCondition cond_seller, cond_client;

int current_ticket=0;
int ticket=0;

int seller(int id){
  int num = id;

  while(1){
    nEnter(mon);
    while(ticket==0 || ticket==current_ticket || ticket < (num-1)*NUM_CLIENTS+1){
      nWaitCondition(cond_seller);
    }
    current_ticket++;
    nSignalCondition(cond_client);
    nExit(mon);

    nPrintf("Vendedor %d llama al cliente con ticket %d\n", num, current_ticket);

    nEnter(mon);
    if (ticket > NUM_SELLERS*NUM_CLIENTS) {
      nExit(mon);
      break;
    }
    nExit(mon);
    nSleep(1);
  }

  nExitTask(0);
  return 0;
}

int client(int id){
  int num = id;
  int my_ticket;

  nEnter(mon);
  my_ticket = ++ticket;
  nPrintf("Cliente %d obtiene el ticket %d\n", num, my_ticket);
  nSignalCondition(cond_seller);
  nExit(mon);

  nEnter(mon);
  while(my_ticket != current_ticket){
    nWaitCondition(cond_client);
  }
  
  nExit(mon);
  nPrintf("Cliente %d es atendido\n", num);

  nExitTask(0);
  return 0;
}

int nMain(){
  int i;

  mon = nMakeMonitor();
  cond_seller = nMakeCondition(mon);
  cond_client = nMakeCondition(mon);

  nTask sellers, clients[NUM_CLIENTS];
  sellers = nEmitTask(seller, 1);

  for (i = 0; i < NUM_CLIENTS; i++){
    clients[i] = nEmitTask(client, i+1);
  }

  for (i = 0; i < NUM_CLIENTS; i++){
    nWaitTask(clients[i]);
  }

  nWaitTask(sellers);

  return 0;
}