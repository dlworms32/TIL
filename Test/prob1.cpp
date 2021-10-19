#include <iostream>
#include <cstring>
#include <vector>
#define MAX 50
using namespace std;

int N;
int cnt = 0; // 잡을 수 있는 장기알
// 방향 북 남 서 동
int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, -1, 1 };
int board[MAX][MAX];
bool catched[MAX][MAX];

pair<int, int> used[3];

void print_board() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << board[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

// 현재 위치에서 갈 수 있는 위치
// c_x, c_y : 현재 좌표
// d : 방향
vector<pair<int, int>> get_enable_pos(int cx, int cy, int d) {
	vector<pair<int, int>> enable_pos;
	int nx = cx + dx[d];
	int ny = cy + dy[d];

	int cnt = 0; // 지나친 알 개수 확인
	while (true) {
		if (nx < 0 || nx >= N || ny < 0 || ny >= N) break;
		if (cnt >= 2) break;
		if (cnt == 0 && board[nx][ny] == 1) { // 첫번째 알 마주침
			cnt++;
		}
		else if (cnt == 1 && board[nx][ny] == 0) {
			enable_pos.push_back({ nx,ny });
		}
		else if (cnt == 1 && board[nx][ny] == 1) {
			enable_pos.push_back({ nx, ny });
			cnt++;
		}
		nx = nx + dx[d];
		ny = ny + dy[d];
	}
	return enable_pos;
}

// 완탐
void dfs(int step, int cx, int cy)
{
	if (step == 3) {
		//for (int i = 0; i < 3; i++) {
		//	cout << "step : " << i + 1 << ", (" << used[i].first << ", " << used[i].second << ")\n";
		//}
		//print_board();
		//cout << cnt << "\n";
		return;
	}
	int temp_board[MAX][MAX] = { 0, };
	memcpy(temp_board, board, sizeof(board));
	for (int i = 0; i < 4; i++) {
		vector<pair<int, int>> ep = get_enable_pos(cx, cy, i);
		for (int p = 0; p < ep.size(); p++) {
			int nx = ep[p].first;
			int ny = ep[p].second;
			if (board[nx][ny] == 1 && !catched[nx][ny]) {
				cnt++;
				catched[nx][ny] = true;
				board[nx][ny] = 0;
			}
			used[step] = { nx, ny };
			dfs(step + 1, ep[p].first, ep[p].second);
			memcpy(board, temp_board, sizeof(board));
		}
	}
}

int prob1()
{
	freopen_s(new FILE*, "sample1.txt", "r", stdin);

	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		cin >> N;
		cnt = 0;
		int p_x, p_y;
		memset(board, 0, sizeof(board));
		memset(catched, 0, sizeof(catched));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> board[i][j];
				if (board[i][j] == 2) {
					p_x = i;
					p_y = j;
					board[i][j] = 0;
				}
			}
		}
		dfs(0, p_x, p_y);
		cout << "#" << t + 1 << " " << cnt << "\n";
	}
	return 0;
}