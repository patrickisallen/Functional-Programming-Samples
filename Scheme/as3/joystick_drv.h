// Joystick module to provide clean intefrace for reading Zen cape's joystick.
#ifndef JOYSTICK_DRV_H_
#define JOYSTICK_DRV_H_
#include <stdint.h>

// Possible Joystick directions
#define JOYSTICK_NONE  0x00
#define JOYSTICK_UP    0x01
#define JOYSTICK_RIGHT 0x02
#define JOYSTICK_DOWN  0x04
#define JOYSTICK_LEFT  0x08
#define JOYSTICK_PRESS 0x10


void Joystick_init();
void Joystick_cleanup();

uint32_t Joystick_readState();

#endif
