CC = gcc
CFLAGS = -Wall -Wextra 
LIBS   = 
SOURCE = sims.c
TARGET = build/sims
HEADER = include/cx.h

all: $(TARGET)

CLANG ?= n
GDB ?= n
RUN_CMD ?= ./$(TARGET)

ifeq ($(GDB), y)
    CFLAGS += -ggdb
	RUN_CMD = gdb ./$(TARGET)	
endif

ifeq ($(CLANG),y)
	CC = clang
	CFLAGS += -fsanitize=address
endif

$(TARGET): $(SOURCE) $(HEADER)
	$(CC)  $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm -f $(TARGET)

.PHONY: run all clean

run: $(TARGET)
	$(RUN_CMD)
