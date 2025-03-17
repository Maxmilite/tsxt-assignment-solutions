#include <arpa/inet.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"

#define PORT 8887
#define QUEUE 20
#define BUFFER_SIZE 1024
#define BUFS PIPE_BUF

char buf[BUFFER_SIZE];
char query_buf[BUFFER_SIZE];

int main(int argc, char *argv[]) {

	int fd = open("/home/maxmilite/app/pipe", O_WRONLY);
	int query_fd = open("/home/maxmilite/app/query_pipe", O_RDONLY);

	int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	

	logger(LOG_INFO, "Socket Server Starting");

	while (bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1) {
		logger(LOG_INFO, "Error occurred while binding 1.");
		sleep(1);
	}
	while (listen(server_sockfd, QUEUE) == -1) {
		logger(LOG_INFO, "Error occurred while binding 2.");
		sleep(1);
	}
	char buffer[BUFFER_SIZE];
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);

	int on = 1;
	setsockopt(server_sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&on, sizeof(on));
    setsockopt(server_sockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&on, sizeof(on));

	for (;;) {
		logger(LOG_INFO, "Waiting for connection. Port: 8887");
		int conn = accept(server_sockfd, (struct sockaddr *)&client_addr, &length);
		if (conn < 0) {
			perror("Error occurred while connecting.");
			continue;
		}
		logger(LOG_INFO, "Connection Established");
		for (;;) {
			memset(buffer, 0, sizeof(buffer));
			recv(conn, buffer, sizeof(buffer), 0);
			printf("[INFO] Message: %s\n", buffer);
			if (strlen(buffer) <= 3)
				break;
			if (strcmp(buffer, "exit\n") == 0)
				break;
			switch (buffer[0]) {
				case 'e':
				case 's':
					write(fd, buffer, BUFFER_SIZE);
					break;
				case 'q':
					write(fd, buffer, BUFFER_SIZE);
					read(query_fd, query_buf, BUFFER_SIZE);
					int s = send(conn, query_buf, BUFFER_SIZE, 0);
					printf("QUERY_BUF: %s\n", query_buf);
					perror("Message sent");
					usleep(100);
					break;
			}
		}
		close(conn);
	}
	close(server_sockfd);
	return 0;
}
