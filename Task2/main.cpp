#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "shortcut.h"

using std::cout;
using std::endl;

int main() {
	int n;//число валют
	double doub;//переменная для считывания элементов матрицы R 
	std::fstream read("in.txt");//файл, из которого будет считываться информация
	if(read.good()) {
		read>>n;
	} else {
		cout<<"Bad file!"<<endl;
		return 0;
	}

	try {
		vector<vector<double> > R(n),add_R(n);//один массив для хранения обменных курсов, другой - для -ln от них
		for(int i=0; i<n; ++i) {
			R[i].resize(n);
			add_R[i].resize(n);
		}
		int i=0,j=0;//переменные для индексирования матрицы R
		while(read>>doub) {
			R[i][j]=doub;
			if(doub <= 0) {
				add_R[i][j]=numeric_limits<double>::min();
			} else {
				add_R[i][j]=-log(doub);
			}
			++j;
			if(j >= n) {
				j%=n;
				++i;
			}
		}
	
		vector<int> pred;
		vector<double> d;
		int vertex=Belman_ford(add_R,pred,d,0);
		if(vertex > -1) {//проверяем на наличие отрицательного цикла
			for(int k=0; k<n; ++k) {/*находим какую-либо вершину из отрицательного цикла, длина любого цикла <=n,
				поэтому за n итераций точно найдем эту вершину*/
				vertex=pred[vertex];
			}

			vector<int> path;
			for(int curr=vertex; ; curr=pred[curr]) {//находи отрицательный цикл
				path.push_back(curr);
				if(curr == vertex && path.size() > 1) {
					break;
				}
			}
			reverse(path.begin(), path.end());
			for(int k=0; k<(int)path.size(); ++k) {
				cout<<path[k]<<' ';
			}
			cout<<endl;

			size_t k=0;
			int cur, next;
			double percent_of_benefits=1;
			while(k+1<path.size()) {//нахождение процента выгоды
				cur=path[k];
				next=path[++k];
				percent_of_benefits*=R[cur][next];
			}
			cout<<"Percentage of benefits: "<<100.0*percent_of_benefits<<"%"<<endl;
		} else {
			cout<<"No"<<endl;
		}
	} catch(...) {
		cout<<"Bad file"<<endl;
	}
	return 0;
}