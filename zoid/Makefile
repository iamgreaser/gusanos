CC= g++
CCC=gcc
SOURCES= gusanos.cpp sprites.cpp water.cpp engine.cpp console.cpp \
	text.cpp keys.cpp render.cpp input.cpp sounds.cpp particles.cpp \
	explosions.cpp weapons.cpp level.cpp player.cpp network.cpp \
	lights.cpp loadpng.c regpng.c savepng.c gfx.cpp
INCLUDES= engine.h sprites.h water.h console.h text.h keys.h render.h \
	input.h sounds.h particles.h explosions.h weapons.h level.h \
	player.h network.h lights.h
CFLAGS= -fno-rtti -O2 -I/usr/local/include/zoidcom -L/usr/local/lib/zoidcom# -Wall
LIBS=  `allegro-config --libs` -lzoidcom -lpng
DEFINES= 
BIN= ../gusanos.exe
OBJECTS= gusanos.o sprites.o water.o engine.o console.o text.o keys.o \
	render.o input.o sounds.o particles.o explosions.o weapons.o \
	level.o player.o network.o lights.o loadpng.o regpng.o savepng.o gfx.o

$(BIN): $(OBJECTS)
	$(CC) -o $(BIN) $(OBJECTS) $(CFLAGS) $(LIBS)
 
.cpp.o:
	$(CC) -c $< $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
