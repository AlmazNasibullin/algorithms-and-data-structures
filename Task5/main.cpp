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

void adding(vector<vector<double> >& adjMatrix, vector<list<int> >& MST) { // ������� ���������� �����
	int j, k;// ����� ������������ �����
	double w; // ��� �����
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
		DFS(MST, cur_pred, k); // ��� ���������� �����, ����������� ��� ���������� �����

		double maximum = w;
		int start = k;
		int end = j;
		for (int i = j; cur_pred[i] > -1; i = cur_pred[i]) { // ������� � ����� ����� ����������� ����
			if (adjMatrix[i][cur_pred[i]] > maximum) {
				maximum = adjMatrix[i][cur_pred[i]];
				start = i;
				end = cur_pred[i];
			}
		}
		if (start != k || end != j) { // ����������� ����� ������������ � �����
			MST[k].push_back(j);
			MST[j].push_back(k);
			for (list<int>::iterator iter = MST[start].begin(); iter != MST[start].end(); ++iter) { // ������� ����� ����������� ����
				if (*iter == end) {
					MST[start].erase(iter);
					break;
				}
			}
			for (list<int>::iterator iter = MST[end].begin(); iter != MST[end].end(); ++iter) { // ������� ����� ����������� ����
				if (*iter == start) {
					MST[end].erase(iter);
					break;
				}
			}
		}
	}
	print_cur_MST(MST); // �������� ������� MST
}

bool in_MST(vector<list<int> >& MST, int j, int k) { // ����������, ��������� ����� ����������� MST ��� ���? ���� ��, �� ������� ���
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

void deleting(vector<vector<double> >& adjMatrix, vector<list<int> >& MST, bool& exit) { // ������� �������� �����
	int j, k; // ����� ������������ �����
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
			Find_Subtree(MST, first_subtree); // ������� �������, ������� ����������� ����� �� ��������� ����������� ��������� MST

			double minimum = numeric_limits<double>::max();
			int start = -1;
			int end = -1;
			for (size_t i = 0; i < adjMatrix.size(); ++i) { // ������� ����������� �����, ����������� ��� ���������� �����������
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
			if (start == -1) { // ����� �������� ����� ���� ���������� �� ��������
				cout<<"Count of edges is not enough! Graph can not be connected."<<endl;
				exit = true;
				return;
			} else {
				MST[start].push_back(end);
				MST[end].push_back(start);
			}
		}
	}
	print_cur_MST(MST); // �������� ������� MST
}

void handler(vector<vector<double> >& adjMatrix, vector<list<int >>& MST) { // ���������� ������� ������
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
		read.open("in.txt", fstream::in); // ���� ��� ����������
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
	��������� ������� � �� ������:
	1) "add v1 v2 w", v1, v2 - ����� �����, w - ��� �����
	2) "delete v1 v2", v1, v2 - ����� �����
	3) "exit" - ���������� ���������
	*/
	return 0;
}