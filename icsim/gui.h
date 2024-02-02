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

char *get_data(char *fname, char *data_file)
{
  if (strlen(DATA_DIR) + strlen(fname) > DATA_FILE_SIZE -1)
    return NULL;
  strncpy(data_file, DATA_DIR, DATA_FILE_SIZE - 1);
  strncat(data_file, fname, DATA_FILE_SIZE - 1 - strlen(data_file));
  return data_file;
}

void blank_ic(gui_data_t gui_data)
{
  SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, NULL, NULL);
}

void update_speed(gui_data_t gui_data, long speed)
{
  SDL_Rect dial_rect;
  SDL_Point center;
  double angle = 0;
  dial_rect.x = 200;
  dial_rect.y = 80;
  dial_rect.h = 130;
  dial_rect.w = 300;
  SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, &dial_rect, &dial_rect);
  /* Because it's a curve we do a smaller rect for the top */
  dial_rect.x = 250;
  dial_rect.y = 30;
  dial_rect.h = 65;
  dial_rect.w = 200;
  SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, &dial_rect, &dial_rect);
  // And one more smaller box for the pivot point of the needle
  dial_rect.x = 323;
  dial_rect.y = 171;
  dial_rect.h = 52;
  dial_rect.w = 47;
  SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, &dial_rect, &dial_rect);
  center.x = 135;
  center.y = 20;
  angle = map(speed, 0, 280, 0, 180);
  if (angle < 0)
    angle = 0;
  if (angle > 180)
    angle = 180;
  SDL_RenderCopyEx(gui_data.renderer, gui_data.needle_tex, NULL, &gui_data.speed_rect, angle, &center, SDL_FLIP_NONE);
}

void update_turn_signals(gui_data_t gui_data, int left_signal, int right_signal)
{
  SDL_Rect left, right, lpos, rpos;
  left.x = 213;
  left.y = 51;
  left.w = 45;
  left.h = 45;
  memcpy(&right, &left, sizeof(SDL_Rect));
  right.x = 482;
  memcpy(&lpos, &left, sizeof(SDL_Rect));
  memcpy(&rpos, &right, sizeof(SDL_Rect));
  lpos.x -= 22;
  lpos.y -= 22;
  rpos.x -= 22;
  rpos.y -= 22;
  if (left_signal == 0)
  {
    SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, &lpos, &lpos);
  }
  else
  {
    SDL_RenderCopy(gui_data.renderer, gui_data.sprite_tex, &left, &lpos);
  }
  if (right_signal == 0)
  {
    SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, &rpos, &rpos);
  }
  else
  {
    SDL_RenderCopy(gui_data.renderer, gui_data.sprite_tex, &right, &rpos);
  }
}

void draw_ic(gui_data_t gui_data)
{
  blank_ic(gui_data);
  update_speed(gui_data, 0);
  update_turn_signals(gui_data, 0, 0);
  SDL_RenderPresent(gui_data.renderer);
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
    gui_data.window = SDL_CreateWindow("IC Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (gui_data.window == NULL)
    {
      printf("Window could not be shown\n");
    }
    gui_data.renderer = SDL_CreateRenderer(gui_data.window, -1, 0);
    gui_data.image = IMG_Load(get_data("ic.png", data_file));
    gui_data.needle = IMG_Load(get_data("needle.png", data_file));
    gui_data.sprites = IMG_Load(get_data("spritesheet.png", data_file));
    gui_data.base_texture = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.image);
    gui_data.needle_tex = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.needle);
    gui_data.sprite_tex = SDL_CreateTextureFromSurface(gui_data.renderer, gui_data.sprites);

    gui_data.speed_rect.x = 212;
    gui_data.speed_rect.y = 175;
    gui_data.speed_rect.h = gui_data.needle->h;
    gui_data.speed_rect.w = gui_data.needle->w;

    draw_ic(gui_data);

    return gui_data;
}

void cleanup_gui_data(gui_data_t gui_data)
{
    SDL_DestroyTexture(gui_data.base_texture);
    SDL_DestroyTexture(gui_data.needle_tex);
    SDL_DestroyTexture(gui_data.sprite_tex);
    SDL_FreeSurface(gui_data.image);
    SDL_FreeSurface(gui_data.needle);
    SDL_FreeSurface(gui_data.sprites);
    SDL_DestroyRenderer(gui_data.renderer);
    SDL_DestroyWindow(gui_data.window);
    IMG_Quit();
    SDL_Quit();
}

#endif
