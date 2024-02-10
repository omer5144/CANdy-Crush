#ifndef UTIL_H
#define UTIL_H

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
  struct msghdr msg;
  struct canfd_frame frame;
  struct sockaddr_can addr;
  struct iovec iov;
  char ctrlmsg[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(__u32))];
} msg_data_t;

int create_can_socket(char *interface_name, msg_data_t *msg_data);

#endif
