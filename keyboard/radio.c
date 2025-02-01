#include "radio.h"

void send_radio(int sock, radio_type_t radio_type, char *song_name, uint8_t key)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = RADIO_KEY_ID;
	cf.len = RADIO_KEY_LEN;
	cf.data[RADIO_KEY_POS] = key;
	cf.data[RADIO_TYPE_POS] = (radio_type == RADIO_FM) ? 'F' : 'A';
	cf.data[RADIO_TYPE_POS + 1] = 'M';
	send_pkt(CAN_MTU, &cf, sock);

	memset(&cf, 0, sizeof(cf));
	cf.can_id = RADIO_ID;
	cf.len = strlen(song_name);
	memcpy(cf.data, song_name, strlen(song_name));
	send_pkt(CAN_MTU, &cf, sock);
}

void check_radio(int sock, int current_time, radio_state_t *radio_state)
{
	if (current_time > radio_state->last_update_time + 2000)
	{
		send_radio(sock, radio_state->radio_type, radio_state->song_name, radio_state->key);
		radio_state->last_update_time = current_time;
	}
}
