#ifndef SPEED_H
#define SPEED_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define SPEED_ID 580
#define SPEED_POS 3

typedef struct
{
    double speed;
} speed_status_t;

void update_speed_status(struct canfd_frame *cf, int maxdlen, speed_status_t *speed_status);

#endif
