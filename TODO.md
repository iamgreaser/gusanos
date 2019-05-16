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
  * [x] Get Ogg Vorbis loading working
  * [ ] Get panning with multiple listeners working

* [ ] Get network back up to speed
  * [ ] Split freakazoid up into multiple cpp files
  * [ ] ???

Some ideas:

* [ ] Switch audio over to OpenAL
* [ ] Consider one of the ENet IPv6 forks?
* [ ] Use SDL2 instead of Allegro 4 as it's more common these days
  * If this is done and we're still using Allegro audio, we'd need to switch to SDL\_Mixer OR implement the whole mixing stuff ourselves. I'd rather go with OpenAL to be honest, as Gusanos tends to take advantage of 3D audio.

Ideas for the next version:

* [ ] Move syntax to Lisp (it's easier to parse so we can cut our codebase back a bit, and we could potentially be able to capture state better than we can with Lua) and add a few converters written in Python
  * [ ] GSS:
    * [ ] Interpreter
    * [ ] Converter
  * [ ] OmfgScript:
    * [ ] Interpreter
    * [ ] Converter
  * [ ] Lua:
    * [ ] Interpreter
    * [ ] Somehow convert what's there
      * Lua 5.1's syntax may fit onto a single A4 page, but it's not actually that easy to parse.

