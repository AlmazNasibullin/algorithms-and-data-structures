#include <map>
using namespace std;

template<class Name>
class BiGraph {
public:
	class Vertex;
	class Edge;
	
	BiGraph(): m_edges_count(0){}
	~BiGraph() {
		clear();
	}

	template<typename A, typename B>
	class Iterator {
	public:
		Iterator(typename map<A,B>::iterator iter): m_iter(iter) {}
		
		bool operator == (const Iterator& iter) {
			return m_iter == iter.m_iter;
		}
		bool operator != (const Iterator& iter) {
			return !(*this==iter);
		}
		Iterator& operator --() {
			--m_iter;
			return *this;
		}
		Iterator& operator ++() {
			++m_iter;
			return *this;
		}
		B operator *() {
			return m_iter->second;
		}
	private:
		typename map<A,B>::iterator m_iter;
	};

	typedef Iterator<Name, Vertex*> Vertex_iterator;
	typedef Iterator<Vertex*, Edge*> Edge_iterator;
	typedef pair<Vertex_iterator,Vertex_iterator>  vertices_range;

	class Vertex {
	public:
		Vertex(const Name& v): m_name(v) {}

		pair<Edge_iterator,Edge_iterator> out_edges() {//метод, возращающий пару итераторов на диапазон исходящих ребер 
			return make_pair(Edge_itartor(m_out_edges.begin()), Edge_iterator(m_out_edges.end()));
		}
		pair<Edge_iterator,Edge_iterator> in_edges() {//метод, возращающий пару итераторов на диапазон входящих ребер
			return make_pair(Edge_itartor(m_in_edges.begin()), Edge_iterator(m_in_edges.end()));
		}

		Name get_name() const{//метод, возвращающий имя вершины
			return m_name;
		}
	private:
		Name m_name;
		map<Vertex*, Edge*> m_out_edges;
		map<Vertex*, Edge*> m_in_edges;
		friend class BiGraph;
	};

	class Edge {
	public:
		Edge(Vertex* start, Vertex* end, double weight): m_start(start), m_end(end), m_weight(weight) {}
	private:
		Vertex* m_start;
		Vertex* m_end;
		double m_weight;
	};

	bool add_vertex(const Name& v) {
		if(m_vertices.find(v) == m_vertices.end()) {//проверяем, была ли уже в графе вершина, которую хотим добавить
			Vertex* new_vertex = new Vertex(v);
			m_vertices.insert(make_pair(v,new_vertex));
			return true;
		}
		return false;
	}

	Edge* add_edge(const Name& start, const Name& end, double weight) {
		if(start == end) {
			return 0;
		}
		Vertex* start_vertex = find_or_create_vertex(start);
		Vertex* end_vertex = find_or_create_vertex(end);
		map<Vertex*,Edge*>::iterator existing_edge = start_vertex->m_out_edges.find(end_vertex);
		if(existing_edge != start_vertex->m_out_edges.end()) {//проверяем, было ли уже в графе ребро, которое хотим добавить
			return existing_edge->second;
		}
		Edge* new_edge = new Edge(start_vertex, end_vertex, weight);
		start_vertex->m_out_edges.insert(make_pair(end_vertex, new_edge));
		end_vertex->m_in_edges.insert(make_pair(start_vertex, new_edge));
		++m_edges_count;
		return new_edge;
	}

	bool delete_edge(const Name& start, const Name& end){
		if(m_vertices.find(start) == m_vertices.end() || m_vertices.find(end) == m_vertices.end()) {/*проверяем, есть ли вообще в
			графе вершины, которые хотим удалить*/
			return false;
		}
		Vertex* start_vertex = m_vertices[start];
		Vertex* end_vertex = m_vertices[end];
		if(start_vertex->m_out_edges.find(end_vertex) == (start_vertex->m_out_edges).end()) {/*проверяем, есть ли вообще в
			графе ребро, которое хотим удалить*/
			return false;
		}
		delete start_vertex->m_out_edges.find(end_vertex)->second;//удаляем нужное ребро
		start_vertex->m_out_edges.erase(end_vertex);
		end_vertex->m_in_edges.erase(start_vertex);
		--m_edges_count;
		return true;
	}

	vertices_range vertices() {
		vertices_range range = make_pair(Vertex_iterator(m_vertices.begin()), Vertex_iterator(m_vertices.end()));
		return range;
	}

	size_t vertices_count() const{
		return m_vertices.size();
	}

	size_t edges_count() const{
		return m_edges_count;
	}

	void clear(){//метод для удаления графа
		typename map<Name,Vertex*>::iterator iterator_vertex = m_vertices.begin();
		for(; iterator_vertex != m_vertices.end(); ++iterator_vertex){//итерируемся по вершинам графа
			Vertex* current_vertex = iterator_vertex->second;
			typename map<Vertex*, Edge*>::iterator iterator_edge = current_vertex->m_out_edges.begin();
			for(; iterator_edge != current_vertex->m_out_edges.end(); ++iterator_edge){//итерируемся по исходящим ребрам каждой вершины
				delete iterator_edge->second;
			}
			delete iterator_vertex->second;
		}
		m_vertices.clear();
	}

private:
	map<Name,Vertex*> m_vertices;//отображение имен вершин в экземпляры класса Vertex
	size_t m_edges_count;

	Vertex* find_or_create_vertex(const Name& v) {
		if(m_vertices.find(v) == m_vertices.end()) {
			Vertex* new_vertex = new Vertex(v);
			m_vertices.insert(make_pair(v,new_vertex));
			return new_vertex;
		}
		return m_vertices[v];
	}
};