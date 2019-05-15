vim: set sts=2 sw=2 et :

Important stuff:

* [ ] Fix the build system
  * [x] Switch to single Makefile w/ no `*.a` files lying around
  * [ ] Then switch it over to something else, possibly CMake

* [ ] Fix some of the old code
  * [ ] Replace all `auto_ptr` things with `unique_ptr` or `shared_ptr` or whatever you use in each situation
  * [ ] Replace some Boost stuff with C++11 equivalents
    * [ ] ???

* [ ] Get audio back up to speed
  * [ ] Get Ogg Vorbis loading working
  * [ ] Get panning with multiple listeners working

* [ ] Get network back up to speed
  * [ ] ???

Some ideas:

* [ ] Switch audio over to OpenAL
* [ ] Consider one of the ENet IPv6 forks?
* [ ] Use SDL2 instead of Allegro 4 as it's more common these days
  * If this is done and we're still using Allegro audio, we'd need to switch to SDL\_Mixer OR implement the whole mixing stuff ourselves. I'd rather go with OpenAL to be honest, as Gusanos tends to take advantage of 3D audio.

