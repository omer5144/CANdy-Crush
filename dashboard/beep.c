#include "beep.h"

void update_beep(struct canfd_frame *cf, int maxdlen, int *beep_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len <= BEEP_POSITION)
    {
        return;
    }
    *beep_status = (int) cf->data[BEEP_POSITION];
}