CC= g++
OBJECTS= \
bindings-gui.o

CFLAGS= -O3 -I.. -I../../Utility -I../../freakazoid -I../../lua51 -DALLEGRO_NO_FIX_ALIASES=1 `allegro-config --cflags`
LIBRARY= libomfgguibindings.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
