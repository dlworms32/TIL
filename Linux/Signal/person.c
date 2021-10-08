#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]) {
	/*
	printf("argc : %d\n", argc);

	for(int i = 0; i < argc; i++) {
		printf("argc[%d] : %s\n", i, argv[i]);
	}
	*/
	if (argc != 2) {
		printf("input PID\n");
		exit(-1);
	}
	int target_pid = atoi(argv[1]);
	printf("target pid : %d\n", target_pid);
	while(1) {
		printf("input num >> ");
		int num = 0;
		scanf("%d", &num);
		if(num == 1) {
			kill(target_pid, SIGUSR1);
			continue;
		}
		if(num == 2) {
			kill(target_pid, SIGUSR2);
			continue;
		}
		if(num == 3) {
			kill(target_pid, SIGTERM);
			continue;
		}
		if (num == -1) {
			break;
		}
	}
	return 0;
}
