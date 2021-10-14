#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#define SOCKET_ERROR -1
#define BIND_ERROR -2
#define LISTEN_ERROR -3
#define CONNECT_ERROR -4

#define PORT 12345

int fd;
bool is_conn = false; // 연결 확인 플래그


char write_buf[200] = {0,};
char read_buf[200] = {0,};

typedef struct {
	int m_type; // 1: save, 2:read, 3:clear, 4:exit
	char m_body[100];
}msg_t;

int write_message(int type, const char* body)
{
	msg_t send_msg = {0,};
	send_msg.m_type = type;
	memcpy(send_msg.m_body, body, strlen(body));

	//memset(write_buf, 0, sizeof(read_buf));
	//memcpy(write_buf, (void*)&send_msg, sizeof(send_msg));
	//printf("msg type : %d, msg body : %s\n, msg size : %d\n", send_msg.m_type, send_msg.m_body, sizeof(msg_t));
	int write_size = write(fd, (void*)&send_msg, sizeof(msg_t));
	//printf("write_size : %d\n", write_size);
	return write_size;
}

int read_message()
{
	memset(read_buf, 0, sizeof(read_buf));
	int read_size = read(fd, read_buf, sizeof(read_buf));
	msg_t read_msg = {0,};
	memcpy((void*)&read_msg, read_buf, sizeof(msg_t));
	printf("read_size : %d, type : %d, body : %s\n", read_size, read_msg.m_type, read_msg.m_body);
	return read_size;	
}

int init(const char* ip)
{
	int ret = 0;

	fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd == -1) return SOCKET_ERROR;

	struct sockaddr_in addr = {0,};
	addr.sin_family= AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(ip);

	ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if(ret == -1) return CONNECT_ERROR;

	return ret;
}

void run()
{
	char input_buf[200];
	char cmd[100];
	char body[100];
	while(1) {
		memset(input_buf, 0, sizeof(input_buf));
		memset(cmd, 0, sizeof(cmd));
		memset(body, 0, sizeof(body));
		printf("SSAFY DB >> ");
		fgets(input_buf, 200, stdin);
		input_buf[strlen(input_buf) - 1] = '\0';
		//if (strcmp(input_buf, "exit") == 0) break;
		
		bool split_flag = false;
		for(int i = 0; i < strlen(input_buf); i++){
			if(input_buf[i] == ' '){
				memcpy(cmd, input_buf, i);
				memcpy(body, &input_buf[i +1], strlen(input_buf) - i - 1);
				split_flag = true;
				break;
			}
		}
		if(!split_flag) memcpy(cmd, input_buf, strlen(input_buf));
		//printf("cmd : %s\n", cmd);
		//printf("body : %s\n", body);

		if (strcmp(cmd,"connect") == 0){		
			int ret = init(body);
			if (ret < 0) {
				printf("Init error!\n");
				printf("error : %d\n", ret);
			}
			printf("연결 완료\n");
			is_conn = true;
			continue;
		}
		if (strcmp(cmd, "save") == 0) {
			int result = write_message(1, body);
			read_message();
			continue;
		}
		if (strcmp(cmd, "read") == 0) {
			int result = write_message(2, body);
			read_message();
			continue;
		}
		if (strcmp(cmd, "clear") == 0) {
			int result = write_message(3, "");
			read_message();
			continue;
		}
		if (strcmp(cmd, "exit") == 0) {
			int result = write_message(4, "");
			read_message();
			close(fd);
			is_conn = false;
			break;
		}
	}

	
}


int main()
{
	//signal(SIGINT, bye);

	run();
	printf("\nBYE\n");	
	return 0;
}
