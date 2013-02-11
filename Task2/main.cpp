#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "shortcut.h"

using std::cout;
using std::endl;

int main() {
	int n;//����� �����
	double doub;//���������� ��� ���������� ��������� ������� R 
	std::fstream read("in.txt");//����, �� �������� ����� ����������� ����������
	if(read.good()) {
		read>>n;
	} else {
		cout<<"Bad file!"<<endl;
		return 0;
	}

	try {
		vector<vector<double> > R(n),add_R(n);//���� ������ ��� �������� �������� ������, ������ - ��� -ln �� ���
		for(int i=0; i<n; ++i) {
			R[i].resize(n);
			add_R[i].resize(n);
		}
		int i=0,j=0;//���������� ��� �������������� ������� R
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
		if(vertex > -1) {//��������� �� ������� �������������� �����
			for(int k=0; k<n; ++k) {/*������� �����-���� ������� �� �������������� �����, ����� ������ ����� <=n,
				������� �� n �������� ����� ������ ��� �������*/
				vertex=pred[vertex];
			}

			vector<int> path;
			for(int curr=vertex; ; curr=pred[curr]) {//������ ������������� ����
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
			while(k+1<path.size()) {//���������� �������� ������
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