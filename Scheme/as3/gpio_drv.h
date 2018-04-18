#ifndef _GPIO_DRV_H_
#define _GPIO_DRV_H_

typedef struct {
	int linux_pin_num;
	_Bool is_output;
} pin_t;

void GPIODrv_init(pin_t *ppin_info);
void GPIODrv_cleanup(pin_t *ppin_info);

// Returns number of bytes read
int GPIODrv_read(pin_t *ppin_info, char* buff, int max_length);

// Returns number of bytes written
int GPIODrv_write(pin_t *ppin_info, char* buff);

#endif
