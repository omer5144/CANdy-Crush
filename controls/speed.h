#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

#ifndef SPEED_H
#define SPEED_H

#define SPEED_ID 580
#define DEFAULT_SPEED_POS 3
#define SPEED_LEN 8

#define MAX_SPEED 90.0
#define ACCEL_RATE 8.0

typedef struct {
	int throttle;
	float current_speed;
	int lastAccel;
} speed_state_t;


void checkAccel(int sock, int currentTime, speed_state_t *speed_state);

#endif
