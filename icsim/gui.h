#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "signal.h"
#include "speed.h"
#include "lights.h"

#define DATA_DIR "./data/"
#define DATA_FILE_SIZE 256

#define SCREEN_WIDTH 692
#define SCREEN_HEIGHT 329

typedef struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *base_texture;
  SDL_Texture *needle_tex;
  SDL_Texture *sprite_tex;
  SDL_Rect speed_rect;
  SDL_Surface *image;
  SDL_Surface *needle;
  SDL_Surface *sprites;
} gui_data_t;

gui_data_t setup_gui();
void draw(gui_data_t *gui_data, signal_status_t *signal_status, speed_status_t *speed_status, lights_status_t *lights_status);
void cleanup_gui_data(gui_data_t *gui_data);

#endif
