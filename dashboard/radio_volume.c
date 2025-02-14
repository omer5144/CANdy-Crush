#include "radio_volume.h"

void update_radio_volume(struct canfd_frame *cf, int maxdlen, int *radio_volume)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len <= 0)
    {
        return;
    }

	int volume_diff = (int32_t)(cf->data[3]) |
                       ((int32_t)cf->data[2] << 8) |
                       ((int32_t)cf->data[1] << 16) |
                       ((int32_t)cf->data[0] << 24);

	if (volume_diff < -(RADIO_VOLUME_MAX_VALUE)) {
		volume_diff = -(RADIO_VOLUME_MAX_VALUE);
	}
	if (volume_diff > RADIO_VOLUME_MAX_VALUE) {
		volume_diff = RADIO_VOLUME_MAX_VALUE;
	}

	*radio_volume += volume_diff;

	if ((*radio_volume) < RADIO_VOLUME_MAX_MIN_VALUE) {
		*radio_volume = RADIO_VOLUME_MAX_MIN_VALUE;
	}
	if ((*radio_volume) > RADIO_VOLUME_MAX_VALUE) {
		*radio_volume = RADIO_VOLUME_MAX_VALUE;
	}
}