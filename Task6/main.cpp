#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <deque>
#include <string>

using std::fstream;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::deque;
using std::numeric_limits;
using std::string;

void BFS(int start, vector<vector<pair<double, double> > >& adjMatrix, vector<int>& parent, double& min_cfp) {
	int size = static_cast<int>(adjMatrix.size());
	deque<int> q;
	parent.assign(size, -1);
	vector<double> min_cf(size, numeric_limits<double>::max());
	vector<bool> color(size, true); // true - white, false - black
	q.push_back(start);
	color[start] = false;

	while(!q.empty()){
		int head = q.front();
		q.pop_front();
		color[head] = false;
		for (int i = 0; i < size; ++i) {
			if ((adjMatrix[head][i].first - adjMatrix[head][i].second) > 0 && color[i]) {
				q.push_back(i);
				color[i] = false;
				parent[i] = head;
				if (adjMatrix[head][i].first - adjMatrix[head][i].second < min_cf[head]) {
					min_cf[i] = adjMatrix[head][i].first - adjMatrix[head][i].second;
				} else {
					min_cf[i] = min_cf[head];
				}
			}
		}
	}
	min_cfp = *min_cf.rbegin();
}

void find_max_stream(vector<vector<pair<double, double> > >& adjMatrix) {
	vector<int> parent; // путь p в остаточной сети
	double min_cfp; // минимальное значение Сf пути p
	BFS(0, adjMatrix, parent, min_cfp);
	while (*parent.rbegin() != -1) {
		for (int i = static_cast<int>(parent.size()) - 1; parent[i] != -1; i = parent[i]) { 
			adjMatrix[parent[i]][i].second += min_cfp;
			adjMatrix[i][parent[i]].second = -adjMatrix[parent[i]][i].second;
		}
		BFS(0, adjMatrix, parent, min_cfp);
	}
}

void read_file(vector<vector<pair<double, double> > >& adjMatrix, string& word, int& count, vector<int>& letters) {
	try {
		fstream read("in.txt", fstream::in);
		read>>word;
		read>>count;
		letters.assign(26, 0);
		for (int i = 0; i < static_cast<int>(word.size()); ++i) {
			++letters[word[i] - 'A'];
		}
		
		adjMatrix.resize(count + 28); // в алфавите 26 букв + 2 фиктивные вершины (начальная и конечная)
		// кубики хранятся с индексами от 1 до count, буквы - от count + 1 до count + 26,
		// начальная вершина - 0, конечная - count + 27
 		for (size_t i = 0; i < adjMatrix.size(); ++i) {
			adjMatrix[i].resize(count + 28);
		}
		
		for (int i = 1; i <= count; ++i) {
			adjMatrix[0][i].first = 1; // от начальной к кубику
			string cub;
			read>>cub;
			if (cub.size() != 6) {
				cout<<"Bad file. Length of words is incorrect"<<endl;
				exit(1);
			}
			for (int j = 0; j < 6; ++j) {
				if (!(cub[j] >= 'A' && cub[j] <= 'Z')) {
					cout<<"Bad file. Incorrect words"<<endl;
					exit(1);
				}
				int pos = cub[j] - 'A'; 
				if (letters[pos] > 0) {
					++adjMatrix[i][pos + count + 1].first; // от кубика к букве
					adjMatrix[pos + count + 1][count + 27].first = letters[pos]; // от буквы к конечной
				}
			}
		}
		read.close();
	} catch (...) {
		cout<<"Bad file!"<<endl;
		exit(1);
	}
}

void print_path_of_cubes(vector<vector<pair<double, double> > >& adjMatrix, string& word, int count) {
	vector<bool> already_printed(count, false);
	for (int i = 0; i < static_cast<int>(word.size()); ++i) {
		int pos = word[i] - 'A';
		for (int j = 1; j <= count; ++j) {
			if (adjMatrix[j][pos + count + 1].second > 0 && !already_printed[j - 1]) {
				cout<<j<<" ";
				already_printed[j - 1] = true; 
				break;
			}
		}
	}
	cout<<endl;
}

bool check_word(string& word, vector<vector<pair<double, double> > >& adjMatrix, vector<int>& letters, int count) {
	int size = static_cast<int>(adjMatrix.size());
	for (size_t i = 0; i < word.size(); ++i) {
		int pos = word[i] - 'A';
		if (adjMatrix[pos + count + 1][size - 1].second != letters[pos]) {
			return false;
		}
	}
	return true;
}

int main() {
	vector<vector<pair<double, double> > > adjMatrix;
	vector<int> letters; // буквы во входном слове и их кол-во
	string word; // входное слово
	int count; // кол-во кубиков
	
	read_file(adjMatrix, word, count, letters);
	find_max_stream(adjMatrix);

	bool YES = check_word(word, adjMatrix, letters, count);
	if (YES) {
		cout<<"YES"<<endl;
		print_path_of_cubes(adjMatrix, word, count);
	} else {
		cout<<"NO"<<endl;
	}
	return 0;
}