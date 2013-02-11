#include <vector>
#include <limits>
#include <map>
#include <set>

using std::vector;
using std::set;
using std::multimap;
using std::numeric_limits;
using std::pair;
using std::make_pair;

void Prima(vector<pair<double,double> >& points, vector<int>& pred) {
	int size = static_cast<int>(points.size());
	vector<double> d(size, numeric_limits<double>::max());
	pred.resize(0);
	pred.assign(size, -1);
	multimap<double, int> Q;
	set<int> closed;
	
	d[0] = 0;
	Q.insert(make_pair(d[0], 0));
	
	while (!Q.empty()) {
		int cur = Q.begin()->second;
		Q.erase(Q.begin());
		closed.insert(cur);
		for (int i = 0; i < size; ++i) {
			if (cur != i && closed.find(i) == closed.end()) {
				double distance = sqrt(pow(points[cur].first - points[i].first, 2) + pow(points[cur].second - points[i].second, 2));
				if (distance < d[i]) {
					multimap<double, int>::iterator iter = Q.find(d[i]); // по-моему, здесь проблем с поиском не будет, потому что 
																		//именно это d[i] я сначала добавлял в качестве ключа, потом по нему и искал
 					pred[i] = cur;
					d[i] = distance;
					if (iter != Q.end()) {
						++count;
						Q.erase(iter);
					}
					Q.insert(make_pair(d[i], i));
				}
			}
		}
	}
}