#include "gui.h"

char *get_data(char *fname, char *data_file)
{
    if (strlen(DATA_DIR) + strlen(fname) > DATA_FILE_SIZE - 1)
        return NULL;
    strncpy(data_file, DATA_DIR, DATA_FILE_SIZE - 1);
    strncat(data_file, fname, DATA_FILE_SIZE - 1 - strlen(data_file));
    return data_file;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void update_speed(gui_data_t *gui_data, speed_status_t *speed_status)
{
    SDL_Point center;
    double angle = 0;

    center.x = 217;
    center.y = gui_data->speed_rect.h / 2;
    angle = map(speed_status->speed, 0, 280, 0, 180);
    if (angle < 0)
        angle = 0;
    if (angle > 180)
        angle = 180;
    SDL_RenderCopyEx(gui_data->renderer, gui_data->needle_tex, NULL, &gui_data->speed_rect, angle, &center, SDL_FLIP_NONE);
}

void update_turn_signals(gui_data_t *gui_data, signal_status_t *signal_status)
{
    SDL_Rect left = {213, 51, 70, 70};
    SDL_Rect right = {482, 51, 70, 70};
    SDL_Rect lpos = {191, 29, 70, 70};
    SDL_Rect rpos = {460, 29, 70, 70};
    
    if (signal_status->left)
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->on_left_signal_tex, NULL, &lpos);
    }
    else
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->off_left_signal_tex, NULL, &lpos);
    }
    if (signal_status->right)
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->on_right_signal_tex, NULL, &rpos);
    }
    else
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->off_right_signal_tex, NULL, &rpos);
    }
}

void update_lights(gui_data_t *gui_data, lights_status_t *lights_status)
{
    // SDL_Rect left, right, lpos, rpos;
    // left.x = 213;
    // left.y = 51;
    // left.w = 45;
    // left.h = 45;
    // memcpy(&right, &left, sizeof(SDL_Rect));
    // right.x = 482;
    // memcpy(&lpos, &left, sizeof(SDL_Rect));
    // memcpy(&rpos, &right, sizeof(SDL_Rect));
    // lpos.x -= 22;
    // lpos.y -= 22;
    // rpos.x -= 22;
    // rpos.y -= 22;

    if (lights_status->is_on)
    {
        //printf("volume: %d\n", lights_status->volume);
    }
    else
    {
        //puts("close");
    }
    // if (left_signal == 0)
    // {
    //   SDL_RenderCopy(gui_data->renderer, gui_data->base_texture, &lpos, &lpos);
    // }
    // else
    // {
    //   SDL_RenderCopy(gui_data->renderer, gui_data->sprite_tex, &left, &lpos);
    // }
    // if (right_signal == 0)
    // {
    //   SDL_RenderCopy(gui_data->renderer, gui_data->base_texture, &rpos, &rpos);
    // }
    // else
    // {
    //   SDL_RenderCopy(gui_data->renderer, gui_data->sprite_tex, &right, &rpos);
    // }
}

void draw(gui_data_t *gui_data, signal_status_t *signal_status, speed_status_t *speed_status, lights_status_t *lights_status)
{
    SDL_RenderCopy(gui_data->renderer, gui_data->base_texture, NULL, NULL);
    update_turn_signals(gui_data, signal_status);
    update_speed(gui_data, speed_status);
    update_lights(gui_data, lights_status);

    SDL_RenderPresent(gui_data->renderer);
}

gui_data_t setup_gui()
{
    gui_data_t gui_data;
    char data_file[DATA_FILE_SIZE];

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Could not initializes\n");
        exit(40);
    }
    gui_data.window = SDL_CreateWindow("ARAMobile", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                       SDL_WINDOW_SHOWN);
    if (gui_data.window == NULL)
    {
        printf("Window could not be shown\n");
    }
    gui_data.renderer = SDL_CreateRenderer(gui_data.window, -1, 0);
    gui_data.image = IMG_Load(get_data("dashboard.png", data_file));
    gui_data.needle = IMG_Load(get_data("needle.png", data_file));
    gui_data.off_left_signal = IMG_Load(get_data("off_left_signal.png", data_file));
    gui_data.off_right_signal = IMG_Load(get_data("off_right_signal.png", data_file));
    gui_data.on_left_signal = IMG_Load(get_data("on_left_signal.png", data_file));
    gui_data.on_right_signal = IMG_Load(get_data("on_right_signal.png", data_file));
    gui_data.base_texture = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.image);
    gui_data.needle_tex = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.needle);
    gui_data.off_left_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.off_left_signal);
    gui_data.off_right_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.off_right_signal);
    gui_data.on_left_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.on_left_signal);
    gui_data.on_right_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.on_right_signal);

    gui_data.speed_rect.x = 285;
    gui_data.speed_rect.y = 373;
    gui_data.speed_rect.h = gui_data.needle->h / 2;
    gui_data.speed_rect.w = gui_data.needle->w / 2;

    return gui_data;
}

void cleanup_gui_data(gui_data_t *gui_data)
{
    SDL_DestroyTexture(gui_data->base_texture);
    SDL_DestroyTexture(gui_data->needle_tex);
    SDL_DestroyTexture(gui_data->off_left_signal_tex);
    SDL_DestroyTexture(gui_data->off_right_signal_tex);
    SDL_DestroyTexture(gui_data->on_left_signal_tex);
    SDL_DestroyTexture(gui_data->on_right_signal_tex);
    SDL_FreeSurface(gui_data->image);
    SDL_FreeSurface(gui_data->needle);
    SDL_FreeSurface(gui_data->off_left_signal);
    SDL_FreeSurface(gui_data->off_right_signal);
    SDL_FreeSurface(gui_data->on_left_signal);
    SDL_FreeSurface(gui_data->on_right_signal);
    SDL_DestroyRenderer(gui_data->renderer);
    SDL_DestroyWindow(gui_data->window);
    IMG_Quit();
    SDL_Quit();
}
