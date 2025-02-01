#ifndef RADIO_H
#define RADIO_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include <inttypes.h>

#define RADIO_ID 555
#define RADIO_KEY_ID 556
#define RADIO_KEY_POS 0
#define RADIO_TYPE_POS 1
#define RADIO_SONG_POS 0

typedef enum
{
    RADIO_OTHER,
    RADIO_FM,
    RADIO_AM
} radio_type_t;

typedef struct
{
    char song_name[9];
    uint8_t key;
    radio_type_t radio_type;
} radio_status_t;

void update_radio(struct canfd_frame *cf, int maxdlen, radio_status_t *radio_status);

#endif
