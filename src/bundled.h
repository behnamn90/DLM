struct VertexProperty{
	//string name;
	int id;
	vector<int> pair_id; // eg 12 is connected by two crossovers
	vector<string> pair_type; //i: inside-crossover, o: outside-crossover
};
struct EdgeProperty{
	//string name;
	//int abs_id; //define in absolut terms relative to vertices.
	int id;
	int length;
	double weight;
	string type; //s: single-stranded, d: double-stranded, i: inside-crossover, o: outside-crossover, m: seam-crossover
	string colour;
	string thickness;
	vector<pair<int,int> > pair_id; //no need for vec
   	vector<string> pair_type; //i: inside-crossover, o: outside-crossover, m: seam-crossover
};

typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty > Graph;
typedef vector< vector< graph_traits<Graph>::edge_descriptor > > Embedding; //for embeddings 
typedef graph_traits<Graph>::vertex_iterator Vertex_iter;
typedef graph_traits<Graph>::edge_iterator Edge_iter;
typedef graph_traits<Graph>::vertex_descriptor Vertex_desc;
typedef graph_traits<Graph>::edge_descriptor Edge_desc;

template <class Graph> void reset_vertex_index(Graph& g) { //no real need... just use get(vertex_index, g)
	//cout << "Resetting vertex_index map..." << endl;
	Vertex_iter vi,vf;
	int i = 0;
	for(tie(vi,vf) = vertices(g); vi != vf; ++vi) {
		g[*vi].id = i;
		i++;
		//cout << *ei << endl;
	}
}
template <class Graph> void reset_edge_index(Graph& g) {
	//cout << "Resetting edge_index map..." << endl;
	Edge_iter ei,ef;
	boost::tie( ei, ef) = boost::edges(g);
	int i = 0;
	for ( ; ei != ef ; ++ei ){
		g[*ei].id = i;
		i++;
		//cout << *ei << endl;
	}
}
template <class Graph> void add_domains(Graph& g) { 
	//cout << "Adding domains..." << endl;
	Vertex_iter vi,vf;
	EdgeProperty EP, EP_long; 
	EP.type = "s"; EP_long.type = "s";
	EP.length = default_domain_size; EP_long.length = default_domain_size * 2;
	for(boost::tie(vi,vf) = vertices(g); vi != vf; ++vi) {
		add_edge(*vi, *vi + 1, EP, g);
	}
	add_edge(num_vertices(g)-1, 0, EP_long, g);
	//pair<typename Graph::edge_descriptor, bool> edgePair = edge(82, 83, g);
	pair<Edge_desc, bool> edgePair = edge(82, 83, g);
	Edge_desc en = edgePair.first;
	//bool haha = edgePair.second; //this will be 0 if there is no edge, 1 if there is an edge.
	g[en].length = default_domain_size * 2;
}
template <class Graph, class Vertex> void add_crossover(Graph& g, Vertex v1, Vertex v2, string type){
	//cout << "Adding crossover..." << endl;
	EdgeProperty EP; 
	EP.type = type;
	EP.length = default_crossover_size;
		add_edge(v1, v2, EP, g);
}
template <class Graph, class Tile> void add_all_crossovers(Graph& g, Tile& tile) {
	//cout << "Adding crossovers..." << endl;
	EdgeProperty EP; 
	EP.length = default_crossover_size;
	/*
	EP.type = "c";
	for (vector< pair<int,int> >::iterator it = tile.v_pairs.begin(); it != tile.v_pairs.end(); ++it) {
		add_edge((*it).first, (*it).second, EP, g);
	}
	*/
	for (vector< crossover >::iterator it = tile.crossovers.begin(); it != tile.crossovers.end(); ++it) {
		EP.type = (*it).type;
		add_edge((*it).v_a, (*it).v_b, EP, g);
	}
}
template <class Graph, class Tile> void add_all_pairs(Graph& g, Tile& tile) {
	//cout << "Adding pairs..." << endl;
	Vertex_iter vi,vf;
	for(tie(vi,vf) = vertices(g); vi != vf; ++vi) {
		for (vector< crossover >::iterator it = tile.crossovers.begin(); it != tile.crossovers.end(); ++it) {
			if (*vi == (*it).v_a) {
				g[*vi].pair_id.push_back((*it).v_b);
			    if ((*it).type == "m") {g[*vi].pair_type.push_back("i");}
				else {g[*vi].pair_type.push_back((*it).type);}
			}	
			if (*vi == (*it).v_b) {
				g[*vi].pair_id.push_back((*it).v_a);
			    if ((*it).type == "m") {g[*vi].pair_type.push_back("i");}
				else {g[*vi].pair_type.push_back((*it).type);}
			}	
		}
	}
	Edge_iter ei,ef;
	for(tie(ei,ef) = edges(g); ei != ef; ++ei) {
		for (vector< crossover >::iterator it = tile.crossovers.begin(); it != tile.crossovers.end(); ++it) {
			if (source(*ei, g) == (*it).d_a1 && target(*ei, g) == (*it).d_a2) {
				g[*ei].pair_id.push_back(make_pair((*it).d_b1,(*it).d_b2)); 
				//if ((*it).type == "m") {g[*ei].pair_type.push_back("i");} 
				//else {g[*ei].pair_type.push_back((*it).type);} 
				g[*ei].pair_type.push_back((*it).type); 
			}	
			if (source(*ei, g) == (*it).d_b1 && target(*ei, g) == (*it).d_b2) {
				g[*ei].pair_id.push_back(make_pair((*it).d_a1,(*it).d_a2)); 
				//if ((*it).type == "m") {g[*ei].pair_type.push_back("i");} 
				//else {g[*ei].pair_type.push_back((*it).type);} 
				g[*ei].pair_type.push_back((*it).type); 
			}	
		}
	}
}
template <class Graph> void bind_all_domains(Graph& g) {
	//cout << "Making all domains double-stranded..." << endl;
	Edge_iter ei,ef;
	tie( ei, ef) = edges(g);
	for ( ; ei != ef ; ++ei ){
		if(g[*ei].type == "s"){g[*ei].type = "d";}
	}
}
template <class Graph> void set_edge_weights(Graph& g) {
	//cout << "Calculating and adding edge_weights based on lengths and model constants..." << endl;
	Edge_iter ei,ef;
	boost::tie( ei, ef) = boost::edges(g);
	for ( ; ei != ef ; ++ei ){
		if(g[*ei].type == "d"){g[*ei].weight = g[*ei].length * l_ds * g[*ei].length * l_ds;}
		else if(g[*ei].type == "s"){g[*ei].weight = g[*ei].length * l_ss * lambda_ss;}
		else if(g[*ei].type == "i" || g[*ei].type == "o" || g[*ei].type == "m"){g[*ei].weight = lambda_ss * lambda_ss;}
		else {cout << "no domain type found for edge" << endl;}
	}
}
template <class Graph> void print_edge_properties(Graph& g) {
	//cout << "Printing edge_properties..." << endl;
    pair<Edge_iter, Edge_iter> edgePair;
  	for(edgePair = edges(g); edgePair.first != edgePair.second; ++edgePair.first){
		cout << *edgePair.first << ": ";
		cout << g[*edgePair.first].weight << " ";
		cout << g[*edgePair.first].type << " ";
		cout << g[*edgePair.first].length << " ";
		cout << endl;
  	}
}
template <class Graph> void write_gv(Graph& g, string filename) {
	//cout << "Writing the graphviz file...\t";
	//cout << "Adding Colours, thicknesses, etc..." << endl;
	Edge_iter ei,ef;
	boost::tie( ei, ef) = boost::edges(g);
	for ( ; ei != ef ; ++ei ){
		if(g[*ei].type == "d"){g[*ei].colour = "black"; g[*ei].thickness = "6.0";}
		else if(g[*ei].type == "s"){g[*ei].colour = "black"; g[*ei].thickness = "2.0";}
		else if(g[*ei].type == "i"){g[*ei].colour = "green"; g[*ei].thickness = "4.0";}
		else if(g[*ei].type == "o"){g[*ei].colour = "blue"; g[*ei].thickness = "4.0";}
		else if(g[*ei].type == "m"){g[*ei].colour = "red"; g[*ei].thickness = "4.0";}
		else {cout << "no domain type found for edge" << endl;}
	}
	boost::dynamic_properties dp;
	dp.property("len", get(&EdgeProperty::length, g));
	//dp.property("weight", get(&EdgeProperty::weight, g));
	dp.property("label", get(&EdgeProperty::weight, g));
	dp.property("type", get(&EdgeProperty::type, g));
	dp.property("color", get(&EdgeProperty::colour, g));
	dp.property("penwidth", get(&EdgeProperty::thickness, g));
	//dp.property("label", get(&VertexProperty::name, g));
	dp.property("node_id", get(&VertexProperty::id, g)); //must have!
	//dp.property("node_id", get(vertex_index, g)); //must have!
	ofstream outfile;
	outfile.open ("../Output/"+filename+".gv");
	write_graphviz_dp(outfile, g, dp);
  	outfile.close();
}
template <class Graph, class edge_matrix> void print_embedding_faces(Graph& g, edge_matrix& embedding) {
	face_counter count_visitor;
	planar_face_traversal(g, &embedding[0], count_visitor, get(&EdgeProperty::id, g));
	cout << "Traversing faces of planar embedding... the planar embedding of the graph has " << count_visitor.count << " faces." << endl;
	/*
	cout << endl << "Vertices on the faces: " << endl;
	vertex_output_visitor v_vis;
	planar_face_traversal(g, &embedding[0], v_vis, get(&EdgeProperty::id, g));
	cout << endl << "Edges on the faces: " << endl;
	edge_output_visitor e_vis;
	planar_face_traversal(g, &embedding[0], e_vis, get(&EdgeProperty::id, g));
	*/
}
template <class Graph> void my_dijkstra(Graph& g, int a) {
	//Create things for Dijkstra
	vector<Vertex_desc> parents(num_vertices(g)); // To store parents
	vector<double> distances(num_vertices(g)); // To store distances
	// Compute shortest paths from v0 to all vertices, and store the output in parents and distances
	//dijkstra_shortest_paths(g, 2, weight_map(get(&EdgeProperty::weight, g)).distance_map(&distances[0]));
	//dijkstra_shortest_paths(g, 2, weight_map(get(&EdgeProperty::weight, g)));
	dijkstra_shortest_paths(g, a, weight_map(get(&EdgeProperty::weight, g)).distance_map(&distances[0]).predecessor_map(&parents[0]));
	//print dijkastra
	cout << "distances and parents:" << endl;
	Vertex_iter vi, vend;
	for (tie(vi, vend) = boost::vertices(g); vi != vend; ++vi) {
		cout << "distance(" << *vi << ") = " << distances[*vi] << ", ";
		cout << "parent(" << *vi << ") = " << parents[*vi] << endl;
	}
	cout << endl;
}
template <class Graph> void my_dijkstra(Graph& g, int a, int b) {
	typedef float Weight;
	typedef typename property_map < Graph, vertex_index_t >::type IndexMap;
	typedef iterator_property_map < Vertex_desc*, IndexMap, Vertex_desc, Vertex_desc& > PredecessorMap;
	typedef iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;
	vector<Vertex_desc> predecessors(num_vertices(g)); // To store parents
	vector<Weight> distances(num_vertices(g)); // To store distances
	IndexMap indexMap = get(vertex_index, g);
	PredecessorMap predecessorMap(&predecessors[0], indexMap);
	DistanceMap distanceMap(&distances[0], indexMap);
	dijkstra_shortest_paths(g, a, weight_map(get(&EdgeProperty::weight, g)).distance_map(distanceMap).predecessor_map(predecessorMap));
	/*
	cout << "distances and parents:" << endl;
	typename graph_traits < Graph >::vertex_iterator vertexIterator, vend;
	for (tie(vertexIterator, vend) = boost::vertices(g); vertexIterator != vend; ++vertexIterator) {
		cout << "distance(" << *vertexIterator << ") = " << distanceMap[*vertexIterator] << ", ";
		cout << "parent(" << *vertexIterator << ") = " << predecessorMap[*vertexIterator] << endl;
	}
	cout << endl;
	*/
	typedef vector<Edge_desc> Path;
	Path path;
	Vertex_desc v = b; // We want to start at the destination and work our way back to the source
	for(Vertex_desc u = predecessorMap[v]; // Start by setting 'u' to the destintaion node's predecessor
		u != v; // Keep tracking the path until we get to the source
		v = u, u = predecessorMap[v]) // Set the current vertex to the current predecessor, and the predecessor to one level up
	{
		pair<Edge_desc, bool> edgePair = edge(u, v, g);
		Edge_desc edge = edgePair.first;
		path.push_back( edge );
	}
	cout << "Shortest path from " << a << " to " << b << ":" << endl;
	float totalDistance = 0;
	for(typename Path::reverse_iterator pathIterator = path.rbegin(); pathIterator != path.rend(); ++pathIterator)
	{
	cout << source(*pathIterator, g) << " -> " << target(*pathIterator, g)
	<< " = " << get( &EdgeProperty::weight, g, *pathIterator ) << endl;
	}
	cout << "Total_Distance: " << distanceMap[b] << endl;
}

template <class Graph> double Er2(Graph& g, int a, int b) {
	typedef float Weight;
	typedef typename property_map < Graph, vertex_index_t >::type IndexMap;
	typedef iterator_property_map < Vertex_desc*, IndexMap, Vertex_desc, Vertex_desc& > PredecessorMap;
	typedef iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;
	vector<Vertex_desc> predecessors(num_vertices(g)); // To store parents
	vector<Weight> distances(num_vertices(g)); // To store distances
	IndexMap indexMap = get(vertex_index, g);
	PredecessorMap predecessorMap(&predecessors[0], indexMap);
	DistanceMap distanceMap(&distances[0], indexMap);
	dijkstra_shortest_paths(g, a, weight_map(get(&EdgeProperty::weight, g)).distance_map(distanceMap).predecessor_map(predecessorMap));
	typedef vector<Edge_desc> Path;
	Path path;
	Vertex_desc v = b; // We want to start at the destination and work our way back to the source
	for(Vertex_desc u = predecessorMap[v]; // Start by setting 'u' to the destintaion node's predecessor
		u != v; // Keep tracking the path until we get to the source
		v = u, u = predecessorMap[v]) // Set the current vertex to the current predecessor, and the predecessor to one level up
	{
		pair<Edge_desc, bool> edgePair = edge(u, v, g);
		Edge_desc edge = edgePair.first;
		path.push_back( edge );
	}
	return distanceMap[b];
}

template <class Graph> void single_reaction(Graph& g, Graph& bound_graph, pair<typename Graph::edge_descriptor, bool> e) {
	vector< pair<int,int> >::iterator id_it; //iterator for pair_id.
	int pt_it; //iterator for the pair_types.
	Edge_desc eg = e.first; //descriptor for edge in current graph.
	Edge_desc eb = edge(source(eg,g),target(eg,g),bound_graph).first; //descriptor for edge in bound graph.
	Edge_desc e2; //descriptor for pair (on current graph).
	if (g[eg].type == "d"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g).first; //the second domain.
				if (g[e2].type == "d"){
					//remove the crossover.
					if( (bound_graph[eb].pair_type)[pt_it] == "i" ) {
						if ( source(eg,g) < source(e2,g) )
							remove_edge(source(eg,g), target(e2,g), g);
						else
							remove_edge(target(eg,g), source(e2,g), g);
					}
					if( (bound_graph[eb].pair_type)[pt_it] == "o" ) {
						if ( source(eg,g) < source(e2,g) )
							remove_edge(target(eg,g), source(e2,g), g);
						else
							remove_edge(source(eg,g), target(e2,g), g);
					}
				}
				else if (g[e2].type == "s"){
					//do nothing.
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
		g[eg].type = "s";
	}
	else if (g[eg].type == "s"){
		if (!((bound_graph[eb].pair_id).empty())){
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
			pt_it = 0;
				e2 = edge((*id_it).first, (*id_it).second, g).first; //the second domain.
				if (g[e2].type == "d"){
					//add a crossover.
					if( (bound_graph[eb].pair_type)[pt_it] == "i" ) {
						if ( source(eg,g) < source(e2,g) )
							add_crossover(g, source(eg,g), target(e2,g), "i");
						else
							add_crossover(g, target(eg,g), source(e2,g), "i");
					}
					if( (bound_graph[eb].pair_type)[pt_it] == "o" ) {
						if ( source(eg,g) < source(e2,g) )
							add_crossover(g, target(eg,g) , source(e2,g), "o");
						else
							add_crossover(g, source(eg,g) , target(e2,g), "o");
					}
				}
				else if (g[e2].type == "s"){
					//do nothing.
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
		g[eg].type = "d";
	}

	else {printf ("Error!\n"); exit (EXIT_FAILURE);}
}
template <class Graph> void single_reaction(Graph& g, Graph& bound_graph, int e) {
	vector< pair<int,int> >::iterator id_it; //iterator for pair_id.
	int pt_it; //iterator for the pair_types.
	Edge_desc eg; //descriptor for edge in current graph.
	Edge_iter ei,ef;
	tie( ei, ef) = edges(g);
	for ( ; ei != ef ; ++ei ){
		if(g[*ei].id == e){eg = *ei;}
	}
	Edge_desc eb = edge(source(eg,g),target(eg,g),bound_graph).first; //descriptor for edge in bound graph.
	Edge_desc e2; //descriptor for pair (on current graph).
	if (g[eg].type == "d"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g).first; //the second domain.
				if (g[e2].type == "d"){
					//remove the crossover.
					if( (bound_graph[eb].pair_type)[pt_it] == "i" || (bound_graph[eb].pair_type)[pt_it] == "m") {
						if ( source(eg,g) < source(e2,g) )
							remove_edge(source(eg,g), target(e2,g), g);
						else
							remove_edge(target(eg,g), source(e2,g), g);
					}
					if( (bound_graph[eb].pair_type)[pt_it] == "o" ) {
						if ( source(eg,g) < source(e2,g) )
							remove_edge(target(eg,g), source(e2,g), g);
						else
							remove_edge(source(eg,g), target(e2,g), g);
					}
				}
				else if (g[e2].type == "s"){
					//do nothing.
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
		g[eg].type = "s";
	}
	else if (g[eg].type == "s"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g).first; //the second domain.
				if (g[e2].type == "d"){
					//add a crossover.
					if( (bound_graph[eb].pair_type)[pt_it] == "i" || (bound_graph[eb].pair_type)[pt_it] == "m" ) {
						if ( source(eg,g) < source(e2,g) )
							add_crossover(g, source(eg,g), target(e2,g), "i");
						else
							add_crossover(g, target(eg,g), source(e2,g), "i");
					}
					if( (bound_graph[eb].pair_type)[pt_it] == "o" ) {
						if ( source(eg,g) < source(e2,g) )
							add_crossover(g, target(eg,g) , source(e2,g), "o");
						else
							add_crossover(g, source(eg,g) , target(e2,g), "o");
					}
				}
				else if (g[e2].type == "s"){
					//do nothing.
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
		g[eg].type = "d";
	}

	else {printf ("Error!\n"); exit (EXIT_FAILURE);}
	//reset_vertex_index(g);  
	//reset_edge_index(g); 
}
/*
	template <class Graph> void add_crossovers(Graph& g) {
		cout << "Adding crossovers..." << endl;
		//vector<edge> crossover_list;
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
		//int crossover_list[] = {75,77,64,74};
		const int n_crossovers = sizeof(crossover_list)/sizeof(crossover_list[0]);
		typedef pair<int,int> crossover;
		crossover my_crossover;
		vector<crossover> crossover_vector;
		EdgeProperty EP; 
		EP.type = "c";
		EP.length = default_crossover_size;
		for (int i = 0; i < n_crossovers; i=i+2){
			add_edge(crossover_list[i], crossover_list[i+1], EP, g);
			my_crossover.first = crossover_list[i]; my_crossover.second = crossover_list[i+1];
			crossover_vector.push_back(my_crossover);
		}
	}
*/
/*
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
*/









