#include<stdio.h>
#include<ncurses.h>
#include<locale.h>
#include<unistd.h>

#define N 7

char map[N][N+1] = {
	"######",
	"#  ^ #",
	"##  ##",
	"#  a #",
	"#M  Y#",
	"#    #",
	"######",
};
int flag = 0;
int hp = 100;
int ny = 1;
int nx = 1;

void print(){
	clear();
	for(int y=0; y<N; y++){
		for(int x=0; x<N; x++){
			if (y == ny && x == nx) printw("@");
			else printw("%c", map[y][x]);
		}
		printw("\n");
	}
	printw("hp : %d\n",hp);
	refresh();
}

int main(){
	setlocale(LC_CTYPE, "ko_KR.utf8");
	initscr();

	nodelay(stdscr, TRUE);
	
	printw(" 한 글 이 여 \n");
	refresh();

	keypad(stdscr, TRUE);
	while(1){
		print();

		int ch = getch();
		if(ch == ERR) ch = 0;
		if (ch == KEY_LEFT) {
			if (map[ny][nx - 1] != '#') {
				nx--;
				flag = 0;
			}
		}
		if (ch == KEY_UP) {
			if (map[ny -1][nx] != '#'){
			       	ny--;
				flag = 0;
			}
		}
		if (ch == KEY_RIGHT) {
			if (map[ny][nx + 1] != '#') {
				nx++;
				flag = 0;
			}
		}
		if (ch == KEY_DOWN) {
			if (map[ny + 1][nx] != '#') {
				ny++;
				flag = 0;
			}
		}
		if(map[ny][nx] == '^' && flag == 0){
			hp -= 10;
			flag = 1;
			print();
			refresh();
			if(hp == 0){
				print();
				sleep(1);
				clear();
				mvprintw(10, 35, "GAMEOVER");
				refresh();
				sleep(1);
				break;
			}
			continue;
		}
		if(map[ny][nx] == 'a'){
			hp = 100;
			print();
			map[ny][nx] = ' ';
			refresh();
		}

		if(map[ny][nx] == 'M' || map[ny][nx] == 'Y'){
			print();
			sleep(1);
			clear();
			if(map[ny][nx] == 'M') {
				mvprintw(10, 35, "GAME OVER");
				sleep(1);
			}
			if(map[ny][nx] == 'Y') {
				mvprintw(10, 35, "WIN (hp : %d)", hp);
			}
			refresh();
			sleep(1);
			break;
		}
	}
	endwin();

	return 0;
}
