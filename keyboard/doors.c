#include "doors.h"

void send_doors(int sock, int doors_bitfield)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = DOORS_ID;
	cf.len = DOORS_LEN;
	cf.data[DOORS_POSITION] = doors_bitfield;
	send_pkt(CAN_MTU, &cf, sock);
}

void check_doors(int sock, doors_state_t *doors_state)
{
	int doors_bitfield = (
		(doors_state->is_front_left_door_open << 0) |
		(doors_state->is_front_right_door_open << 1) |
		(doors_state->is_back_left_door_open << 2) |
		(doors_state->is_back_right_door_open << 3)
	);

	if (doors_state->last_bitfield == doors_bitfield)
	{
		return;
	}
	doors_state->last_bitfield = doors_bitfield;

	send_doors(sock, doors_bitfield);
}
