CC= g++
CFLAGS= -O3 -I ../loadpng -I ../Console  -I ../GUI -I ../Utility -I ../freakazoid -I ../lua51 -I ../http -I ../OmfgScript -I . `allegro-config --cflags` -DALLEGRO_NO_FIX_ALIASES=1
LIBS= -L../Console -L../GUI/detail -lpng -lz `allegro-config --libs` -lzoidcom -lgcon -lfmod \
      -lboost_filesystem-gcc -lboost_signals-gcc -lomfggui --export-dynamic
DEFINES= 
BIN= ../gusanos
OLD_OBJECTS= \
	liero.o \
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
	server.o \
	sfx.o \
	sound.o \
	sprite.o \
	vec.o \
	viewport.o \
	weapon.o \
	weapon_type.o \
	worm.o \
	base_worm.o \
	client.o \
	distortion.o \
	events.o \
	font.o \
	game.o \
	game_actions.o \
	gconsole.o \
	gfx.o \
	gui.o \
	guilist.o \
	gusanos.o \
	keyboard.o \
	keys.o \
	level.o \
	animators.o \
	base_action.o \
	base_animator.o \
	base_object.o \
	base_player.o \
	loaders/gusanos.o \
	loaders/liero.o \
	loaders/lierox.o \
	loaders/losp.o \
	proxy_player.o \
	objects_list.o \
	../loadpng/loadpng.o ../loadpng/regpng.o ../loadpng/savepng.o

OBJECTS= gusanos.o keyboard.o gconsole.o keys.o resource_list.o sprite.o \
	font.o level.o game.o material.o viewport.o base_object.o \
	worm.o part_type.o particle.o base_action.o events.o \
	game_actions.o parser.o sound.o gfx.o sfx.o base_animator.o animators.o distortion.o \
	player.o player_input.o base_player.o player_options.o weapon.o weapon_type.o player_ai.o \
	ninjarope.o gui.o guilist.o server.o client.o network.o base_worm.o net_worm.o liero.o \
	loaders/gusanos.o loaders/liero.o loaders/lierox.o
	

$(BIN): $(OBJECTS)
	$(CC) -o $(BIN) $(OBJECTS) $(CFLAGS) $(LIBS)

.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(DEFINES)

clean:
	rm $(OBJECTS)
