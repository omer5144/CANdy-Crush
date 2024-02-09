#ifndef RADIO_H
#define RADIO_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include <inttypes.h>

#define RADIO_ID 777
#define RADIO_TYPE_POS 0
#define RADIO_STATION_POS 2

typedef enum
{
    RADIO_OTHER,
    RADIO_FM,
    RADIO_AM
} radio_type_t;

typedef struct
{
    uint8_t station;
    radio_type_t radio_type;
    char *song_name;
} radio_status_t;

void update_radio(struct canfd_frame *cf, int maxdlen, radio_status_t *radio_status);

#endif
