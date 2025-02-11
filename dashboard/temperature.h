#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <stddef.h>
#include <inttypes.h>

#define TEMPERATURE_ID 0x3E7
#define TEMPERATURE_POS 0
#define TEMPERATURE_CHECKSUM_POS (sizeof(float))

typedef struct
{
    float temperature;
    float current;
} temperature_status_t;

uint8_t checksum_of(char *temperature, size_t length);

void update_temperature(struct canfd_frame *cf, int maxdlen, temperature_status_t *temperature_status);

#endif