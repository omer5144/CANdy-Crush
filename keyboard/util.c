#include "util.h"

pid_t traffic_pid = -1;

void kill_child(int sig)
{
	if (sig == SIGALRM)
	{
		kill(traffic_pid, SIGKILL);
	}
}

pid_t create_can_traffic_process(char *interface_name, int is_log)
{
	char can2can[50];
	int sock;
	struct canfd_frame cf;
	random_option_t options[] = {
		{0x166, 4, 4},
		{0x158, 8, 1},
		{0x161, 8, 6},
		{0x191, 7, 7},
		{0x18E, 3, 1},
		{0x133, 5, 1},
		{0x136, 8, 7},
		{0x13A, 8, 1},
		{0x13F, 8, 5},
		{0x164, 8, 6},
		{0x17C, 8, 4},
		{0x183, 8, 5},
		{0x039, 2, 1},
		{0x143, 4, 4},
		{0x095, 8, 8},
		{0x1CF, 6, 6},
		{0x1DC, 4, 4},
		{0x320, 3, 1},
		{0x324, 8, 8},
		{0x37C, 8, 8},
		{0x1A4, 8, 5},
		{0x1AA, 8, 8},
		{0x1B0, 7, 6},
		{0x1D0, 8, 1},
		{0x294, 8, 8},
		{0x21E, 7, 7},
		{0x309, 8, 1},
		{0x333, 7, 1},
		{0x305, 2, 2},
		{0x40C, 8, 8},
		{0x454, 3, 3},
		{0x428, 7, 7},
		{0x405, 8, 6},
		{0x5A1, 8, 8},
	};

	signal(SIGALRM, (void (*)(int))kill_child);
	traffic_pid = fork();
	if (traffic_pid == -1)
	{
		perror("Couldn't fork bg player\n");
		exit(-1);
	}
	else if (traffic_pid == 0)
	{
		if (is_log)
		{
			snprintf(can2can, 49, "%s=can0", interface_name);
			if (execlp("canplayer", "canplayer", "-I", CAN_TRAFFIC_FILE_PATH, "-l", "i", can2can, NULL) == -1)
			{
				printf("WARNING: Could not execute canplayer. No bg data\n");
			}
		}
		else
		{
			srand(time(NULL));

			sock = create_can_socket(interface_name);
			while (1) {
				memset(&cf, 0, sizeof(cf));
				r = rand() % 34;
				cf.can_id = options[r].id;
				cf.len = options[r].size;
				for (int i = 0; i < options[r].len; i++) {
					cf.data[i] = rand() % 256;
				}

				send_pkt(CAN_MTU, &cf, sock);
				usleep((rand() % 91 + 10) * 1000);
			}

			cleanup_can_socket(sock);
		}
		exit(0);
	}

	return traffic_pid;
}

int create_can_socket(char *interface_name)
{
	int sock;
	struct sockaddr_can addr;
	struct ifreq ifr;
	int enable_canfd = 1;

	if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		perror("socket");
		return 1;
	}

	addr.can_family = AF_CAN;

	strcpy(ifr.ifr_name, interface_name);
	if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
	{
		perror("SIOCGIFINDEX");
		return 1;
	}
	addr.can_ifindex = ifr.ifr_ifindex;

	if (setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FD_FRAMES,
				   &enable_canfd, sizeof(enable_canfd)))
	{
		perror("error when enabling CAN FD support\n");
		return 1;
	}

	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return 1;
	}

	return sock;
}

void send_pkt(int mtu, struct canfd_frame *cf, int sock)
{
	if (write(sock, cf, mtu) != mtu)
	{
		perror("write");
	}
}

void cleanup_can_socket(int sock)
{
	close(sock);
}

void cleanup_can_traffic_process()
{
	kill_child(SIGALRM);
}

int is_process_running(pid_t pid)
{
	int status;
	pid_t result = waitpid(pid, &status, WNOHANG);

	if (result == 0) {
		return 1;
	} else if (result > 0) {
		return 0;
	} else {
		perror("waitpid");
		return 0;
	}
}
