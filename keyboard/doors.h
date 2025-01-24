#ifndef DOORS_H
#define DOORS_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include "util.h"

#define DOORS_ID 500
#define DOORS_POSITION 0
#define DOORS_LEN 8

typedef enum
{
	FRONT_LEFT_DOOR = 0,
	FRONT_RIGHT_DOOR = 1,
	BACK_LEFT_DOOR = 2,
	BACK_RIGHT_DOOR = 3
} door_t;

typedef struct
{
	int is_front_left_door_open;
	int is_front_right_door_open;
	int is_back_left_door_open;
	int is_back_right_door_open;
} doors_state_t;

void check_doors(int sock, doors_state_t *doors_state);

#endif
