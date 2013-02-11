#include <iostream>
#include "BiGraph.h"

int main() {
	const int n=10;
	BiGraph<int> my_graph;
	for(int i=0;i<n;++i) {//добавляем в граф 10 вершин
		if(!my_graph.add_vertex(i)) {
			cout<<"Vertex isn't added"<<endl;
		}
	}
	for(int i=0;i<n;++i) {//добавляем в граф ребра между соседними по счету вершинами
		if(i+1<n) {
			my_graph.add_edge(i,i+1,1.0*i);
		}
	}
	
	cout<<"Count of vertices: "<<my_graph.vertices_count()<<endl;//выведем кол-во вершин в графе, должно быть n
	if(!my_graph.delete_edge(n-2,n-1)) {//удаляем одно ребро
		cout<<"Edge isn't deleted"<<endl;
	}
	cout<<"Count of edges: "<<my_graph.edges_count()<<endl;//выведем кол-во ребер, должно быть n-2, т.к. одно удалено, а изначально было n-1
	
	pair<BiGraph<int>::Vertex_iterator, BiGraph<int>::Vertex_iterator> vertex_iters = my_graph.vertices();
	for(; vertex_iters.first != vertex_iters.second; ++vertex_iters.first) {//итерируемся по вершинам графа и выводим их имена
		cout<<(*vertex_iters.first)->get_name()<<" ";
	}
	cout<<endl;

	my_graph.clear();//удаляем наш граф
	return 0;
}