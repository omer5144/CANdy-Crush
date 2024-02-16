#ifndef UTIL_H
#define UTIL_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DATA_DIR "./data/"
#define CAN_TRAFFIC_FILE_PATH DATA_DIR "sample-can.log"

pid_t create_can_traffic_process(char *interface_name);
int create_can_socket(char *interface_name);
void send_pkt(int mtu, struct canfd_frame *cf, int sock);
void cleanup_can_socket(int sock);
void cleanup_can_traffic_process();
int is_process_running(pid_t pid);

#endif
