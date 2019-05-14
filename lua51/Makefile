CC= gcc
OBJECTS= \
lapi.o \
lauxlib.o \
lbaselib.o \
lcode.o \
ldblib.o \
ldebug.o \
ldo.o \
ldump.o \
lfunc.o \
lgc.o \
linit.o \
liolib.o \
llex.o \
lmathlib.o \
lmem.o \
loadlib.o \
lobject.o \
lopcodes.o \
loslib.o \
lparser.o \
lstate.o \
lstring.o \
lstrlib.o \
ltable.o \
ltablib.o \
ltm.o \
lundump.o \
lvm.o \
lzio.o \
print.o

CFLAGS= -O3
LIBRARY= liblua51-gusanos.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
