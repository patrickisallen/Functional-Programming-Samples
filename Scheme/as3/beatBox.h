// Module which, in real time, generates a drum beat.
// Spawns a background thread to do most of the generating work.
#ifndef BEATBOX_H_
#define BEATBOX_H_

#define BEATBOX_SOUND_BASE  0
#define BEATBOX_SOUND_HIHAT 1
#define BEATBOX_SOUND_SNARE 2
#define BEATBOX_NUMBER_SOUNDS (BEATBOX_SOUND_SNARE + 1)

#define BEATBOX_MODE_NONE 0
#define BEATBOX_MODE_ROCK1 1
#define BEATBOX_MODE_ROCK2 2
#define BEATBOX_NUMBER_MODES (BEATBOX_MODE_ROCK2 + 1)

#define BEATBOX_MIN_TEMPO 40
#define BEATBOX_MAX_TEMPO 300

void BeatBox_init();

void BeatBox_playExtraSound(int soundNumber);

int  BeatBox_getMode();
void BeatBox_setMode(int newMode);

int  BeatBox_getBeatsPerMinute();
void BeatBox_setBeatsPerMinute(int bpm);

void BeatBox_cleanup();

#endif
