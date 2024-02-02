#ifndef SPEED_H
#define SPEED_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include "gui.h"

#define SPEED_ID 580
#define SPEED_POS 3

void update_speed_status(struct canfd_frame *cf, int maxdlen, gui_data_t gui_data)
{
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if (len < SPEED_POS + 1)
    return;

  int speed = cf->data[SPEED_POS] << 8;
  speed += cf->data[SPEED_POS + 1];
  speed = speed / 100;
  
  update_speed(gui_data, speed * 0.6213751);
  SDL_RenderPresent(gui_data.renderer);
}

long current_speed = 0;

#endif
