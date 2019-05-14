CC= g++
SOURCES= gusanos.cpp sprites.cpp water.cpp engine.cpp console.cpp \
	text.cpp keys.cpp render.cpp input.cpp sounds.cpp particles.cpp \
	explosions.cpp weapons.cpp level.cpp player.cpp network.cpp \
	lights.cpp
INCLUDES= engine.h sprites.h water.h console.h text.h keys.h render.h \
	input.h sounds.h particles.h explosions.h weapons.h level.h \
	player.h network.h lights.h
CFLAGS= -O2 -fno-rtti #-Wall
LIBS= `allegro-config --libs release` -lzoidcom -I/usr/local/include/zoidcom
BIN= gusanos.exe
OBJECTS= gusanos.o sprites.o water.o engine.o console.o text.o keys.o \
	render.o input.o sounds.o particles.o explosions.o weapons.o \
	level.o player.o network.o lights.o

$(BIN): $(OBJECTS)
	$(CC) -o $(BIN) $(OBJECTS) $(CFLAGS) $(LIBS)
 
gusanos.o: gusanos.cpp
	$(CC) -c gusanos.cpp $(CFLAGS) $(LIBS)

sprites.o: sprites.cpp sprites.h
	$(CC) -c sprites.cpp $(CFLAGS) $(LIBS)

water.o: water.cpp water.h engine.h
	$(CC) -c water.cpp $(CFLAGS) $(LIBS)

engine.o: engine.cpp engine.h console.h water.h sprites.h sounds.h \
	particles.h explosions.h
	$(CC) -c engine.cpp $(CFLAGS) $(LIBS)

console.o: console.cpp console.h
	$(CC) -c console.cpp $(CFLAGS) $(LIBS)

text.o: text.cpp text.h
	$(CC) -c text.cpp $(CFLAGS) $(LIBS)

keys.o: keys.cpp keys.h
	$(CC) -c keys.cpp $(CFLAGS) $(LIBS)

render.o: render.cpp render.h engine.h
	$(CC) -c render.cpp $(CFLAGS) $(LIBS)

input.o: input.cpp input.h console.h
	$(CC) -c input.cpp $(CFLAGS) $(LIBS)

sounds.o: sounds.cpp sounds.h 
	$(CC) -c sounds.cpp $(CFLAGS) $(LIBS)

particles.o: particles.cpp particles.h 
	$(CC) -c particles.cpp $(CFLAGS) $(LIBS)

explosions.o: explosions.cpp explosions.h 
	$(CC) -c explosions.cpp $(CFLAGS) $(LIBS)

weapons.o: weapons.cpp weapons.h 
	$(CC) -c weapons.cpp $(CFLAGS) $(LIBS)

level.o: level.cpp level.h 
	$(CC) -c level.cpp $(CFLAGS) $(LIBS)

player.o: player.cpp player.h 
	$(CC) -c player.cpp $(CFLAGS) $(LIBS)

network.o: network.cpp network.h 
	$(CC) -c network.cpp $(CFLAGS) $(LIBS)

lights.o: lights.cpp lights.h 
	$(CC) -c lights.cpp $(CFLAGS) $(LIBS)
