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

void draw_signals(gui_data_t *gui_data, signals_status_t *signal_status)
{    
    if (signal_status->left)
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->on_left_signal_tex, NULL, &gui_data->left_signal_rect);
    }
    else
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->off_left_signal_tex, NULL, &gui_data->left_signal_rect);
    }
    if (signal_status->right)
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->on_right_signal_tex, NULL, &gui_data->right_signal_rect);
    }
    else
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->off_right_signal_tex, NULL, &gui_data->right_signal_rect);
    }
}

void draw_speed(gui_data_t *gui_data, speed_status_t *speed_status)
{
    double angle = 0;

    angle = map(speed_status->speed, 0, 280, 0, 180);
    if (angle < 0)
    {
        angle = 0;
    }
    if (angle > 180)
    {
        angle = 180;
    }

    SDL_RenderCopyEx(gui_data->renderer, gui_data->needle_tex, NULL, &gui_data->speed_rect, angle, &gui_data->speed_center_rect, SDL_FLIP_NONE);
}

void draw_lights(gui_data_t *gui_data, lights_status_t *lights_status)
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

void draw(gui_data_t *gui_data, signals_status_t *signal_status, speed_status_t *speed_status, lights_status_t *lights_status)
{
    SDL_RenderCopy(gui_data->renderer, gui_data->dashboard_tex, NULL, NULL);

    draw_signals(gui_data, signal_status);
    draw_speed(gui_data, speed_status);
    draw_lights(gui_data, lights_status);

    SDL_RenderPresent(gui_data->renderer);
}

gui_data_t setup_gui()
{
    gui_data_t gui_data;
    char data_file[DATA_FILE_SIZE];
    SDL_Surface *dashboard, *needle, *off_left_signal, *off_right_signal, *on_left_signal, *on_right_signal;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        perror("SDL_Init");
        goto error;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        perror("SDL_Init");
        goto error;
    }
    
    gui_data.window = SDL_CreateWindow("ARAMobile", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                       SDL_WINDOW_SHOWN);
    if (gui_data.window == NULL)
    {
        perror("SDL_CreateWindow");
        goto error;
    }

    gui_data.renderer = SDL_CreateRenderer(gui_data.window, -1, 0);
    if (gui_data.renderer == NULL){
        perror("SDL_CreateRenderer");
        goto error;
    }
    
    dashboard = IMG_Load(get_data("dashboard.png", data_file));
    needle = IMG_Load(get_data("needle.png", data_file));
    off_left_signal = IMG_Load(get_data("off_left_signal.png", data_file));
    off_right_signal = IMG_Load(get_data("off_right_signal.png", data_file));
    on_left_signal = IMG_Load(get_data("on_left_signal.png", data_file));
    on_right_signal = IMG_Load(get_data("on_right_signal.png", data_file));
    if (dashboard == NULL || needle == NULL || off_left_signal == NULL || off_right_signal == NULL || on_left_signal == NULL || on_right_signal == NULL)
    {
        perror("IMG_Load");
        goto error;
    }

    gui_data.dashboard_tex = SDL_CreateTextureFromSurface(gui_data.renderer, dashboard);
    gui_data.needle_tex = SDL_CreateTextureFromSurface(gui_data.renderer, needle);
    gui_data.off_left_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, off_left_signal);
    gui_data.off_right_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, off_right_signal);
    gui_data.on_left_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, on_left_signal);
    gui_data.on_right_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, on_right_signal);
    if (gui_data.dashboard_tex == NULL || gui_data.needle_tex == NULL || gui_data.off_left_signal_tex == NULL ||
        gui_data.off_right_signal_tex == NULL || gui_data.on_left_signal_tex == NULL || gui_data.on_right_signal_tex == NULL)
    {
        perror("SDL_CreateTextureFromSurface");
        goto error;
    }

    gui_data.speed_rect.x = SCREEN_WIDTH / 3.5;
    gui_data.speed_rect.y = SCREEN_HEIGHT / 1.47;
    gui_data.speed_rect.w = needle->w / 2;
    gui_data.speed_rect.h = needle->h / 2;
    gui_data.speed_center_rect.x = gui_data.speed_rect.w * 0.87;
    gui_data.speed_center_rect.y = gui_data.speed_rect.h / 2;
    gui_data.left_signal_rect.x = SCREEN_WIDTH * 0.2;
    gui_data.left_signal_rect.y = SCREEN_HEIGHT * 0.1;
    gui_data.left_signal_rect.w = off_left_signal->w / 2;
    gui_data.left_signal_rect.h = off_left_signal->h / 2;
    gui_data.right_signal_rect.x = SCREEN_WIDTH * 0.8 - gui_data.left_signal_rect.w;
    gui_data.right_signal_rect.y = SCREEN_HEIGHT * 0.1;
    gui_data.right_signal_rect.w = off_right_signal->w / 2;
    gui_data.right_signal_rect.h = off_right_signal->h / 2;

    SDL_FreeSurface(dashboard);
    SDL_FreeSurface(needle);
    SDL_FreeSurface(off_left_signal);
    SDL_FreeSurface(off_right_signal);
    SDL_FreeSurface(on_left_signal);
    SDL_FreeSurface(on_right_signal);

    goto success;

error:
    cleanup_gui(&gui_data);
    exit(1);

success:
    return gui_data;
}

void cleanup_gui(gui_data_t *gui_data)
{
    SDL_DestroyTexture(gui_data->dashboard_tex);
    SDL_DestroyTexture(gui_data->needle_tex);
    SDL_DestroyTexture(gui_data->off_left_signal_tex);
    SDL_DestroyTexture(gui_data->off_right_signal_tex);
    SDL_DestroyTexture(gui_data->on_left_signal_tex);
    SDL_DestroyTexture(gui_data->on_right_signal_tex);

    SDL_DestroyRenderer(gui_data->renderer);
    SDL_DestroyWindow(gui_data->window);

    IMG_Quit();
    SDL_Quit();
}
