#include "util.h"

pid_t traffic_pid = -1;

void kill_child(int sig)
{
	if (sig == SIGALRM)
	{
		kill(traffic_pid, SIGKILL);
	}
}

pid_t create_can_traffic_process(char *interface_name, int is_random)
{
	char can2can[50];

	signal(SIGALRM, (void (*)(int))kill_child);
	traffic_pid = fork();
	if (traffic_pid == -1)
	{
		perror("Couldn't fork bg player\n");
		exit(-1);
	}
	else if (traffic_pid == 0)
	{
		if (is_random)
		{
			snprintf(can2can, 49, "%s=can0", interface_name);
			if (execlp("canplayer", "canplayer", "-I", CAN_TRAFFIC_FILE_PATH, "-l", "i", can2can, NULL) == -1)
			{
				printf("WARNING: Could not execute canplayer. No bg data\n");
			}
			exit(0);
		}
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
