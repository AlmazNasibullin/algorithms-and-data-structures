#include <vector>
#include <cmath>

using std::vector;
using std::numeric_limits;
using std::pair;

void Close_Neighbor(vector<pair<double,double> >& points, vector<int>& pred, int start) {
	int size = static_cast<int> (points.size());
	vector<bool> closed;
	closed.assign(size, false);
	int cur = start;
	pred.resize(size);

	for (int i = 0; i < size; ++i) {
		closed[cur] = true;
		double Min = numeric_limits<double>::max();
		int Min_vertex;
		for (int j = 0; j < size; ++j) {
			if (!closed[j]) {
				double distance = sqrt(pow(points[cur].first - points[j].first, 2) + pow(points[cur].second - points[j].second, 2));
				if (distance < Min) { 
					Min = distance;
					Min_vertex = j;
				}
			}
		}
		if (i+1 == size) {
			pred[start] = cur;
		} else {
			pred[Min_vertex] = cur;
			cur = Min_vertex;
		}
	}
}