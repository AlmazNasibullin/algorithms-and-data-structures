#include <iostream>
#include <fstream>
#include <String>
#include "Prima.h"
#include "DFS.h"

using std::fstream;
using std::cout;
using std::cin;
using std::endl;
using std::string;

void print_cur_MST(vector<list<int> >& MST) {
	cout<<"Current MST"<<endl;
	for (int i = 0; i < static_cast<int>(MST.size()); ++i) {
		for (list<int>::iterator iter = MST[i].begin(); iter != MST[i].end(); ++iter) {
			if (i < *iter) {
				cout<<i<<" "<<*iter<<endl;
			}
		}
	}
	cout<<endl;
}

void adding(vector<vector<double> >& adjMatrix, vector<list<int> >& MST) { // функция добавления ребра
	int j, k;// концы добавляемого ребра
	double w; // вес ребра
	cin>>j>>k>>w;
	
	int size = static_cast<int>(adjMatrix.size());
	if (!(j >= 0 && j < size)) {
		cout<<"Vertex "<<j<<" doesn't exists! This edge didn't add."<<endl;
		return;
	}
	if (!(k >= 0 && k < size)) {
		cout<<"Vertex "<<k<<" doesn't exists! This edge didn't add."<<endl;
		return;
	}
	
	if (adjMatrix[j][k] > 0) {
		cout<<"This edge already exists! It didn't add."<<endl;
	} else {
		adjMatrix[j][k] = w;
		adjMatrix[k][j] = w;

		vector<int> cur_pred;
		DFS(MST, cur_pred, k); // для нахождения цикла, полученного при добавлении ребра

		double maximum = w;
		int start = k;
		int end = j;
		for (int i = j; cur_pred[i] > -1; i = cur_pred[i]) { // находим в цикле ребро наибольшего веса
			if (adjMatrix[i][cur_pred[i]] > maximum) {
				maximum = adjMatrix[i][cur_pred[i]];
				start = i;
				end = cur_pred[i];
			}
		}
		if (start != k || end != j) { // добавленное ребро ненаибольшее в цикле
			MST[k].push_back(j);
			MST[j].push_back(k);
			for (list<int>::iterator iter = MST[start].begin(); iter != MST[start].end(); ++iter) { // удаляем ребро наибольшего веса
				if (*iter == end) {
					MST[start].erase(iter);
					break;
				}
			}
			for (list<int>::iterator iter = MST[end].begin(); iter != MST[end].end(); ++iter) { // удаляем ребро наибольшего веса
				if (*iter == start) {
					MST[end].erase(iter);
					break;
				}
			}
		}
	}
	print_cur_MST(MST); // печатаем текущее MST
}

bool in_MST(vector<list<int> >& MST, int j, int k) { // определяем, удаляемое ребро принадлежит MST или нет? Если да, то удаляем его
	for (list<int>::iterator iter = MST[j].begin(); iter != MST[j].end(); ++iter) {
		if (k == *iter) {
			MST[j].erase(iter);
			for (iter = MST[k].begin(); iter != MST[k].end(); ++iter) {
				if (j == *iter) {
					MST[k].erase(iter);
					break;
				}
			}
			return true;
		}
	}
	return false;
}

void deleting(vector<vector<double> >& adjMatrix, vector<list<int> >& MST, bool& exit) { // функция удаления ребра
	int j, k; // концы добавляемого ребра
	cin>>j>>k;
	
	int size = static_cast<int>(adjMatrix.size());
	if (!(j >= 0 && j < size)) {
		cout<<"Vertex "<<j<<" doesn't exists! This edge didn't delete."<<endl;
		return;
	}
	if (!(k >= 0 && k < size)) {
		cout<<"Vertex "<<k<<" doesn't exists! This edge didn't delete."<<endl;
		return;
	}

	if (adjMatrix[j][k] == 0) {
		cout<<"This edge doesn't exist! It didn't delete."<<endl;
	} else {
		bool in_mst = in_MST(MST, j, k);
		adjMatrix[j][k] = 0;
		adjMatrix[k][j] = 0;
		
		if (in_mst) {
			vector<bool> first_subtree;
			Find_Subtree(MST, first_subtree); // находим вершины, которые принадлежат одной из компонент связанности исходного MST

			double minimum = numeric_limits<double>::max();
			int start = -1;
			int end = -1;
			for (size_t i = 0; i < adjMatrix.size(); ++i) { // находим минимальное ребро, связывающее две компоненты связанности
				for (size_t l = 0; l < adjMatrix.size(); ++l) {
					if (first_subtree[i] && !first_subtree[l] && adjMatrix[i][l] > 0) {
						if (adjMatrix[i][l] < minimum) {
							minimum = adjMatrix[i][l];
							start = i;
							end = l;
						}
					}
				}
			}
			if (start == -1) { // после удаления ребра граф становится не связаным
				cout<<"Count of edges is not enough! Graph can not be connected."<<endl;
				exit = true;
				return;
			} else {
				MST[start].push_back(end);
				MST[end].push_back(start);
			}
		}
	}
	print_cur_MST(MST); // печатаем текущее MST
}

void handler(vector<vector<double> >& adjMatrix, vector<list<int >>& MST) { // обработчик входных команд
	bool exit = false;
	while (!exit) {
		string s;
		cin>>s;
		if (s == "exit") {
			exit = true;
		} else if (s == "add") {
			adding(adjMatrix, MST);
		} else if (s == "delete") {
			deleting(adjMatrix, MST, exit);
		} else {
			cout<<"Bad command: "<<s<<endl;
			exit = true;
		}
	}
}

int main() {
	fstream read;
	int n;
	int count;
	vector<list<int> > MST;
	vector<vector<double> > adjMatrix;
	try {
		read.open("in.txt", fstream::in); // файл для считывания
		read>>n;
		read>>count;
		adjMatrix.resize(n);
		for (int i = 0; i < n; ++i) {
			adjMatrix[i].resize(n);
		}
		for (int i = 0; i < count; ++i) {
			int k, j;
			read>>k>>j;
			read>>adjMatrix[k][j];
			adjMatrix[j][k] = adjMatrix[k][j];
		}
	} catch (...) {
		cout<<"Bad file!"<<endl;
		return 1;
	}
	Prima(adjMatrix, MST);
	print_cur_MST(MST);

	handler(adjMatrix, MST);
	/*
	возможные команды и их формат:
	1) "add v1 v2 w", v1, v2 - концы ребра, w - вес ребра
	2) "delete v1 v2", v1, v2 - концы ребра
	3) "exit" - завершение программы
	*/
	return 0;
}