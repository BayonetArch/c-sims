CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIBS = `pkg-config --libs sdl3-ttf`

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,build/%.o,$(SOURCES))
EXTHEADERS = $(wildcard src/*.h ext/include/*.h)

TARGET = build/sims

# Optional flags
GDB ?= n
CLANG ?= n

ifeq ($(GDB),y)
    CFLAGS += -ggdb
endif

ifeq ($(CLANG),y)
    CC = clang
    CFLAGS += -fsanitize=address
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@

build/%.o: src/%.c $(HEADERS) | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -f $(TARGET) $(OBJECTS)

run: $(TARGET)
ifeq ($(GDB),y)
	gdb ./$(TARGET)
else
	./$(TARGET)
endif

.PHONY: all clean run
