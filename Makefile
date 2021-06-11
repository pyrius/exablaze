PREFIX=/usr/local
CC=gcc
CFLAGS=-O3 -Wall -I../../libs
LDLIBS=-L../../libs/exanic -lexanic -lpthread

BIN=sendp recp senda sendap

all: $(BIN) $(TEST_BIN)

sendp: sendp.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

recp: recp.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

senda: senda.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

sendap: sendap.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f $(BIN)
