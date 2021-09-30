#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char* DAY[] = { "","Mon","Tue","Wed","Thu","Fri","Sat","Sun" };

#pragma pack(push, 1) // 구조체 정렬 피하기
typedef union
{
	uint8_t buf[7];
	struct {
		uint8_t sec;
		uint8_t min;
		uint8_t hour;
		uint8_t day;
		uint8_t date;
		uint8_t month;
		uint8_t year;
	};
}ds_time_t;
#pragma pack(pop) // 구조체 정렬 피하기 해제

void print_year(ds_time_t time) 
{
	uint8_t ten_year = (time.year >> 4) & 0xF;
	uint8_t year = time.year & 0xF;

	printf("%d%d/", ten_year, year);
}

void print_month(ds_time_t time) 
{
	uint8_t ten_month = (time.month >> 4) & 0x1;
	uint8_t month = time.month & 0xF;

	printf("%d%d/", ten_month, month);
}

void print_date(ds_time_t time)
{
	uint8_t ten_date = (time.date >> 4) & 0x3;
	uint8_t date = time.date & 0xF;

	printf("%d%d ", ten_date, date);
}

void print_day(ds_time_t time)
{
	printf("%s ", DAY[time.day & 0xF]);
}

void print_hour(ds_time_t time)
{
	uint8_t hour = time.hour & 0xF;
	uint8_t ten_hour;
	// bit 6 HIGH : 12hour
	if ((time.hour >> 6) & 0x01) {
		ten_hour = (time.hour >> 4) & 0x01;
		// bit 5 HIGH : PM
		if ((time.hour >> 5) & 0x01) {
			printf("PM %d%d:", ten_hour, hour);
		}
		else {
			printf("AM %d%d:", ten_hour, hour);
		}
	}
	else {
		ten_hour = (time.hour >> 4) & 0x03;
		printf("%d%d:", ten_hour, hour);
	}
}

void print_minutes(ds_time_t time) 
{
	uint8_t ten_minutes = (time.min >> 4) & 0x03;
	uint8_t minutes = time.min & 0x0F;

	printf("%d%d:", ten_minutes, minutes);
}

void print_seconds(ds_time_t time)
{
	uint8_t ten_seconds = (time.sec >> 4) & 0x03;
	uint8_t seconds = time.sec & 0xF;

	printf("%d%d", ten_seconds, seconds);
}

void print_time(ds_time_t time)
{
	print_year(time);
	print_month(time);
	print_date(time);
	print_day(time);
	print_hour(time);
	print_minutes(time);
	print_seconds(time);
	printf("\n");
}

int main()
{
	FILE* fp = fopen("DS1307(12)", "rb");
	// FILE* fp = fopen("DS1307(24)", "rb");
	uint8_t buf[7];
	fread(buf, 1, 7, fp);
	fclose(fp);
	ds_time_t read_time;
	memcpy(read_time.buf, buf, sizeof(buf));

	print_time(read_time);
	return 0;
}