CC=gcc

CFLAGS= -Wall
CFLAGS_DEBUG= -Wall -Wextra -g

TARGET=trashctl
TARGET_DEBUG=trashctl-dbg

SRC=trashctl.c
OBJ=trashctl.o

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o ../releases/$(TARGET)

$(TARGET_DEBUG):
	$(CC) $(CFLAGS_DEBUG) $(SRC) -o ../releases/$(TARGET_DEBUG)
