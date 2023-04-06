#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "nSystem.h"

#define NUM_THREADS 4 // número de hilos a utilizar
#define LIST_SIZE 12 // tamaño de la lista

nTask tasks[NUM_THREADS];

// función que cada hilo ejecuta para buscar el valor máximo en su sección de la lista
int find_max_thread(int arr[], int l, int m) {
  int max = arr[0];
  // buscar el valor máximo en la sección de la lista correspondiente al hilo
  for (int i = l; i < m; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }

  // devolver el valor máximo encontrado en la sección correspondiente
  nExitTask(max);
  return 0;
}

// función que busca el valor máximo en una lista de números utilizando varios hilos
int find_max_parallel(int arr[], int list_size) {
  int sep = list_size / NUM_THREADS;
  int max;
  int i;
  printf("%d\n", sep);
  // crear los hilos y dividir la lista en secciones para cada hilo
  for (i = 0; i < NUM_THREADS; i++) {
    int l = i * sep;
    int m = (i+1)*sep;
    if (i == NUM_THREADS - 1) {
      m = list_size;
    }
    tasks[i] = nEmitTask(find_max_thread, arr, l, m);
    
  }

  // esperar a que los hilos terminen y obtener el valor máximo de cada sección
  int max_task;
  for (i = 0; i < NUM_THREADS; i++) {
    max_task = nWaitTask(tasks[i]);
    if (i == 0) {
      max = max_task;
    }
    else {
      if (max_task > max){
        max = max_task;
      }
    }
  }

  // devolver el valor máximo encontrado
  return max;
}


int nMain() {
  int numbers[LIST_SIZE];
  int i, max;

  // inicializar la lista con valores aleatorios
  for (i = 0; i < LIST_SIZE; i++) {
    numbers[i] = rand() %1000;
  }

  // Imprimir todos los elementos de la lista
  for (int i = 0; i < LIST_SIZE; i++) {
    printf("%d ", numbers[i]);
  }
  printf("\n");

  // buscar el valor máximo en paralelo
  max = find_max_parallel(numbers, LIST_SIZE);

  // imprimir el resultado
  printf("El valor máximo en la lista es: %d\n", max);

  return 0;
}