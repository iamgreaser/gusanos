CC= g++

OBJECTS= \
angle.o \
log.o \
math_func.o \
text.o \
vec.o

CFLAGS= -O3 -I../../freakazoid -I..
LIBRARY= libomfgutil.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
