#ifndef SPEED_H
#define SPEED_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

#define SPEED_ID 580
#define DEFAULT_SPEED_POS 3
#define SPEED_LEN 8

#define MAX_SPEED 90.0
#define ACCEL_RATE 8.0

typedef enum {
	DECCEL_THROTTLE = -1,
	ACCEL_THROTTLE = 1
} throttle_t;

typedef struct {
	throttle_t throttle;
	float current_speed;
	int last_accel_time;
} speed_state_t;


void check_accel(int sock, int currentTime, speed_state_t *speed_state);

#endif
