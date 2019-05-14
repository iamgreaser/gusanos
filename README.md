http://gusanos.sourceforge.net/

----------------------------------------------------------------------------

This is based on what is probably the last commit to the CVS repo ever made. wesz was kind enough to put the codebase on GitHub back in 2014 which is what this forks from: https://github.com/wesz/gusanos

The last time this code was actually touched was probably around about 2006. At the time of writing this section, it is now 2019. This is a C++ codebase. Therefore, this code was guaranteed not to compile.

So we are dealing with a codebase which expects:

* A 2006 C++ codebase
* Some version of SCons
* Allegro 4.2
* Boost 1.32
* FMod
* ZoidCom

Now, blow-by-blow:

* 2006 was before 2011, and some of the code was outright not C++11-compliant.
* SCons 3.0.5 fails with a rather cryptic message pertaining to metaclasses. Kinda figures as SCons 3.0.5 came out in 2019, SCons 1.0.0 came out in 2008, and I'm not sure when Python 3 came out, but I'm surprised it didn't hit a syntax error. Thus, I had to make do with using, modifying, and making more Makefiles.
* Allegro 4.4 is pretty damn near compatible with Allegro 4.2.
* Boost 1.69 was what I had, and I'm surprised that most of the issues were due to a few clashes with stuff promoted to C++11, native paths using `native()` instead of `native_file_string()` and some other pathing stuff, and Boost.Signals actually being Boost.Signals2 now but there wasn't actually that much which had an issue with it.
* I could technically hunt down a version of FMod which worked, but it honestly doesn't look that hard to replace. So I've stubbed it in favour of eventually replacing it with Allegro 4's audio system, or maybe something else which isn't closed-source.
* ZoidCom, on the other hand, absolutely had to go. The website is dead and I'm shocked it doesn't have one of those terrible "What you need, when you want it" pages (you just get 403 Forbidden everywhere instead). I had to stub the whole thing out and I'm likely to end up replacing it, although I may have to skip on ENet as I need IPv6 support.

I spent about 5 hours adding Makefiles, making the code actually work with Boost 1.69 and at the very least C++11 (using GCC 8.2.0), and stubbing out FMod and ZoidCom. Once it finally linked, I threw in the files from the Windows version and I was actually shocked that the damn thing Just Worked™.

Anyway, original readme is as follows...

----------------------------------------------------------------------------

GUSANOS is an open-source clone of the DOS game Liero. It currently works on Windows and Linux.

In the game each player controls a small worm armed to the teeth and fights other worms in a destructible battleground. It can be described as "Real-time Worms" or "2D Quake", or a combination of both.

The gameplay is as close as possible to Liero's. Hardcore Liero players will feel at home. Newbies, on the other hand, may find the controls hard to dominate at first, but not less enjoyable. Practice makes perfect.

GUSANOS improves greatly on what the original Liero left, adding modern graphics, audio and networking capabilites along with a myriad of other features.
