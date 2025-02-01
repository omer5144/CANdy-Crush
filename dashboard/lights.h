#ifndef LIGHTS_H
#define LIGHTS_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define LIGHTS_IS_ON_ID 0x29A
#define LIGHTS_VOLUME_ID 0x29B
#define LIGHTS_POS 0

typedef struct
{
    int is_on;
    int volume;
} lights_status_t;

void update_lights(struct canfd_frame *cf, int maxdlen, lights_status_t *lights_status);

#endif
