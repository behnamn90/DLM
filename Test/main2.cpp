
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




template <class PredecessorMap>
class record_predecessors : public dijkstra_visitor<>
{
	public:
		record_predecessors(PredecessorMap p)
		: m_predecessor(p) { }

		template <class Edge, class Graph>
		void edge_relaxed(Edge e, Graph& g) {
			put(m_predecessor, target(e, g), source(e, g));
		}
	protected:
		PredecessorMap m_predecessor;
};

template <class PredecessorMap>
record_predecessors<PredecessorMap>
make_predecessor_recorder(PredecessorMap p) {
return record_predecessors<PredecessorMap>(p);
}

int main(int argc, const char * argv[]) {
typedef adjacency_list<listS, vecS, directedS, 
no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::pair<int,int> E;

const int num_nodes = 5;
E edges[] = { E(0,2), 
E(1,1), E(1,3), E(1,4),
E(2,1), E(2,3), 
E(3,4),
E(4,0), E(4,1) };
int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1};

Graph G(edges, edges + sizeof(edges) / sizeof(E), weights, num_nodes);

std::vector<int> d(num_vertices(G));

Vertex s = *(vertices(G).first);
dijkstra_shortest_paths(G, s, distance_map(&d[0]));

std::cout << "distances from start vertex:" << std::endl;
graph_traits<Graph>::vertex_iterator vi;
for(vi = vertices(G).first; vi != vertices(G).second; ++vi)
std::cout << "distance(" << index(*vi) << ") = " 
<< d[*vi] << std::endl;
std::cout << std::endl;

vector<Vertex> p(num_vertices(G), graph_traits<G>::null_vertex()); //the predecessor array
dijkstra_shortest_paths(G, s, distance_map(&d[0]). 
visitor(make_predecessor_recorder(&p[0])));

cout << "parents in the tree of shortest paths:" << endl;
for(vi = vertices(G).first; vi != vertices(G).second; ++vi) {
cout << "parent(" << *vi;
if (p[*vi] == graph_traits<G>::null_vertex())
cout << ") = no parent" << endl; 
else 
cout << ") = " << p[*vi] << endl;
}
return 0;
}
