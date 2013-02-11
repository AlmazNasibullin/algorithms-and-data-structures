#include <iostream>
#include <fstream>
#include "trie.h"

using std::fstream;
using std::cerr;
using std::cout;
using std::endl;

// сначала задается кол-во слов из словаря, потом сами слова, далее кол-во строк-образцов, далее сами строки-образцы
void read_input_string(vector<string>& words, vector<string>& texts) {
	try {
		fstream in("in.txt");
		int number;
		
		in>>number;
		for (int i = 0; i < number; ++i) {
			string s;
			in>>s;
			words.push_back(s);
		}

		in>>number;
		for (int i = 0; i < number; ++i) {
			string s;
			in>>s;
			texts.push_back(s);
		}
		in.close();
	} catch (...) {
		cerr<<"Bad file"<<endl;
		exit(1);
	}
}

int main() {
	vector<string> words;
	vector<string> texts;
	
	read_input_string(words, texts);
	Node* root = buildTrie(words);

	for (size_t i = 0; i < texts.size(); ++i) {
		set<int> result;
		findSubstrings(root, texts[i], result);
		cout<<"Text # "<<i<<":"<<endl;
		for (set<int>::iterator iter = result.begin(); iter != result.end(); ++iter) {
			cout<<*iter<<" ";
		}
		cout<<endl;
	}
	return 0;
}