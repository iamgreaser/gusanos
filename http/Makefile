CC= g++
OBJECTS= \
http.o \
sockets.o \
tcp.o \

CFLAGS= -O3 -I ../Utility -I ../freakazoid
LIBRARY= libomfghttp.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
