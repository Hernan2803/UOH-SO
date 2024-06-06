#include <nSystem.h>

typedef struct {
  int number;
} Item;

void produce(Item *p_it){
  nSleep(rand()%100);
}
void consume(Item *p_it){
  nSleep(rand()%100);
}

int prodProc(nTask dest) {
  for (;;) {
    Item it;
    produce(&it);
    nSend(dest, &it);
    nPrintf("Producto enviado al buffer\n");
  }
}

int consProc() {
  for (;;) {
    nTask t;
    Item *p_it= (Item*)nReceive(&t, -1);
    nPrintf("Mensaje recibido\n");
    nReply(t, 0);
    nPrintf("Mensaje respondido\n");
    consume(p_it);
  }
}

int bufProc(nTask dest) {
  for (;;) {
    nTask t;
    Item it= *(Item*)nReceive(&t, -1);
    nReply(t, 0);
    nSend(dest, &it);
  }
}

int nMain() {
  nTask cons= nEmitTask(consProc);
  nTask buf= nEmitTask(bufProc, cons);
  nTask prod= nEmitTask(prodProc, buf);

  nWaitTask(cons);
  nWaitTask(buf);
  nWaitTask(prod);

  return 0;
}