#ifndef BEEP_H
#define BEEP_H

#include <linux/can.h>
#include <linux/can/raw.h>

#define BEEP_ID 0xBEE
#define BEEP_POSITION 0

void update_beep(struct canfd_frame *cf, int maxdlen, int *beep_status);

#endif
