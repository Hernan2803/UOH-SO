#include <stdio.h>
#include <stdbool.h>
#include "nSystem.h"

#define MAX_PROCESOS 10
#define QUANTUM 4 //ms

nSem sem;

// Estructura para almacenar información del proceso
typedef struct {
    int id;          // Identificador del proceso
    int tiempo_cpu;  // Tiempo de CPU restante
} proceso;

// Variable compartida para almacenar el tiempo total utilizado
int tiempo_total = 0;

// Función para simular la ejecución del proceso en el quantum
// Devuelve true si el proceso terminó, false en caso contrario
bool ejecutar_proceso(proceso *p) {
    if (p->tiempo_cpu > QUANTUM) {
        nPrintf("Ejecutando proceso %d por %d unidades de tiempo\n", p->id, QUANTUM);
        p->tiempo_cpu -= QUANTUM;
        nSleep(2);
        return false;   // El proceso no terminó
    } else {
        nPrintf("Ejecutando proceso %d por %d unidades de tiempo (terminado)\n", p->id, p->tiempo_cpu);
        p->tiempo_cpu = 0;
        return true;    // El proceso terminó
    }
}

// Función que se ejecuta en el hilo para ejecutar un proceso
int ejecutar_proceso_hilo(void *arg) {
    proceso *p = (proceso *)arg;
    while (p->tiempo_cpu > 0) {
        bool terminado = ejecutar_proceso(p);
        nWaitSem(sem);
        tiempo_total += QUANTUM;
        if (terminado) {
            nPrintf("Proceso %d terminado en tiempo %d\n", p->id, tiempo_total);
        }
        nSignalSem(sem);
    }
    return 0;
}

int nMain() {
    int n_procesos;
    proceso procesos[MAX_PROCESOS];
    nSetTimeSlice(QUANTUM);
    sem = nMakeSem(1);

    nPrintf("Ingrese el número de procesos (máximo %d): ", MAX_PROCESOS);
    scanf("%d", &n_procesos);

    // Inicializar los procesos
    for (int i = 0; i < n_procesos; i++) {
        nPrintf("Ingrese el tiempo de CPU del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].tiempo_cpu);
        procesos[i].id = i + 1;
    }

    // Crear un hilo para cada proceso y ejecutarlos en paralelo
    nTask hilos[MAX_PROCESOS];
    for (int i = 0; i < n_procesos; i++) {
        hilos[i] = nEmitTask(ejecutar_proceso_hilo, &procesos[i]);
    }

    // Esperar a que terminen todos los hilos
    for (int i = 0; i < n_procesos; i++) {
        nWaitTask(hilos[i]);
    }

    nPrintf("Todos los procesos terminados en tiempo total %d\n", tiempo_total);
    return 0;
}
