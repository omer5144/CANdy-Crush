#include "radio.h"

void update_radio(struct canfd_frame *cf, int maxdlen, radio_status_t *radio_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;

    if (cf->can_id == RADIO_KEY_ID)
    {
        if (len <= RADIO_TYPE_POS + 1)
        {
            return;
        }

        radio_status->key = cf->data[RADIO_KEY_POS];
        
        if (strncmp((char *)cf->data + RADIO_TYPE_POS, "FM", 2) == 0)
        {
            radio_status->radio_type = RADIO_FM;
        }
        else if (strncmp((char *)cf->data + RADIO_TYPE_POS, "AM", 2) == 0)
        {
            radio_status->radio_type = RADIO_AM;
        } 
    } 
    else if (cf->can_id == RADIO_ID)
    {
        if (len > 20)
        {
            len = 20
        }

        memcpy(radio_status->song_name, cf->data, len);
        radio_status->song_name[len] = 0;

        for (size_t i = 0; i < len; ++i)
        {
            radio_status->song_name[i] ^= radio_status->key;
        }
    }
}
