#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using std::cout;
using std::endl;
using std::vector;
using std::fstream;
using std::cerr;
using std::pair;

void read_file(vector<double>& mas, vector<pair<int, int>>& requests) {
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

int find_log2(int N) {
	int logN = 0;
	int num = N>>1;
	while (num != 0) {
		num = num>>1;
		++logN;
	}
	return logN;
}

void RMQ(vector<double>& mas, vector<pair<int, int>>& requests, vector<int>& result) {
	int size = static_cast<int>(requests.size());
	result.resize(size);

	int N = static_cast<int>(mas.size());
	vector<vector<int> > matrix(N);
	
	int logN = find_log2(N);
	
	for (int i = 0; i < N; ++i) {
		matrix[i].resize(logN  + 1);
		matrix[i][0] = i;
	}
	
	int pow = 1;
	for (int j = 1; j < logN  + 1; ++j) {
		for (int i = 0; i + pow < N; ++i) {
			if (mas[matrix[i][j - 1]] <= mas[matrix[i + pow][j - 1]]) {
				matrix[i][j] = matrix[i][j - 1];
			} else {
				matrix[i][j] = matrix[i + pow][j - 1];
			}
		}
		pow *= 2;
	}

	for (int l = 0; l < size; ++l) {
		// минимум находитс€ на отрезке от i до j, включа€ концы
		int i = requests[l].first;
		int j = requests[l].second;
		int k = find_log2(j - i + 1);
		pow = 1<<k;
		if (mas[matrix[i][k]] <= mas[matrix[j - pow + 1][k]]) {
			result[l] = matrix[i][k];
		} else {
			result[l] = matrix[j - pow + 1][k];
		}
	}
}

int main() {
	vector<double> mas;
	vector<pair<int, int>> requests;
	vector<int> result;

	read_file(mas, requests);
	RMQ(mas, requests, result);

	for (size_t i = 0; i < result.size(); ++i) {
		cout<<requests[i].first<<"\t"<<requests[i].second<<"\t"<<mas[result[i]]<<endl;
	}
	return 0;
}