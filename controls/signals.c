#include "signals.h"

void send_turn_signal(int sock, char signal)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = SIGNAL_ID;
	cf.len = SIGNAL_LEN;
	cf.data[SIGNAL_POS] = signal;
	send_pkt(CAN_MTU, cf, sock);
}


void checkTurn(int sock, int currentTime, signal_state_t *signal_state)
{
	if (currentTime > signal_state->lastTurnSignal + 500)
	{
		if (signal_state->turning < 0)
		{
			signal_state->signal ^= LEFT_SIGNAL_VALUE;
		}
		else if (signal_state->turning > 0)
		{
			signal_state->signal ^= RIGHT_SIGNAL_VALUE;
		}
		else
		{
			signal_state->signal = 0;
		}
		send_turn_signal(sock, signal_state->signal);
		signal_state->lastTurnSignal = currentTime;
	}
}
