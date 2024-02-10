#include "radio.h"

void send_radio(int sock, radio_type_t radio_type, uint8_t station)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = RADIO_ID;
	cf.len = RADIO_LEN;
	cf.data[RADIO_TYPE_POS] = (radio_type == RADIO_FM) ? 'F' : 'A';
	cf.data[RADIO_TYPE_POS + 1] = 'M';
	cf.data[RADIO_STATION_POS] = station;
	cf.data[RADIO_LEN - 1] = rand() % 256;
	send_pkt(CAN_MTU, &cf, sock);
}

void check_radio(int sock, int current_time, radio_state_t *radio_state)
{
	if (current_time > radio_state->last_update_time + 500)
	{
		send_radio(sock, radio_state->radio_type, radio_state->station);
		radio_state->last_update_time = current_time;
	}
}
