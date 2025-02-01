#include <linux/can.h>
#include <linux/can/raw.h>
#include <time.h>
#include <stdlib.h>
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
#include "radio.h"
#include "doors.h"
#include "gui.h"
#include "beep.h"
#include "temperature.h"

void usage(char *msg)
{
	if (msg)
		printf("%s\n", msg);
	printf("Usage: keyboard <can>\n");
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

radio_state_t generate_random_radio()
{
	radio_state_t radio_state;
    char *songs[] = {
		"Skittles",
		"Twix",
		"KitKat",
		"Snickers",
		"Reese's",
		"M&M's",
		"MilkyWay",
		"Hershey",
		"Rolo",
		"Smarties",
		"Crunch",
		"Mentos",
		"Bounty",
		"AirHeads",
		"Pez",
		"DumDums",
		"Trident",
		"Andes",
		"Goobers",
	};
	char *song_name;
	int length;

	srand(time(NULL));
	radio_state.last_update_time = 0;
	radio_state.key = rand() % 256;
	song_name = songs[rand() % 19];
	length = strlen(song_name);
	memset(radio_state.song_name, 0, sizeof(radio_state.song_name));
	memcpy(radio_state.song_name, song_name, length);
	radio_state.song_name_length = length;
	for (int i = 0; i < length; ++i)
	{
		radio_state.song_name[i] ^= radio_state.key;
	}
	radio_state.radio_type = rand() % 2;

	return radio_state;
}

void main_loop(gui_data_t *gui_data, int sock, pid_t traffic_pid)
{
	int running = 1;
	SDL_Event event;
	speed_state_t speed_state = {0, 0, 0};
	signal_state_t signal_state = {0, 0, 0};
	lights_state_t lights_state = {0, VOLUME_NONE, VOLUME_NONE};
	radio_state_t radio_state = generate_random_radio();
	doors_state_t doors_state = {0, 0, 0, 0, 0};
	beep_state_t beep_state = {0, 0};
	temperature_state_t temperature_state = {TEMPERATURE_NONE, TEMPERATURE_NONE};
	int current_time;

	while (running)
	{
		if (is_process_running(traffic_pid) == 0) {
			running = 0;
			break;
		}
		
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
					redraw_screen(gui_data);
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
				case SDLK_SPACE:
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
				case SDLK_KP_7:
					doors_state.is_front_left_door_open = 1 - doors_state.is_front_left_door_open;
					break;
				case SDLK_KP_9:
					doors_state.is_front_right_door_open = 1 - doors_state.is_front_right_door_open;
					break;
				case SDLK_KP_1:
					doors_state.is_back_left_door_open = 1 - doors_state.is_back_left_door_open;
					break;
				case SDLK_KP_3:
					doors_state.is_back_right_door_open = 1 - doors_state.is_back_right_door_open;
					break;
				case SDLK_e:
					beep_state.is_on = 1;
					break;
				case SDLK_EQUALS:
					temperature_state.temperature = TEMPERATURE_HOT;
					break;
				case SDLK_MINUS:
					temperature_state.temperature = TEMPERATURE_COLD;
					break;
				case SDLK_0:
					temperature_state.temperature = TEMPERATURE_NONE;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
				case SDLK_SPACE:
					speed_state.throttle = DECCEL_THROTTLE;
					break;
				case SDLK_LEFT:
				case SDLK_RIGHT:
					signal_state.turn = NO_TURN;
					break;
				case SDLK_1:
				case SDLK_2:
				case SDLK_3:
				case SDLK_KP_1:
				case SDLK_KP_2:
				case SDLK_KP_3:
					lights_state.new_lights = VOLUME_NONE;
					break;
				case SDLK_e:
					beep_state.is_on = 0;
					break;
				}
				break;
			}
		}
		current_time = SDL_GetTicks();
		check_accel(sock, current_time, &speed_state);
		check_turn_signal(sock, current_time, &signal_state);
		check_lights(sock, &lights_state);
		check_radio(sock, current_time, &radio_state);
		check_doors(sock, &doors_state);
		check_beep(sock, &beep_state);
		check_temperature(sock, &temperature_state);
		SDL_Delay(5);
	}
}

void cleanup(gui_data_t *gui_data, int sock)
{
	cleanup_can_socket(sock);
	cleanup_gui(gui_data);
	cleanup_can_traffic_process();
}

int main(int argc, char *argv[])
{
	char *interface_name = NULL;
	pid_t traffic_pid;
	int sock = -1;
	gui_data_t gui_data = {NULL, NULL, NULL, NULL};

	interface_name = parse_arguments(argc, argv);
	traffic_pid = create_can_traffic_process(interface_name);
	sock = create_can_socket(interface_name);
	gui_data = setup_gui();

	main_loop(&gui_data, sock, traffic_pid);
	cleanup(&gui_data, sock);

	return 0;
}
