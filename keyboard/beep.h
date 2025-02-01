#ifndef BEEP_H
#define BEEP_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include "util.h"

#define BEEP_ID 101
#define BEEP_POSITION 0
#define BEEP_LEN 8

typedef struct
{
	int is_on;
    int last_is_on;
} beep_state_t;

void check_beep(int sock, beep_state_t *beep_state);

#endif