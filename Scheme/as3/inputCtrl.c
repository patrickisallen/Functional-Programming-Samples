#include "inputCtrl.h"
#include "beatBox.h"
#include <stdbool.h>
#include <pthread.h>
#include "accel_drv.h"
#include <math.h>
#include "general.h"
#include "joystick_drv.h"
#include "audioMixer.h"


static void *inputPollingThread(void* arg);
static pthread_t threadId = 0;
static _Bool stopping = false;

#define RESETTIME_SHORT 10
#define RESETTIME_LONG 100
#define ACCEL_THRESHOLD_SMALL 1.0
#define ACCEL_THRESHOLD_LARGE 1.9
#define VOLUME_STEP_SIZE 5
#define TEMPO_STEP_SIZE  5


void InputCtrl_init()
{
	// Configure accelerometer
	AccelDrv_init();
	Joystick_init();

	// Start threads:
	pthread_create(&threadId, NULL, inputPollingThread, NULL);

}

void InputCtrl_cleanup()
{
	stopping = true;
	pthread_join(threadId, NULL);

	AccelDrv_cleanup();
	Joystick_cleanup();
}


static _Bool debounce(_Bool condition, int *cooldown, int resetTime)
{
	if (*cooldown > 0) {
		*cooldown = *cooldown - 1;
	}

	if (condition && *cooldown == 0) {
		*cooldown = resetTime;
		return true;
	} else {
		return false;
	}
}
static void *inputPollingThread(void* arg)
{
	int debounceX = 0;
	int debounceY = 0;
	int debounceZ = 0;

	int debounceUp = 0;
	int debounceDown = 0;
	int debouncePress = 0;
	int debounceLeft = 0;
	int debounceRight = 0;

	while(!stopping) {
		// Accelerometer to play sound.
		double dx, dy, dz;
		AccelDrv_getReading(&dx, &dy, &dz);
		// Display for debugging:
		if (fabs(dx) > ACCEL_THRESHOLD_SMALL
				|| fabs(dy) > ACCEL_THRESHOLD_SMALL
				|| fabs(dz>ACCEL_THRESHOLD_LARGE)) {
			printf("     Accelerometer Data:   x=%+.2f  y=%+.2f  z=%+.2f\n", dx, dy, dz);
		}

		// Play audio:
		if (debounce((fabs(dx) > ACCEL_THRESHOLD_SMALL), &debounceX, RESETTIME_SHORT)) {
			BeatBox_playExtraSound(BEATBOX_SOUND_SNARE);
		}
		if (debounce((fabs(dy) > ACCEL_THRESHOLD_SMALL), &debounceY, RESETTIME_SHORT)) {
			BeatBox_playExtraSound(BEATBOX_SOUND_HIHAT);
		}
		if (debounce((fabs(dz) > ACCEL_THRESHOLD_LARGE), &debounceZ, RESETTIME_SHORT)) {
			BeatBox_playExtraSound(BEATBOX_SOUND_BASE);
		}


		// Handle Joystick
		uint32_t joystick = Joystick_readState();
		if (debounce((joystick & JOYSTICK_UP), &debounceUp, RESETTIME_SHORT)) {
			int volume = AudioMixer_getVolume();
			volume += VOLUME_STEP_SIZE;
			if (volume > AUDIOMIXER_MAX_VOLUME) {
				volume = AUDIOMIXER_MAX_VOLUME;
			}
			AudioMixer_setVolume(volume);
			printf("Changing volume to %d / %d.\n", volume, AUDIOMIXER_MAX_VOLUME);
		}
		if (debounce((joystick & JOYSTICK_DOWN), &debounceDown, RESETTIME_SHORT)) {
			int volume = AudioMixer_getVolume();
			volume -= VOLUME_STEP_SIZE;
			if (volume < 0) {
				volume = 0;
			}
			AudioMixer_setVolume(volume);
			printf("Changing volume to %d / %d.\n", volume, AUDIOMIXER_MAX_VOLUME);
		}
		if (debounce((joystick & JOYSTICK_RIGHT), &debounceRight, RESETTIME_SHORT)) {
			int bpm = BeatBox_getBeatsPerMinute() + VOLUME_STEP_SIZE;
			if (bpm > BEATBOX_MAX_TEMPO) {
				bpm = BEATBOX_MIN_TEMPO;
			}
			BeatBox_setBeatsPerMinute(bpm);
			printf("Changing BPM to %d.\n", bpm);
		}
		if (debounce((joystick & JOYSTICK_LEFT), &debounceLeft, RESETTIME_SHORT)) {
			int bpm = BeatBox_getBeatsPerMinute() - VOLUME_STEP_SIZE;
			if (bpm < BEATBOX_MIN_TEMPO) {
				bpm = BEATBOX_MIN_TEMPO;
			}
			BeatBox_setBeatsPerMinute(bpm);
			printf("Changing BPM to %d.\n", bpm);
		}
		if (debounce((joystick & JOYSTICK_PRESS), &debouncePress, RESETTIME_LONG)) {
			int mode = (BeatBox_getMode() + 1) % BEATBOX_NUMBER_MODES;
			BeatBox_setMode(mode);
			printf("Changing to beat mode #%d.\n", mode);
		}

		// Don't continuously poll:
		sleep_usec(10000);	 // 0.1s
	}

	return NULL;
}
