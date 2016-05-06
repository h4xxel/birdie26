#include "soundeffects.h"
#include "main.h"

void soundeffects_init() {
	s->sfx.sound[SOUND_UPROOT] = d_sound_streamed_load("res/sfx/uproot.ogg", DARNIT_SOUND_PRELOAD, 1);
	s->sfx.sound[SOUND_HIT] = d_sound_streamed_load("res/sfx/hit.ogg", DARNIT_SOUND_PRELOAD, 1);
	s->sfx.sound[SOUND_CRUNCH] = d_sound_streamed_load("res/sfx/crunch.ogg", DARNIT_SOUND_PRELOAD, 1);
	s->sfx.sound[SOUND_SHITLER_WIN] = d_sound_streamed_load("res/sfx/shitler_win.ogg", DARNIT_SOUND_PRELOAD, 1);
	s->sfx.sound[SOUND_RUMP_WIN] = d_sound_streamed_load("res/sfx/rump_win.ogg", DARNIT_SOUND_PRELOAD, 1);
	s->sfx.sound[SOUND_DIM_WIN] = d_sound_streamed_load("res/sfx/dim_win.ogg", DARNIT_SOUND_PRELOAD, 1);
	s->sfx.sound[SOUND_STAHLIN_WIN] = d_sound_streamed_load("res/sfx/stahlin_win.ogg", DARNIT_SOUND_PRELOAD, 1);
}


void soundeffects_play(enum SoundeffectSound sound) {
	if (sound <= SOUND_UPROOT)
		d_sound_play(s->sfx.sound[sound], -1, 100, 100, 0);
	else
		d_sound_play(s->sfx.sound[sound], -1, 50, 50, 0);
	return;
}
