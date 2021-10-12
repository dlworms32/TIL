/*
	0과 1로 대화
	0, 1로 된 메시지 입력시 : 영문 메시지로 출력
	영문 메시지로 입력시 : 0, 1로 된 메시지 출력

	최대 128글자, 영문 대 소문자만 가능(숫자 불가)
	
	0, 1 메시지
	맨 앞 8비트는 글자 수, [글자 수(7bit) 짝수 패리티(1bit)]
	문자 [글자(7bit), 짝수 패리티(1bit)]

*/

#include <iostream>
#include <cstring>
using namespace std;

char buf[150]; // 입력 버퍼

bool get_format() {
	if (buf[0] == '0' || buf[0] == '1') return true;
	else return false;
}

// num으로 들어온 수를 2진수로 변환 후 출력
void print_binary(int num) {
	int bits[8] = { 0, };
	int bits_size = 1;
	while (num > 1) {
		bits[bits_size++] = num % 2;
		num /= 2;
	}
	bits[bits_size] = num; 

	int on_cnt = 0; // 1 개수 세기
	for (int i = 0; i <= bits_size; i++) {
		if (bits[i] == 1) on_cnt++;
	}
	// 패리티 비트 설정
	if (on_cnt % 2 != 0) {
		bits[0] = 1;
	}
	
	for (int i = 7; i >= 0; i--)
		cout << bits[i];
	//cout << "\n";
}

// 2진수를 10진수로 변환
int get_code(int offset) {
	char temp_buf[8] = { 0, };
	memcpy(temp_buf, &buf[offset], sizeof(temp_buf));
	int on_cnt = 0;
	for (int i = 0; i < 7; i++) {
		if (buf[i] == '1') on_cnt++;
	}
	// 패리티 오류 검사
	if (on_cnt % 2 != 0) { 
		cout << "error bit!" << "\n";
		cout << "bit : " << temp_buf << ", offset : " << offset << "\n";
		exit(1);
	}
	int code = 0;
	int val = 1;
	for (int i = 6; i >= 0; i--) {
		if (temp_buf[i] == '1') {
			code += val;
		}
		val *= 2;
	}
	return code;
}

void encoding() {
	int str_size = int(strlen(buf));
	print_binary(str_size);
	for (int i = 0; i < str_size; i++) { // 문자 수 만큼 반복
		print_binary(int(buf[i]));
	}
}

void decoding() {
	int char_num = get_code(0);
	for (int i = 1; i <= char_num; i++) {
		cout << char(get_code(i * 8));
	}
}

void my_protocol() {
	cin >> buf;
	//cout << buf << "\n";
	bool format = get_format(); // true : 2진수, false : 영문자
	if (format) { // 2진수 -> 영문자
		decoding();
	}
	else {
		encoding(); // 영문자 -> 2진수
	}
}

int main() {
	freopen_s(new FILE*, "text.txt", "r", stdin);
	my_protocol();
}