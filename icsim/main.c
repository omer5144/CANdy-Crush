#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define DATA_DIR "./data/" // Needs trailing slash

#define SCREEN_WIDTH 692
#define SCREEN_HEIGHT 329
#define OFF 0
#define ON 1
#define SIGNAL_ID 392 // 0x188
#define SIGNAL_POS 0
#define CAN_LEFT_SIGNAL 1
#define CAN_RIGHT_SIGNAL 2
#define SPEED_ID 580 // 0x244
#define SPEED_POS 3 // bytes 3,4

long current_speed = 0;
int turn_status[2];
char data_file[256];

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

typedef struct
{
  struct msghdr msg;
  struct canfd_frame frame;
  struct sockaddr_can addr;
  struct iovec iov;
  char ctrlmsg[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(__u32))];
} msg_data_t;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

char *get_data(char *fname)
{
  if (strlen(DATA_DIR) + strlen(fname) > 255)
    return NULL;
  strncpy(data_file, DATA_DIR, 255);
  strncat(data_file, fname, 255 - strlen(data_file));
  return data_file;
}

void init_car_state()
{
  turn_status[0] = OFF;
  turn_status[1] = OFF;
}

void blank_ic(gui_data_t gui_data)
{
  SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, NULL, NULL);
}

void update_speed(gui_data_t gui_data)
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
  angle = map(current_speed, 0, 280, 0, 180);
  if (angle < 0)
    angle = 0;
  if (angle > 180)
    angle = 180;
  SDL_RenderCopyEx(gui_data.renderer, gui_data.needle_tex, NULL, &gui_data.speed_rect, angle, &center, SDL_FLIP_NONE);
}

void update_turn_signals(gui_data_t gui_data)
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
  if (turn_status[0] == OFF)
  {
    SDL_RenderCopy(gui_data.renderer, gui_data.base_texture, &lpos, &lpos);
  }
  else
  {
    SDL_RenderCopy(gui_data.renderer, gui_data.sprite_tex, &left, &lpos);
  }
  if (turn_status[1] == OFF)
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
  update_speed(gui_data);
  update_turn_signals(gui_data);
  SDL_RenderPresent(gui_data.renderer);
}

void update_speed_status(struct canfd_frame *cf, int maxdlen, gui_data_t gui_data)
{
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if (len < SPEED_POS + 1)
    return;

  int speed = cf->data[SPEED_POS] << 8;
  speed += cf->data[SPEED_POS + 1];
  speed = speed / 100;               // speed in kilometers
  current_speed = speed * 0.6213751; // mph
  
  update_speed(gui_data);
  SDL_RenderPresent(gui_data.renderer);
}

void update_signal_status(struct canfd_frame *cf, int maxdlen, gui_data_t gui_data)
{
  int len = (cf->len > maxdlen) ? maxdlen : cf->len;
  if (len < SIGNAL_POS)
    return;
  if (cf->data[SIGNAL_POS] & CAN_LEFT_SIGNAL)
  {
    turn_status[0] = ON;
  }
  else
  {
    turn_status[0] = OFF;
  }
  if (cf->data[SIGNAL_POS] & CAN_RIGHT_SIGNAL)
  {
    turn_status[1] = ON;
  }
  else
  {
    turn_status[1] = OFF;
  }
  update_turn_signals(gui_data);
  SDL_RenderPresent(gui_data.renderer);
}

void Usage(char *msg)
{
  if (msg)
    printf("%s\n", msg);
  printf("Usage: icsim <can>\n");
  exit(1);
}

char *parse_arguments(int argc, char *argv[])
{
  int opt;

  while ((opt = getopt(argc, argv, "h?")) != -1)
  {
    switch (opt)
    {
    case 'h':
    case '?':
    default:
      Usage(NULL);
      break;
    }
  }

  if (optind >= argc)
    Usage("You must specify at least one can device");

  return argv[optind];
}

int create_can_socket(char *interface_name, msg_data_t *msg_data)
{
  int sock = -1;
  struct ifreq ifr;
  int canfd_on = 1;

  sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (sock < 0)
    Usage("Couldn't create raw socket");

  msg_data->addr.can_family = AF_CAN;
  memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
  strncpy(ifr.ifr_name, interface_name, strlen(interface_name));
  printf("Using CAN interface %s\n", ifr.ifr_name);
  if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
  {
    perror("SIOCGIFINDEX");
    exit(1);
  }
  msg_data->addr.can_ifindex = ifr.ifr_ifindex;
  setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on));

  msg_data->iov.iov_base = &msg_data->frame;
  msg_data->iov.iov_len = sizeof(msg_data->frame);
  msg_data->msg.msg_name = &msg_data->addr;
  msg_data->msg.msg_namelen = sizeof(msg_data->addr);
  msg_data->msg.msg_iov = &msg_data->iov;
  msg_data->msg.msg_iovlen = 1;
  msg_data->msg.msg_control = &msg_data->ctrlmsg;
  msg_data->msg.msg_controllen = sizeof(msg_data->ctrlmsg);
  msg_data->msg.msg_flags = 0;

  if (bind(sock, (struct sockaddr *)&msg_data->addr, sizeof(msg_data->addr)) < 0)
  {
    perror("bind");
    return 1;
  }

  return sock;
}

gui_data_t setup_gui()
{
  gui_data_t gui_data;

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
    gui_data.image = IMG_Load(get_data("ic.png"));
    gui_data.needle = IMG_Load(get_data("needle.png"));
    gui_data.sprites = IMG_Load(get_data("spritesheet.png"));
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

int main(int argc, char *argv[])
{
  char *interface_name = NULL;
  int sock = -1;
  msg_data_t msg_data;
  gui_data_t gui_data = {NULL, NULL, NULL, NULL, NULL, {0, 0, 0, 0}, NULL, NULL, NULL};

  interface_name = parse_arguments(argc, argv);
  sock = create_can_socket(interface_name, &msg_data);
  init_car_state();
  gui_data = setup_gui();
  
  int running = 1;
  SDL_Event event;
  int nbytes, maxdlen;

  while (running)
  {
    while (SDL_PollEvent(&event) != 0)
    {
      switch (event.type)
      {
      case SDL_QUIT:
        puts("simulator ended!");
        running = 0;
        break;
      }
      SDL_Delay(3);
    }

    nbytes = recvmsg(sock, &msg_data.msg, 0);
    if (nbytes < 0)
    {
      perror("read");
      running = 0;
      break;    
    }

    if ((size_t)nbytes == CAN_MTU)
      maxdlen = CAN_MAX_DLEN;
    else if ((size_t)nbytes == CANFD_MTU)
      maxdlen = CANFD_MAX_DLEN;
    else
    {
      fprintf(stderr, "read: incomplete CAN frame\n");
      running = 0;
      break;
    }

    if (msg_data.frame.can_id == SIGNAL_ID)
      update_signal_status(&msg_data.frame, maxdlen, gui_data);
    if (msg_data.frame.can_id == SPEED_ID)
      update_speed_status(&msg_data.frame, maxdlen, gui_data);
  }
  
  cleanup_gui_data(gui_data);

  return 0;
}
