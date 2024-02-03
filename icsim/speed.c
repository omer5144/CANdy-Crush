#include "speed.h"

void update_speed(struct canfd_frame *cf, int maxdlen, speed_status_t *speed_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len < SPEED_POS + 1)
    {
        return;
    }
    
    speed_status->speed = (((cf->data[SPEED_POS] << 8) + cf->data[SPEED_POS + 1]) / 100) * 0.6213751;
}
