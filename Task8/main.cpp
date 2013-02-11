#include <iostream>
#include <vector>
#include <list>

using namespace std;

void DFS_visit(int cur, vector<list<int> >& adjList, vector<bool>& color, vector<int>& level, int lev,int& max_depth) {
	color[cur] = false;
	level[cur] = lev;
	if (lev > max_depth) {
		max_depth = lev;
	}

	for (list<int>::iterator iter = adjList[cur].begin(); iter != adjList[cur].end() ; ++iter) {
		if (color[*iter]) {
			DFS_visit(*iter, adjList, color, level, lev + 1, max_depth);
		}
	}
}

int DFS(vector<list<int> >& adjList, vector<int>& level) {
	int size = static_cast<int>(adjList.size());
	vector<bool> color(size, true);
	level.resize(size);
	int lev = 0;
	int max_depth = 0;

	DFS_visit(0, adjList, color, level, lev, max_depth);
	
	return max_depth;
}

int find_log2(int N) {
	int logN = 0;
	int num = N>>1;

	while (num != 0) {
		num = num>>1;
		++logN;
	}

	return logN;
}

void make_same_level(int& i, int& j, vector<int>& level, vector<vector<int> >& parent) {
	// level[j] > level[i]
	while (level[j] != level[i]) {
		int delta = level[j] - level[i];
		int log = find_log2(delta);
		j = parent[j][log];
	}
}

int LCA(int i, int j, vector<int>& level, vector<vector<int> >& parent) {
	if (level[i] > level[j]) {
		make_same_level(j, i, level, parent);
	} else if (level[i] < level[j]) {
		make_same_level(i, j, level, parent);
	}

	if (i == j) {
		return i;
	}

	int log = find_log2(level[j]) + 1;

	while (parent[i][0] != parent[j][0]) {
		while(log >= 0) {
			if (parent[i][log] != parent[j][log]) {
				i = parent[i][log];
				j = parent[j][log];
				break;
			}
			--log;
		}
	}
	return parent[i][0];
}

void get_result(vector<int>& level, vector<pair<int, int> >& requests, vector<vector<int> >& parent) {
	for (size_t i = 0; i < requests.size(); ++i) {
		printf("%d\n", LCA(requests[i].first, requests[i].second, level, parent));
	}
}

int main() {
	int size;
	scanf("%d", &size);

	vector<int> mas(size + 1, 0);
	vector<list<int> > adjList(size + 1);
	vector<int> level;
	vector<pair<int, int> > requests;
	vector<vector<int> > parent(size + 1);

	for (int i = 0; i < size; ++i) {
		int v;
		int pred_v;
		scanf("%d%d", &v, &pred_v);
		mas[v] = pred_v;
		adjList[v].push_back(pred_v);
		adjList[pred_v].push_back(v);
	}

	int count;
	scanf("%d", &count);
	
	requests.resize(count);
	for (int i = 0; i < count; ++i) {
		scanf("%d%d", &requests[i].first, &requests[i].second);
	}

	int max_depth = DFS(adjList, level);
	int log = find_log2(max_depth) + 2;

	for (int i = 0; i < size + 1; ++i) {
		parent[i].resize(log);
	}
	for (int i = 0; i < size + 1; ++i) {
		parent[i][0] = mas[i];
	}
	for (int j = 1; j < log; ++j) {
		for (int i = 0; i < size + 1; ++i) {
			parent[i][j] = parent[parent[i][j-1]][j-1];
		}
	}

	get_result(level, requests, parent);
	return 0;
}
