#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

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

	int log = find_log2(level[j]);

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

int main() {
	int size;
	scanf("%d", &size);

	vector<int> level(size + 1, 0);;
	vector<vector<int> > parent(size + 1);
	int log = 19; // этого хватает, т.к. максимальное значение size = 400000

	for (int i = 0; i < size + 1; ++i) {
		parent[i].resize(log);
	}

	for (int i = 0; i < size; ++i) {
		int v;
		int pred_v;
		scanf("%d%d", &v, &pred_v);
		parent[v][0] = pred_v;
		level[v] = level[pred_v] + 1;
	}
	
	for (int j = 1; j < log; ++j) {
		for (int i = 0; i < size + 1; ++i) {
			parent[i][j] = parent[parent[i][j-1]][j-1];
		}
	}

	int count;
	scanf("%d", &count);

	for (int i = 0; i < count; ++i) {
		int first;
		int second;
		scanf("%d%d", &first, &second);
		printf("%d\n", LCA(first, second, level, parent));
	}
    return 0;
}
