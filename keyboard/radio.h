#ifndef RADIO_H
#define RADIO_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include <inttypes.h>
#include "util.h"

#define RADIO_ID 555
#define RADIO_KEY_ID 556
#define RADIO_KEY_POS 0
#define RADIO_TYPE_POS 1
#define RADIO_SONG_POS 0
#define RADIO_KEY_LEN 8

typedef enum
{
	RADIO_FM = 0,
	RADIO_AM = 1,
} radio_type_t;

typedef struct
{
	radio_type_t radio_type;
	char *song_name;
	uint8_t key;
	int last_update_time;
} radio_state_t;

void check_radio(int sock, int current_time, radio_state_t *radio_state);

#endif
