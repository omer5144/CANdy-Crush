#ifndef SIGNALS_H
#define SIGNALS_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include "util.h"

#define SIGNAL_ID 392
#define SIGNAL_POS 0
#define SIGNAL_LEN 8

typedef enum
{
	NO_SIGNAL = 0,
	LEFT_SIGNAL = 1,
	RIGHT_SIGNAL = 2
} signal_t;

typedef struct
{
	signal_t signal;
	turn_t turn;
	int last_turn_time;
} signal_state_t;

void check_turn_signal(int sock, int current_time, signal_state_t *signal_state);

#endif
