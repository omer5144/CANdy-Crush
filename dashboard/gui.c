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
    SDL_Texture *texture = NULL;

    if (lights_status->is_on)
    {
        switch(lights_status->volume)
        {
            case 1:
                texture = gui_data->low_light_tex;
                break;
            case 2:
                texture = gui_data->medium_light_tex;
                break;
            case 3:
                texture = gui_data->high_light_tex;
                break;
        }

        SDL_RenderCopy(gui_data->renderer, texture, NULL, &gui_data->left_light_rect);
        SDL_RenderCopy(gui_data->renderer, texture, NULL, &gui_data->right_light_rect);
    }
}

void draw_radio(gui_data_t *gui_data, radio_status_t *radio_status)
{
    SDL_Color on_color = {6, 172, 14, 255}; // Green
    SDL_Color off_color = {49, 49, 49, 255}; // Gray
    SDL_Surface *song_surface, *fm_surface, *am_surface;
    SDL_Texture *song_tex, *fm_tex, *am_tex;
    SDL_Rect song_rect, fm_rect, am_rect;
    
    SDL_SetRenderDrawColor(gui_data->renderer, 49, 49, 49, 255); // Gray
    SDL_RenderFillRect(gui_data->renderer, &gui_data->radio_frame_rect);
    SDL_SetRenderDrawColor(gui_data->renderer, 20, 20, 20, 255); // Dark Gray
    SDL_RenderFillRect(gui_data->renderer, &gui_data->radio_data_rect);

    song_surface = TTF_RenderText_Solid(gui_data->font_big, radio_status->song_name, on_color);

    switch (radio_status->radio_type)
    {
    case RADIO_FM:
        fm_surface = TTF_RenderText_Solid(gui_data->font_small, "FM", on_color);
        am_surface = TTF_RenderText_Solid(gui_data->font_small, "AM", off_color);
        break;
    case RADIO_AM:
        fm_surface = TTF_RenderText_Solid(gui_data->font_small, "FM", off_color);
        am_surface = TTF_RenderText_Solid(gui_data->font_small, "AM", on_color);
        break;
    case RADIO_OTHER:
        fm_surface = TTF_RenderText_Solid(gui_data->font_small, "FM", off_color);
        am_surface = TTF_RenderText_Solid(gui_data->font_small, "AM", off_color);
        break;
    }

    song_tex = SDL_CreateTextureFromSurface(gui_data->renderer, song_surface);
    fm_tex = SDL_CreateTextureFromSurface(gui_data->renderer, fm_surface);
    am_tex = SDL_CreateTextureFromSurface(gui_data->renderer, am_surface);

    song_rect.x = gui_data->radio_frame_rect.x + (gui_data->radio_frame_rect.w - song_surface->w) / 2;
    song_rect.y = gui_data->radio_frame_rect.y + (gui_data->radio_frame_rect.h - song_surface->h) / 2;
    song_rect.w = song_surface->w;
    song_rect.h = song_surface->h;

    fm_rect.x = gui_data->radio_frame_rect.x + 10;
    fm_rect.y = gui_data->radio_frame_rect.y + 10;
    fm_rect.w = fm_surface->w;
    fm_rect.h = fm_surface->h;

    am_rect.x = gui_data->radio_frame_rect.x + gui_data->radio_frame_rect.w - am_surface->w - 10;
    am_rect.y = gui_data->radio_frame_rect.y + 10;
    am_rect.w = am_surface->w;
    am_rect.h = am_surface->h;

    SDL_RenderCopy(gui_data->renderer, song_tex, NULL, &song_rect);
    SDL_RenderCopy(gui_data->renderer, fm_tex, NULL, &fm_rect);
    SDL_RenderCopy(gui_data->renderer, am_tex, NULL, &am_rect);

    SDL_FreeSurface(song_surface);
    SDL_FreeSurface(fm_surface);
    SDL_FreeSurface(am_surface);
    SDL_DestroyTexture(song_tex);
    SDL_DestroyTexture(fm_tex);
    SDL_DestroyTexture(am_tex);
}

void draw_doors(gui_data_t *gui_data, doors_status_t *doors_status)
{    
    SDL_RenderCopy(gui_data->renderer, gui_data->doors_tex, NULL, &gui_data->doors_rect);

    if (doors_status->front_left)
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->left_door_tex, NULL, &gui_data->front_left_door_rect);
    }
    if (doors_status->front_right)
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->right_door_tex, NULL, &gui_data->front_right_door_rect);
    }
    if (doors_status->back_left)
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->left_door_tex, NULL, &gui_data->back_left_door_rect);
    }
    if (doors_status->back_right)
    {
        SDL_RenderCopy(gui_data->renderer, gui_data->right_door_tex, NULL, &gui_data->back_right_door_rect);
    }
}

void draw(gui_data_t *gui_data, signals_status_t *signal_status, speed_status_t *speed_status, lights_status_t *lights_status, radio_status_t *radio_status, doors_status_t* doors_status)
{
    SDL_RenderCopy(gui_data->renderer, gui_data->dashboard_tex, NULL, NULL);

    draw_signals(gui_data, signal_status);
    draw_speed(gui_data, speed_status);
    draw_lights(gui_data, lights_status);
    draw_radio(gui_data, radio_status);
    draw_doors(gui_data, doors_status);

    SDL_RenderPresent(gui_data->renderer);
}

gui_data_t setup_gui()
{
    gui_data_t gui_data;
    char data_file[DATA_FILE_SIZE];
    SDL_Surface *dashboard, *needle, *off_left_signal, *off_right_signal, *on_left_signal, *on_right_signal, *low_light, *medium_light, *high_light, *icon, *doors, *left_door, *right_door;
    SDL_SysWMinfo wmInfo;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        perror("SDL_Init");
        goto error;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        perror("IMG_Init");
        goto error;
    }

    if (TTF_Init() < 0)
    {
        perror("TTF_Init");
        goto error;
    }
    
    gui_data.window = SDL_CreateWindow("ARAMobile Dashboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
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
    low_light = IMG_Load(get_data("low_light.png", data_file));
    medium_light = IMG_Load(get_data("medium_light.png", data_file));
    high_light = IMG_Load(get_data("high_light.png", data_file));
    icon = IMG_Load(get_data("dashboard_icon.png", data_file));
    doors = IMG_Load(get_data("doors.png", data_file));
    left_door = IMG_Load(get_data("left_door.png", data_file));
    right_door = IMG_Load(get_data("right_door.png", data_file));

    if (dashboard == NULL || needle == NULL || off_left_signal == NULL || off_right_signal == NULL || on_left_signal == NULL || on_right_signal == NULL ||
        low_light == NULL || medium_light == NULL || high_light == NULL || icon == NULL || doors == NULL || left_door == NULL || right_door == NULL)
    {
        perror("IMG_Load");
        goto error;
    }

    SDL_VERSION(&wmInfo.version);
    if (SDL_GetWindowWMInfo(gui_data.window, &wmInfo) == 0) {
        perror("SDL_GetWindowWMInfo");
        goto error;
    }
    SDL_SetWindowIcon(gui_data.window, icon);

    gui_data.font_big = TTF_OpenFont(DATA_DIR "BAHNSCHRIFT.ttf", 24);
    gui_data.font_small = TTF_OpenFont(DATA_DIR "BAHNSCHRIFT.ttf", 18);
    if (gui_data.font_big == NULL || gui_data.font_small == NULL)
    {
        perror("TTF_OpenFont");
        goto error;
    }

    gui_data.dashboard_tex = SDL_CreateTextureFromSurface(gui_data.renderer, dashboard);
    gui_data.needle_tex = SDL_CreateTextureFromSurface(gui_data.renderer, needle);
    gui_data.off_left_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, off_left_signal);
    gui_data.off_right_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, off_right_signal);
    gui_data.on_left_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, on_left_signal);
    gui_data.on_right_signal_tex = SDL_CreateTextureFromSurface(gui_data.renderer, on_right_signal);
    gui_data.low_light_tex = SDL_CreateTextureFromSurface(gui_data.renderer, low_light);
    gui_data.medium_light_tex = SDL_CreateTextureFromSurface(gui_data.renderer, medium_light);
    gui_data.high_light_tex = SDL_CreateTextureFromSurface(gui_data.renderer, high_light);
    gui_data.doors_tex = SDL_CreateTextureFromSurface(gui_data.renderer, doors);
    gui_data.left_door_tex = SDL_CreateTextureFromSurface(gui_data.renderer, left_door);
    gui_data.right_door_tex = SDL_CreateTextureFromSurface(gui_data.renderer, right_door);

    if (gui_data.dashboard_tex == NULL || gui_data.needle_tex == NULL || gui_data.off_left_signal_tex == NULL ||
        gui_data.off_right_signal_tex == NULL || gui_data.on_left_signal_tex == NULL || gui_data.on_right_signal_tex == NULL ||
        gui_data.low_light_tex == NULL || gui_data.medium_light_tex == NULL || gui_data.high_light_tex == NULL ||
        gui_data.doors_tex == NULL || gui_data.left_door_tex == NULL || gui_data.right_door_tex == NULL)
    {
        perror("SDL_CreateTextureFromSurface");
        goto error;
    }

    gui_data.speed_rect.x = SCREEN_WIDTH / 3.5;
    gui_data.speed_rect.y = SCREEN_HEIGHT / 1.26;
    gui_data.speed_rect.w = needle->w / 2;
    gui_data.speed_rect.h = needle->h / 2;
    gui_data.speed_center_rect.x = gui_data.speed_rect.w * 0.85;
    gui_data.speed_center_rect.y = gui_data.speed_rect.h / 2;
    gui_data.left_signal_rect.x = SCREEN_WIDTH * 0.15;
    gui_data.left_signal_rect.y = SCREEN_HEIGHT * 0.12;
    gui_data.left_signal_rect.w = off_left_signal->w / 2;
    gui_data.left_signal_rect.h = off_left_signal->h / 2;
    gui_data.right_signal_rect.x = SCREEN_WIDTH * 0.85 - gui_data.left_signal_rect.w;
    gui_data.right_signal_rect.y = SCREEN_HEIGHT * 0.12;
    gui_data.right_signal_rect.w = off_right_signal->w / 2;
    gui_data.right_signal_rect.h = off_right_signal->h / 2;
    gui_data.left_light_rect.x = SCREEN_WIDTH * 0.075;
    gui_data.left_light_rect.y = SCREEN_HEIGHT * 0.35;
    gui_data.left_light_rect.w = low_light->w / 2;
    gui_data.left_light_rect.h = low_light->h / 2;
    gui_data.right_light_rect.x = SCREEN_WIDTH * 0.925 - gui_data.left_light_rect.w;
    gui_data.right_light_rect.y = SCREEN_HEIGHT * 0.35;
    gui_data.right_light_rect.w = low_light->w / 2;
    gui_data.right_light_rect.h = low_light->h / 2;
    gui_data.radio_frame_rect.x = SCREEN_WIDTH * 0.3;
    gui_data.radio_frame_rect.y = SCREEN_HEIGHT * 0.056;
    gui_data.radio_frame_rect.w = SCREEN_WIDTH * 0.4;
    gui_data.radio_frame_rect.h = SCREEN_HEIGHT * 0.17;
    gui_data.radio_data_rect.x = gui_data.radio_frame_rect.x + 5;
    gui_data.radio_data_rect.y = gui_data.radio_frame_rect.y + 5;
    gui_data.radio_data_rect.w = gui_data.radio_frame_rect.w - 10;
    gui_data.radio_data_rect.h = gui_data.radio_frame_rect.h - 10;
    gui_data.doors_rect.x = SCREEN_WIDTH * 0.1;
    gui_data.doors_rect.y = SCREEN_WIDTH * 0.75;
    gui_data.doors_rect.w = doors->w;
    gui_data.doors_rect.h = doors->h;

    gui_data.doors_rect.x = SCREEN_WIDTH * 0.1;
    gui_data.doors_rect.y = SCREEN_WIDTH * 0.75;
    gui_data.doors_rect.w = doors->w;
    gui_data.doors_rect.h = doors->h;

    gui_data.front_left_door_rect.x = SCREEN_WIDTH * 0.1;
    gui_data.front_left_door_rect.y = SCREEN_WIDTH * 0.75;
    gui_data.front_left_door_rect.w = left_door->w;
    gui_data.front_left_door_rect.h = left_door->h;

    gui_data.front_right_door_rect.x = SCREEN_WIDTH * 0.2;
    gui_data.front_right_door_rect.y = SCREEN_WIDTH * 0.75;
    gui_data.front_right_door_rect.w = right_door->w;
    gui_data.front_right_door_rect.h = right_door->h;

    gui_data.back_left_door_rect.x = SCREEN_WIDTH * 0.1;
    gui_data.back_left_door_rect.y = SCREEN_WIDTH * 0.85;
    gui_data.back_left_door_rect.w = left_door->w;
    gui_data.back_left_door_rect.h = left_door->h;

    gui_data.back_right_door_rect.x = SCREEN_WIDTH * 0.2;
    gui_data.back_right_door_rect.y = SCREEN_WIDTH * 0.85;
    gui_data.back_right_door_rect.w = right_door->w;
    gui_data.back_right_door_rect.h = right_door->h;

    SDL_FreeSurface(dashboard);
    SDL_FreeSurface(needle);
    SDL_FreeSurface(off_left_signal);
    SDL_FreeSurface(off_right_signal);
    SDL_FreeSurface(on_left_signal);
    SDL_FreeSurface(on_right_signal);
    SDL_FreeSurface(low_light);
    SDL_FreeSurface(medium_light);
    SDL_FreeSurface(high_light);
    SDL_FreeSurface(icon);
    SDL_FreeSurface(doors);
    SDL_FreeSurface(left_door);
    SDL_FreeSurface(right_door);

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
    SDL_DestroyTexture(gui_data->low_light_tex);
    SDL_DestroyTexture(gui_data->medium_light_tex);
    SDL_DestroyTexture(gui_data->high_light_tex);
    SDL_DestroyTexture(gui_data->doors_tex);
    SDL_DestroyTexture(gui_data->left_door_tex);
    SDL_DestroyTexture(gui_data->right_door_tex);

    TTF_CloseFont(gui_data->font_big);
    TTF_CloseFont(gui_data->font_small);

    SDL_DestroyRenderer(gui_data->renderer);
    SDL_DestroyWindow(gui_data->window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
