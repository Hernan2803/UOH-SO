/*
Del enunciado:
- no existen herramientas de sincronización, por lo que nos piden implementar nExchange()

Funcionamiento de nExchange(nTask task, int val)
- dos tareas deben invocar este procedimiento pasando como argumento:
    task -> identificador de la tarea con quien realizaran el intercambio
    val -> valor que desean intercambiar

*/
//#include "nSystem.h"

#define READY 1
#define PROCESS_nExchange 2
#define WAIT_nExchange 3
#define PENDING_nExchange 4


int nExchange(nTask task, int val){
  int rc;
  nTask this_task = current_task;

  if (this_task.status == READY){
    this_task.status = PROCESS_nExchange;
  }

  while(this_task.status != READY){
    START_CRITICAL();
    //revisa si tiene un cambio pendiente
    if (this_task.status == PENDING_nExchange) {
      task.rc = val;
      task.status = READY;
    }
    else{
      this_task.status = WAIT_nExchange;
    }

    //se queda esperando a que el otro este dispuesto a hacer un cambio
    if (task.status == WAIT_nExchange){
      task.rc = val;

      //cambiar estado de las tareas
      task.status == PENDING_nExchange;
      this_task.status = READY;
    }
    
    if ((this_task.status == READY) && (task.status == READY)){
      PutTask(ready_queue, this_task);
      PutTask(ready_queue, task);
    }
    END_CRITICAL();
  }
  
}