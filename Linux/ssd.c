#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SSD "nand.txt"
#define SSD_SIZE 128
#define LBA_SIZE 4
#define VALUE_SIZE 10

void write_ssd(int, const char*);
int check_lba(const char*);
int check_value(const char*);
void read_ssd(int);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("no args\n");
		return 0;
	}
	if (strcmp(argv[1], "w") == 0) {
		if (argc != 4) {
			printf("invalid argv length\n");
			return 0;
		}
		if (!check_lba(argv[2]))	{
			return 0;
		}
		if (!check_value(argv[3])) {
			return 0;
		}
		write_ssd(atoi(argv[2]), argv[3]);
	}
	else if (strcmp(argv[1], "r") == 0) {
		if (argc != 3) {
			printf("invalid argv length\n");
			return 0;
		}
		if(!check_lba(argv[2])) {
			return 0;
		}
		read_ssd(atoi(argv[2]));
	}
	else {
		printf("invalid option!\n");
	}		
	return 0;
}

int check_lba(const char* address)
{
	for(int i = 0; i < strlen(address); i++) {
		if (address[i] < '0' || address[i] > '9'){
			printf("invalid address\n");
			return 0;
		}
	}
	int addr = atoi(address);
	if( addr < 0 || addr > 127) {
		printf("invalue address range!\n");
		return 0;
	}
	return 1;
}

int check_value(const char* value)
{
	if( strlen(value) != 10 ) {
		printf("invalid value length\n");
		return 0;
	}
	if( value[0] != '0' || value[1] != 'x') {
		printf("input value must be hexadeciaml string\n");
		return 0;
	}
	int flag = 0;
	for(int i = 2; i <10; i++) {
		if(('0' <= value[i] && value[i] <= '9') || ('A' <= value[i] && value[i] <= 'F')) {
			flag = 1;
		}
		else {
			printf("invalid value : %c\n", value[i]);
			flag = 0;
			break;
		}
	}
	if (!flag) {
		printf("invalue value value. range(0 ~ 9) or (A ~ F)\n");
		return 0;
	}
	else return 1;
}

void write_ssd(int addr, const char* value)
{
	FILE* fp = fopen(SSD, "r+");
	if (fp == NULL){
		printf("%s not found!\n", SSD);
		exit(0);
	}
	int seek_pos = addr * (LBA_SIZE + VALUE_SIZE + 2) + LBA_SIZE + 1;
	fseek(fp, seek_pos, SEEK_SET);
	fputs(value, fp);
	fclose(fp);
}

void read_ssd(int addr)
{
	FILE* fp = fopen(SSD, "r");
	if (fp == NULL){
		printf("%s not found!\n", SSD);
		exit(0);
	}
	char buf[20] = {0, };
	int seek_pos = addr * (LBA_SIZE + VALUE_SIZE + 2) + LBA_SIZE + 1;
	fseek(fp, seek_pos, SEEK_SET);
	fgets(buf, sizeof(buf), fp);
	printf("%s", buf);
	fclose(fp);
}
	
