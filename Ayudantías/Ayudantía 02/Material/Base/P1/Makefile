NSYSTEM=/home/hernan/nsystem

LIBNSYS = $(NSYSTEM)/lib/libnSys.a

CFLAGS= -g -Wall -pedantic -std=c99 -I$(NSYSTEM)/include -I$(NSYSTEM)/src
LFLAGS= -g
LDLIBS= $(LIBNSYS) -pthread

all: p1

p1: $(NSYSTEM)/include/nSystem.h $(NSYSTEM)/include/fifoqueues.h $(LIBNSYS)

clean:
	rm -f *.o *~

cleanall:
	rm -f *.o *~ p1
