#ifndef DOORS_H
#define DOORS_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define DOORS_ID 500
#define DOORS_POS 0

typedef struct
{
    int front_left;
    int front_right;
    int back_left;
    int back_right;
} doors_status_t;

void update_doors(struct canfd_frame *cf, int maxdlen, doors_status_t *doors_status);

#endif
