#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void sig_handler(){
	printf("SYSTEM ERROR\n");
	exit(1);
}

int main(){
	signal(SIGALRM, sig_handler);

	char input_buf[20] ={0,};
	while(1) {
		memset(input_buf, 0, sizeof(input_buf));
		alarm(3);
		printf("input command >> ");
		scanf("%s", input_buf);
		printf("%s\n", input_buf);
	}
	return 0;
}

