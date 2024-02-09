#ifndef RADIO_H
#define RADIO_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include <inttypes.h>
#include "util.h"

#define RADIO_ID 0x777
#define RADIO_TYPE_POS 0
#define RADIO_STATION_POS 2
#define RADIO_LEN 8

typedef enum
{
	RADIO_FM = 0,
	RADIO_AM = 1,
} radio_type_t;

typedef struct
{
	radio_type_t radio_type;
	uint8_t station;
	int last_update_time;
} radio_state_t;

void check_radio(int sock, int current_time, radio_state_t *radio_state);

#endif
