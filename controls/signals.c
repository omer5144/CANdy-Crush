#include "signals.h"

void send_turn_signal(int sock, signal_t signal)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = SIGNAL_ID;
	cf.len = SIGNAL_LEN;
	cf.data[SIGNAL_POS] = signal;
	send_pkt(CAN_MTU, &cf, sock);
}


void check_turn_signal(int sock, int current_time, signal_state_t *signal_state)
{
	if (current_time > signal_state->last_turn_time + 500)
	{
		switch (signal_state->turn)
		{
		case LEFT_TURN:
			signal_state->signal = signal_state->signal == NO_SIGNAL ? LEFT_SIGNAL : NO_SIGNAL;
			break;
		case RIGHT_TURN:
			signal_state->signal = signal_state->signal == NO_SIGNAL ? RIGHT_SIGNAL : NO_SIGNAL;
			break;
		case NO_TURN:
			signal_state->signal = NO_SIGNAL;
			break;
		}
		
		send_turn_signal(sock, signal_state->signal);
		signal_state->last_turn_time = current_time;
	}
}
