#include <list>
#include <vector>
#include <queue>

using std::list;
using std::vector;
using std::deque;

void DFS_visit(int cur, vector<list<int> >& adjList, vector<int>& times, vector<bool>& color) {
	int size = static_cast<int>(adjList.size());
	color[cur] = false;
	times.push_back(cur);

	for (list<int>::iterator iter = adjList[cur].begin(); iter != adjList[cur].end() ; ++iter) {
		if (color[*iter]) {
			DFS_visit(*iter, adjList, times, color);
		}
	}
}

void DFS(vector<list<int> >& adjList, vector<int>& times) { // times соответсвует времени начала обследования вершин
	int size = static_cast<int>(adjList.size());
	vector<bool> color(size, true);
	times.resize(0);
	
	for (int i = 0; i < size; ++i) {
		if (color[i]) {
			DFS_visit(i, adjList, times, color);
        }
	}
}