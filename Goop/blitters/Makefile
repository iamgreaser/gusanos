CC= g++
OBJECTS= \
add.o \
add_simd.o \
blend.o \
blend_simd.o \
mult.o \
mult_simd.o


CFLAGS= -O3 -I../../Utility -DALLEGRO_NO_FIX_ALIASES=1 `allegro-config --cflags`
LIBRARY= libomfgblitters.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
