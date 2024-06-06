#include <stdio.h>
#include <stdbool.h>
#include "nSystem.h"

#define MAX_PROCESOS 10
#define QUANTUM 0 //ms

// Estructura para almacenar información del proceso
typedef struct {
    int id;     // Identificador del proceso
    int numero; // numero a calcular
} proceso;

int fib(int num){
    if (num==0){
        return 0;
    }
    if (num==1){
        return 1;
    }
    return fib(num-1) + fib(num-2);
}

// Función que se ejecuta en el hilo para ejecutar un proceso
int ejecutar_proceso_hilo(void *arg) {
    proceso *p = (proceso *)arg;
    int value = fib(p->numero);
    nPrintf("Proceso %d terminado, valor %d\n", p->id, value);
    return 0;
}

int nMain() {
    int n_procesos;
    proceso procesos[MAX_PROCESOS];
    //nSetTimeSlice(QUANTUM);

    nPrintf("Ingrese el número de procesos (máximo %d): ", MAX_PROCESOS);
    scanf("%d", &n_procesos);

    // Inicializar los procesos
    for (int i = 0; i < n_procesos; i++) {
        nPrintf("Ingrese numero de Fibonacci a calcular por el proceso %d: ", i + 1);
        scanf("%d", &procesos[i].numero);
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

    return 0;
}
