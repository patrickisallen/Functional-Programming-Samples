#include "joystick_drv.h"
#include "general.h"
#include "gpio_drv.h"


#define TERMINATOR -1
#define BUFF_SIZE 255
pin_t pins[] = {
		{26, false},	// up
		{47, false},	// right
		{46, false},	// down
		{65, false},	// left
		{27, false},	// press
		{TERMINATOR, false}		// indicate end of array
};

_Bool is_initialized = false;

void Joystick_init()
{
	assert(!is_initialized);
	is_initialized = true;

	for (int i = 0; true; i++) {
		if (pins[i].linux_pin_num == TERMINATOR) {
			break;
		}
		GPIODrv_init(&pins[i]);
	}
}
void Joystick_cleanup()
{
	assert(is_initialized);
	for (int i = 0; true; i++) {
		if (pins[i].linux_pin_num == TERMINATOR) {
			break;
		}
		GPIODrv_cleanup(&pins[i]);
	}
	is_initialized = false;
}

uint32_t Joystick_readState()
{
	assert(is_initialized);
	uint32_t result = 0;
	for (int i = 0; true; i++) {
		if (pins[i].linux_pin_num == TERMINATOR) {
			break;
		}
		char buff[BUFF_SIZE];
		buff[0] = 0;
		if (GPIODrv_read(&pins[i], buff, BUFF_SIZE) < 0) {
			die_on_failed("Unable to read joystick state.");
		}

		// Reads 1 for not pressed:
		if (buff[0] == '0') {
			result |= (1 << i);
		} else if (buff[0] == '1') {
			// Do nothing; not pressed
		} else {
			printf("JOYSTICK DRV ERROR: Unexpected read from pin IDX %d = '%s'.\n",
					i, buff);
		}
	}
	return result;
}
