CC= g++
OBJECTS= \
button.o \
check.o \
context.o \
edit.o \
group.o \
gss.o \
label.o \
list.o \
renderer.o \
wnd.o \
xml.o \

CFLAGS= -O3 -I../../Utility -I../../freakazoid -I../../lua51 -DALLEGRO_NO_FIX_ALIASES=1 `allegro-config --cflags`
LIBRARY= libomfggui.a

$(LIBRARY): $(OBJECTS)
	ar rs $@ $^

.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
