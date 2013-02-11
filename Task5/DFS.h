#include <list>
#include <vector>
#include <queue>

using std::list;
using std::vector;
using std::deque;

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

void Find_Subtree_visit(int cur, vector<list<int> >& adjList, vector<bool>& first_subtree) {
	int size = static_cast<int>(adjList.size());
	first_subtree[cur] = true;

	for (list<int>::iterator iter = adjList[cur].begin(); iter != adjList[cur].end() ; ++iter) {
		if (!first_subtree[*iter]) {
			Find_Subtree_visit(*iter, adjList, first_subtree);
		}
	}
}

void Find_Subtree(vector<list<int> >& adjList, vector<bool>& first_subtree) { // находим связный подграф и распавшемся дереве
	int size = static_cast<int>(adjList.size());
	first_subtree.resize(size, false);
	Find_Subtree_visit(0, adjList, first_subtree); // запуск происходит единожды, т.к. ищем один связный подграф
}