#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/visitors.hpp>

#include <boost/config.hpp>
#include <vector>
#include <string>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>

using namespace boost;
using namespace std;

struct VertexProperty{
	int id; 
};
struct EdgeProperty{
	int id;
	int weight;
};

//typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty > Graph;
//typedef adjacency_list<vecS, vecS, undirectedS> Graph;
//typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;
typedef adjacency_list<listS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;
//typedef adjacency_list<vecS, vecS, directedS, VertexProperty, EdgeProperty > Graph;
typedef std::pair<int,int> Edge;
typedef graph_traits<Graph>::vertex_iterator Vertex_iter;
typedef graph_traits<Graph>::edge_iterator Edge_iter;
typedef graph_traits<Graph>::vertex_descriptor Vertex_desc;
typedef graph_traits<Graph>::edge_descriptor Edge_desc;

template <class PredecessorMap>
class record_predecessors : public dijkstra_visitor<> {
	public:
		record_predecessors(PredecessorMap p) : m_predecessor(p) { }
		template <class Edge, class Graph>
		void edge_relaxed(Edge e, Graph& g) {
			put(m_predecessor, target(e, g), source(e, g));
		}

	protected:
		PredecessorMap m_predecessor;
};

template <class PredecessorMap>
record_predecessors<PredecessorMap> make_predecessor_recorder(PredecessorMap p) {
	return record_predecessors<PredecessorMap>(p);
};

template <class Graph> struct exercise_vertex {
	exercise_vertex(Graph& g_) : g(g_) {}
	Graph& g;
	void operator()(const Vertex_desc& v) const {
		typename property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
		
		cout << "out-edges: ";
		typename graph_traits<Graph>::out_edge_iterator out_i, out_end;
		Edge_desc e;
		for (tie(out_i, out_end) = out_edges(v, g); out_i != out_end; ++out_i) {
			e = *out_i;
			Vertex_desc src = source(e, g), targ = target(e, g);
			cout << "(" << index[src] << "," << index[targ] << ") ";
		}
		cout << endl;

		/*
		cout << "in-edges: ";
		typename graph_traits<Graph>::in_edge_iterator in_i, in_end;
		for (tie(in_i, in_end) = in_edges(v,g); in_i != in_end; ++in_i) {
			e = *in_i;
			Vertex_desc src = source(e, g), targ = target(e, g);
			cout << "(" << index[src] << "," << index[targ] << ") ";
		}
		cout << endl;
		*/

		cout << "adjacent vertices: ";
		typename graph_traits<Graph>::adjacency_iterator ai;
		typename graph_traits<Graph>::adjacency_iterator ai_end;
		for (tie(ai, ai_end) = adjacent_vertices(v, g); ai != ai_end; ++ai) {
			cout << index[*ai] <<  " ";
		}
		cout << endl;

		cout << "distances: ";
		vector<int> d(num_vertices(g));
		//vector<Vertex_desc> p(num_vertices(g), graph_traits<Graph>::null_vertex());
		//dijkstra_shortest_paths(g, v, distance_map(&d[0]), visitor(make_predecessor_recorder(&p[0])));	
		dijkstra_shortest_paths(g, v, distance_map(&d[0]));	
		Vertex_iter vi;
		for(vi = vertices(g).first; vi != vertices(g).second; ++vi)
			cout << "d(" << index(*vi) << ") = " << d[*vi] << "\t,\t";
		cout << endl;
		/*
		cout << "parents: ";
		for(vi = vertices(g).first; vi != vertices(g).second; ++vi) {
			cout << "p(" << *vi;
			if (p[*vi] == graph_traits<Graph>::null_vertex())
				cout << ") = no parent" << "\t,\t"; 
			else 
				cout << ") = " << p[*vi] << "\t,\t";
		}
		cout << endl;
		*/




		cout << endl;
	}
};



int main(int argc, const char * argv[]) {
	//Create Graph
	enum { A, B, C, D, E, N };
	const int num_vertices = N;
	const char* name = "ABCDE";
    Edge edge_array[] = { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C), Edge(C,E), Edge(B,D), Edge(D,E) };
    const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);	
	//int weights[] = { 8, 3, 2, 5, 7, 9, 3 };
	int weights[] = { 1, 1, 1, 1, 1, 1, 1 };
	Graph g(edge_array, edge_array + num_edges, weights, num_vertices);
	//Graph g(num_vertices);
	//for (int i = 0; i < num_edges; ++i) {add_edge(edge_array[i].first, edge_array[i].second, g);}

	//Access Vertex set
	typedef property_map<Graph, vertex_index_t>::type IndexMap;	
	IndexMap index = get(vertex_index, g);
	cout << "vertices(g) = ";
	pair<Vertex_iter, Vertex_iter> vp;
	for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
		Vertex_desc v = *vp.first;
		cout << index[v] <<  " ";
	}
    cout << endl;		
    cout << endl;		

	//Access Edge set
	cout << "edges(g) = ";
	Edge_iter ei, ei_end;
	for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		cout << "(" << index[source(*ei, g)] << "," << index[target(*ei, g)] << ") ";
	cout << endl;
	cout << endl;

	//Exercise vertex
	for_each(vertices(g).first, vertices(g).second, exercise_vertex<Graph>(g));	

	return 0;
}















