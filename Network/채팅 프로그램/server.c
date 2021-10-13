#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define SOCKET_ERROR -1
#define BIND_ERROR -2
#define LISTEN_ERROR -3

#define PORT 12345

int fd; // 서버 소켓

bool is_valid = false; // 연결 확인 플래그
char write_buf[100] = { 0, };
char read_buf[100] = { 0, };

void* write_thread(void* client_fd)
{
	int c_handle = *(int*)client_fd;
	while (1)
	{
		memset(write_buf, 0, sizeof(read_buf));
		scanf("%s", write_buf); // 입력 받음
		if (!is_valid) { // 연결이 종료된 경우
			printf("Connection end!\n");
			break;
		}
		int write_size = write(c_handle, write_buf, strlen(write_buf) + 1);
		printf("write_size : %d\n", write_size);
		if (write_size <= 0) break; // write 오류
		sleep(1);
	}
}

void* read_thread(void* client_fd)
{
	int c_handle = *(int*)client_fd;
	while (1)
	{
		memset(read_buf, 0, sizeof(read_buf));
		int read_size = read(c_handle, read_buf, sizeof(read_buf));
		printf("read_size : %d, message : %s\n", read_size, read_buf);
		if (read_size <= 0) { // read 오류 또는 연결 종료
			printf("return read_thread\n");
			break;
		}
		sleep(1);
	}
}

int init()
{
	int ret = 0;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) return SOCKET_ERROR;

	struct sockaddr_in addr = { 0, };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = 0;

	ret = bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr));
	if (ret == -1) return BIND_ERROR;

	ret = listen(fd, 2);
	if (ret == -1) return LISTEN_ERROR;

	return ret;
}

void bye()
{
	printf("\nbye\n");
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

	struct sockaddr new_addr = { 0 };

	while (1) {
		printf("Waiting...\n");
		int new_fd = accept(fd, &new_addr, &len);
		is_valid = true; // 연결 플래그
		printf("START\n");

		pthread_t wt, rd;
		pthread_create(&wt, NULL, write_thread, &new_fd);
		pthread_create(&rd, NULL, read_thread, &new_fd);

		pthread_join(rd, NULL);
		is_valid = false; // 연결 종료 플래그
		pthread_join(wt, NULL);
		close(new_fd);
	}

	return 0;
}
