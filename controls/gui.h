#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "util.h"

#ifndef GUI_H
#define GUI_H

#define SCREEN_WIDTH 835
#define SCREEN_HEIGHT 608

#define DATA_FILE_SIZE 256

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *image;
	SDL_Texture *base_texture;
} gui_data_t;

void redraw_screen(SDL_Texture *base_texture, SDL_Renderer *renderer);
gui_data_t setup_gui();
void cleanup_gui(gui_data_t gui_data);

#endif
