/*

Temporary stubs for FMod while I work out how to make this game use Allegro's sound system instead

Honestly, while this library is not really what I would call "dead", it does not play nice with my system. Instead, it thinks it knows better than my asoundrc.

Therefore, I have to hack ALSA to deliberately force this out of having any choice over what device to use.

WE DON'T REALLY USE OPEN SOUND SYSTEM THESE DAYS SO STOP GRABBING HARDWARE DEVICES YOU STUPID DUMPASS.

Although to be fair, at least this code existed when ALSA did, ESounD was kind of a thing, and PulseAudio was possibly not even a mere fart out of Lennart's arse. So as far as sound on Linux was concerned, this was the era where your sound actually worked.

--GM
*/
// And now let's rate these while I'm here.

// Underrated. Best kind of audio output when you're playing ads.
#define FSOUND_OUTPUT_NOSOUND 0

// The bronze standard of audio. A bit hard to get working at first, but I've never known a program that uses it with stuttery audio.
#define FSOUND_OUTPUT_WINMM 1

// Apparently this is what you're supposed to use. Or what you *were* supposed to use and then WASAPI came along. I've not looked into this.
#define FSOUND_OUTPUT_DSOUND 2

// No idea what this is.
#define FSOUND_OUTPUT_A3D 3

// Underrated, but kinda got dropped in favour of The Second System.
#define FSOUND_OUTPUT_OSS 4

// ...I don't recall if there was ever any good documentation for this thing. At least it actually worked. I'd actually be tempted to dust this off and try a push to replace PulseAudio with it. Maybe rebranding it would work?
#define FSOUND_OUTPUT_ESD 5

// This is kinda what we have now.
#define FSOUND_OUTPUT_ALSA 6

// A bit of a meme if you ask me.
#define FSOUND_OUTPUT_ASIO 7

// Not the first "bullshit console" (locked-down computer), but it fits the bill.
#define FSOUND_OUTPUT_XBOX 8

// I can get stuff onto the screen in an emulator on this thing, but trying to get the IOP to behave is a black art I have not been blessed with. As for the PS1, I've had way more success with sound on that thing.
#define FSOUND_OUTPUT_PS2 9

// I'm not sure if this is before or after CoreAudio was a thing. I've never looked into CoreAudio to be honest.
#define FSOUND_OUTPUT_MAC 10

// Why is there no good documentation for this console?!?!
#define FSOUND_OUTPUT_GC 11

//
// And here's the stubs. Enjoy.
//

#define FSOUND_Sample_Free(ptr) free(ptr)

#define FMODSTUB(x) (x)

#define FSOUND_3D_SetAttributes(chan, pos, attrs) FMODSTUB({})
#define FSOUND_3D_SetDistanceFactor(unk001) FMODSTUB({})
#define FSOUND_3D_SetMinMaxDistance(chan, mindist, maxdist) FMODSTUB({})
#define FSOUND_3D_SetRolloffFactor(unk001) FMODSTUB({})
#define FSOUND_3D_Listener_SetAttributes(pos, unk002, unk003, unk004, unk005, unk006, unk007, unk008) FMODSTUB({})
#define FSOUND_3D_Listener_SetCurrent(unk001, unk002) FMODSTUB({})
#define FSOUND_Close() FMODSTUB({})
#define FSOUND_GetDriver() FMODSTUB(FSOUND_OUTPUT_ESD)
#define FSOUND_GetDriverName(driver) FMODSTUB("Enlightened Sound Daemon")
#define FSOUND_GetFrequency() FMODSTUB(44100)
#define FSOUND_GetVolume() FMODSTUB(256)
#define FSOUND_Init(freq, unk002, unk003) FMODSTUB({})
#define FSOUND_IsPlaying(chan) FMODSTUB(false)
#define FSOUND_PlaySoundEx(unk001, sound, unk003, unk004) FMODSTUB(1)
#define FSOUND_Sample_Load(unk001, fname, flags, unk004, unk005) FMODSTUB((FSOUND_SAMPLE *)malloc(1))
#define FSOUND_SetDriver(driver) FMODSTUB({})
#define FSOUND_SetFrequency(chan, freq) FMODSTUB({})
#define FSOUND_SetLoopMode(chan, loopmode) FMODSTUB({})
#define FSOUND_SetOutput(outputmode) FMODSTUB({})
#define FSOUND_SetPaused(chan, ispaused) FMODSTUB({})
#define FSOUND_SetSFXMasterVolume(volume) FMODSTUB({})
#define FSOUND_SetVolume(chan, vol) FMODSTUB({})
#define FSOUND_Update() FMODSTUB({})

#define FSOUND_LOOP_OFF 0

#define FSOUND_FREE 0

