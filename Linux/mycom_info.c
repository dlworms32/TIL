#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define CPUINFO "cpu.txt"
#define MEMINFO "mem.txt"

void get_cpu_info();
void get_mem_info();
void print_cpu_info();
void print_mem_info();
float KBtoMiB(int);

int main()
{
	print_cpu_info();
	print_mem_info();
	return 0;
}

// CPU 정보 저장
void get_cpu_info()
{	
	char cmd[100] = {0,};
	sprintf(cmd, "cat /proc/cpuinfo | grep \"cpu MHz\" > %s", CPUINFO);
	system(cmd);
}

// 메모리 정보 저장
void get_mem_info()
{
	char cmd[100] = {0,};
	sprintf(cmd, "cat /proc/meminfo | grep Mem > %s", MEMINFO);
	system(cmd);
}

// CPU 정보 출력
void print_cpu_info()
{
	FILE * fp = fopen(CPUINFO, "r");
	if (fp == NULL)
	{
		get_cpu_info();
		fp = fopen(CPUINFO, "r");
	}
	int core_cnt = 0;
	printf("CPU clock \n");
	while(true)
	{
		if (feof(fp) != 0) break;
		char c1[100] = {0,};
		char c2[100] = {0,};
		char c3[100] = {0,};
		char c4[100] = {0,};
		fscanf(fp, "%s %s %s %s", c1, c2, c3, c4);
		if(strlen(c1) == 0 || strlen(c2) == 0 || strlen(c3) == 0 || strlen(c4) == 0) break;
		printf("core %d : %s %s\n", core_cnt++, c4, c2);
	}
	printf("-------------------------------\n");
}

// 메모리 정보 출력
void print_mem_info()
{
	FILE * fp = fopen(MEMINFO, "r");
	if (fp == NULL)
	{
		get_mem_info();
		fp = fopen(MEMINFO, "r");
	}
	printf("Memory info\n");
	while(true)
	{
		if (feof(fp) != 0) break;
		char info[100] = {0,};
		int value = 0;
		char unit[100] = {0,};
		fscanf(fp, "%s %d %s", info, &value, unit);
		if(strlen(info) == 0 || strlen(unit) == 0) break;
		printf("%s %.4lf MiB\n", info, KBtoMiB(value));
	}
}

// KB -> MiB
float KBtoMiB(int value)
{
	return value / 1049.;
}