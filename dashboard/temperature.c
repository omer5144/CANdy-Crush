#include "temperature.h"
#include <stdio.h>

uint8_t checksum_of(char *temperature, size_t length)
{
    uint8_t checksum = 0;
    for (int i = 0; i < length; ++i)
    {
        uint8_t byte = temperature[i];
        while (byte) {
            checksum += (byte & 1);
            byte = byte >> 1;
        } 
    }
    return checksum;
}

void update_temperature(struct canfd_frame *cf, int maxdlen, temperature_status_t *temperature_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    double temperature;
    uint8_t checksum;

    if (len <= TEMPERATURE_CHECKSUM_POS)
    {
        return;
    }

    checksum = cf->data[TEMPERATURE_CHECKSUM_POS];
    if (checksum != checksum_of(cf->data, TEMPERATURE_CHECKSUM_POS))
    {
        return;
    }

    temperature = *((float*)(cf->data));
    if  (temperature < 16)
    {
        temperature = 16;
    }

    if (temperature > 32)
    {
        temperature = 32;
    }

    temperature_status->temperature = temperature;
}