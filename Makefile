SOURCES=heavy/SignalPhasor.c heavy/MessageQueue.c heavy/HvTable.c heavy/HvLightPipe.c heavy/HvBase.c heavy/Heavy.c heavy/SignalVar.c heavy/MessagePool.c heavy/HvMessage.c heavy/HvContext_alsa_heavy1.c alsa_heavy.c
OBJECTS=$(SOURCES:.c=.o)
MAIN=alsa_heavy
CC=gcc-5
LDFLAGS=-lm -g -lasound -std=c11

all: $(MAIN)

$(MAIN): $(OBJECTS)
	$(CC) -o $@ $^ ${LDFLAGS}

clean:
	rm -f $(OBJECTS) $(MAIN)
