#include "lights.h"

void send_lights_volume(int sock, lights_volume_t volume)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = LIGHTS_VOLUME_ID;
	cf.len = LIGHTS_LEN;
	cf.data[LIGHTS_POS] = volume;
	send_pkt(CAN_MTU, &cf, sock);
}

void send_lights_on(int sock, int is_on)
{
	struct canfd_frame cf;

	memset(&cf, 0, sizeof(cf));
	cf.can_id = LIGHTS_IS_ON_ID;
	cf.len = LIGHTS_LEN;
	cf.data[LIGHTS_POS] = is_on;
	send_pkt(CAN_MTU, &cf, sock);
}

void check_lights(int sock, lights_state_t *lights_state)
{
	if (lights_state->new_lights && lights_state->new_lights != lights_state->volume)
	{
		lights_state->volume = lights_state->new_lights;
		send_lights_volume(sock, lights_state->volume);
	}

	if ((!lights_state->new_lights) == lights_state->is_on)
	{
		lights_state->is_on = !lights_state->is_on;
		send_lights_on(sock, lights_state->is_on);
	}
}
