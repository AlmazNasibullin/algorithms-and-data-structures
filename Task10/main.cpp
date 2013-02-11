#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cerr;
using std::fstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
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
			cout<<s;
		}
		in.close();
	} catch (...) {
		cerr<<"Bad file"<<endl;
		exit(1);
	}
	return s;
}

unsigned int get_hash(const string& s, unsigned int p) {
	unsigned int hash = 0;
	unsigned int k = 1;
	int length = static_cast<int>(s.length());

	for (int i = length - 1; i >= 0; --i) {
		hash += k * s[i];
		k *= p;
	}
	return hash;
}

bool equal(const string& s, int start, const string& sub_s, int sub_s_length) {
	for (int i = 0; i < sub_s_length; ++i) {
		if (s[i + start] != sub_s[i]) {
			return false;
		}
	}
	return true;
}

unsigned int recount_hash(const string& s, int start,unsigned int cur_hash, int sub_s_length, unsigned int p) {
	int deg = 1;
	for (int i = 1; i < sub_s_length; ++i) {
		deg *=p;
	}

	return (cur_hash - s[start] * deg) * p + s[start + sub_s_length];
}

int Rabin_Karp(const string& s, const string& sub_s) {
	int s_length = static_cast<int>(s.length());
	int sub_s_length = static_cast<int>(sub_s.length());
	
	if (s_length < sub_s_length) {
		return -1;
	}

	unsigned int sub_s_hash = get_hash(sub_s, 33);
	unsigned int cur_hash = get_hash(s.substr(0, sub_s_length), 33);

	for (int i = 0; i < s_length - sub_s_length + 1; ++i) {
		if (sub_s_hash == cur_hash && equal(s, i, sub_s, sub_s_length)) {
			return i;
		}
		if ( i != s_length - sub_s_length) {
			cur_hash = recount_hash(s, i, cur_hash, sub_s_length, 33);
		}
	}
	return -1;
}

int main() {
	string s = read_input_string();
	for (;;) {
		string sub_s;
		cin>>sub_s;
		cout<<Rabin_Karp(s,sub_s)<<endl;
	}
	return 0;
}