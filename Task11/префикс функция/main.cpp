#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::cerr;
using std::ios;

string read_input_string() {
	string s;
	try {
		fstream in("in.txt");
		in.seekg(0, ios::end);
		int length = in.tellg();
		in.seekg (0, ios::beg);
		if (length > 0) {
			vector<char> buffer(length);
			in.read(&buffer[0], length);
			s = string(&buffer[0], length);
		}
		in.close();
	} catch (...) {
		cerr<<"Bad file"<<endl;
		exit(1);
	}
	return s;
}

int change_prefix(const string& s, const string& sub_s, vector<int>& p, int i, int cur, int sub_length) {
	if (cur < sub_length && p[i - 1] < sub_length && sub_s[cur] == sub_s[p[i - 1]]) { // сравнение в sub_s
		return p[i - 1] + 1;
	}
	if ( p[i - 1] < sub_length && cur > sub_length && sub_s[p[i - 1]] == s[cur - sub_length - 1]) {
		// сравнение между sub_s и s
		return p[i - 1] + 1;
	}
	return -1;
}

int calculate_prefix_function(const string& s, const string& sub_s, vector<int>& p) {
	// параллельно находится индекс начала подстроки в строке 
	int length = static_cast<int>(s.length());
	int sub_length = static_cast<int>(sub_s.length());

	if (length == 0 || sub_length == 0 || length < sub_length) {
		return -1;
	}

	p.assign(length + sub_length + 1, -1);
	p[0] = 0;

	for (int i = 1; i < length + sub_length + 1; ++i) {
		if (i == sub_length) { // индекс уникального символа
			p[i] = 0;
			continue;
		}
		if (p[i - 1] != sub_length) { // неуникальный символ
			p[i] = change_prefix(s, sub_s, p, i, i, sub_length);
		}
		if (p[i] == -1) {
			/* пусть str = sub_s + 'уникальный символ' + s,
				тогда этот блок соответсвует случаю str[i] != str[p[i-1]
			*/
			int k = p[i - 1];
			while (k > 0 && change_prefix(s, sub_s, p, k, i, sub_length) == -1) {
				k = p[k - 1];
			}
			if (k <= 0) {
				p[i] = 0;
			} else {
				p[i] = change_prefix(s, sub_s, p, k, i, sub_length);
			}
		}
		if (i > sub_length && p[i] == sub_length) { // подстрока найдена в строке
			return i - sub_length - 1 - (sub_length - 1);
		}
	}
	return -1;
}

int main() {
	string s = read_input_string();
	vector<int> z;
	for (;;) {
		string sub_s;
		char c;
		cin.get(c);
		while (c != '\n') {
			sub_s +=c;
			cin.get(c);
		}
		cout<<calculate_prefix_function(s, sub_s, z)<<endl;
	}
	return 0;	
}