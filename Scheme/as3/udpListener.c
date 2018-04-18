// udpListener.c
#include "udpListener.h"
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <pthread.h>
#include <stdbool.h>
#include "beatBox.h"
#include "audioMixer.h"

#define UDP_PORT 12345
#define MAX_RECEIVE_MESSAGE_LENGTH 1024
#define REPLY_BUFFER_SIZE (10*1024)
#define PRIMES_PER_LINE 4

#define COMMAND_HELP  "help"
#define COMMAND_PLAY  "play"
#define COMMAND_VOLUME "volume"
#define COMMAND_MODE  "mode"
#define COMMAND_TEMPO "tempo"

static pthread_t s_threadId;
static char replyBuffer[REPLY_BUFFER_SIZE];
static _Bool stopping = false;

// Header
static void *udpListeningThread(void *args);
static void processCommand(char* command);
static int secondWordToInt(char *string);

void UdpListener_startListening(void)
{
	pthread_create(&s_threadId, NULL, &udpListeningThread, NULL);
}

void UdpListener_cleanup(void)
{
	stopping = true;
	pthread_join(s_threadId, NULL);
}

/*
 *	On the host:
 *		> netcat -u 192.168.0.171 12345
 *		(Change the IP address to your board)
 *
 *	On the host, type in a command and press enter:
 *		help<Enter>
 *
 */



static void *udpListeningThread(void *args)
{
	// Buffer to hold packet data:
	char message[MAX_RECEIVE_MESSAGE_LENGTH];

	// Address
	struct sockaddr_in sin;
	unsigned int sin_len;						// May change.
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);	// Host to Network long
	sin.sin_port = htons(UDP_PORT);				// Host to Network short

	// Create the socket for UDP
	int socket_descriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port that we specify
	bind(socket_descriptor, (struct sockaddr*) &sin, sizeof(sin));

	while (!stopping) {
		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		sin_len = sizeof(sin);
		int bytesRx = recvfrom(socket_descriptor, message, MAX_RECEIVE_MESSAGE_LENGTH, 0,
				(struct sockaddr *) &sin, &sin_len);

		// Make it null terminated (so string functions work):
		message[bytesRx] = 0;
		//printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, message);

		processCommand(message);

		// Transmit a reply:
		sin_len = sizeof(sin);
		sendto( socket_descriptor,
			replyBuffer, strnlen(replyBuffer, REPLY_BUFFER_SIZE),
			0,
			(struct sockaddr *) &sin, sin_len);
	}

	// Close (signals to main that we are ending)
	close(socket_descriptor);
	return NULL;
}

static void processCommand(char* command)
{
	replyBuffer[0] = 0;

	int numFromCommand = secondWordToInt(command);
	int numToReply = -1;

	// Extract the value from the message:
	if (strncmp(command, COMMAND_HELP, strlen(COMMAND_HELP)) == 0) {
		sprintf(replyBuffer, "Accepted command examples:\n");
		strcat(replyBuffer,  "play 0     -- Play the sound 0.\n");
		strcat(replyBuffer,  "volume 100 -- Sets the volume to 100 (value optional).\n");
		strcat(replyBuffer,  "              Replies with current volume.\n");
		strcat(replyBuffer,  "mode 1     -- Sets the beat mode to 1 (value optional).\n");
		strcat(replyBuffer,  "              Replies with current mode.\n");
		strcat(replyBuffer,  "tempo 120  -- Sets the beats per minute to 100 (value optional). \n");
		strcat(replyBuffer,  "              Replies with current tempo.\n");
	}

	else if (strncmp(command, COMMAND_PLAY, strlen(COMMAND_PLAY)) == 0) {
		// Check range
		if (numFromCommand == -1) {
			sprintf(replyBuffer, "Invalid argument; must be specified.\n");
		}
		else if (numFromCommand < 0 || numFromCommand >= BEATBOX_NUMBER_SOUNDS) {
			sprintf(replyBuffer, "Invalid argument. Must be between 0 and %d.\n",
					BEATBOX_NUMBER_SOUNDS-1);
		} else {
			// Apply the command
			BeatBox_playExtraSound(numFromCommand);
			numToReply = numFromCommand;
		}
	}
	else if (strncmp(command, COMMAND_VOLUME, strlen(COMMAND_VOLUME)) == 0) {
		// Check range
		if (numFromCommand == -1) {
			numToReply = AudioMixer_getVolume();
		}
		else if (numFromCommand < 0 || numFromCommand > AUDIOMIXER_MAX_VOLUME) {
			sprintf(replyBuffer, "Invalid argument. Must be between 0 and %d.\n",
					AUDIOMIXER_MAX_VOLUME);
		} else {
			// Apply the command
			AudioMixer_setVolume(numFromCommand);
			numToReply = AudioMixer_getVolume();
		}
	}
	else if (strncmp(command, COMMAND_MODE, strlen(COMMAND_MODE)) == 0) {
		// Check range
		if (numFromCommand == -1) {
			numToReply = BeatBox_getMode();
		}
		else if (numFromCommand < 0 || numFromCommand >= BEATBOX_NUMBER_MODES) {
			sprintf(replyBuffer, "Invalid argument. Must be between 0 and %d.\n",
					BEATBOX_NUMBER_MODES-1);
		} else {
			// Apply the command
			BeatBox_setMode(numFromCommand);
			numToReply = BeatBox_getMode();
		}
	}
	else if (strncmp(command, COMMAND_TEMPO, strlen(COMMAND_TEMPO)) == 0) {
		// Check range
		if (numFromCommand == -1) {
			numToReply = BeatBox_getBeatsPerMinute();
		}
		else if (numFromCommand < BEATBOX_MIN_TEMPO || numFromCommand > BEATBOX_MAX_TEMPO) {
			sprintf(replyBuffer, "Invalid argument. Must be +, - or between %d and %d.\n",
					BEATBOX_MIN_TEMPO, BEATBOX_MAX_TEMPO);
		}
		else {
			// Apply the command
			BeatBox_setBeatsPerMinute(numFromCommand);
			numToReply = BeatBox_getBeatsPerMinute();
		}
	}


	else {
		sprintf(replyBuffer, "Unknown command. Type 'help' for command list.\n");
	}

	if (numToReply != -1) {
		sprintf(replyBuffer, "%d\n", numToReply);
	}
}

static int secondWordToInt(char *string)
{
	// Default to a -1 if nothing.
	int value = -1;
	sscanf(string, "%*s%d", &value);
	return value;
}
