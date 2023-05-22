/*
Del enunciado:
- no existen herramientas de sincronización, por lo que nos piden implementar nExchange()

Funcionamiento de nExchange(nTask task, int val)
- dos tareas deben invocar este procedimiento pasando como argumento:
    task -> identificador de la tarea con quien realizaran el intercambio
    val -> valor que desean intercambiar

*/
#define READY 0             // estado para tareas listas para continuar su ejecucion
#define Pending 1           // estado para tareas con nExchange pendiente
#define nExchangeState 2    // estado para tareas que llaman a nExchange

typedef struct Task{
    int status;
    int rc;
} *nTask

Queue list_wait = MakeQueue(); // lista para guardar los pid de las tareas que estan pendientes
Queue list_wait_change = MakeQueue(); // lista para guardar los pid de las tareas con las que se quieren hacer los cambios (las de list_wait)

int nExchange(nTask task, int val){
    nTask this_task = currentTask;  // obtengo el pid de la tarea en ejecucion
    this_task->status = Pending;     // cambio su estado a pendiente
    
    START_CRITICAL();
    PutTask(list_wait, this_task);  // agrego la tarea a la lista de tareas pendientes
    PutTask(list_wait_change, task);// agrego la tarea con la que quiere cambiar

    while(task->status != Pending){  // verificar en seccion critica
        END_CRITICAL();
        // dar un espacio para que los demas puedan tomar la seccion critica
        // se queda en bucle hasta que la otra este en estado "Pending"
        START_CRITICAL();
    }

    while(1){
        // revisar si el cambio de task es con esta tarea u otra
        nTask taskAux = GetTask(list_wait); // obtengo el pid de la primera tarea en list_wait
        while (taskAux != task){    // si no es el pid que se busca, ponerlo al final y pedir el siguiente
            PutTask(list_wait, taskAux);
            PutTask(list_wait_change, GetTask(list_wait_change));   
            taskAux = GetTask(list_wait);
        }
        
        // verificacion de que el cambio es con esta tarea (this_task)
        nTask taskAuxChange = GetTask(list_wait_change);
        if (taskAuxChange != this_task){    // si no lo es poner task y taskAuxChange al final de sus queue (respectivamente)
            PutTask(list_wait, taskAux);
            PutTask(list_wait_change, taskAuxChange);   
            END_CRITICAL();
            // esperar a que el estado de task ya no sea "Pending" y que despues lo vuelva a ser, esto para asegurarnos que realizo el intercambio que tenia pendiente y ahora debe realizar otro, ojo todavia no sabemos si es con esta tarea, por eso la ejecucion esta dentro de un while(True)
            while(task->status == Pending){continue;}
            while(task->status != Pending){continue;}
            START_CRITICAL();
        }
        else{
            END_CRITICAL();
            // realizar intercambio
            task->rc = val; // mandamos val a task, en la ejecucion de task se nos deberia mandar su valor
            // cambiar estados
            task->status = READY; // mandamos el estado de task
            break; //salir del bucle while
        }
        
    }
}
