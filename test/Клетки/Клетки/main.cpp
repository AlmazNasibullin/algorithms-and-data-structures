#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <list>

using namespace std;

void BFS(int start, vector<list<int> >& adjList, vector<int>& parent) {
	int size = static_cast<int>(adjList.size());
	deque<int> q;
	parent.assign(size, -1);
	vector<bool> color(size, true); // true - white, false - black
	q.push_back(start);
	parent[start] = start;
	color[start] = false;

	while(!q.empty()){
		int head = q.front();
		q.pop_front();
		color[head] = false;
		for (list<int>::iterator iter = adjList[head].begin(); iter != adjList[head].end(); ++iter) {
			if (color[*iter]) {
				q.push_back(*iter);
				color[*iter] = false;
				parent[*iter] = head;
			}
		}
	}
}

void read_file(vector<vector<bool> >& matrix) {
	try {
		fstream read("in.txt");
		int size;
		read>>size;
		matrix.resize(size);
		for (int i = 0; i < size; ++i) {
			matrix[i].resize(size);
			for (int j = 0; j < size; ++j) {
				matrix[i][j] = true;
			}
		}
		while (read.good()) {
			int i;
			int j;
			read>>i>>j;
			matrix[i][j] = false;
		}
		read.close();
	} catch (...) {
		cout<<"in.txt: bad file."<<endl;
		exit(1);
	}
}

void make_adjList(vector<vector<bool> >& matrix, vector<list<int>>& adjList) {
	int size = static_cast<int>(matrix.size());
	adjList.resize(size*size);
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (matrix[i][j]) {
				if (i - 1 >= 0 && matrix[i - 1][j]) {
					adjList[i * size + j].push_back((i - 1) * size + j);
				}
				if (i + 1 < size && matrix[i + 1][j]) {
					adjList[i * size + j].push_back((i + 1) * size + j);
				}
				if (j - 1 >= 0 && matrix[i][j - 1]) {
					adjList[i * size + j].push_back(i * size + j - 1);
				}
				if (j + 1 < size && matrix[i][j + 1]) {
					adjList[i * size + j].push_back(i * size + j + 1);
				}
			} else {
				adjList[i * size + j].push_back(-1);
			}
		}
	}
}

int count_pieces(vector<vector<bool> >& matrix, vector<list<int>>& adjList) {
	int size = static_cast<int>(adjList.size());
	vector<bool> closed(size, false);
	int start = -1;
	for (int i = 0; i < size; ++i) {
		if (!adjList[i].empty() && *adjList[i].begin() == -1) {
			closed[i] = true;
		} else {
			if (start == -1) {
				start = i;
			}
		}
	}
	if (start == -1) {
		return 0;
	}
	int count = 0;
	for (;;) {
		++count;
		vector<int> parent;
		BFS(start, adjList, parent);
		for (size_t i = 0; i < parent.size(); ++i) {
			if (parent[i] > -1) {
				closed[i] = true;
			}
		}
		start = -1;
		for (int i = 0; i < static_cast<int>(closed.size()); ++i) {
			if (!closed[i]) {
				start = i;
				break;
			}
		}
		if (start == -1) {
			return count;
		}
	}
}

int main() {
	vector<vector<bool> > matrix;
	vector<list<int>> adjList;
	read_file(matrix);
	make_adjList(matrix, adjList);
	int count = count_pieces( matrix, adjList);
	try {
		fstream write("out.txt");
		write<<count;
		write.close();
	} catch (...) {
		cout<<"out.txt: bad file."<<endl;
		exit(1);
	}
	return 0;
}
