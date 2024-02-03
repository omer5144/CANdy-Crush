#include "lights.h"

void update_lights_status(struct canfd_frame *cf, int maxdlen, lights_status_t *lights_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len < LIGHTS_POS)
    {
        return;
    }

    if (cf->can_id == LIGHTS_IS_ON_ID)
    {
        lights_status->is_on = cf->data[LIGHTS_POS];
    } 
    else if (cf->can_id == LIGHTS_VOLUME_ID)
    {
        lights_status->volume = cf->data[LIGHTS_POS];
    }
}
