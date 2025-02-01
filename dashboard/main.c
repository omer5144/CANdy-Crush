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
#include <errno.h>
#include <stddef.h>
#include "util.h"
#include "gui.h"
#include "signals.h"
#include "speed.h"
#include "lights.h"
#include "radio.h"
#include "doors.h"
#include "beep.h"
#include "temperature.h"

void Usage(char *msg)
{
    if (msg)
        printf("%s\n", msg);
    printf("Usage: dashboard <can>\n");
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
    gui_data_t gui_data;
    int sock = -1;
    msg_data_t msg_data;
    signals_status_t signal_status = {0, 0};
    speed_status_t speed_status = {0};
    lights_status_t lights_status = {0, 0};
    radio_status_t radio_status = {0, RADIO_OTHER, "----------"};
    doors_status_t doors_status = {0, 0, 0, 0};
    int beep_status = 0;
    temperature_status_t temperature_status = {18};

    char *songs[] = {"Sugar, Sugar",
                    "Candy",
                    "Candyman",
                    "Lollipop",
                    "I Want Candy",
                    "Candy Shop",
                    "Candy Everybody Wants",
                    "Candy Kisses",
                    "Candy Says",
                    "Candy Girl",
                    "Candy Rain",
                    "Cotton Candy",
                    "Candy Store Rock",
                    "Rock Candy",
                    "Candy Walls",
                    "Candy's Room",
                    "Candy-O",
                    "Candy Perfume Girl",
                    "Candy's Going Bad",
                    "Candyman Blues"};

    interface_name = parse_arguments(argc, argv);
    gui_data = setup_gui();
    sock = create_can_socket(interface_name, &msg_data);
    if (sock < 0)
    {
        exit(1);
    }

    int running = 1;
    SDL_Event event;
    int nbytes, maxdlen;
    int is_changed = 1;

    draw(&gui_data, &signal_status, &speed_status, &lights_status, &radio_status, &doors_status, beep_status, &temperature_status);
    
    while (running)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                goto cleanup;
            }
            SDL_Delay(5);
        }

        nbytes = recvmsg(sock, &msg_data.msg, MSG_DONTWAIT);
        if (nbytes < 0)
        {
            if (errno == EAGAIN)
            {
                continue;
            }
            else
            {
                fprintf(stderr, "read failed\n");
                running = 0;
                goto cleanup;
            }
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
            update_signals(&msg_data.frame, maxdlen, &signal_status);
            break;
        case SPEED_ID:
            update_speed(&msg_data.frame, maxdlen, &speed_status);
            break;
        case LIGHTS_IS_ON_ID:
        case LIGHTS_VOLUME_ID:
            update_lights(&msg_data.frame, maxdlen, &lights_status);
            break;
        case RADIO_ID:
            update_radio(&msg_data.frame, maxdlen, &radio_status, songs);
            break;
        case DOORS_ID:
            update_doors(&msg_data.frame, maxdlen, &doors_status);
            break;
        case BEEP_ID:
            update_beep(&msg_data.frame, maxdlen, &beep_status);
            break;
        case TEMPERATURE_ID:
            update_temperature(&msg_data.frame, maxdlen, &temperature_status);
            break;
        default:
            is_changed = 0;
        }

        if (is_changed)
        {
            draw(&gui_data, &signal_status, &speed_status, &lights_status, &radio_status, &doors_status, beep_status, &temperature_status);
        }
    }

cleanup:
    cleanup_gui(&gui_data);
    close(sock);

    return 0;
}
