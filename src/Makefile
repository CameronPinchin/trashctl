CC=gcc

CFLAGS= -Wall
CFLAGS_DEBUG= -Wall -Wextra -g

TARGET=trashctl
TARGET_DEBUG=trashctl-dbg

SRCS=trashctl.c init.c opt_list.c
OBJS=trashctl.o init.o opt_list.o

TARGET_DIR=releases

$(shell mkdir -p ../$(TARGET_DIR))

.PHONY: clean

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o ../releases/$(TARGET)

$(TARGET_DEBUG):
	$(CC) $(CFLAGS_DEBUG) $(SRCS) -o ../releases/$(TARGET_DEBUG)

clean:
	rm -f *.o ../releases/*
