#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>

#define TRIG 22
#define ECHO 23
#define BUFFER_SIZE 1024

int fd;

void t_up(unsigned int delay_time) {
	write(fd, "execute f\n", BUFFER_SIZE);
	delay(delay_time);
}

void t_down(unsigned int delay_time) {
	write(fd, "execute r\n", BUFFER_SIZE);
	delay(delay_time);
}

void t_stop(unsigned int delay_time) {
	write(fd, "execute st\n", BUFFER_SIZE);
	delay(delay_time);
}

void t_left(unsigned int delay_time) {
	write(fd, "execute lf\n", BUFFER_SIZE);
	delay(delay_time);
}

void t_right(unsigned int delay_time) {
	write(fd, "execute rf\n", BUFFER_SIZE);
	delay(delay_time);
}

void ultraInit() {
	pinMode(ECHO, INPUT);
	pinMode(TRIG, OUTPUT);
}

float disMeasure() {
	struct timeval tv1;
	struct timeval tv2;
	long start, stop;
	float dis;

	digitalWrite(TRIG, LOW);
	delayMicroseconds(2);

	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);

	while (!(digitalRead(ECHO) == 1))
		;
	gettimeofday(&tv1, NULL);

	while (!(digitalRead(ECHO) == 0))
		;
	gettimeofday(&tv2, NULL);

	start = tv1.tv_sec * 1000000 + tv1.tv_usec;
	stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

	dis = (float)(stop - start) / 1000000 * 34990 / 2;

	return dis;
}

int main(int argc, char *argv[]) {

	float dis;
	int time = 1;
	wiringPiSetupGpio();
	fd = open("/home/maxmilite/app/pipe", O_WRONLY);
	ultraInit();

	for (;;) {
		dis = disMeasure();
		if (dis < 30) {
			t_down(300);
			t_left(300);
		} else {
			t_up(10);
		}
		delay(20);
	}
	return 0;
}
