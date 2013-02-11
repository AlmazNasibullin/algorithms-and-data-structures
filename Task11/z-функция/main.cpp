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

int calculate_z_function(const string& s, const string& sub_s, vector<int>& z) {
	// параллельно находится индекс подстроки в строке 
	int length = static_cast<int>(s.length());
	int sub_length = static_cast<int>(sub_s.length());

	if (length == 0 || sub_length == 0 || length < sub_length) {
		return -1;
	}

	z.assign(length + sub_length + 1, 0);
	z[0] = length + sub_length + 1;
	int r = -1;
	int max = -1;

	for (int i = 1; i < length  + sub_length + 1; ++i) {
		if (i == sub_length) { // позиция уникального символа
			continue;
		}
		if (max <= i) {
			int k = 0;
			while (k + i < length  + sub_length + 1 && k != sub_length && k + i != sub_length) {
				if (k + i < sub_length && sub_s[k] != sub_s[k + i]) { // сравнение в строке sub_s
					break;
				} else if (k + i > sub_length && sub_s[k] != s[k + i - sub_length - 1]) {
					// сравнение между строками sub_s и s
					break;
				}
				++k;
			}
			z[i] = k;
		} else {
			int k = i - r;
			if (z[k] < z[r] - k) {
				z[i] = z[k];
				continue;
			}
			int j = 1;
			while (j + max < length  + sub_length + 1 && max + j != sub_length && max - i + j != sub_length) {
				if (max + j < sub_length && sub_s[max - i + j] != sub_s[max + j]) {
					// сравнение в строке sub_s
					break;
				} else if (max - i + j < sub_length && max + j > sub_length && sub_s[max - i + j] != s[max + j - sub_length - 1]) {
					// сравнение между строками sub_s и s
					break;
				}
				++j;
			}
			z[i] = max - i + j;
		}
		if (i + z[i] - 1 > max) {
			max = i + z[i] - 1;
			r = i;
		}
		if (i > sub_length && z[i] == sub_length) { // нашли подстроку в строке
			return i - sub_length - 1;
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
		cout<<calculate_z_function(s, sub_s, z)<<endl;
	}
	return 0;	
}