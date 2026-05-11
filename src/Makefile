CC=gcc

CFLAGS= -Wall
CFLAGS_DEBUG= -Wall -Wextra -g

TARGET=safe-dd
TARGET_DEBUG=safe-dd-dbg

SRC=safe_dd.c
OBJ=safe_dd.o

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o ../releases/$(TARGET)

$(TARGET_DEBUG):
	$(CC) $(CFLAGS_DEBUG) $(SRC) -o ../releases/$(TARGET_DEBUG)
