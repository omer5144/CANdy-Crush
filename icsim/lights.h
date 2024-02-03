#ifndef LIGHTS_H
#define LIGHTS_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define LIGHTS_IS_ON_ID 666
#define LIGHTS_VOLUME_ID 667
#define LIGHTS_POS 0

typedef struct
{
    int is_on;
    int volume;
} lights_status_t;

void update_lights_status(struct canfd_frame *cf, int maxdlen, lights_status_t *lights_status);

#endif
