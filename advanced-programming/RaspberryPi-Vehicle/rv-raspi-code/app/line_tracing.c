#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <softPwm.h>
#include <wiringPi.h>

#define BUFFER_SIZE 1024

const int left_pin[2] = {4, 5};
const int right_pin[2] = {12, 13};
int fd;

void t_up() {
	write(fd, "execute f\n", BUFFER_SIZE);
}

void t_stop() {
	write(fd, "execute st\n", BUFFER_SIZE);
}

void t_left() {
	write(fd, "execute lf\n", BUFFER_SIZE);
}

void t_right() {
	write(fd, "execute rf\n", BUFFER_SIZE);
}

int main(int argc, char *argv[]) {

	wiringPiSetupGpio();
	fd = open("/home/maxmilite/app/pipe", O_WRONLY);

	t_up();

	for (;;) {
		int cur = 0, abs_cur = 0;
		for (int i = 0; i < 2; ++i) {
			cur += digitalRead(right_pin[i]);
			cur -= digitalRead(left_pin[i]);
			abs_cur += digitalRead(right_pin[i]);
			abs_cur += digitalRead(left_pin[i]);
		}
		if (abs_cur == 4 && digitalRead(6) == 0) {
			t_up();
		} else if (abs_cur == 0) {
			continue;
		} else if (cur < 0) {
			t_right();
		} else if (cur > 0) { 
			t_left();
		}
		delay(10);
	}
	return 0;
}
