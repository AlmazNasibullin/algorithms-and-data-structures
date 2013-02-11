#include <iostream>
#include "BiGraph.h"

int main() {
	const int n=10;
	BiGraph<int> my_graph;
	for(int i=0;i<n;++i) {//��������� � ���� 10 ������
		if(!my_graph.add_vertex(i)) {
			cout<<"Vertex isn't added"<<endl;
		}
	}
	for(int i=0;i<n;++i) {//��������� � ���� ����� ����� ��������� �� ����� ���������
		if(i+1<n) {
			my_graph.add_edge(i,i+1,1.0*i);
		}
	}
	
	cout<<"Count of vertices: "<<my_graph.vertices_count()<<endl;//������� ���-�� ������ � �����, ������ ���� n
	if(!my_graph.delete_edge(n-2,n-1)) {//������� ���� �����
		cout<<"Edge isn't deleted"<<endl;
	}
	cout<<"Count of edges: "<<my_graph.edges_count()<<endl;//������� ���-�� �����, ������ ���� n-2, �.�. ���� �������, � ���������� ���� n-1
	
	pair<BiGraph<int>::Vertex_iterator, BiGraph<int>::Vertex_iterator> vertex_iters = my_graph.vertices();
	for(; vertex_iters.first != vertex_iters.second; ++vertex_iters.first) {//����������� �� �������� ����� � ������� �� �����
		cout<<(*vertex_iters.first)->get_name()<<" ";
	}
	cout<<endl;

	my_graph.clear();//������� ��� ����
	return 0;
}