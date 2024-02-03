#ifndef SIGNAL_H
#define SIGNAL_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define SIGNAL_ID 392
#define SIGNAL_POS 0

typedef enum
{
    VALUE_LEFT = 1,
    VALUE_RIGHT = 2,
    VALUE_BOTH = 3
} signal_values_t;

typedef struct
{
    int left;
    int right;
} signal_status_t;

void update_signal_status(struct canfd_frame *cf, int maxdlen, signal_status_t *signal_status);

#endif
