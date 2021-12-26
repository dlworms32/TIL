#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

typedef struct {
	char name[100];
	char value[100];
}macro_variable;

macro_variable arr[1000];
int macro_num = 0;

char hist[100][100];
int hist_index = 0;

void cli_history();
int run_history(int);
void clear_history();
void run_cmd(const char*);
void cli_shell();
void read_macro();
void print_macro();
void write_macro();
void add_macro(const char*);
void print_macro_value(const char*);
void delete_space(const char*, char*, int);

int main()
{
	read_macro();
	cli_shell();
	write_macro();
	return 0;
}

// history 출력
void cli_history()
{
	for (int i = 0; i < hist_index; i++)
	{
		printf("%d %s\n", i, hist[i]);
	}
}

// history의 index 저장된 명령 실행
int run_history(int index)
{
	if (index <= hist_index)
	{
		run_cmd(hist[index]);
		return true;
	}
	else
	{
		printf("index error!\n");
		return false;
	}
}

// history 삭제
void clear_history()
{
	memset(hist, 0, sizeof(hist));
	hist_index = 0;
}

// 입력 명령어 실행
void run_cmd(const char* cmd)
{
	char ocmd[100] = { 0, };
	char opt[100] = { 0, };
	int is_whitespace = 0;
	int is_equal = 0;
	for (int i = 0; i < strlen(cmd); i++) {
		if (cmd[i] == '=') {
			add_macro(cmd);
			return;
		}
	}

	for (int i = 0; i < strlen(cmd); i++) {
		if (cmd[i] == ' ') {
			memcpy(ocmd, cmd, sizeof(char) * i);
			memcpy(opt, &cmd[i + 1], sizeof(char) * (strlen(cmd) - 1 - i));
			is_whitespace = 1;
			break;
		}
	}
	if (!is_whitespace) memcpy(ocmd, cmd, sizeof(char) * strlen(cmd));

	//printf("ocmd: %s\n", ocmd);
	//printf("opt: %s\n", opt);

	if (strcmp(ocmd, "date") == 0) {
		system("date");
		return;
	}
	if (strcmp(ocmd, "uptime") == 0) {
		system("uptime");
		return;
	}
	if (strcmp(ocmd, "ls") == 0) {
		system("ls -al");
		return;
	}
	if (strcmp(ocmd, "log") == 0) {
		system("dmesg");
		return;
	}
	if (strcmp(ocmd, "history") == 0) {
		if (is_whitespace)
		{
			run_history(atoi(opt));
		}
		else {
			cli_history();
		}
		return;
	}
	if (strcmp(ocmd, "hclear") == 0) {
		clear_history();
		return;
	}
	if (strcmp(ocmd, "echo") == 0) {
		print_macro_value(opt);
		return;
	}

	printf("ERROR\n");
}

// 쉘 명령어 입력
void cli_shell()
{
	char cmd[100] = { 0, };
	while (true)
	{
		printf("SSAFY > ");
		//scanf("%s", cmd);
		fgets(cmd, 100, stdin);
		cmd[strlen(cmd) - 1] = '\0';
		//printf("input command : %s\n", cmd);
		if (strcmp(cmd, "exit") == 0) return;
		run_cmd(cmd);
		memcpy(hist[hist_index++], cmd, sizeof(char) * strlen(cmd));
	}
}

// 메크로 파일 읽어와 저장
void read_macro()
{
	FILE* fp = fopen("macro_var.txt", "r");
	if (fp == NULL) return;
	char line[100] = { 0, };
	while (1)
	{
		if (feof(fp) != 0) break;
		fgets(line, 100, fp);
		line[strlen(line) - 1] = '\0';
		if (strlen(line) == 0) break;
		for (int i = 0; i < strlen(line); i++) {
			if (line[i] == ' ') {
				memcpy(arr[macro_num].name, line, sizeof(char) * i);
				memcpy(arr[macro_num].value, &line[i + 1], sizeof(char) * (strlen(line) - 1 - i));
				macro_num++;
				break;
			}
		}
		memset(line, 0, sizeof(line));
	}
	fclose(fp);
}

// 저장된 메크로 출력
void print_macro()
{
	printf("total macro num : %d\n", macro_num);
	for (int i = 0; i < macro_num; i++)
	{
		printf("macro name : %s, macro value : %s\n", arr[i].name, arr[i].value);
	}
}

// 메크로 파일로 쓰기
void write_macro()
{
	FILE* fp = fopen("macro_var.txt", "w");
	char output_str[250] = { 0, };
	for (int i = 0; i < macro_num; i++) {
		sprintf(output_str, "%s %s\n", arr[i].name, arr[i].value);
		fputs(output_str, fp);
	}
	fclose(fp);
}

// 메크로 추가
void add_macro(const char* cmd)
{
	char delete_space_cmd[100] = { 0, };
	char m_name[100] = { 0, };
	char m_value[100] = { 0, };
	delete_space(cmd, delete_space_cmd, 100);
	for (int i = 0; i < strlen(delete_space_cmd); i++)
	{
		if (delete_space_cmd[i] == '=')
		{
			memcpy(m_name, delete_space_cmd, sizeof(char) * i);
			memcpy(m_value, &delete_space_cmd[i + 1], sizeof(char) * (strlen(delete_space_cmd) - 1 - i));
		}
	}
	if (strlen(m_name) == 0 || strlen(m_value) == 0)
	{
		printf("Invalid input\n");
		return;
	}
	//printf("m_name : %s\n", m_name);
	//printf("m_value : %s\n", m_value);
	// 중복 제거
	for (int i = 0; i < macro_num; i++)
	{
		if (strcmp(m_name, arr[i].name) == 0)
		{
			printf("already exists macro variable\n");
			return;
		}
	}

	memcpy(arr[macro_num].name, m_name, sizeof(char) * strlen(m_name));
	memcpy(arr[macro_num].value, m_value, sizeof(char) * strlen(m_value));
	macro_num++;
}

// 공백 제거
void delete_space(const char* cmd, char* dst, int dst_size)
{
	int dst_index = 0;
	for (int i = 0; i < strlen(cmd); i++)
	{
		if (dst_index >= dst_size) return;
		if (cmd[i] != ' ') dst[dst_index++] = cmd[i];
	}
}

// 메크로 값 출력
void print_macro_value(const char* m_name)
{
	for (int i = 0; i < macro_num; i++)
	{
		if (strcmp(m_name, arr[i].name) == 0)
		{
			printf("%s\n", arr[i].value);
			return;
		}
	}
	printf("not exist macro variable mapping to %s\n", m_name);
}