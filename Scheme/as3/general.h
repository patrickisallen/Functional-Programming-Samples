// Shared includes and functions

#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

void die_on_failed(char *msg);
void sleep_usec(long usec);


#endif
