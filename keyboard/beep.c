#include "beep.h"

void check_beep(int sock, beep_state_t *beep_state)
{
	struct canfd_frame cf;

	if (beep_state->is_on == beep_state->last_is_on)
	{
		return;
	}
	beep_state->last_is_on = beep_state->is_on;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = BEEP_ID;
	cf.len = BEEP_LEN;
	cf.data[BEEP_POSITION] = beep_state->is_on;
	send_pkt(CAN_MTU, &cf, sock);
}
