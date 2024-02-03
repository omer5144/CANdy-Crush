#ifndef LIGHTS_H
#define LIGHTS_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include "gui.h"

#define SIGNAL_ID 392
#define SIGNAL_POS 0

typedef enum
{
  VALUE_LEFT = 1,
  VALUE_RIGHT = 2,
  VALUE_BOTH = 3
} signal_values_t;

void update_signal_status(struct canfd_frame *cf, int maxdlen, gui_data_t *gui_data);

#endif
