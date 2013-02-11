#include <vector>
#include <limits>
#include <map>
#include <set>
#include <list>
#include <fstream>
#include <iostream>

using namespace std;

template <typename T>
void Prima(vector<vector<T> >& adjMatrix, vector<list<int> >& MST) {
	int size = static_cast<int>(adjMatrix.size());
	vector<T> d(size, numeric_limits<T>::max());
	vector<int> pred(size, -1);
	multimap<T, int> Q;
	set<int> closed;
	MST.resize(0);
	MST.resize(size);

	d[0] = 0;
	Q.insert(make_pair(d[0], 0));

	while (!Q.empty()) {
		int cur = Q.begin()->second;
		Q.erase(Q.begin());
		closed.insert(cur);
		for (int i = 0; i < size; ++i) {
			if (adjMatrix[cur][i] != 0 && closed.find(i) == closed.end() && adjMatrix[cur][i] < d[i]) {
				pred[i] = cur;
				pair<multimap<T, int>::iterator, multimap<T, int>::iterator> range = Q.equal_range(d[i]);
				d[i] = adjMatrix[cur][i];
				for ( ; range.first != range.second; ++range.first) {
					if (range.first->second == i) {
						Q.erase(range.first);
						break;
					}
				}
				Q.insert(make_pair(d[i], i));
			}
		}
	}
	for (size_t i = 0; i < pred.size(); ++i) {
		if (pred[i] > -1) {
			MST[i].push_back(pred[i]);
			MST[pred[i]].push_back(i);
		}
	}
}

void DFS_visit(int cur, vector<list<int> >& adjList, vector<bool>& color, vector<int>& pred) {
	color[cur] = false;

	for (list<int>::iterator iter = adjList[cur].begin(); iter != adjList[cur].end() ; ++iter) {
		if (color[*iter]) {
			pred[*iter] = cur;
			DFS_visit(*iter, adjList, color, pred);
		}
	}
}

void DFS(vector<list<int> >& adjList, vector<int>& pred, int start) {
	int size = static_cast<int>(adjList.size());
	vector<bool> color(size, true);
	pred.resize(size, 0);

	pred[start] = -1;
	DFS_visit(start, adjList, color, pred);
	for (int i = 0; i < size; ++i) {
		if (color[i]) {
			DFS_visit(i, adjList, color, pred);
        }
	}
}

void read_file(vector<vector<double> >& adjMatrix, int& start ,int& end, bool& yes) {
	try {
		fstream read("in.txt");
		int size;
		read>>size;
		read>>start>>end;
		adjMatrix.resize(size);
		for (int i = 0; i < size; ++i) {
			adjMatrix[i].resize(size);
		}
		while (read.good()) {
			int i;
			int j;
			double w;
			read>>i>>j>>w;
			adjMatrix[i][j] = w;
			adjMatrix[j][i] = w;
		}
		if (adjMatrix[start][end] > 0) {
			yes = true;
		} else {
			yes = false;
		}
		read.close();
	} catch (...) {
		cout<<"in.txt: bad file."<<endl;
		exit(1);
	}
}

int main() {
	vector<vector<double> > adjMatrix;
	int start;
	int end;
	bool yes;
	read_file(adjMatrix, start , end, yes);
	if (yes) {
		vector<list<int> > MST;
		Prima(adjMatrix, MST);
		bool rebuild_MST = true;
		for (size_t i = 0; i < MST.size(); ++i) {
			for (list<int>::iterator iter = MST[i].begin(); iter != MST[i].end(); ++iter) {
				if (i == start && *iter == end) {
					rebuild_MST = false;
					break;
				}
			}
		}
		if (rebuild_MST) {
			vector<int> cur_pred;
			DFS(MST, cur_pred, start);
			int st;
			int en;
			double maximum = numeric_limits<double>::min();
			for (int i = end; cur_pred[i] > -1; i = cur_pred[i]) {
				if (adjMatrix[i][cur_pred[i]] > maximum) {
					maximum = adjMatrix[i][cur_pred[i]];
					st = i;
					en = cur_pred[i];
				}
			}
			for (list<int>::iterator iter = MST[st].begin(); iter != MST[st].end(); ++iter) {
				if (*iter == en) {
					MST[st].erase(iter);
					break;
				}
			}
			for (list<int>::iterator iter = MST[en].begin(); iter != MST[en].end(); ++iter) {
				if (*iter == st) {
					MST[en].erase(iter);
					break;
				}
			}
			MST[start].push_back(end);
			MST[end].push_back(start);
		}
		try {
			fstream write("out.txt");
			write<<"YES"<<endl;
			for (size_t i = 0; i < MST.size(); ++i) {
				for (list<int>::iterator iter = MST[i].begin(); iter != MST[i].end(); ++iter) {
					if (i < *iter) {
						write<<i<<" "<<*iter<<" "<<adjMatrix[i][*iter]<<endl;
					}
				}
			}
			write.close();
		} catch (...) {
			cout<<"out.txt: bad file."<<endl;
			exit(1);
		}
	} else {
		try {
			fstream write("out.txt");
			write<<"NO";
			write.close();
		} catch (...) {
			cout<<"out.txt: bad file."<<endl;
			exit(1);
		}
	}
	return 0;
}