#ifndef _SOUNDEFFECT_H__
#define	_SOUNDEFFECT_H__


#include <darnit/darnit.h>

enum SoundeffectSound {
	SOUND_RUMP_WIN,
	SOUND_DIM_WIN,
	SOUND_SHITLER_WIN,
	SOUND_STAHLIN_WIN,
	SOUND_UPROOT,
	SOUND_HIT,
	SOUND_CRUNCH,
	SOUND_SOUNDS
};

struct SoundeffectStruct {
	DARNIT_SOUND		*sound[SOUND_SOUNDS];
};

void soundeffects_init();
void soundeffects_play(enum SoundeffectSound sound);


#endif
