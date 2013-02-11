#include <vector>
#include <limits>
#include <map>
#include <set>
#include <list>

using std::vector;
using std::set;
using std::list;
using std::multimap;
using std::numeric_limits;
using std::pair;
using std::make_pair;

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