#ifndef BEEP_H
#define BEEP_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>
#include "util.h"

#define BEEP_ID 101
#define BEEP_POSITION 0
#define BEEP_LEN 8

void send_beep(int sock, int beep_state);

#endif