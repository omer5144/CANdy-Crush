#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include <stddef.h>
#include <inttypes.h>
#include "util.h"

#define TEMPERATURE_ID 999
#define TEMPERATURE_POS 0
#define TEMPERATURE_CHECKSUM_POS (sizeof(float))

typedef enum
{
    TEMPERATURE_COLD = 0,
    TEMPERATURE_NONE = 1,
    TEMPERATURE_HOT = 2
} temperature_volume_t;

typedef struct
{
    temperature_volume_t temperature;
} temperature_state_t;

void check_temperature(int sock, temperature_state_t *temperature_state);

#endif
