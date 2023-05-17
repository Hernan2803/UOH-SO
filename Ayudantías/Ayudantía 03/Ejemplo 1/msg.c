#include <nSystem.h>

typedef struct {
  int number;
} Item;

void produce(Item *p_it){
  nSleep(rand()%100);
}
void consume(Item *p_it){
  nSleep(rand()%10);
}

int prodProc(nTask dest) {
  for (;;) {
    Item it;
    produce(&it);
    nSend(dest, &it);
    nPrintf("Producto enviado\n");
  }
}

int consProc() {
  for (;;) {
    nTask t;
    Item *p_it= (Item*)nReceive(&t, -1);
    nPrintf("Mensaje recivido\n");
    consume(p_it);
    nReply(t, 0);
    nPrintf("Mensaje respondido\n");
  }
}

int bufProc(nTask dest) {
  for (;;) {
    nTask t;
    Item it= *(Item*)nReceive(&t, -1);//&t para guardar el pid del proceso que envio
    nReply(t, 0); /* se puede porque el item quedo almacenado localmente */
    nSend(dest, &it);
  }
}

int nMain() {
  nTask cons= nEmitTask(consProc);
  nTask buf= nEmitTask(bufProc, cons);
  nTask prod= nEmitTask(prodProc, buf);
  
  nWaitTask(cons); /* Nunca termina: solo para evitar que nMain termine */
  nWaitTask(buf);
  nWaitTask(prod);

  return 0;
}