CC = gcc
CFLAGS = -DGL_GLEXT_PROTOTYPES -O3
INCLUDES = -I./include
BUILD = build
LIB = lib
LDFLAGS = -lSDL2 -lGL -L./$(LIB)
OUTPUT = $(LIB)/libmaprender.so


all: maprender

maprender: maprender.c
	mkdir -p $(LIB) $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -fPIC -c -o $(BUILD)/$@.o $^
	$(CC) -shared $(BUILD)/$@.o -o $(LIB)/lib$@.so $(LDFLAGS)

example: example/main.c $(OUTPUT)
	mkdir -p bin
	$(CC) $(INCLUDES) -o bin/map -I./example example/main.c $(LDFLAGS) -lmaprender

clean:
	rm build/*
	rm bin/*
	rm lib/*
