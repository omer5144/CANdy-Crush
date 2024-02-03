#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "signals.h"
#include "speed.h"
#include "lights.h"

#define DATA_DIR "./data/"
#define DATA_FILE_SIZE 2048

#define SCREEN_WIDTH 990
#define SCREEN_HEIGHT 545

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *dashboard_tex;
  SDL_Texture *needle_tex;
  SDL_Texture *off_left_signal_tex;
  SDL_Texture *on_left_signal_tex;
  SDL_Texture *off_right_signal_tex;
  SDL_Texture *on_right_signal_tex;
  SDL_Texture *low_light_tex;
  SDL_Texture *medium_light_tex;
  SDL_Texture *high_light_tex;
  SDL_Rect speed_rect;
  SDL_Point speed_center_rect;
  SDL_Rect left_signal_rect;
  SDL_Rect right_signal_rect;
  SDL_Rect left_light_rect;
  SDL_Rect right_light_rect;
} gui_data_t;

gui_data_t setup_gui();
void draw(gui_data_t *gui_data, signals_status_t *signal_status, speed_status_t *speed_status, lights_status_t *lights_status);
void cleanup_gui(gui_data_t *gui_data);

#endif
