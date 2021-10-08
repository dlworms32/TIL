#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sig_handler_ur1()
{
	printf("[Button Clicked]\n");
}

void sig_handler_ur2()
{
	system("clear");
	printf("RESET\n");
}

void sig_handler_term()
{
	printf("BYE\n");
	exit(0);
}

int main() {
	signal(SIGUSR1, sig_handler_ur1);
	signal(SIGUSR2, sig_handler_ur2);
	signal(SIGTERM, sig_handler_term);

	while(1) {
		for(char c = 'A'; c <= 'Z'; c++) {
			printf("%c", c);
			fflush(stdout);
			usleep(1000 * 100);
		}
		for(char c = 'Z'; c >= 'A'; c--) {
			printf("%c", c);
			fflush(stdout);
			usleep(1000 * 100);
		}
	}
	return 0;
}
