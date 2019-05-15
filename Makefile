CC= gcc
CXX= g++
CFLAGS= -O3 -I ./loadpng -I ./Console -I ./GUI -I ./Goop -I ./Utility -I ./freakazoid -I ./lua51 -I ./http -I ./OmfgScript `allegro-config --cflags` -DALLEGRO_NO_FIX_ALIASES=1
LIBS= -lvorbisfile -lpng -lz -lboost_filesystem `allegro-config --libs` -export-dynamic
DEFINES= 
BIN= ./gusanos
OBJECTS=
include Console/objects.make
include freakazoid/objects.make
include Goop/objects.make
include GUI/objects.make
include http/objects.make
include loadpng/objects.make
include lua51/objects.make
include OmfgScript/objects.make
include Utility/objects.make

$(BIN): $(OBJECTS)
	$(CXX) -o $(BIN) $(OBJECTS) $(CFLAGS) $(LIBS)

.c.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(DEFINES)

.cpp.o:
	$(CXX) -c $< -o $@ $(CFLAGS) $(DEFINES)

clean:
	rm -f $(OBJECTS)
