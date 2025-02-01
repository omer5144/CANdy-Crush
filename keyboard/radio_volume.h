#ifndef RADIO_VOLUME_H
#define RADIO_VOLUME_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include "util.h"

#define RADIO_VOLUME_ID 650
#define RADIO_VOLUME_POSITION 0
#define RADIO_VOLUME_LEN 4

void send_radio_volume(int sock, int radio_volume);

#endif