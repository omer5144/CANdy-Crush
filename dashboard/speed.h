#ifndef SPEED_H
#define SPEED_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define SPEED_ID 0x3C4
#define SPEED_POS 3

typedef struct
{
    double speed;
} speed_status_t;

void update_speed(struct canfd_frame *cf, int maxdlen, speed_status_t *speed_status);

#endif
