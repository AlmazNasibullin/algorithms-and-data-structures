#include <vector>
#include <limits>
#include <iostream>
#include <fstream>

using namespace std;

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
	
	d.assign(size, numeric_limits<T>::max());//делаем это для того, чтобы d имел нужный размер и данные
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

void read_file(vector<vector<double> >& adjMatrix, int& start ,int& end) {
	try {
		fstream read("in.txt");
		int size;
		read>>size;
		read>>start>>end;
		adjMatrix.resize(size);
		for (int i = 0; i < size; ++i) {
			adjMatrix[i].resize(size);
			for (int j = 0; j < size; ++j) {
				adjMatrix[i][j] = numeric_limits<double>::min();
			}
		}
		while (read.good()) {
			int i;
			int j;
			double w;
			read>>i>>j>>w;
			adjMatrix[i][j] = w;
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

	read_file(adjMatrix, start, end);

	vector<int> pred;
	vector<double> d;
	if (Belman_ford(adjMatrix, pred, d, start) > -1 || pred[end] == -1) {
		try {
			fstream write("out.txt");
			write<<"NO";
			write.close();
		} catch (...) {
			cout<<"out.txt: bad file."<<endl;
			exit(1);
		}
	} else {
		double MIN = numeric_limits<double>::max();
		int prev = -1;
		int pred_prev = -1;
		double w_prev = 0;
		for (int i = end; pred[i] != -1; i = pred[i]) {
			if (prev != -1) {
				adjMatrix[pred_prev][prev] = w_prev;
			}
			adjMatrix[pred[i]][i] = numeric_limits<double>::min();

			vector<int> parent;
			Belman_ford(adjMatrix, parent, d, start);
			if (d[end] < MIN) {
				MIN = d[end];
			}

			prev = i;
			pred_prev = pred[i];
			w_prev = adjMatrix[pred[i]][i];
		}

		if (MIN < numeric_limits<double>::max()) {
			try {
				fstream write("out.txt");
				write<<"YES"<<endl;
				write<<MIN;
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
	}
	return 0;
}