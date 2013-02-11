#include <map>
#include <cmath>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

using std::vector;
using std::swap;
using std::multimap;
using std::make_pair;
using std::pair;
using std::set;
using std::cout;
using std::endl;
using std::fstream;
using std::pair;

struct Status;

typedef multimap<int, Status*>::iterator Iter;
typedef multimap<int, pair<Status*, int> >::iterator Iter_add;
typedef pair<multimap<int, Status*>::iterator, multimap<int, Status*>::iterator> diapason;
typedef pair<multimap<int, pair<Status*, int> >::iterator, multimap<int, pair<Status*, int> >::iterator> diapason_add;

struct Status {
	char matrix[4][4];
	Status* pred;
	int empty_1; // координаты пустой клетки
	int empty_2; // координаты пустой клетки
	int H, G;
	
	Status(const Status& stat): pred(NULL), H(stat.H), G(-1), empty_1(stat.empty_1), empty_2(stat.empty_2) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				matrix[i][j] = stat.matrix[i][j];
			}
		}
	}

	Status(char Matrix[4][4]): pred(NULL), G(0) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (Matrix[i][j] == 0) {
					empty_1 = i;
					empty_2 = j;
				}
				matrix[i][j] = Matrix[i][j];
			}
		}
		counting_H();
	}

	void counting_H() {
		H = 0;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (matrix[i][j] > 0) {
					int k =(static_cast<int>(matrix[i][j]) - 1) / 4;
					int l = static_cast<int>(matrix[i][j]) - k * 4 - 1;
					H += (abs(k - i) + abs(l - j));
				}
			}
		}
		calc_linear_conflicts();
	}

	void calc_linear_conflicts() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = j + 1; k < 4; ++k) {
					int pos1 =(static_cast<int>(matrix[i][j]) - 1) / 4;
					int pos2 =(static_cast<int>(matrix[i][k]) - 1) / 4;
					if (pos1 == pos2 && matrix[i][j] > matrix[i][k] && matrix[i][k] > 0) {
						H +=2;
					}
				}
			}
		}
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = j + 1; k < 4; ++k) {
					int pos1 =(static_cast<int>(matrix[j][i]) - 1) % 4;
					int pos2 =(static_cast<int>(matrix[k][i]) - 1) % 4;
					if (pos1 == pos2 && matrix[j][i] > matrix[k][i] && matrix[k][i] > 0) {
						H +=2;
					}
				}
			}
		}
	}

	void get_neighbors(vector<Status*>& neighbors) {
		neighbors.resize(0);
		if (empty_1 + 1 < 4) {
			Status* stat = new Status(*this);
			swap(stat->matrix[empty_1][empty_2], stat->matrix[empty_1 + 1][empty_2]);
			++stat->empty_1;
			stat->counting_H();
			stat->pred = this;
			stat->G = G + 1;
			neighbors.push_back(stat);
		}
		if (empty_1 - 1 >= 0) {
			Status* stat = new Status(*this);
			swap(stat->matrix[empty_1][empty_2], stat->matrix[empty_1 - 1][empty_2]);
			--stat->empty_1;
			stat->counting_H();
			stat->pred = this;
			stat->G = G + 1;
			neighbors.push_back(stat);
		}
		if (empty_2 + 1 < 4) {
			Status* stat = new Status(*this);
			swap(stat->matrix[empty_1][empty_2], stat->matrix[empty_1][empty_2 + 1]);
			++stat->empty_2;
			stat->counting_H();
			stat->pred = this;
			stat->G = G + 1;
			neighbors.push_back(stat);
		}
		if (empty_2 - 1 >= 0) {
			Status* stat = new Status(*this);
			swap(stat->matrix[empty_1][empty_2], stat->matrix[empty_1][empty_2 - 1]);
			--stat->empty_2;
			stat->counting_H();
			stat->pred = this;
			stat->G = G + 1;
			neighbors.push_back(stat);
		}
	}

	bool operator < (const Status& stat) const{
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (matrix[i][j] < stat.matrix[i][j]) {
					return true;
				} else if (matrix[i][j] > stat.matrix[i][j]) {
					return false;
				}
			}
		}
		return false;
	}

	bool operator == (const Status& stat) const{
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (matrix[i][j] != stat.matrix[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	bool operator != (const Status& stat) const {
		return !(*this == stat);
	}

	Status& operator = (const Status& stat) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				matrix[i][j] = stat.matrix[i][j];
			}
		}
		pred = stat.pred;
		empty_1 = stat.empty_1;
		empty_2 = stat.empty_2;
		H = stat.H;
		G = stat.G;
		return *this;
	}

	void print() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				cout<<static_cast<int>(matrix[i][j])<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
};

Status* get_final_stat() { // генерирует завершающее состояние
	char Matrix[4][4];
	short counter = 0;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			++counter;
			Matrix[i][j] = static_cast<char>(counter);
		}
	}
	Matrix[3][3] = 0;
	return new Status(Matrix);
}

bool check_configuration (Status* stat) { // проверяет состояние на достижимость из него конечного
	int count = stat->empty_1;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (stat->matrix[i][j] > 0) {
				for (int m = j + 1; m < 4; ++m) {
					if (stat->matrix[i][j] > stat->matrix[i][m] && stat->matrix[i][m] != 0) {
						++count;
					}
				}
				for (int k = i + 1; k < 4; ++k) {
					for (int l = 0; l < 4; ++l) {
						if (stat->matrix[i][j] > stat->matrix[k][l] && stat->matrix[k][l] != 0) {
							++count;
						}
					}
				}
			}
		}
	}
	return (count%2) != 0;
}

void print_path(Status* final, Status* start_stat) {
	vector<Status*> path;
	for ( ; final != start_stat; final = final->pred) { // находим путь он начального состояния до завершающего
		path.push_back(final);
	}
	path.push_back(start_stat);
	cout<<"Path:"<<endl<<endl;
	for (vector<Status*>::reverse_iterator iter = path.rbegin(); iter != path.rend(); ++iter) {
		(*iter)->print();
		cout<<endl;
	}
	cout<<"Count: "<<static_cast<int>(path.size())-1<<endl; // кол-во ходов
}

void clear_memory(multimap<int, Status*>& PQ, multimap<int, Status*>& closed_stats, 
				  multimap<int, pair<Status*, int> >& intermediate_stats) {
	for (Iter iter = PQ.begin(); iter != PQ.end(); ++iter) { // очищаем память
		delete iter->second;
	}
	for (Iter iter = closed_stats.begin(); iter != closed_stats.end(); ++iter) {
		delete iter->second;
	}
	intermediate_stats.clear();
	closed_stats.clear();
	PQ.clear();
}

void analyse_neighbors(vector<Status*>& neighbors, multimap<int, Status*>& closed_stats, multimap<int, 
					   pair<Status*, int> >& intermediate_stats, Status* cur_stat, multimap<int, Status*>& PQ) {
	for (size_t i = 0; i < neighbors.size(); ++i){
		bool in_closed_stats = false; // находится или нет вершина в кратчайшем пути на данный момент
		bool in_intermediate_stats = false; // находится или нет вершина в очереди с приоритетом на данный момент
		int gh;

		if (closed_stats.find(neighbors[i]->H) == closed_stats.end()) {
			in_closed_stats = false;
		} else {
			diapason diapas = closed_stats.equal_range(neighbors[i]->H);
			for (Iter iter = diapas.first; iter != diapas.second; ++iter) {
				if (*neighbors[i] == *(iter->second)) {
					in_closed_stats = true;
					break;
				}
			}
		}
		if (intermediate_stats.find(neighbors[i]->H) == intermediate_stats.end()) {
			in_intermediate_stats = false;
		} else {
			diapason_add diapas_add = intermediate_stats.equal_range(neighbors[i]->H);
			for (Iter_add iter = diapas_add.first; iter != diapas_add.second; ++iter) {
				if (*neighbors[i] == *(iter->second.first)) {
					in_intermediate_stats = true;
					gh = iter->second.second;
					break;
				}
			}
		}
		if (!(in_closed_stats || in_intermediate_stats)) {
			neighbors[i]->G = 1 + cur_stat->G;
			neighbors[i]->pred = cur_stat;
			PQ.insert(make_pair(neighbors[i]->G + neighbors[i]->H, neighbors[i]));
			intermediate_stats.insert(make_pair(neighbors[i]->H, make_pair(neighbors[i], neighbors[i]->G + neighbors[i]->H)));
		} else if (in_intermediate_stats) {
			diapason diapas = PQ.equal_range(gh);
			for (Iter iter = diapas.first; iter != diapas.second; ++iter) {
				if (*(iter->second) == *neighbors[i]) {
					if(gh - neighbors[i]->H > cur_stat->G + 1) {
						neighbors[i]->G = cur_stat->G + 1;
						PQ.erase(iter);
						neighbors[i]->pred = cur_stat;
						PQ.insert(make_pair(neighbors[i]->G + neighbors[i]->H, neighbors[i]));
						diapason_add diapas_add = intermediate_stats.equal_range(neighbors[i]->H);
						for (Iter_add iter = diapas_add.first; iter != diapas_add.second; ++iter) {
							if (*neighbors[i] == *(iter->second.first)) {
								iter->second.second = neighbors[i]->G + neighbors[i]->H;
							}
							break;
						}
					}
					break;
				}
			}
		}
	}
}

void barley_break(Status* start_stat) {
	Status* final_stat = get_final_stat();
	multimap<int, Status*> PQ;
	multimap<int, Status*> closed_stats; // состояния, которые уже лежат в кратчайшем пути
	multimap<int, pair<Status*, int> > intermediate_stats; // состояния, которые находятся в очереди
	start_stat->G = 0;
	PQ.insert(make_pair(start_stat->G + start_stat->H, start_stat));
	Status* final;

	while (!PQ.empty()) {
		Status* cur_stat = PQ.begin()->second;
		PQ.erase(PQ.begin());
		closed_stats.insert(make_pair(cur_stat->H, cur_stat));
		if (intermediate_stats.find(cur_stat->H) != intermediate_stats.end()) { // удаляем состояние, которое только что достали из очереди, из intermediate_stats
			diapason_add diapas_add = intermediate_stats.equal_range(cur_stat->H);
			for (Iter_add iter = diapas_add.first; iter != diapas_add.second; ++iter) {
				if (*cur_stat == *(iter->second.first)) {
					intermediate_stats.erase(iter);
					break;
				}
			}
		}
		if (*cur_stat == *final_stat) { // проверяем, не достигли ли конечного состояния
			final = cur_stat;
			break;
		}
		vector<Status*> neighbors;
		cur_stat->get_neighbors(neighbors); // получаеи соседей текущей вершины
		analyse_neighbors(neighbors, closed_stats, intermediate_stats,cur_stat, PQ);
	}
	print_path(final, start_stat);
	clear_memory(PQ, closed_stats, intermediate_stats);
}

int main() {
	char start_matrix[4][4];
	fstream read("in.txt"); // файл для считывания
	try {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				short sh;
				read>>sh;
				start_matrix[i][j] = static_cast<char>(sh);
			}
		}
	} catch (...) {
		cout<<"Bad file"<<endl;
	}
	Status* start_stat = new Status(start_matrix);
	if (check_configuration(start_stat)) {
		barley_break(start_stat);
	} else {
		cout<<"NO"<<endl;
	}
	return 0;
}