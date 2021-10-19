#include <iostream>
#include <algorithm>
#include <cstring>

#define MAX 20
using namespace std;

int N = 10;
int max_value = 0;
int cnt;
int station[MAX];
bool used[MAX];

// 노선 1, 노선 1, 노선 2, 노선 2 의 인덱스
int comb[4] = { 0, 6, 2, 4 }; // 

bool get_cond1_result() {
	bool a = false; // 노선 2의 역 1 이 노선 1의 범위에 있는 경우
	bool b = false;

	int std_st = min(comb[0], comb[1]);
	int std_ed = max(comb[0], comb[1]);
	if (std_st < comb[2] && comb[2] < std_ed) a = true;
	if (std_st < comb[3] && comb[3] < std_ed) b = true;

	// 노선 2의 역이 하나만 노선 1의 범위 내 있는 경우
	return !(a ^ b);
}

bool get_cond2_result() {
	if (comb[0] == (comb[1] + 1) % N || comb[0] == (comb[1] + N - 1) % N) return false;
	if (comb[2] == (comb[3] + 1) % N || comb[2] == (comb[3] + N - 1) % N) return false;
	return true;
}

bool get_cond3_result() {
	int std_index = comb[0];
	if ((comb[2] + 1 % N) == std_index || (comb[2] + N - 1) % N == std_index) return false;
	if ((comb[3] + 1 % N) == std_index || (comb[3] + N - 1) % N == std_index) return false;

	std_index = comb[1];
	if ((comb[2] + 1 % N) == std_index || (comb[2] + N - 1) % N == std_index) return false;
	if ((comb[3] + 1 % N) == std_index || (comb[3] + N - 1) % N == std_index) return false;
	return true;
}

int calc_value() {
	int A = station[comb[0]] + station[comb[1]];
	int B = station[comb[2]] + station[comb[3]];

	return A * A + B * B;
}

void dfs(int level) {
	if (level == 4) {
		if (!get_cond1_result()) return;
		if (!get_cond2_result()) return;
		if (!get_cond3_result()) return;
		max_value = max(max_value, calc_value());
		return;
	}
	for (int i = 0; i < N; i++) {
		if (!used[i]) {
			used[i] = true;
			comb[level] = i;
			dfs(level + 1);
			used[i] = false;
		}
	}
}

int main()
{
	freopen_s(new FILE*, "sample2.txt", "r", stdin);
	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		cout << "#" << t + 1 << " ";
		cin >> N;
		memset(station, 0, sizeof(station));
		max_value = 0;
		for (int i = 0; i < N; i++) cin >> station[i];
		dfs(0);
		cout << max_value << "\n";
	}
	return 0;
}