#include "doors.h"

void update_doors(struct canfd_frame *cf, int maxdlen, doors_status_t *doors_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len <= DOORS_POS)
    {
        return;
    }

    doors_status->front_left = cf->data[DOORS_POS] & (1 << 0) ? 1 : 0;
    doors_status->front_right = cf->data[DOORS_POS] & (1 << 1) ? 1 : 0;
    doors_status->back_left = cf->data[DOORS_POS] & (1 << 2) ? 1 : 0;
    doors_status->back_right = cf->data[DOORS_POS] & (1 << 3) ? 1 : 0;
}
