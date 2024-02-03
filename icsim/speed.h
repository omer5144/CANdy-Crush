#ifndef SPEED_H
#define SPEED_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include "gui.h"

#define SPEED_ID 580
#define SPEED_POS 3

void update_speed_status(struct canfd_frame *cf, int maxdlen, gui_data_t *gui_data);

#endif
