#include "gui.h"

void redraw_screen(gui_data_t *gui_data)
{
	SDL_RenderCopy(gui_data->renderer, gui_data->base_texture, NULL, NULL);
	SDL_RenderPresent(gui_data->renderer);
}

char *get_data(char *fname, char *data_file)
{
	if (strlen(DATA_DIR) + strlen(fname) > DATA_FILE_SIZE - 1)
		return NULL;
	strncpy(data_file, DATA_DIR, DATA_FILE_SIZE - 1);
	strncat(data_file, fname, DATA_FILE_SIZE - 1 - strlen(data_file));
	return data_file;
}

gui_data_t setup_gui()
{
	gui_data_t gui_data = {NULL, NULL, NULL, NULL};
	SDL_Surface *icon;
	char data_file[DATA_FILE_SIZE];
    SDL_SysWMinfo wmInfo;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		perror("SDL Could not initializes\n");
		goto error;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        perror("IMG_Init");
        goto error;
    }

	gui_data.window = SDL_CreateWindow("ILANehiga Keyboard", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gui_data.window == NULL)
	{
		perror("Window could not be shown\n");
		goto error;
	}

	icon = IMG_Load(get_data("keyboard_icon.png", data_file));
    if (icon == NULL)
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

	gui_data.renderer = SDL_CreateRenderer(gui_data.window, -1, 0);
	gui_data.image = IMG_Load(get_data("keyboard_tutorial.png", data_file));
	gui_data.base_texture = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.image);
	SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, NULL, NULL);
	SDL_RenderPresent(gui_data.renderer);

	goto success;

error:
	SDL_FreeSurface(icon);
	cleanup_gui(&gui_data);
	exit(0);
	
success:
	SDL_FreeSurface(icon);

	return gui_data;
}

void cleanup_gui(gui_data_t *gui_data)
{
	SDL_DestroyTexture(gui_data->base_texture);
	SDL_FreeSurface(gui_data->image);
	SDL_DestroyRenderer(gui_data->renderer);
	SDL_DestroyWindow(gui_data->window);
	IMG_Quit();
	SDL_Quit();
}
