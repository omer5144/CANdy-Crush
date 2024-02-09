#include "radio.h"

void update_radio(struct canfd_frame *cf, int maxdlen, radio_status_t *radio_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len < RADIO_STATION_POS)
    {
        return;
    }

    if (strncmp((char *)cf->data + RADIO_TYPE_POS, "FM", 2))
    {
        radio_status->radio_type = RADIO_FM;
    }
    else if (strncmp((char *)cf->data + RADIO_TYPE_POS, "AM", 2))
    {
        radio_status->radio_type = RADIO_AM;
    }
    else
    {
        radio_status->radio_type = RADIO_OTHER;
    }

    radio_status->station = *((uint8_t *)cf->data);
    radio_status->song_name = "lalala";
}
