CC= gcc
OBJECTS= \
loadpng.o \
regpng.o \
savepng.o

CFLAGS= -O3
LIBRARY= libloadpng.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
