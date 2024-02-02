#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include "util.h"
#include "signals.h"
#include "speed.h"
#include "lights.h"
#include "gui.h"

void usage(char *msg)
{
	if (msg)
		printf("%s\n", msg);
	printf("Usage: controls <can>\n");
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
			usage(NULL);
			break;
		}
	}

	if (optind >= argc)
		usage("You must specify at least one can device");
	
	return argv[optind];
}

void main_loop(gui_data_t gui_data, int sock)
{
	int running = 1;
	SDL_Event event;
	speed_state_t speed_state = {0, 0, 0};
	signal_state_t signal_state = {0, 0, 0};
	lights_state_t lights_state = {0, VOLUME_NONE, VOLUME_NONE};
	int current_time;

	while (running)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_RESIZED:
					redraw_screen(gui_data.base_texture, gui_data.renderer);
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					speed_state.throttle = ACCEL_THROTTLE;
					break;
				case SDLK_LEFT:
					signal_state.turn = LEFT_TURN;
					break;
				case SDLK_RIGHT:
					signal_state.turn = RIGHT_TURN;
					break;
				case SDLK_1:
					lights_state.new_lights = VOLUME_LOW;
					break;
				case SDLK_2:
					lights_state.new_lights = VOLUME_MEDIUM;
					break;
				case SDLK_3:
					lights_state.new_lights = VOLUME_HIGH;
					break;
				break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					speed_state.throttle = DECCEL_THROTTLE;
					break;
				case SDLK_LEFT:
				case SDLK_RIGHT:
					signal_state.turn = NO_TURN;
					break;
				case SDLK_1:
				case SDLK_2:
				case SDLK_3:
					lights_state.new_lights = VOLUME_NONE;
					break;
				}
				break;
			}
		}
		current_time = SDL_GetTicks();
		check_accel(sock, current_time, &speed_state);
		check_turn_signal(sock, current_time, &signal_state);
		check_lights(sock, &lights_state);
		SDL_Delay(5);
	}
}

void cleanup(gui_data_t gui_data, int sock)
{
	cleanup_can_socket(sock);
	cleanup_gui(gui_data);
	cleanup_can_traffic_process();
}

int main(int argc, char *argv[])
{	
	char *interface_name = NULL;
	int sock = -1;
	gui_data_t gui_data ={NULL, NULL, NULL, NULL};

	interface_name = parse_arguments(argc, argv);
	create_can_traffic_process(interface_name);
	sock = create_can_socket(interface_name);
	gui_data = setup_gui(&gui_data);	
	
	main_loop(gui_data, sock);
	cleanup(gui_data, sock);

	return 0;
}
