CC= g++
OBJECTS= \
omfg_script.o \

CFLAGS= -O3 -I ../Utility `allegro-config --cflags` -DALLEGRO_NO_FIX_ALIASES=1
LIBRARY= libomfgscript.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
