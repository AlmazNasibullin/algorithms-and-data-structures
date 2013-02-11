#include <iostream>
#include <fstream>
#include "Suffix_array.h"

using std::cerr;
using std::cout;
using std::endl;
using std::fstream;

// сначала идет техт до пробела, далее кол-во строк, которые будем искать в тексте, далее сами строки
void read_input_string(string& text, vector<string>& words) {
	try {
		fstream in("in.txt");
		in>>text;

		int number;
		in>>number;
		for (int i = 0; i < number; ++i) {
			string s;
			in>>s;
			words.push_back(s);
		}
		in.close();
	} catch (...) {
		cerr<<"Bad file"<<endl;
		exit(1);
	}
}

int main() {
	string text;
	vector<string> words;
	vector<int> suffixs;
	read_input_string(text, words);
	buildSuffixArray(suffixs, text);
	int size = static_cast<int>(text.size());

	if (size == 0) {
		return 0;
	}

	for (size_t i = 0; i < words.size(); ++i) {
		vector<bool> positions(size, false);
		int leftDepth = findCommonPrefix(suffixs, text, words[i], 0, 0);
		int rightDepth = findCommonPrefix(suffixs, text, words[i], size - 1, 0); 
		
		findSubstr(suffixs, positions, text, words[i], 0, size - 1, leftDepth, rightDepth);
		cout<<"Word # "<<i<<":"<<endl;
		for (int j = 0; j < size; ++j) {
			if (positions[j]) {
				cout<<j<<" ";
			}
		}
		cout<<endl;
	}
	return 0;
}