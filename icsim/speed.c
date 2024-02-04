#include "speed.h"
#include <stdio.h>
void update_speed(struct canfd_frame *cf, int maxdlen, speed_status_t *speed_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    double speed;
    double delta;

    if (len < SPEED_POS + 1)
    {
        return;
    }

    speed = (((cf->data[SPEED_POS] << 8) + cf->data[SPEED_POS + 1]) / 100) * 0.6213751;
    delta = speed - speed_status->speed;
    if (-2 < delta && delta < 2)
    {
        speed_status->speed = speed;
    }
}
