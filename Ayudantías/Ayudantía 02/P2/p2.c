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
    // llamar al siguiente cliente


    nPrintf("Vendedor %d llama al cliente con ticket %d\n", num, current_ticket);

    // revisar si ya se atendieron a todos
  }

  nExitTask(0);
  return 0;
}

int client(int id){
  int num = id;
  int my_ticket;

  // obtener un ticket
  
  

  // revisar si es su turno (despierta con cond_client)
  


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