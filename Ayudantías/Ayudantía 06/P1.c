/*
Del enunciado:
- crear funciones get y put para multiproceso
- no existen herramientas de sincronización mas avanzadas que los spin-locks
- el buffer solo puede almacenar hasta 20 elementos
*/
#define BUFFER_SIZE 20

typedef struct {
  int i;
} Item;

typedef struct {
    Item buffer[BUFFER_SIZE];
    int ult;
    int prim;
    int count;
} Buffer;

Buffer buffer;

void put(Item it){
  spinLock(&lock);
  while (buffer.count==BUFFER_SIZE){
    spinUnlock(&lock);
    spinLock(&lock);
  }
  buffer.buffer[buffer.ult] = it;
  buffer.ult = (buffer.ult+1)%BUFFER_SIZE;
  buffer.count++;
  spinUnlock(&lock);
}

void get(){
  Item it;
  spinLock(&lock);
  while ((buffer.ult - buffer.prim)<0){
    spinUnlock(&lock);
    spinLock(&lock);
  }
  it = buffer.buffer[buffer.prim];
  buffer.prim = (buffer.prim+1)%BUFFER_SIZE;
  buffer.count--;
  spinUnlock(&lock);
}