LIBNSYS = $(NSYSTEM)/lib/libnSys.a

CFLAGS= -g -Wall -pedantic -std=c99 -I$(NSYSTEM)/include -I$(NSYSTEM)/src
LFLAGS= -g
LDLIBS= $(LIBNSYS) -pthread

all: rr

rr: $(NSYSTEM)/include/nSystem.h $(NSYSTEM)/include/fifoqueues.h $(LIBNSYS)

clean:
	rm -f *.o *~

cleanall:
	rm -f *.o *~ rr
