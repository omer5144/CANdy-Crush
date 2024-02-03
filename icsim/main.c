#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "util.h"
#include "gui.h"
#include "signal.h"
#include "speed.h"
#include "lights.h"

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

int main(int argc, char *argv[])
{
    char *interface_name = NULL;
    int sock = -1;
    msg_data_t msg_data;
    gui_data_t gui_data = {NULL, NULL, NULL, NULL, NULL, {0, 0, 0, 0}, NULL, NULL, NULL};
    signal_status_t signal_status = {0, 0};
    speed_status_t speed_status = {0};
    lights_status_t lights_status = {0, 0};

    interface_name = parse_arguments(argc, argv);
    sock = create_can_socket(interface_name, &msg_data);
    gui_data = setup_gui();

    int running = 1;
    SDL_Event event;
    int nbytes, maxdlen;
    int is_changed = 1;

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
            SDL_Delay(5);
        }

        nbytes = recvmsg(sock, &msg_data.msg, 0);
        if (nbytes < 0)
        {
            perror("read");
            running = 0;
            break;
        }

        if ((size_t)nbytes == CAN_MTU)
        {
            maxdlen = CAN_MAX_DLEN;
        }
        else if ((size_t)nbytes == CANFD_MTU)
        {
            maxdlen = CANFD_MAX_DLEN;
        }
        else
        {
            fprintf(stderr, "read: incomplete CAN frame\n");
            running = 0;
            break;
        }

        is_changed = 1;
        switch (msg_data.frame.can_id)
        {
        case SIGNAL_ID:
            update_signal_status(&msg_data.frame, maxdlen, &signal_status);
            break;
        case SPEED_ID:
            update_speed_status(&msg_data.frame, maxdlen, &speed_status);
            break;
        case LIGHTS_IS_ON_ID:
        case LIGHTS_VOLUME_ID:
            update_lights_status(&msg_data.frame, maxdlen, &lights_status);
            break;
        default:
            is_changed = 0;
        }

        if (is_changed)
        {
            draw(&gui_data, &signal_status, &speed_status, &lights_status);
        }
    }

    cleanup_gui_data(&gui_data);

    return 0;
}
