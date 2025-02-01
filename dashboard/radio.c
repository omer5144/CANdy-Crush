#include "radio.h"

void update_radio(struct canfd_frame *cf, int maxdlen, radio_status_t *radio_status, char *songs[])
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    int index;

    if (len <= RADIO_STATION_POS)
    {
        return;
    }

    if (strncmp((char *)cf->data + RADIO_TYPE_POS, "FM", 2) == 0)
    {
        radio_status->radio_type = RADIO_FM;
    }
    else if (strncmp((char *)cf->data + RADIO_TYPE_POS, "AM", 2) == 0)
    {
        radio_status->radio_type = RADIO_AM;
    }
    else
    {
        radio_status->radio_type = RADIO_OTHER;
    }

    radio_status->station = cf->data[RADIO_STATION_POS];
    index = radio_status->station - 90;
    if (0 <= index && index < 20)
    {
        radio_status->song_name = songs[index];
    }
}
