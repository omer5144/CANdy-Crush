#include "radio.h"
#include <stdio.h>

void send_radio(int sock, char *song_name, int length, uint8_t key)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = RADIO_ID;
	cf.len = length;
	memcpy(cf.data, song_name, length);
	for (int i = 0; i < length; ++i)
	{
		cf.data[i] ^= key;
	}
	send_pkt(CAN_MTU, &cf, sock);
}

void send_radio_key(int sock, radio_type_t radio_type, uint8_t key)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = RADIO_KEY_ID;
	cf.len = RADIO_KEY_LEN;
	cf.data[RADIO_KEY_POS] = key;
	cf.data[RADIO_TYPE_POS] = (radio_type == RADIO_FM) ? 'F' : 'A';
	cf.data[RADIO_TYPE_POS + 1] = 'M';
	send_pkt(CAN_MTU, &cf, sock);
}

void check_radio(int sock, int current_time, radio_state_t *radio_state)
{
	if (current_time > radio_state->last_update_time + 2000)
	{
		radio_state->key = rand() % 256;
		send_radio_key(sock, radio_state->radio_type, radio_state->key);
		send_radio(sock, radio_state->song_name, radio_state->song_name_length, radio_state->key);
		radio_state->last_update_time = current_time;
	}
}
