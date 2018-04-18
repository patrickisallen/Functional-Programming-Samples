#include "beatBox.h"

#include <alsa/asoundlib.h>
#include "audioMixer.h"

#include <stdbool.h>
#include <pthread.h>
#include "general.h"


static char* soundFiles[] = {
		"beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav",
		"beatbox-wav-files/100053__menegass__gui-drum-cc.wav",
		"beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav",
};
//		"beatbox-wav-files/100060__menegass__gui-drum-splash-hard.wav",

static wavedata_t sounds[BEATBOX_NUMBER_SOUNDS];

static _Bool stopping = false;
static pthread_t beatThreadId = 0;
static void *beatThread(void* arg);

#define STARTING_BPM 120
int beatsPerMinute = STARTING_BPM;

static int mode = BEATBOX_MODE_ROCK1;

#define MSEC_PER_SEC   1000
#define USEC_PER_MSEC  1000
#define SEC_PER_MINUTE 60
#define TICKS_PER_BEAT 2


void BeatBox_init()
{
	// Load wave file we want to play:
	for (int i = 0; i < BEATBOX_NUMBER_SOUNDS; i++) {
		AudioMixer_readWaveFileIntoMemory(soundFiles[i], &sounds[i]);
	}

	// Configure Output Device
	AudioMixer_init();

	// Start threads:
	pthread_create(&beatThreadId, NULL, beatThread, NULL);
}

void BeatBox_playExtraSound(int soundNumber)
{
	assert(soundNumber >= 0 && soundNumber < BEATBOX_NUMBER_SOUNDS);

	AudioMixer_queueSound(&sounds[soundNumber]);
}

int BeatBox_getMode()
{
	return mode;
}
void BeatBox_setMode(int newMode)
{
	if (newMode >= 0 && newMode < BEATBOX_NUMBER_MODES) {
		mode = newMode;
	} else {
		printf("ERROR: Invalid beatbox mode (%d)\n", newMode);
	}
}

int BeatBox_getBeatsPerMinute()
{
	return beatsPerMinute;
}
void BeatBox_setBeatsPerMinute(int bpm)
{
	if (bpm >= 0 && bpm <= BEATBOX_MAX_TEMPO) {
		beatsPerMinute = bpm;
	}
}


void BeatBox_cleanup()
{
	stopping = true;
	pthread_join(beatThreadId, NULL);

	AudioMixer_cleanup();

	for (int i = 0; i < BEATBOX_NUMBER_SOUNDS; i++) {
		AudioMixer_freeWaveFileData(&sounds[i]);
	}
}



static void *beatThread(void* arg)
{
	// Generate the beat
	for (int beat = 0; !stopping; beat++) {
//		printf("loop %d\n", beat);
		fflush(stdout);

		if (mode == BEATBOX_MODE_ROCK1) {
			if (beat % 1 == 0) {
				BeatBox_playExtraSound(BEATBOX_SOUND_HIHAT);
			}
			if (beat % 4 == 0) {
				BeatBox_playExtraSound(BEATBOX_SOUND_BASE);
			}
			if (beat % 4 == 2) {
				BeatBox_playExtraSound(BEATBOX_SOUND_SNARE);
			}
		}

		if (mode == BEATBOX_MODE_ROCK2) {
			if (beat % 1 == 0) {
				BeatBox_playExtraSound(BEATBOX_SOUND_HIHAT);
			}
			if (beat % 4 == 3) {
				BeatBox_playExtraSound(BEATBOX_SOUND_BASE);
			}
			if ((1 << beat % 8) & (0x1B)) {
				BeatBox_playExtraSound(BEATBOX_SOUND_SNARE);
			}
		}

		long mSecDelay = (MSEC_PER_SEC * SEC_PER_MINUTE)
				/ (beatsPerMinute * TICKS_PER_BEAT);
		sleep_usec(mSecDelay * USEC_PER_MSEC);
	}

	return NULL;
}


















