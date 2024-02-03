#include "lights.h"

void update_lights_status(struct canfd_frame *cf, int maxdlen, lights_state_t *lights_state, gui_data_t *gui_data)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len < LIGHTS_POS)
        return;

    if (cf->can_id == LIGHTS_IS_ON_ID)
    {
        lights_state->is_on = cf->data[LIGHTS_POS];
    } 
    else if (cf->can_id == LIGHTS_VOLUME_ID)
    {
        lights_state->volume = cf->data[LIGHTS_POS];
    }
    update_lights(gui_data, lights_state->is_on, lights_state->volume);
    SDL_RenderPresent(gui_data->renderer);
}
