#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_NUM 3

typedef struct {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
} customSemStruct;

typedef customSemStruct* customSem;

customSem customMakeSem(int count) {
    customSem sem = (customSem)malloc(sizeof(customSemStruct));
    sem->count = count;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->condition, NULL);
    return sem;
}

void customWaitSem(customSem sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->count <= 0) {
        pthread_cond_wait(&sem->condition, &sem->mutex);
    }
    sem->count--;
    pthread_mutex_unlock(&sem->mutex);
}

void customSignalSem(customSem sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->count++;
    pthread_cond_signal(&sem->condition);
    pthread_mutex_unlock(&sem->mutex);
}

void customDestroySem(customSem sem) {
    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->condition);
    free(sem);
}

customSem semaforo;

// Función de hilo para probar las operaciones de espera y señalización del semáforo
void *thread_function(void *arg) {
    int thread_id = *((int *)arg);
    printf("Thread %d: Esperando el semáforo\n", thread_id);
    // Espera aleatoria para simular concurrencia
    sleep(rand() % 3); 
    customWaitSem(semaforo);
    printf("Thread %d: Semáforo adquirido\n", thread_id);

    // Simular trabajo dentro de la sección crítica
    printf("Thread %d: Dentro de la sección crítica\n", thread_id);
    sleep(2); // Simular trabajo durante 2 segundos
    printf("Thread %d: Saliendo de la sección crítica\n", thread_id);

    customSignalSem(semaforo);
    printf("Thread %d: Semáforo liberado\n", thread_id);


    //Mismo hilo pide semaforo nuevamente
    printf("Thread %d: Esperando el semáforo\n", thread_id);
    // Espera aleatoria para simular concurrencia
    sleep(rand() % 3); 
    customWaitSem(semaforo);
    printf("Thread %d: Semáforo adquirido\n", thread_id);

    // Simular trabajo dentro de la sección crítica
    printf("Thread %d: Dentro de la sección crítica\n", thread_id);
    sleep(2); // Simular trabajo durante 2 segundos
    printf("Thread %d: Saliendo de la sección crítica\n", thread_id);

    customSignalSem(semaforo);
    printf("Thread %d: Semáforo liberado\n", thread_id);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_NUM]; // Modificar a más hilos si se desea
    int thread_ids[THREAD_NUM]; // Modificar a más hilos si se desea
    int i;

    // Crear un semáforo con un contador inicial de 1
    semaforo = customMakeSem(2);

    // Crear múltiples hilos
    for (i = 0; i < THREAD_NUM; ++i) { // Modificar a más hilos si se desea
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            fprintf(stderr, "Error al crear hilo %d\n", i);
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    for (i = 0; i < THREAD_NUM; ++i) { // Modificar a más hilos si se desea
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error al unir el hilo %d\n", i);
            return 1;
        }
    }

    // Destruir el semáforo
    customDestroySem(semaforo);

    return 0;
}