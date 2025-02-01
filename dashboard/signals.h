#ifndef SIGNAL_H
#define SIGNAL_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define SIGNAL_ID 0x188
#define SIGNAL_POS 0

typedef struct
{
    int left;
    int right;
} signals_status_t;

void update_signals(struct canfd_frame *cf, int maxdlen, signals_status_t *signals_status);

#endif
