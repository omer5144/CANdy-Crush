#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_syswm.h>
#include "util.h"

#define SCREEN_WIDTH 909
#define SCREEN_HEIGHT 454

#define DATA_FILE_SIZE 256

typedef struct
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *image;
	SDL_Texture *base_texture;
} gui_data_t;

void redraw_screen(gui_data_t *gui_data);
gui_data_t setup_gui();
void cleanup_gui(gui_data_t *gui_data);

#endif
