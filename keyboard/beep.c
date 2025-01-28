#include "beep.h"

void send_beep(int sock, int beep_state)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = BEEP_ID;
	cf.len = BEEP_LEN;
	cf.data[BEEP_POSITION] = beep_state;
	send_pkt(CAN_MTU, &cf, sock);
}
