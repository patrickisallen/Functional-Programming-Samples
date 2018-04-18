#include "gpio_drv.h"
#include "general.h"

#include "fileio_drv.h"


#define FILE_NAME_EXPORT   "/sys/class/gpio/export"
#define FILE_NAME_UNEXPORT "/sys/class/gpio/unexport"

#define BUFF_SIZE 1024

static void export_or_unexport(int linux_pin_num, char* file_name)
{
	char buff[BUFF_SIZE];
	sprintf(buff, "%d", linux_pin_num);
	FileIODrv_echo_to_file(file_name, buff);
}
static void set_direction(int linux_pin_num, _Bool is_output)
{
	// Open direction file
	char buff[BUFF_SIZE];
	snprintf(buff, BUFF_SIZE, "/sys/class/gpio/gpio%d/direction", linux_pin_num);

	// Set direction
	char data[BUFF_SIZE];
	if (is_output) {
		snprintf(data, BUFF_SIZE, "out");
	} else {
		snprintf(data, BUFF_SIZE, "in");
	}

	FileIODrv_echo_to_file(buff, data);
}


void GPIODrv_init(pin_t *ppin_info)
{
	export_or_unexport(ppin_info->linux_pin_num, FILE_NAME_EXPORT);
	set_direction(ppin_info->linux_pin_num, ppin_info->is_output);
}
void GPIODrv_cleanup(pin_t *ppin_info)
{
	export_or_unexport(ppin_info->linux_pin_num, FILE_NAME_UNEXPORT);
}

// Returns number of bytes read
int GPIODrv_read(pin_t *ppin_info, char* buff, int max_length)
{
	char file_name[BUFF_SIZE];
	snprintf(file_name, BUFF_SIZE, "/sys/class/gpio/gpio%d/value", ppin_info->linux_pin_num);

	return FileIODrv_read_str(file_name, buff, max_length);
}

// Returns number of bytes written
int GPIODrv_write(pin_t *ppin_info, char* buff)
{
	char file_name[BUFF_SIZE];
	snprintf(file_name, BUFF_SIZE, "/sys/class/gpio/gpio%d/value", ppin_info->linux_pin_num);

	return FileIODrv_echo_to_file(file_name, buff);
}

