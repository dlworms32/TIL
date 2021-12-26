#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define BUF_SIZE 300
#define LOG "kernel_log.txt"

void get_kernel_log();
int print_kernel_log(const char*);
int print_log(const char*, const char*);

int main()
{
	// 조건 1. sd를 포함하는 로그 출력
	if (print_kernel_log("sd") == 0)
	{
		// 조건 2. syste를 포함하는 로그 출력
		print_kernel_log("syste");
	}
	return 0;
}

// 커널 로그 저장
void get_kernel_log()
{
	char cmd[100]={0,};
	sprintf(cmd, "dmesg > %s", LOG);
	system(cmd);
}

// 커널 로그 출력
int print_kernel_log(const char* word)
{
	get_kernel_log();
	FILE * fp = fopen(LOG, "r");
	if (fp == NULL)
	{
		printf("파일 없음\n");
		exit(0);
	}
	char log_buf[BUF_SIZE] = {0,};
	int cnt = 0;
	while (true)
	{
		if (feof(fp) != 0) break;
		fgets(log_buf, sizeof(char) * BUF_SIZE, fp);
		log_buf[strlen(log_buf) - 1] = '\0';
		if (strlen(log_buf) == 0) break;
		cnt += print_log(log_buf, word);
	}
	return cnt;
}

int print_log(const char* buf, const char* word)
{
	char log_time[100] = {0,};
	for(int i = 1; i < strlen(buf); i++){
		if(buf[i] == ']')
		{
			memcpy(log_time, &buf[1], sizeof(char) * (i - 1));
			break;
		}
	}
	if (strlen(log_time) == 0) return 0;
	//printf("%s\n", log_time);
	int last_num = log_time[strlen(log_time) - 1] - '0';
	//printf("%d\n", last_num);
	if (last_num % 2 == 0 && strstr(buf, word) != NULL)
	{
		printf("%s\n", buf);
		return 1;
	}
	return 0;
}