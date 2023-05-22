/*
Del enunciado:
- crear funciones get y put para multiproceso
- no existen herramientas de sincronización mas avanzadas que los spin-locks
- el buffer solo puede almacenar hasta 20 elementos
*/

#define BUFFER_SIZE 20

int ult=0, prim=0, count=0;
Item buf[BUFFER_SIZE];

/*
void put(Item it){
    nWaitSem(vacios);
    buf[ult] =it;
    ult= (ult+1)%20;
    nSignalSem(llenos);
}
*/

void put(Item it){
  spinLock(&lock);
  while(count==BUFFER_SIZE){
    spinUnlock(&lock);
    spinLock(&lock);
  }
  buf[ult] = it;
  ult = (ult+1)%BUFFER_SIZE;
  count++;
  spinUnlock(&lock);
}

/*
Item get(){
    Item it;
    nWaitSem(llenos);
    it = buf[prim];
    prim= (prim+1)%20;
    nsignalSem(vacios);
    return it;
}
*/
Item get(){
  Item it;
  spinLock(&lock);
  while(count==0){
    spinUnlock(&lock);
    spinLock(&lock);
  }
  it = buf[prim];
  prim = (prim+1)%BUFFER_SIZE;
  count--;
  spinUnlock(&lock);
  return it;
}