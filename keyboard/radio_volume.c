#include "radio_volume.h"

void send_radio_volume(int sock, int radio_volume)
{
    struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = RADIO_VOLUME_ID;
	cf.len = RADIO_VOLUME_LEN;

	cf.data[0] = (radio_volume >> 0) & 0xFF;
    cf.data[1] = (radio_volume >> 8) & 0xFF;
    cf.data[2] = (radio_volume >> 16) & 0xFF;
    cf.data[3] = (radio_volume >> 24) & 0xFF;
	
	send_pkt(CAN_MTU, &cf, sock);
}