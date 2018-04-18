// I2C driver
#include "i2c_drv.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

void I2cDrv_init(i2c_device_t *pdevice)
{
	char *bus = NULL;
	switch (pdevice->hw_bus) {
	case I2C_HW_BUS_0: bus = I2CDRV_LINUX_BUS0; break;
	case I2C_HW_BUS_1: bus = I2CDRV_LINUX_BUS1; break;
	case I2C_HW_BUS_2: bus = I2CDRV_LINUX_BUS2; break;
	default: assert(false);
	}

	pdevice->file_desc = open(bus, O_RDWR);
	if (pdevice->file_desc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(pdevice->file_desc, I2C_SLAVE, pdevice->address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
}
void I2cDrv_cleanup(i2c_device_t *pdevice)
{
	close(pdevice->file_desc);
	pdevice->file_desc = -1;
}

static void write_byte(i2c_device_t *pdevice, unsigned char value)
{
	int res = write(pdevice->file_desc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}
//	printf("I2C Writing to bus %d value 0x%02x\n", pdevice->hw_bus, value);
}

unsigned char I2cDrv_read_register(i2c_device_t *pdevice, unsigned char reg_address)
{
	// Write the address of the register out.
	write_byte(pdevice, reg_address);

	unsigned char value = 0;
	int res = read(pdevice->file_desc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}

int I2cDrv_read_registers(i2c_device_t *pdevice, unsigned char reg_address, unsigned char buff[], int size)
{
	write_byte(pdevice, reg_address);

	int bytes_read = read(pdevice->file_desc, buff, size);
	if (bytes_read <= 0) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return bytes_read;
}

void I2cDrv_write_register(i2c_device_t *pdevice, unsigned char reg_address, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = reg_address;
	buff[1] = value;
	int res = write(pdevice->file_desc, buff, 2);
	if (res != 2) {
		perror("Unable to write i2c register");
		exit(-1);
	}
}

void I2cDrv_write_register_check_retry(i2c_device_t *pdevice, unsigned char reg_address, unsigned char value, int retry)
{
	for (int i = 0; i < retry; i++) {
		I2cDrv_write_register(pdevice, reg_address, value);
		printf("reading\n");
		int check = I2cDrv_read_register(pdevice, reg_address);
		printf("done reading %0x\n", check);
		if (check == value) {
			break;
		} else {
			printf("I2C ERROR: readback value from register 0x%02x is 0x%02x (expect 0x%02x) (trial %d)\n",
					reg_address, check, value, i+1);
		}
	}

}
