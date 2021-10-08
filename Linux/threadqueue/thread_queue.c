#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_QUEUE 10
#define LOCK(x) pthread_mutex_lock(&x)
#define UNLOCK(x) pthread_mutex_unlock(&x)

pthread_mutex_t mlock;

typedef struct Node {
	char c;
	struct Node* next;
}Node;

char* files[2] = {
	"text1.txt\0",
	"text2.txt\0",
};

char text[2] = {0,};

Node* head = NULL; // 헤드
Node* tail = NULL; // 꼬리
int q_size = 0;

void push_queue(char c)
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->c = c;
	new_node->next = NULL;
	q_size++;
	if (head == NULL){
		head = new_node;
		tail = new_node;
		return;
	}
	tail->next = new_node;
	tail = new_node;
}

char pop_queue(){
	if(head == NULL){
		return '\0';
	}
	Node* rm = head;
	head = rm->next;
	char c = rm->c;
	free(rm);
	q_size--;
	return c;
}

void print_queue() {
	Node* p = head;
	while(p != NULL){
		printf("%c -> ", p->c);
		p = p->next;
	}
	printf("\n");
}

void* run_push(void* arg)
{
	int tid = *(int*)arg;
	while(1){
		LOCK(mlock);
		if (q_size < 10)
		push_queue(text[tid]);
		else printf("full queue size!\n");
		UNLOCK(mlock);
		usleep(1000 * 100);
	}
}
	
void* run_pop()
{
	while(1){
		LOCK(mlock);
		char c = pop_queue();
		UNLOCK(mlock);
		if (c == '\0') printf("empty queue!\n");
		else printf("pop : %c\n", c);
		usleep(1000 * 100);
	}
}

void read_text()
{
	char buf[10] = {0,};
	for(int i = 0; i < 2; i++) {
		int fd = open(files[i], O_RDONLY, 0);
		read(fd, buf, 1);
		text[i] = buf[0];
		printf("%s\n", buf);
		close(fd);
	}
	for(int i = 0; i < 2; i++){
		printf("%c\n", text[i]);
	}
}

int main(){
	read_text();
	int tid[2] = {0, 1};
	pthread_t push_tid[2];
	pthread_t pop_tid;
	
	pthread_create(&pop_tid, 0, run_pop, 0);
	for(int i = 0; i < 2; i++) {
		pthread_create(&push_tid[i], 0, run_push, &tid[i]);
	}

	for(int i = 0; i < 2; i++){
		pthread_join(push_tid[i], 0);
	}
	pthread_join(pop_tid, 0);

	return 0;

}

