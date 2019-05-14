CC= gcc
OBJECTS= \
context.o

CFLAGS= -O3 -I../../freakazoid
LIBRARY= libluaapi-gusanos.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
