#include <iostream>
#include <ctime>
#include <cmath>
#include <hash_map>
#include "Prima.h"
#include "Close_Neighbor.h"
#include "DFS.h"

using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
using stdext::hash_map;

double get_distance(vector<pair<double,double> >& points, int i, int j) {
	return sqrt(pow(points[i].first - points[j].first, 2) + pow(points[i].second - points[j].second, 2));
}

void generate_graph(vector<pair<double,double> >& start_points, vector<pair<double,double> >& points,
					int count1, int count2, double s1, double s2) {
	hash_map<double, double> hm; // для хранения значения натурального логарифма

	for (int i = 0; i < count1; ++i) { // генерация вспомогательных точек
		double d1 = 1.0 - static_cast<double>(rand())/RAND_MAX;
		double d2 = 1.0 - static_cast<double>(rand())/RAND_MAX;
		double COS = cos(2.0 * 3.14 * d1);
		
		double LOG;
		hash_map<double, double>::iterator iter = hm.find(d2);
		if (iter != hm.end()) {
			LOG = iter->second;
		} else {
			LOG = log(d2);
			hm.insert(make_pair(d2, LOG));
		}
		start_points[i] = make_pair(s1 * COS * sqrt(-2 * LOG), s2 * sqrt (1.0 - COS * COS) * sqrt(-2 * LOG));
	}
	for (int i = 0; i < count1; ++i) { // генерация точек
		for (int j = 0; j < count2; ++j) {
			double d1 = 1.0 - static_cast<double>(rand())/RAND_MAX;
			double d2 = 1.0 - static_cast<double>(rand())/RAND_MAX;
			double COS = cos(2.0 * 3.14 * d1);

			double LOG;
			hash_map<double, double>::iterator iter = hm.find(d2);
			if (iter != hm.end()) {
				LOG = iter->second;
			} else {
				LOG = log(d2);
				hm.insert(make_pair(d2, LOG));
			}
			points[i * count2+j] = make_pair(s2 * COS * sqrt(-2 * LOG) + start_points[i].first,
				s2 * sqrt(1 - COS * COS) * sqrt(-2 * LOG) + start_points[i].second);
		}
	}
	hm.clear();
}

int main() {
	srand(time(NULL));
	const int count1 = 100; // соответствующее кол-во точек
	const int count2 = 100; // соответствующее кол-во точек
	const double s1 = 100;
	const double s2 = 10;
	vector<pair<double,double> > start_points(count1);
	vector<pair<double,double> > points(count1 * count2);
	generate_graph(start_points, points, count1, count2, s1, s2);

	vector<int> pred;
	clock_t t = clock();
	Close_Neighbor(points, pred, 0);
	cout<<"Close_Neighbor time: "<<static_cast<double>(clock() - t) / CLOCKS_PER_SEC<<endl;
	double weight1 = 0, weight2 = 0; // сумма обхода ребер в соответсвующих алгоритмах
	for (size_t i = 0; i < pred.size(); ++i) {
		weight1 += get_distance(points, i, pred[i]);
	}
	cout<<"Close_Neighbor: "<<weight1<<endl;
	pred.resize(0);

	t = clock();
	Prima(points, pred);
	cout<<"Prima time: "<<static_cast<double>(clock() - t) / CLOCKS_PER_SEC<<endl;
	vector<list<int> > MST(count1 * count2);
	for (int i = 0; i < static_cast<int>(pred.size()); ++i) {
		if (pred[i] > -1) {
			MST[i].push_back(pred[i]);
			MST[pred[i]].push_back(i);
		}
	}
	vector<int> times;
	DFS(MST, times);

	for (int i = 1; i < static_cast<int>(times.size()); ++i) {
		weight2 += get_distance(points, i, pred[i]);
	}
	weight2 += get_distance(points, times[0], times[static_cast<int>(times.size())-1]);

	cout<<"Prima: "<<weight2<<endl;
}