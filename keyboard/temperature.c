#include "temperature.h"

uint8_t checksum_of(char *temperature, size_t length)
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; ++i)
    {
        uint8_t byte = temperature[i];
        while (byte) {
            checksum += (byte & 1);
            byte = byte >> 1;
        } 
    }
    return checksum;
}

void check_temperature(int sock, temperature_state_t *temperature_state)
{
	float temperature = 25;
	struct canfd_frame cf;

	if (temperature_state->temperature == temperature_state->last_temperature)
	{
		return;
	}
	temperature_state->last_temperature = temperature_state->temperature;

	if (temperature_state->temperature == TEMPERATURE_COLD)
	{
		temperature = 16;
	}
	else if (temperature_state->temperature == TEMPERATURE_NONE)
	{
		temperature = 25;
	}
	else if (temperature_state->temperature == TEMPERATURE_HOT)
	{
		temperature = 34;
	}

	memset(&cf, 0, sizeof(cf));
	cf.can_id = TEMPERATURE_ID;
	cf.len = TEMPERATURE_CHECKSUM_POS + 1;
	memcpy(cf.data, &temperature, sizeof(temperature));
	cf.data[TEMPERATURE_CHECKSUM_POS] = checksum_of((char*)cf.data, sizeof(temperature));
	send_pkt(CAN_MTU, &cf, sock);
}
