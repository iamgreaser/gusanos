CC= g++
CFLAGS= -O3 -I ../loadpng -I ../Console  -I ../GUI -I ../Utility -I ../freakazoid -I ../lua51 -I ../http -I ../OmfgScript -I . `allegro-config --cflags` -DALLEGRO_NO_FIX_ALIASES=1
LIBS= -L../Console -L../GUI/detail -L../GUI/lua -L../http -L../loadpng -L../Utility/util -L../OmfgScript -L../lua51 -L../lua51/luaapi -L../freakazoid -Llua -Lblitters -lpng -lz `allegro-config --libs` -lgcon \
      -lboost_filesystem -lomfgblitters -lomfghttp -lomfgscript -lomfgutil -lluabindings -lomfgguibindings -lomfggui  -lluaapi-gusanos -llua51-gusanos -lfreakazoid -lloadpng -export-dynamic
DEFINES= 
BIN= ../gusanos
OBJECTS= \
2xsai.o \
animators.o \
base_action.o \
base_animator.o \
base_object.o \
base_player.o \
base_worm.o \
client.o \
detect_event.o \
distortion.o \
encoding.o \
events.o \
exp_type.o \
explosion.o \
font.o \
game.o \
game_actions.o \
gconsole.o \
gfx.o \
glua.o \
gusanos.o \
hash_table.o \
keyboard.o \
keys.o \
level.o \
level_effect.o \
loaders/gusanos.o \
loaders/liero.o \
loaders/lierox.o \
loaders/losp.o \
material.o \
menu.o \
mouse.o \
net_worm.o \
network.o \
ninjarope.o \
noise_line.o \
objects_list.o \
parser.o \
part_type.o \
particle.o \
player.o \
player_ai.o \
player_input.o \
player_options.o \
posspd_replicator.o \
proxy_player.o \
resource_base.o \
resource_list.o \
script.o \
server.o \
sfx.o \
simple_particle.o \
sound.o \
sound1d.o \
sprite.o \
sprite_set.o \
stl_str_replicator.o \
timer_event.o \
updater.o \
vector_repliactor.o \
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
