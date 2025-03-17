#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"
#include "music_player.h"

#ifdef _WIN64
#include "softPwm.h"
#include "wiringPi.h"
#else
#include <softPwm.h>
#include <wiringPi.h>
#endif

const int enab_pin[] = {16, 17};		
const int inx_pin[] = {18, 19, 20, 21}; 

const int right_ahead_pin = 21, right_back_pin = 20;
const int left_ahead_pin = 19, left_back_pin = 18;
const int ena_pin = 16;
const int enb_pin = 17;
const int fan_pin = 0;

#define BUFFER_SIZE 1024
#define ARG_LENGTH 200
#define BUFS PIPE_BUF

char args[ARG_LENGTH][BUFFER_SIZE];
int args_cnt;
char buf[BUFFER_SIZE];
char query_buf[BUFFER_SIZE];
int base_speed[] = {100, 80};
int target_speed[] = {100, 100};
int stop_speed = 1;

int config[15];

enum Config {
	MANUAL_CONTROL = 0,
	LINE_TRACING = 1,
	ULTRA_SONIC = 2,
	COLOR_IDENTIFICATION = 3
};

void setup() {
	for (int i = 0; i < 4; ++i) {
		pinMode(inx_pin[i], OUTPUT);
		digitalWrite(inx_pin[i], LOW);
	}
}

void front() {
	setup();
	digitalWrite(right_ahead_pin, HIGH);
	digitalWrite(left_ahead_pin, HIGH);
	logger(LOG_INFO, "Mode Switched to \"front\"");
}

void leftFront() {
	setup();
	digitalWrite(right_ahead_pin, HIGH);
	digitalWrite(left_back_pin, HIGH);
	logger(LOG_INFO, "Mode Switched to \"left front\"");
}

void rightFront() {
	setup();
	digitalWrite(left_ahead_pin, HIGH);
	digitalWrite(right_back_pin, HIGH);
	logger(LOG_INFO, "Mode Switched to \"right front\"");
}

void rear() {
	setup();
	digitalWrite(right_back_pin, HIGH);
	digitalWrite(left_back_pin, HIGH);
	logger(LOG_INFO, "Mode Switched to \"rear\"");
}

void leftRear() {
	setup();
	digitalWrite(left_ahead_pin, HIGH);
	digitalWrite(right_back_pin, HIGH);
	logger(LOG_INFO, "Mode Switched to \"left rear\"");
}

void rightRear() {
	setup();
	digitalWrite(left_back_pin, HIGH);
	digitalWrite(right_ahead_pin, HIGH);
	logger(LOG_INFO, "Mode Switched to \"right rear\"");
}

void changeSpeed() {
	softPwmWrite(ena_pin, stop_speed * base_speed[0] * target_speed[0] / 100);
	softPwmWrite(enb_pin, stop_speed * base_speed[1] * target_speed[1] / 100);
	logger(LOG_INFO, "Speed Changed.");
}

void split_buf() {
	args_cnt = 0;
	if (strlen(buf) <= 1)
		return;
	char *token;
	token = strtok(buf, " ");
	while (token != NULL) {
		strcpy(args[++args_cnt], token);
		token = strtok(NULL, " ");
	}
}

int main(int argc, char *argv[]) {

	config[MANUAL_CONTROL] = 1;

	if (argc != 1) {
		logger(LOG_INFO, "Unknown Commands.");
		return 0;
	}

	int fd = open("/home/maxmilite/app/pipe", O_RDONLY);
	int query_fd = open("/home/maxmilite/app/query_pipe", O_WRONLY);
	// int python_fd = open("/home/maxmilite/app/python_pipe", O_WRONLY);

	wiringPiSetupGpio();
	setup();
	for (int i = 0; i < 2; ++i) {
		pinMode(enab_pin[i], OUTPUT);
		softPwmCreate(enab_pin[i], target_speed[0], 100);
	}
	for (;;) {
		read(fd, buf, BUFS);
		if (strlen(buf) > 1)
			buf[strlen(buf) - 1] = 0;
		split_buf();
		if (args_cnt < 1)
			continue;
		if (strcmp(args[1], "exit") == 0) {
			exit(0);
		}
		if (strcmp(args[1], "execute") == 0) {
			if (args_cnt < 2) continue;
			if (config[MANUAL_CONTROL]) {
				if (strcmp(args[2], "f") == 0) {
					front();
				} else if (strcmp(args[2], "lf") == 0) {
					leftFront();
				} else if (strcmp(args[2], "rf") == 0) {
					rightFront();
				} else if (strcmp(args[2], "r") == 0) {
					rear();
				} else if (strcmp(args[2], "lr") == 0) {
					leftRear();
				} else if (strcmp(args[2], "rr") == 0) {
					rightRear();
				} else if (strcmp(args[2], "st") == 0) {
					setup();
					logger(LOG_INFO, "Mode Switched to \"stop\"");
				}
			}
			if (strcmp(args[2], "speed") == 0) {
				if (args_cnt < 4) continue;
				int speed = atoi(args[3]), port = atoi(args[4]);
				if (port == -1) {
					target_speed[0] = target_speed[1] = speed;
				} else
					target_speed[port] = speed;
				changeSpeed();
			} else if (strcmp(args[2], "stop_speed") == 0) {
				if (args_cnt < 3) continue;
				stop_speed = atoi(args[3]);
				changeSpeed();
			}
		} else if (strcmp(args[1], "query") == 0) {
			memset(query_buf, 0, BUFFER_SIZE);
			if (args_cnt < 2) continue;
			if (strcmp(args[2], "speed") == 0) {
				sprintf(query_buf, "%d %d\n", target_speed[0], target_speed[1]);
			} else if (strcmp(args[2], "line_tracing") == 0) {
				sprintf(query_buf, "%d\n", config[LINE_TRACING]);
			} else if (strcmp(args[2], "ultra_sonic") == 0) {
				sprintf(query_buf, "%d\n", config[ULTRA_SONIC]);
			} else if (strcmp(args[2], "color_identification") == 0) {
				sprintf(query_buf, "%d\n", config[COLOR_IDENTIFICATION]);
			} else if (strcmp(args[2], "manual") == 0) {
				sprintf(query_buf, "%d\n", config[MANUAL_CONTROL]);
			}
			write(query_fd, query_buf, BUFFER_SIZE);
		} else if (strcmp(args[1], "switch") == 0) {
			if (args_cnt < 3) continue;
			if (strcmp(args[2], "manual") == 0) {
				if (strcmp(args[3], "1") == 0) {
					if (config[MANUAL_CONTROL] == 1) continue;
					config[MANUAL_CONTROL] = 1;
				} else if (strcmp(args[3], "0") == 0) {
					if (config[MANUAL_CONTROL] == 0) continue;
					config[MANUAL_CONTROL] = 0;
				}
			} else if (strcmp(args[2], "line_tracing") == 0) {
				if (strcmp(args[3], "1") == 0) {
					if (config[LINE_TRACING] == 1) continue;
					config[LINE_TRACING] = 1;
					system("/home/maxmilite/app/line_tracing &");
				} else if (strcmp(args[3], "0") == 0) {
					if (config[LINE_TRACING] == 0) continue;
					config[LINE_TRACING] = 0;
					system("sudo killall line_tracing");
					delay(50);
					setup();
				}
			} else if (strcmp(args[2], "ultra_sonic") == 0) {
				if (strcmp(args[3], "1") == 0) {
					if (config[ULTRA_SONIC] == 1) continue;
					config[ULTRA_SONIC] = 1;
					system("/home/maxmilite/app/ultra_sonic &");
				} else if (strcmp(args[3], "0") == 0) {
					if (config[ULTRA_SONIC] == 0) continue;
					config[ULTRA_SONIC] = 0;
					system("sudo killall ultra_sonic");
					delay(200);
					setup();
				}
			} else if (strcmp(args[2], "color_identification") == 0) {
					// write(python_fd, "color_identification 1", BUFFER_SIZE);
				if (strcmp(args[3], "1") == 0) {
					if (config[COLOR_IDENTIFICATION] == 1) continue;
					config[COLOR_IDENTIFICATION] = 1;
				} else if (strcmp(args[3], "0") == 0) {
					if (config[COLOR_IDENTIFICATION] == 0) continue;
					config[COLOR_IDENTIFICATION] = 0;
					// write(python_fd, "color_identification 0", BUFFER_SIZE);
				}
			} else if (strcmp(args[2], "music") == 0) {
				int val = atoi(args[3]);
				play_music(val);
			} else if (strcmp(args[2], "jukebox") == 0) {
				int val = atoi(argv[3]);
				play_jukebox(val);
			}
		}
	}
}
