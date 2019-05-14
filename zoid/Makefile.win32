CC= g++
SOURCES= gusanos.cpp sprites.cpp water.cpp engine.cpp console.cpp \
	text.cpp keys.cpp render.cpp input.cpp sounds.cpp particles.cpp \
	explosions.cpp weapons.cpp level.cpp player.cpp network.cpp \
	lights.cpp gfx.cpp
INCLUDES= engine.h sprites.h water.h console.h text.h keys.h render.h \
	input.h sounds.h particles.h explosions.h weapons.h level.h \
	player.h network.h lights.h gfx.h
CFLAGS= -O3 -mwindows
LIBS= -lfblend -l2xsai -lldpng -lpng -lz -lalleg -lzoidcom_mw
DEFINES= -DAA2XSAI -DWINDOWS -DAAFBLEND
BIN= ../gusanos.exe
OBJECTS= gusanos.o sprites.o water.o engine.o console.o text.o keys.o \
	render.o input.o sounds.o particles.o explosions.o weapons.o \
	level.o player.o network.o lights.o gfx.o


$(BIN): $(OBJECTS)
	$(CC) -o $(BIN) $(OBJECTS) $(CFLAGS) $(LIBS)
 
gusanos.o: gusanos.cpp
	$(CC) -c gusanos.cpp $(CFLAGS) $(DEFINES)

sprites.o: sprites.cpp sprites.h
	$(CC) -c sprites.cpp $(CFLAGS) $(DEFINES)

water.o: water.cpp water.h engine.h
	$(CC) -c water.cpp $(CFLAGS) $(DEFINES)

engine.o: engine.cpp engine.h console.h water.h sprites.h sounds.h \
	particles.h explosions.h
	$(CC) -c engine.cpp $(CFLAGS) $(DEFINES)

console.o: console.cpp console.h
	$(CC) -c console.cpp $(CFLAGS) $(DEFINES)

text.o: text.cpp text.h
	$(CC) -c text.cpp $(CFLAGS) $(DEFINES)

keys.o: keys.cpp keys.h
	$(CC) -c keys.cpp $(CFLAGS) $(DEFINES)

render.o: render.cpp render.h engine.h
	$(CC) -c render.cpp $(CFLAGS) $(DEFINES)

input.o: input.cpp input.h console.h
	$(CC) -c input.cpp $(CFLAGS) $(DEFINES)

sounds.o: sounds.cpp sounds.h 
	$(CC) -c sounds.cpp $(CFLAGS) $(DEFINES)

particles.o: particles.cpp particles.h 
	$(CC) -c particles.cpp $(CFLAGS) $(DEFINES)

explosions.o: explosions.cpp explosions.h 
	$(CC) -c explosions.cpp $(CFLAGS) $(DEFINES)

weapons.o: weapons.cpp weapons.h 
	$(CC) -c weapons.cpp $(CFLAGS) $(DEFINES)

level.o: level.cpp level.h 
	$(CC) -c level.cpp $(CFLAGS) $(DEFINES)

player.o: player.cpp player.h 
	$(CC) -c player.cpp $(CFLAGS) $(DEFINES)

network.o: network.cpp network.h 
	$(CC) -c network.cpp $(CFLAGS) $(DEFINES)

lights.o: lights.cpp lights.h 
	$(CC) -c lights.cpp $(CFLAGS) $(DEFINES)
  
gfx.o: gfx.cpp gfx.h 
	$(CC) -c gfx.cpp $(CFLAGS) $(DEFINES)
