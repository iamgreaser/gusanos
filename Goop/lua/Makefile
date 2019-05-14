CC= gcc
OBJECTS= \
bindings-game.o \
bindings-gfx.o \
bindings-math.o \
bindings-network.o \
bindings-objects.o \
bindings-resources.o \
bindings.o

CFLAGS= -O3 -I../../lua51 -I../../Utility -I../../Console -I../../GUI -I../../freakazoid -I../../http `allegro-config --cflags` -DALLEGRO_NO_FIX_ALIASES=1
LIBRARY= libluabindings.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
