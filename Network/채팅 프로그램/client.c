#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define SOCKET_ERROR -1
#define BIND_ERROR -2
#define LISTEN_ERROR -3
#define CONNECT_ERROR -4

#define PORT 12345

int fd;

char write_buf[100] = {0,};
char read_buf[100] = {0,};

void* write_thread()
{
	while(1)
	{
		memset(write_buf, 0, sizeof(read_buf));
		scanf("%s", write_buf); // 입력

		int write_size = write(fd, write_buf, strlen(write_buf) +1);
		sleep(1);
	}
}

void* read_thread()
{
	while(1)
	{
		memset(read_buf, 0, sizeof(read_buf));
		int read_size = read(fd, read_buf, sizeof(read_buf));
		if(read_size <= 0) break;
		printf("read_size : %d, meessage : %s\n", read_size, read_buf);
		sleep(1);
	}
}

int init()
{
	int ret = 0;

	fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd == -1) return SOCKET_ERROR;

	struct sockaddr_in addr = {0,};
	addr.sin_family= AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if(ret == -1) return CONNECT_ERROR;

	return ret;
}

void bye()
{
		printf("\nBYE\n");
		shutdown(fd, SHUT_RDWR); // half close 
		close(fd);
		exit(0);
}

int main()
{
	signal(SIGINT, bye);

	int ret = init();
	if (ret < 0) {
		printf("INIT ERROR\n");
		printf("Error : %d\n", ret);
		exit(1);
	}

	pthread_t wt, rd;
	pthread_create(&wt, NULL, write_thread, NULL);
	pthread_create(&rd, NULL, read_thread, NULL);

	pthread_join(wt, NULL);
	pthread_join(rd, NULL);
	
	return 0;
}
