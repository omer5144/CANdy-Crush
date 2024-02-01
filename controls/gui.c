#include "gui.h"

void redraw_screen(SDL_Texture *base_texture, SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, base_texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

char *get_data(char *fname, char *data_file)
{
	if (strlen(DATA_DIR) + strlen(fname) > DATA_FILE_SIZE - 1)
		return NULL;
	strncpy(data_file, DATA_DIR, 255);
	strncat(data_file, fname, 255 - strlen(data_file));
	return data_file;
}

gui_data_t setup_gui()
{
	gui_data_t gui_data = {NULL, NULL, NULL, NULL};

	char data_file[DATA_FILE_SIZE];

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		perror("SDL Could not initializes\n");
		exit(40);
	}
	
	gui_data.window = SDL_CreateWindow("CANBus Control Panel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (gui_data.window == NULL)
	{
		perror("Window could not be shown\n");
		exit(50);
	}

	gui_data.renderer = SDL_CreateRenderer(gui_data.window, -1, 0);
	gui_data.image = IMG_Load(get_data("joypad.png", data_file));
	gui_data.base_texture = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.image);
	SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, NULL, NULL);
	SDL_RenderPresent(gui_data.renderer);

	return gui_data;
}

void cleanup_gui(gui_data_t gui_data)
{
    SDL_DestroyTexture(gui_data.base_texture);
	SDL_FreeSurface(gui_data.image);
	SDL_DestroyRenderer(gui_data.renderer);
	SDL_DestroyWindow(gui_data.window);
	SDL_Quit();
}
