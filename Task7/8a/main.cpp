#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int find_log2(int N) {
	int logN = 0;
	int num = N>>1;
	while (num != 0) {
		num = num>>1;
		++logN;
	}
	return logN;
}

void RMQ(vector<float>& mas) {
	int N = (int)(mas.size());
	int logN = find_log2(N);

	vector<vector<int> > matrix(N);
	
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

	int size;
	scanf("%d", &size);
	vector<float> result(size);

	for (int l = 0; l < size; ++l) {
		int i;
		int j;
		scanf("%d", &i);
		scanf("%d", &j);
		--j;
		int k = find_log2(j - i + 1);
		pow = 1<<k;
		if (mas[matrix[i][k]] <= mas[matrix[j - pow + 1][k]]) {
			result[l] = mas[matrix[i][k]];
		} else {
			result[l] = mas[matrix[j - pow + 1][k]];
		}
	}

	for (int i = 0; i < size; ++i) {
		printf("%f ",result[i]);
	}
}

int main() {
	int size;
	scanf("%d", &size);
	vector<float> mas(size);
	for (int i = 0; i < size; ++i) {
		scanf("%f", &mas[i]);
	}
	RMQ(mas);
	return 0;
}
