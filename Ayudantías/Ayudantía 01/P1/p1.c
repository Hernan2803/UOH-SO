#include <stdio.h>
#include <stdlib.h>
#include "nSystem.h"

#define NUM_THREADS 4 // número de hilos a utilizar
#define LIST_SIZE 12 // tamaño de la lista

nTask tasks[NUM_THREADS];

// función que cada hilo ejecuta para buscar el valor máximo en su sección de la lista
int find_max_thread(int arr[], int init, int end) {
  
  // implementacion
  int max = arr[init];
  for(int i = init; i<end; i++){
    if(arr[i]>max){
      max=arr[i];
    }
  }
  nExitTask(max);
  return 0;
}

// función que busca el valor máximo en una lista de números utilizando varios hilos
int find_max_parallel(int arr[], int list_size) {
  int sep = list_size/NUM_THREADS;
  nTask task[NUM_THREADS];
  
  for (int i = 0; i<NUM_THREADS; i++){
    int l = i*sep;
    int m = (i+1)*sep;

    task[i] = nEmitTask(find_max_thread, arr, l, m);
  }

  int max = arr[0];
  for (int i = 0; i<NUM_THREADS; i++){
    int local_max = nWaitTask(task[i]);
    if(local_max>max){
      max = local_max;
    }
  }
  
  
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
  for (i = 0; i < LIST_SIZE; i++) {
    printf("%d ", numbers[i]);
  }
  printf("\n");

  // buscar el valor máximo en paralelo
  max = find_max_parallel(numbers, LIST_SIZE);

  // imprimir el resultado
  printf("El valor máximo en la lista es: %d\n", max);

  return 0;
}