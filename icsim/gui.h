#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

void update_speed(gui_data_t *gui_data, long speed);
void update_turn_signals(gui_data_t *gui_data, int left_signal, int right_signal);
void update_lights(gui_data_t *gui_data, int is_on, int volume);
void draw_ic(gui_data_t *gui_data);
gui_data_t setup_gui();
void cleanup_gui_data(gui_data_t *gui_data);

#endif
