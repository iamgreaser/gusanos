CC= g++
CFLAGS= -O3
LIBRARY= libfreakazoid.a
OBJECTS= \
zoidcom.o

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
