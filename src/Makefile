CC=gcc

CFLAGS= -Wall
CFLAGS_DEBUG= -Wall -Wextra -g

TARGET=trashctl
TARGET_DEBUG=trashctl-dbg

SRC=trashctl.c init.c
OBJ=trashctl.o init.o

.PHONY: clean

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o ../releases/$(TARGET)

$(TARGET_DEBUG):
	$(CC) $(CFLAGS_DEBUG) $(SRC) -o ../releases/$(TARGET_DEBUG)

clean:
	rm -f *.o ../releases/*
