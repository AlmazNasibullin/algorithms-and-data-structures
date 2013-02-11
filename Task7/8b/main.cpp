#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::fstream;
using std::cerr;
using std::pair;
using std::min;
using std::string;
using std::cin;

void read_file(vector<int>& mas, vector<pair<int, int>>& requests) {
	try {
		fstream read("in.txt");
		int size;
		read>>size;
		mas.resize(size);
		for (int i = 0; i < size; ++i) {
			read>>mas[i];
		}
		read>>size;
		requests.resize(size);
		for (int i = 0; i < size; ++i) {
			read>>requests[i].first>>requests[i].second;
		}
		read.close();
	} catch (...) {
		cerr<<"Bad file!"<<endl;
		exit(1);
	}
}

int find_NOD(int num1, int num2) {
	if (num1 == 0) {
		return num2;
	}
	if (num2 == 0) {
		return num1;
	}
	if (num1 > num2) {
		return find_NOD(num2, num1 - (num1 / num2) * num2);
	}
	return find_NOD(num1, num2 - (num2 / num1) * num1);
}

void build(int beg, int end, vector<int>& ST, int cur, vector<int>& mas) {
	if (beg < end) {
		int mid = (end + beg) / 2;
		build (beg, mid, ST, 2 * cur, mas);
		build (mid + 1, end, ST, 2 * cur + 1, mas);
		ST[cur] = find_NOD(ST[2 * cur], ST[2 * cur + 1]);
	} else {
		ST[cur] = mas[beg];
	}
}

void build_segment_tree(vector<int>& mas, vector<int>& ST) {
	int mas_size = static_cast<int>(mas.size());
	int ST_size = 4 * mas_size;
	ST.assign(ST_size, -1);
	build(0, mas_size - 1, ST, 1, mas);
}

int find_NOD_from_ST(vector<int>& ST,int beg, int end, int cur, int find_beg, int find_end) {
	// минимум находится на отрезке от find_beg до find_end, включая концы

	if (beg == find_beg && end == find_end) { // входной отрезок полностью совпал с текущим
		return ST[cur];
	}
	int mid = (end + beg) / 2;
	if (beg <= find_beg && find_end <= mid) { // входной отрезок лежит полностью в левом поддереве
		return find_NOD_from_ST(ST, beg, mid, 2 * cur, find_beg, find_end);
	}
	if (mid + 1 <= find_beg && find_end <= end) { // входной отрезок лежит полностью в правом поддереве
		return find_NOD_from_ST(ST, mid + 1, end, 2 * cur + 1, find_beg, find_end);
	}

	// входной отрезок лежит частью в левом поддереве, частью - в правом
	int leftNOD = find_NOD_from_ST(ST, beg, mid, 2 * cur, find_beg, mid);
	int rightNOD = find_NOD_from_ST(ST, mid + 1, end, 2 * cur + 1, mid + 1, find_end);
	return find_NOD(leftNOD, rightNOD);
}

void RNODQ(vector<int>& ST, vector<pair<int, int>>& requests, vector<int>& result, int mas_size) {
	int req_size = static_cast<int>(requests.size());
	result.resize(req_size);

	for (int l = 0; l < req_size; ++l) {
		int i = requests[l].first;
		int j = requests[l].second;
		result[l] = find_NOD_from_ST(ST, 0, mas_size - 1, 1, i, j);
	}
}

void update_ST(vector<int>& ST, int index, int value, int mas_size) { // обновление дерева отрезков по индексу во входном массиве
	int beg = 0;
	int end = mas_size - 1;
	int ind = 1;

	while (beg < end) {
		int mid = (beg + end) / 2;
		if (index <= mid) {
			end = mid;
			ind *= 2;
		} else {
			beg = mid + 1;
			ind = 2 * ind + 1;
		}
	}

	ST[ind] = value;

	while (ind >= 1) {
		ind /= 2;
		ST[ind] = find_NOD(ST[2 * ind], ST[2 * ind + 1]);
	}
}

void handler_console(vector<int>& ST, int mas_size) {
	cout<<"You can update sparce table and get NOD of range."<<endl;
	cout<<"You should put 'update index value' to update sparce table. "<<endl;
	cout<<"To get NOD of range you should put 'get range_start range_end'."<<endl;
	cout<<"If you want to exit you should put 'exit'"<<endl;

	for (;;) {
		string in;
		cin>>in;
		if (in == "exit") {
			exit(0);
		}
		if (in == "update") {
			int index;
			int value;
			cin>>index>>value;
			if (value <= 0) {
				cerr<<"Bad value: "<<value<<endl;
				exit(1);
			}
			update_ST(ST, index, value, mas_size);
		} else if (in == "get") {
			int beg;
			int end;
			cin>>beg>>end;
			if (!(0 <= beg && beg <= end && end < mas_size)) {
				cerr<<"Bad range: "<<beg<<" "<<end<<endl;
				exit(1);
			}
			cout<<find_NOD_from_ST(ST, 0, mas_size - 1, 1, beg, end)<<endl;
		} else {
			cerr<<"Bad command: "<<in<<endl;
			exit(1);
		}
	}
}

int main() {
	vector<int> mas; // входной массив чисел
	vector<pair<int, int>> requests; // запрросы
	vector<int> ST; // дерево отрезков
	vector<int> result; // результаты запросов

	read_file(mas, requests);
	build_segment_tree(mas,ST);
	RNODQ(ST, requests, result, static_cast<int>(mas.size()));

	cout<<"NODs:"<<endl;
	for (size_t i = 0; i < result.size(); ++i) {
		cout<<result[i]<<endl;
	}
	cout<<endl;

	handler_console(ST, static_cast<int>(mas.size()));
	return 0;
}