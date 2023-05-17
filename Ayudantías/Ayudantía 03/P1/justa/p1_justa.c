#include <nSystem.h>

#define N 10

nMonitor mon;
nCondition cond_var, cond_dam;

int damas_in = 0;
int varones_in = 0;
int damas_out = 0;
int varones_out = 0;

int no_amanezca(){
  return 1;
}

void beber_cerveza(){
  nSleep(rand()%10);
}

void ocupar_baño(){
  nSleep(rand()%10);
}

int damas(){
  while (no_amanezca()) {

    beber_cerveza();
    nEnter(mon);
    damas_out++;
    nPrintf("%d damas esperando\n", damas_out);
    // condicion: si hay varones dentro || (si hay 3 o mas varones esperando afuera && hay menos de tres muejeres tratando de entrar)
    while(varones_in>=1 || (varones_out>=3 && damas_out<3)){
      nWaitCondition(cond_dam);
    }
    damas_out--;
    nPrintf("%d damas esperando\n", damas_out);-
    damas_in++;
    nPrintf("%d damas dentro\n", damas_in);
    nExit(mon);

    ocupar_baño();

    nEnter(mon);
    damas_in--;
    nPrintf("%d damas dentro\n", damas_in);
    if(damas_in==0){
      nPrintf("%d damas dentro\n", damas_in);
      nSignalCondition(cond_var);
    }
    nExit(mon);
  }
  nExitTask(0);
  return 0;
}

int varones(){
  while (no_amanezca()) {
    beber_cerveza();
    
    nEnter(mon);
    varones_out++;
    nPrintf("%d varones esperando\n", varones_out);
    // condicion de espera: si hay damas dentro || (si hay 3 o mas damas esperando afuera && hay menos de tres varones tratando de entrar)
    while(damas_in>=1 || (damas_out>=3 && varones_out<3)){
      nWaitCondition(cond_var);
    }
    varones_out--;
    nPrintf("%d varones esperando\n", varones_out);
    varones_in++;
    nPrintf("%d varones dentro\n", varones_in);
    nExit(mon);

    ocupar_baño();
    
    nEnter(mon);
    varones_in--;
    nPrintf("%d varones dentro\n", varones_in);
    if(varones_in==0){
      nPrintf("%d varones dentro\n", varones_in);
      nSignalCondition(cond_dam);
    }
    nExit(mon);
  }
  nExitTask(0);
  return 0;
}

int nMain(){
  int i;
  mon = nMakeMonitor();
  cond_var = nMakeCondition(mon);
  cond_dam = nMakeCondition(mon);

  nTask varonesTask[N];
  nTask damasTask[N];

  for(i=0; i<N; i++){
    varonesTask[i] = nEmitTask(varones);
    damasTask[i] = nEmitTask(damas);
    nSleep(rand()%10/2);
  }

  for(i=0; i<N; i++){
    nWaitTask(varonesTask[i]);
    nWaitTask(damasTask[i]);
  }
  
  
  nDestroyCondition(cond_var);
  nDestroyCondition(cond_dam);
  return 0;
}