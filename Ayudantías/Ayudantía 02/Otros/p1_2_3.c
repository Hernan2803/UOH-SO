#include <stdio.h>
#include <stdlib.h>
#include "nSystem.h"

#define NUM_THREADS 4 // número de hilos a utilizar
#define LIST_SIZE 12 // tamaño de la lista

nTask tasks[NUM_THREADS];
nMonitor mon;

int result = 1;

// función que cada hilo ejecuta para buscar el valor máximo en su sección de la lista
int multiplication_thread(int arr[], int left, int right, int id) {
  int value_local = arr[left];
  // buscar el valor máximo en la sección de la lista correspondiente al hilo
  for (int i = left+1; i < right; i++) {
    value_local = value_local*arr[i];
  }
  nEnter(mon);
  result = result*value_local;
  nExit(mon);

  nExitTask(0);
  return 0;
}

// función que busca el valor máximo en una lista de números utilizando varios hilos
int multiplication_parallel(int arr[], int list_size) {
  int sep = list_size / NUM_THREADS;
  int max;
  int i;
  // crear los hilos y dividir la lista en secciones para cada hilo
  for (i = 0; i < NUM_THREADS; i++) {
    int l = i * sep;
    int m = (i+1)*sep;
    if (i == NUM_THREADS - 1) {
      m = list_size;
    }
    tasks[i] = nEmitTask(multiplication_thread, arr, l, m);
    
  }

  // devolver el valor resultado
  return nWaitTask(tasks[NUM_THREADS - 1]);
}


int nMain() {
  int numbers[LIST_SIZE] = [1,1,1, 1,1,1, 1,1,1, 1,1,1];
  int i, max;

  // inicializar la lista con valores aleatorios
  for (i = 0; i < LIST_SIZE; i++) {
    numbers[i] = rand() %1000;
  }

  // Imprimir todos los elementos de la lista
  for (int i = 0; i < LIST_SIZE; i++) {
    nPrintf("%d ", numbers[i]);
  }

  // buscar el valor máximo en paralelo
  max = find_max_parallel(numbers, LIST_SIZE);

  // imprimir el resultado
  nPrintf("El valor máximo en la lista es: %d\n", max);

  return 0;
}