#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define SOCKET_ERROR -1
#define BIND_ERROR -2
#define LISTEN_ERROR -3

int fd;
int c_fd;

typedef struct {
    int m_type;
    char m_body[100];
}msg_t;

typedef struct {
	char m_key[100];
	char m_value[100];
}tuple;

tuple table[100];
int table_num = 0;

int init() {
	int ret = 0;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) return SOCKET_ERROR;

	struct sockaddr_in addr = {0,};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = 0;

	ret = bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr));
	if (ret == -1) return BIND_ERROR;

	ret = listen(fd, 1);
	if (ret == -1) return LISTEN_ERROR;

	return ret;
}

void read_db(const char* client_ip)
{
	FILE* fp = fopen(client_ip, "r");
	if (fp == NULL) return;
	char line[100] = {0,};
	while(1)
	{
		if (feof(fp) != 0) break;
		fgets(line, 100, fp);
		line[strlen(line) - 1] = '\0';
		if (strlen(line) == 0) break;
		for(int i = 0; i < strlen(line); i++){
			if (line[i] == ' '){
				memcpy(table[table_num].m_key, line, sizeof(char) * i);
				memcpy(table[table_num].m_value, &line[i + 1], sizeof(char) * (strlen(line) - 1 - i));
				table_num++;
				break;
			}
		}
		memset(line, 0, sizeof(line));
	}
	fclose(fp);
}
void write_db(const char* client_ip)
{
	FILE* fp = fopen(client_ip, "w");
	char output_str[250] = {0,};
	for(int i = 0; i < table_num; i++){
		sprintf(output_str, "%s %s\n", table[i].m_key, table[i].m_value);
		fputs(output_str, fp);
	}
	fclose(fp);
}

int write_polling(int type, const char* msg)
{
        msg_t write_msg = {0,};
        write_msg.m_type = type;
        strcpy(write_msg.m_body, msg);
        int write_size = write(c_fd, (void*)&write_msg, sizeof(msg_t));
        return write_size;
}

// 키 확인
int check_table(const char* key)
{
	for(int i = 0; i < table_num; i++)
	{
		if(strcmp(key, table[i].m_key) == 0){
			return i;
		}
	}
	return -1;
}

void save_db(const char* msg)
{
	char key[50] = {0,};
	char value[50] = {0,};
	for(int i = 0; i < strlen(msg); i++) {
		if(msg[i] == ':'){
			memcpy(key, msg, i);
			memcpy(value, &msg[i + 1], strlen(msg) - i - 1);
			break;
		}
	}
	if (check_table(key) >= 0 || strlen(value) == 0){ // 키 중복 or 입력 오류
		write_polling(1, "REJECT");
		return;
	}
	memcpy(table[table_num].m_key, key, strlen(key));
	memcpy(table[table_num].m_value, value, strlen(value));
	table_num++;
	write_polling(1, "OK");
	//printf("key : %s\n", key);
	//printf("value : %s\n", value);
}

void write_value(const char* key)
{
    int index = check_table(key);
    if(index == -1) {// 키 없음
        write_polling(2, "NO KEY");
		return;
    }
	write_polling(2, table[index].m_value);
}

void processing()
{
	char read_buf[200];
	char write_buf[200];
	while(1) {
		bool result_flag = true;
		memset(read_buf, 0, sizeof(read_buf));
		memset(write_buf, 0, sizeof(write_buf));
		int read_size = read(c_fd, read_buf, sizeof(read_buf));
		msg_t read_msg = {0,};
		memcpy((void*)&read_msg, (void*)&read_buf, sizeof(msg_t));
		
		printf("read type : %d, read body : %s, read size : %d\n", read_msg.m_type, read_msg.m_body, read_size);
		if (read_msg.m_type == 1)
		{
			// save key:value 값 저장
			save_db(read_msg.m_body);
		}
		else if(read_msg.m_type == 2)
		{
			// read key값의 value 전송
			write_value(read_msg.m_body);
		}
		else if(read_msg.m_type == 3)
		{
			// clear 모든 key, value 삭제
			memset(table, 0, sizeof(table));
			table_num = 0;
			write_polling(3, "OK");
		}
		else if (read_msg.m_type == 4) // exit
		{
			write_polling(4, "BYE");
			close(c_fd);
			return;
		}
	}
}

void run_server()
{
	int ret = init();
	if(ret < 0) {
		printf("Init error!\n");
		printf("error : %d\n", ret);
		exit(1);
	}

	struct sockaddr_in client_addr = {0,};
	int len;
	while(1) {
		printf("Waiting...\n");
		c_fd = accept(fd, (struct sockaddr*)&client_addr, &len);
		char c_ip[20] = {0,};
		strcpy(c_ip, inet_ntoa(client_addr.sin_addr));
		printf("Connect IP : %s\n", c_ip);
		read_db(c_ip);
		processing();
		write_db(c_ip);
		close(c_fd);
	}
}

void save()
{
	// key, value 저장
	printf("\nBYE\n");
	// 연결된 클라 있을 경우 종료
	
	close(fd); // 서버 종료
	exit(0);
}
int main()
{
	signal(SIGINT, save);
	run_server();
}
