#include "speed.h"

void send_speed(int sock, int speed)
{
	struct canfd_frame cf;

	int kph = (speed / 0.6213751) * 100;
	memset(&cf, 0, sizeof(cf));
	cf.can_id = SPEED_ID;
	cf.len = SPEED_LEN;
	if (kph == 0) // IDLE
	{
		cf.data[DEFAULT_SPEED_POS] = 1;
		cf.data[DEFAULT_SPEED_POS + 1] = rand() % 255 + 100;
	}
	else
	{
		cf.data[DEFAULT_SPEED_POS] = (char)(kph >> 8) & 0xff;
		cf.data[DEFAULT_SPEED_POS + 1] = (char)kph & 0xff;
	}
	send_pkt(CAN_MTU, cf, sock);
}

void checkAccel(int sock, int currentTime, speed_state_t *speed_state)
{
	float rate = MAX_SPEED / (ACCEL_RATE * 100);
	if (currentTime > speed_state->lastAccel + 10) // Updated every 10 ms
	{
		if (speed_state->throttle < 0)
		{
			speed_state->current_speed -= rate;
			if (speed_state->current_speed < 1)
				speed_state->current_speed = 0;
		}
		else if (speed_state->throttle > 0)
		{
			speed_state->current_speed += rate;
			if (speed_state->current_speed > MAX_SPEED) // Limiter
			{ 
				speed_state->current_speed = MAX_SPEED;
			}
		}
		send_speed(sock, speed_state->current_speed);
		speed_state->lastAccel = currentTime;
	}
}
