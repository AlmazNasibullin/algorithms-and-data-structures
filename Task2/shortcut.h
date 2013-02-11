#include <vector>
#include <limits>
using std::vector;
using std::numeric_limits;

template <typename T>
void Relax(int u, int v, const vector<vector<T> >& AdjMatrix, vector<T>& d, vector<int>& pred, bool& continue_of_search) {
	if(d[v] > d[u] + AdjMatrix[u][v]) {
		d[v] = d[u] + AdjMatrix[u][v];
		pred[v] = u;
		continue_of_search = true;
	}
}

template <typename T>
int Belman_ford(const vector<vector<T> >& AdjMatrix, vector<int>& pred, vector<T>& d, int start_vertex) {//vector<int> pred для хранения предков в крат. путях
	int size = static_cast<int> (AdjMatrix.size());//считаем, что если вершины u и v не соединены, то AdjMatrix[u][v]==numeric_limits<T>::min
	
	d.assign(size, numeric_limits<int>::max());//делаем это для того, чтобы d имел нужный размер и данные
	d[start_vertex]=0;
	
	pred.assign(size, -1);//делаем это для того, чтобы pred имел нужный размер и данные
	
	bool continue_of_search = true;
	for(int i=0; i+1<size; ++i) {
		if(continue_of_search) {
			continue_of_search = false;
			for(int k=0; k<size; ++k) {
				for(int j=0; j<size; ++j) {
					if(AdjMatrix[k][j] != numeric_limits<T>::min()) { /* если в графе есть отрицательные ребра, то вершины не соединены 
																	  <=> соединяющее их ребро имеет вес numeric_limits<T>::min*/
						Relax(k, j, AdjMatrix, d, pred, continue_of_search);
					}
				}
			}
		}
	}
	for(int k=0; k<size; ++k) {
		for(int j=0; j<size; ++j) {
			if(AdjMatrix[k][j] != numeric_limits<T>::min() && d[j] > d[k] + AdjMatrix[k][j]) {
				return j;//возвращается номер вершины, коротчайший путь до которой содержит отрицательный цикл
			}
		}
	}
	return -1;
}