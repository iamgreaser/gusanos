CC= g++
CFLAGS= -O3 -I ../loadpng -I ../Console  -I ../GUI -I ../Utility -I ../freakazoid -I ../lua51 -I ../http -I ../OmfgScript -I . `allegro-config --cflags` -DALLEGRO_NO_FIX_ALIASES=1
LIBS= -L../Console -L../GUI/detail -L../GUI/lua -L../http -L../Utility/util -L../OmfgScript -L../lua51 -L../lua51/luaapi -Llua -Lblitters -lpng -lz `allegro-config --libs` -lgcon \
      -lboost_filesystem -lomfgblitters -lomfggui -lomfgguibindings -lomfghttp -lomfgscript -lomfgutil -lluaapi-gusanos -lluabindings -llua51-gusanos -export-dynamic
DEFINES= 
BIN= ../gusanos
OBJECTS= \
animators.o \
base_action.o \
base_animator.o \
base_object.o \
base_player.o \
base_worm.o \
client.o \
distortion.o \
events.o \
font.o \
game.o \
game_actions.o \
gconsole.o \
gfx.o \
glua.o \
gusanos.o \
keyboard.o \
keys.o \
level.o \
loaders/gusanos.o \
loaders/liero.o \
loaders/lierox.o \
material.o \
menu.o \
net_worm.o \
network.o \
ninjarope.o \
parser.o \
part_type.o \
particle.o \
player.o \
player_ai.o \
player_input.o \
player_options.o \
resource_list.o \
script.o \
server.o \
sfx.o \
sound.o \
sprite.o \
sprite_set.o \
viewport.o \
weapon.o \
weapon_type.o \
worm.o

$(BIN): $(OBJECTS)
	$(CC) -o $(BIN) $(OBJECTS) $(CFLAGS) $(LIBS)

.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
