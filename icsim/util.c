#include "util.h"

int create_can_socket(char *interface_name, msg_data_t *msg_data)
{
    int sock = -1;
    struct ifreq ifr;
    int canfd_on = 1;

    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0)
    {
        perror("Couldn't create raw socket");
        exit(1);
    }

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
