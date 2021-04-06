CC = gcc
CFLAGS = -DGL_GLEXT_PROTOTYPES -O3
INCLUDES = -I./include
BUILD = build
LIB = lib
LDFLAGS = -lSDL2 -lGL -L./$(LIB)
LIBRARY = $(LIB)/libmaprender.so


all: $(LIBRARY)

$(LIBRARY): maprender.c
	@mkdir -p $(LIB) $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -fPIC -c -o $(BUILD)/maprender.o $^
	$(CC) -shared $(BUILD)/maprender.o -o $(LIBRARY) $(LDFLAGS)

interactive: examples/interactive/main.c $(LIBRARY)
	@mkdir -p bin
	$(CC) $(INCLUDES) $(CFLAGS) -o bin/interactive -I./examples/interactive examples/interactive/main.c $(LDFLAGS) -lmaprender

imagewriter: examples/imagewriter/main.c $(LIBRARY)
	@mkdir -p bin
	$(CC) $(INCLUDES) $(CFLAGS) -o bin/imagewriter -I./examples/imagewriter examples/imagewriter/main.c $(LDFLAGS) -lmaprender

examples: interactive imagewriter

clean:
	rm build/*
	rm bin/*
	rm lib/*
