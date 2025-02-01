#ifndef RADIO_VOLUME_H
#define RADIO_VOLUME_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include "util.h"

#define RADIO_VOLUME_ID 0x28A
#define RADIO_VOLUME_POSITION 0
#define RADIO_VOLUME_LEN 4
#define RADIO_VOLUME_MAX_VALUE 100
#define RADIO_VOLUME_MAX_MIN_VALUE (0)

void update_radio_volume(struct canfd_frame *cf, int maxdlen, int32_t *radio_volume);

#endif