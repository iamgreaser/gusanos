/*

Temporary stubs for FMod while I work out how to make this game use Allegro's sound system instead

Honestly, while this library is not really what I would call "dead", it does not play nice with my system. Instead, it thinks it knows better than my asoundrc.

Therefore, I have to hack ALSA to deliberately force this out of having any choice over what device to use.

WE DON'T REALLY USE OPEN SOUND SYSTEM THESE DAYS SO STOP GRABBING HARDWARE DEVICES YOU STUPID DUMPASS.

Although to be fair, at least this code existed when ALSA did, ESounD was kind of a thing, and PulseAudio was possibly not even a mere fart out of Lennart's arse. So as far as sound on Linux was concerned, this was the era where your sound actually worked.

--GM
*/

//
// And here's the stubs. Enjoy.
//

#define FMODSTUB(x) (x)

#define FSOUND_GetFrequency() FMODSTUB(44100)
#define FSOUND_GetVolume() FMODSTUB(256)
#define FSOUND_IsPlaying(chan) FMODSTUB(false)
#define FSOUND_SetDriver(driver) FMODSTUB({})
#define FSOUND_SetFrequency(chan, freq) FMODSTUB({})
#define FSOUND_SetOutput(outputmode) FMODSTUB({})
#define FSOUND_SetVolume(chan, vol) FMODSTUB({})

#define FSOUND_LOOP_OFF 0

#define FSOUND_FREE 0

