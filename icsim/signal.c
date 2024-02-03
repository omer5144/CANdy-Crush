#include "signal.h"

void update_signal_status(struct canfd_frame *cf, int maxdlen, gui_data_t *gui_data)
{
    int left;
    int right;

    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len < SIGNAL_POS)
        return;
    if (cf->data[SIGNAL_POS] & VALUE_LEFT)
    {
        left = 1;
    }
    else
    {
        left = 0;
    }
    if (cf->data[SIGNAL_POS] & VALUE_RIGHT)
    {
        right = 1;
    }
    else
    {
        right = 0;
    }
    update_turn_signals(gui_data, left, right);
    SDL_RenderPresent(gui_data->renderer);
}
