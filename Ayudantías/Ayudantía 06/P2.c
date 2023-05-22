/*
Del enunciado:
- no existen herramientas de sincronización, por lo que nos piden implementar nExchange()

Funcionamiento de nExchange(nTask task, int val)
- dos tareas deben invocar este procedimiento pasando como argumento:
    task -> identificador de la tarea con quien realizaran el intercambio
    val -> valor que desean intercambiar

*/
#define Pending 1

Queue list_wait = MakeQueue();

int nExchange(nTask task, int val){
    START_CRITICAL();
    nTask this_task = currentTask;
    this_task.status = Pending;

    while(task.status != Pending){
        END_CRITICAL();
        START_CRITICAL();
    }
    
    PutTask(list_wait, this_task);
    END_CRITICAL();
}

int nMain(){
    nTask tasks[2];
    tasks[0] = nEmitTask(nExchange, tasks[1], 3);
    tasks[1] = nEmitTask(nExchange, tasks[0], 5);

    nPrintf("Tarea 0: %d", nWaitTask(tasks[0]));
    nPrintf("Tarea 1: %d", nWaitTask(tasks[1]));
}