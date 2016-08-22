#include "common.h"

struct edge_type_t { typedef edge_property_tag kind; };
struct domain_bps_t { typedef edge_property_tag kind; };
typedef adjacency_list<
		vecS, vecS, undirectedS,
		// Vertex properties
		property<vertex_name_t, std::string,
		property<vertex_index_t, int > >,
		// Edge properties
		property<boost::edge_name_t, string,
		property<edge_type_t, string,
		property<domain_bps_t, int,
		property<boost::edge_weight_t, double,
		property<edge_index_t, int > > > > > >
		Graph;
typedef vector< vector< graph_traits<Graph>::edge_descriptor > > edge_matrix; //for embeddings see http://www.boost.org/doc/libs/1_61_0/libs/graph/doc/PlanarEmbedding.html

template <class Graph> void reset_edge_index(Graph& g) {
	cout << "Resetting edge_index map..." << endl;
	typename property_map<Graph, edge_index_t>::type e_index = get(edge_index, g);
	typename graph_traits<Graph>::edges_size_type edge_count = 0;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	for(boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
		put(e_index, *ei, edge_count++);
	}
}
template <class Graph> void print_edge_properties(Graph& g) {
	typename property_map<Graph, edge_weight_t>::type EdgeWeightMap = get(edge_weight_t(), g);	
	typename property_map<Graph, edge_type_t>::type EdgeTypeMap = get(edge_type_t(), g);
	typename property_map<Graph, domain_bps_t>::type DomainSizeMap = get(domain_bps_t(), g);
	typedef typename graph_traits<Graph>::edge_iterator edge_iter;
    pair<edge_iter, edge_iter> edgePair;
  	for(edgePair = edges(g); edgePair.first != edgePair.second; ++edgePair.first){
		cout << *edgePair.first << ": ";
		cout << EdgeWeightMap[*edgePair.first] << " ";
		cout << EdgeTypeMap[*edgePair.first] << " ";
  	}
	cout << endl;
}
template <class Graph> void add_edge_properties(Graph& g) {
	double default_edge_weight = 2.5;
	int default_edge_size = 16;
	string default_edge_type = "ss";	
	typename property_map<Graph, edge_weight_t>::type EdgeWeightMap = get(edge_weight_t(), g);	
	typename property_map<Graph, edge_type_t>::type EdgeTypeMap = get(edge_type_t(), g);
	typename property_map<Graph, domain_bps_t>::type DomainSizeMap = get(domain_bps_t(), g);
	typedef typename graph_traits<Graph>::edge_iterator edge_iter;
    pair<edge_iter, edge_iter> edgePair;
	//int i = 0;
  	for(edgePair = edges(g); edgePair.first != edgePair.second; ++edgePair.first){
		EdgeWeightMap[*edgePair.first] = default_edge_weight; 
		DomainSizeMap[*edgePair.first] = default_edge_size; 
		EdgeTypeMap[*edgePair.first] = default_edge_type; 
		//put(DomainTypeMap,*edgePair.first,domain_types[i]); //equivalent to above!
		//i++;
  	}
}
template <class Graph> void write_gv(Graph& g) {
	typename property_map<Graph, edge_weight_t>::type EdgeWeightMap = get(edge_weight_t(), g);	
	typename property_map<Graph, edge_type_t>::type EdgeTypeMap = get(edge_type_t(), g);
	typename property_map<Graph, domain_bps_t>::type DomainSizeMap = get(domain_bps_t(), g);	
	boost::dynamic_properties dp;
	dp.property("label", EdgeWeightMap);
	dp.property("type", get(edge_type_t(),g));
	dp.property("node_id", get(vertex_index, g)); //must have!
	ofstream outfile;
	outfile.open ("graph.gv");
	write_graphviz_dp(outfile, g, dp);
  	outfile.close();
}
template <class Graph, class edge_matrix> void print_embedding_faces(Graph& g, edge_matrix& embedding) {
	face_counter count_visitor;
	planar_face_traversal(g, &embedding[0], count_visitor);
	cout << "The planar embedding of the graph has " << count_visitor.count << " faces." << endl;
	cout << endl << "Vertices on the faces: " << endl;
	vertex_output_visitor v_vis;
	planar_face_traversal(g, &embedding[0], v_vis);
	cout << endl << "Edges on the faces: " << endl;
	edge_output_visitor e_vis;
	planar_face_traversal(g, &embedding[0], e_vis);
}
template <class Graph> void fill_crossovers(Graph& g) {
	int crossover_list[] = {	75,77,
					73,79,
					71,81,
					84,94,
					86,92,
					88,90,
					74,64,
					72,66,
					70,68,
					//69,96,
					97,95,
					99,93,
					91,101,
					63,61,
					65,59,
					67,57,
					//69,96,
					98,108,
					100,106,
					102,104,
					60,50,
					58,52,
					56,54,
					//55,110,
					109,111,
					107,113,
					105,115,
					49,47,
					51,45,
					53,43,
					//55,100,
					112,122,
					114,120,
					116,118,
					46,36,
					44,38,
					42,40,
					//41,124,
					123,125,
					121,127,
					119,129,
					35,33,
					37,31,
					39,29,
					//41,124,
					126,136,
					128,134,
					130,132,
					32,22,
					30,24,
					28,26,
					//27,138,
					137,139,
					135,141,
					133,143,
					21,19,
					23,17,
					25,15,
					//27,138,
					140,150,
					142,148,
					144,146,
					18,8,
					16,10,
					14,12,
					//13,152,
					151,153,
					149,155,
					147,157,
					7,5,
					9,3,
					11,1,
					//13,152,
					154,164,
					156,162,
					158,160,
					
					70,95,
					68,97,

					56,109,
					54,111,

					42,123,
					40,125,
					
					28,137,
					26,139,

					14,151,
					12,153
					};
	const int n_crossovers = sizeof(crossover_list)/sizeof(crossover_list[0]);
    for (int i = 0; i < n_crossovers; i=i+2){
		      add_edge(crossover_list[i], crossover_list[i+1],g);
	}
}

