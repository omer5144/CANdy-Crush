#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include "util.h"

#ifndef SIGNALS_H
#define SIGNALS_H

#define SIGNAL_ID 392
#define SIGNAL_POS 0
#define SIGNAL_LEN 8

#define LEFT_SIGNAL_VALUE 1
#define RIGHT_SIGNAL_VALUE 2

typedef struct {
	char signal;
	int turning;
	int lastTurnSignal;
} signal_state_t;


void checkTurn(int sock, int currentTime, signal_state_t *signal_state);

#endif
