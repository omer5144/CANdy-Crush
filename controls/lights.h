#ifndef LIGHTS_H
#define LIGHTS_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include "util.h"

#define LIGHTS_IS_ON_ID 666
#define LIGHTS_VOLUME_ID 667
#define LIGHTS_POS 0
#define LIGHTS_LEN 8

typedef enum {
    VOLUME_NONE = 0,
	VOLUME_LOW = 1,
	VOLUME_MEDIUM = 2,
	VOLUME_HIGH = 3
} lights_volume_t;

typedef struct {
    int is_on;
    lights_volume_t volume;
    lights_volume_t new_lights;
} lights_state_t;


void check_lights(int sock, lights_state_t *lights_state);

#endif
