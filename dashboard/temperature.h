#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define TEMPERATURE_ID 999
#define TEMPERATURE_POS 0
#define TEMPERATURE_CHECKSUM_POS (sizeof(float))

typedef struct
{
    float temperature;
} temperature_status_t;

uint8_t checksum_of(char *temperature, size_t length);

void update_temperature(struct canfd_frame *cf, int maxdlen, temperature_status_t *temperature_status);

#endif