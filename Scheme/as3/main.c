/*
 *  Drum-beat application for working with the Zen cape.
 *  - Joystick for volume, mode, and tempo
 *  - Audio output
 *  - Accelerometer for "air-drumming"
 *  - UDP interface for web client.
 */

#include <stdio.h>
#include "beatBox.h"
#include "inputCtrl.h"
#include "udpListener.h"


#include "accel_drv.h"

int main(void)
{
	printf("Beginning drumbeat playback\n");

	// Initialize modules
	BeatBox_init();
	InputCtrl_init();
//	UdpListener_startListening();

	// Allow the program to gracefully terminate
	// (not required for assignment).
	printf("Enter a number to quit.");
	int num;
	scanf("%d", &num);

	// Cleanup modules
	printf("Shutting down application.\n");
	UdpListener_cleanup();
	InputCtrl_cleanup();
	BeatBox_cleanup();
	printf("Done!\n");

	return 0;
}
