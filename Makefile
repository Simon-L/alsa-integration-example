SOURCES=heavy/SignalPhasor.c heavy/MessageQueue.c heavy/HvTable.c heavy/HvLightPipe.c heavy/HvBase.c heavy/Heavy.c heavy/SignalVar.c heavy/MessagePool.c heavy/HvMessage.c heavy/HvContext_alsa_heavy1.c alsa_heavy1.c
OBJECTS=$(SOURCES:.c=.o)
MAIN=alsaheavy1
CC=gcc-5
CFLAGS=-I./heavy/
# LDFLAGS=-lrdmacm -libverbs -lpthread

all: $(MAIN)

$(MAIN): $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(MAIN)
