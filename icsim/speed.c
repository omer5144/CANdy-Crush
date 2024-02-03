#include "speed.h"

void update_speed_status(struct canfd_frame *cf, int maxdlen, gui_data_t *gui_data)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len < SPEED_POS + 1)
        return;

    int speed = cf->data[SPEED_POS] << 8;
    speed += cf->data[SPEED_POS + 1];
    speed = speed / 100;

    update_speed(gui_data, speed * 0.6213751);
    SDL_RenderPresent(gui_data->renderer);
}
