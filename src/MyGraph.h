struct VertexProperty{
	int id;
};
struct EdgeProperty{
	int id;
	int length;
	double weight;
	char type; //s: single-stranded, d: double-stranded, i: inside-crossover, o: outside-crossover, m: seam-crossover
	string colour;
	string thickness;
};
typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty > Graph;
typedef vector< vector< graph_traits<Graph>::edge_descriptor > > Embedding; //for embeddings 
typedef graph_traits<Graph>::vertex_iterator Vertex_iter;
typedef graph_traits<Graph>::edge_iterator Edge_iter;
typedef graph_traits<Graph>::vertex_descriptor Vertex_desc;
typedef graph_traits<Graph>::edge_descriptor Edge_desc;
typedef vector<Edge_desc> Path;

class MyGraph{
	public:
		MyGraph (Design design_, Constants *constants_){
			design = design_;
			constants = constants_;
			add_vertices();
			add_domains();
		}
		~MyGraph(){}
		Design design;
		Constants *constants;
		Graph g;
		Embedding emb;
		void add_vertices(){
			for(int i=0; i<design.n_vertices; i++){
				add_vertex(g);
			}
			reset_vertex_index();
		}
		void add_domains(){
			EdgeProperty EP; EP.type = 's';
		    for (vector<Domain>::iterator d = design.domains.begin(); d != design.domains.end(); ++d) {	
				EP.length = (*d).length;
				EP.id = distance(design.domains.begin(), d);
				add_edge(d->source, (*d).target, EP, g);
			}
			reset_edge_index();
			set_edge_weights();
		}
		Edge_desc id_to_edge(int e){
			Edge_desc result;
			Edge_iter ei,ef;
			tie(ei,ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].id == e){result = *ei;}
			}
			return result;
		}
		void reset_vertex_index() {
			Vertex_iter vi,vf;
			int i = 0;
			for(tie(vi,vf) = vertices(g); vi != vf; ++vi) {
				g[*vi].id = i;
				i++;
			}
		}	
		void reset_edge_index() {
			Edge_iter ei,ef;
			tie(ei,ef) = edges(g);
			int i = 0;
			for ( ; ei != ef ; ++ei){
				g[*ei].id = i;
				i++;
			}
		}
		void add_crossover_v(pair<int,int> vpair){
			int cross_id = design.vpair_to_crossover(vpair);
			EdgeProperty EP; 
			EP.type = design.crossovers[cross_id].type;
			EP.length = constants->default_crossover_size;
			add_edge(design.crossovers[cross_id].v_a, design.crossovers[cross_id].v_b, EP, g);
			reset_edge_index();
			set_edge_weights();
		}
		void add_crossover_d(pair<int,int> dpair){
			int cross_id = design.dpair_to_crossover(dpair);
			EdgeProperty EP; 
			EP.type = design.crossovers[cross_id].type;
			EP.length = constants->default_crossover_size;
			add_edge(design.crossovers[cross_id].v_a, design.crossovers[cross_id].v_b, EP, g);
			reset_edge_index();
			set_edge_weights();
		}
		void add_crossover(int c){
			Crossover crossover = design.crossovers[c];
			EdgeProperty EP; 
			EP.type = crossover.type;
			EP.length = constants->default_crossover_size;
			add_edge(crossover.v_a, crossover.v_b, EP, g);
			reset_edge_index();
			set_edge_weights();
		}
		void remove_crossover(int c){
			Crossover crossover = design.crossovers[c];
			remove_edge(crossover.v_a, crossover.v_b, g);
			reset_edge_index();
			set_edge_weights();
		}
		void add_all_crossovers(){
			EdgeProperty EP; 
			EP.length = constants->default_crossover_size;
			for (vector<Crossover>::iterator c = design.crossovers.begin(); c != design.crossovers.end(); ++c) {
				EP.type = (*c).type;
				add_edge((*c).v_a, (*c).v_b, EP, g);
			}
			reset_edge_index();
			set_edge_weights();
		}
		void bind_all_domains() {
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].type == 's'){g[*ei].type = 'd';}
			}
			set_edge_weights();
		}
		void set_edge_weights(){	
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].type == 'd'){g[*ei].weight = g[*ei].length * constants->l_ds * g[*ei].length * constants->l_ds;}
				else if(g[*ei].type == 's'){g[*ei].weight = g[*ei].length * constants->l_ss * constants->lambda_ss;}
				else if(g[*ei].type == 'i' || g[*ei].type == 'o' || g[*ei].type == 'm'){g[*ei].weight = constants->lambda_ss * constants->lambda_ss;}
				else {cout << "no domain type found for edge" << endl;}
			}
		}
		void print_edges(){
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				cout << g[*ei].id << "\t";
				cout << *ei << "\t";
				cout << g[*ei].type << "\t";
				cout << g[*ei].length << "\t";
				cout << g[*ei].weight << "\n";
			}	
		}
		void write_gv(string filename) {
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].type == 'd'){g[*ei].colour = "black"; g[*ei].thickness = "6.0";}
				else if(g[*ei].type == 's'){g[*ei].colour = "black"; g[*ei].thickness = "2.0";}
				else if(g[*ei].type == 'i'){g[*ei].colour = "green"; g[*ei].thickness = "4.0";}
				else if(g[*ei].type == 'o'){g[*ei].colour = "blue"; g[*ei].thickness = "4.0";}
				else if(g[*ei].type == 'm'){g[*ei].colour = "red"; g[*ei].thickness = "4.0";}
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
			outfile.open("../Output/"+filename+".gv");
			write_graphviz_dp(outfile, g, dp);
			outfile.close();
		}
		double total_weight(int a, int b) {
			typedef double Weight;
			typedef property_map <Graph,vertex_index_t>::type IndexMap;
			typedef iterator_property_map <Vertex_desc*,IndexMap,Vertex_desc,Vertex_desc&> PredecessorMap;
			typedef iterator_property_map <Weight*,IndexMap,Weight,Weight&> DistanceMap;
			vector<Vertex_desc> predecessors(num_vertices(g)); // To store parents
			vector<Weight> distances(num_vertices(g)); // To store distances
			IndexMap indexMap = get(vertex_index, g);
			PredecessorMap predecessorMap(&predecessors[0], indexMap);
			DistanceMap distanceMap(&distances[0], indexMap);
			dijkstra_shortest_paths(g, a, weight_map(get(&EdgeProperty::weight, g)).distance_map(distanceMap).predecessor_map(predecessorMap));
			//typedef vector<Edge_desc> Path;
			Path path;
			Vertex_desc v=b; // We want to start at the destination and work our way back to the source
			for(Vertex_desc u=predecessorMap[v]; u!=v; v=u, u=predecessorMap[v]) {
				pair<Edge_desc, bool> edgePair = edge(u, v, g);
				Edge_desc edge = edgePair.first;
				path.push_back(edge);
			}
			return distanceMap[b];
		}
		void make_transition(int e, bool add) {
			if (add == false) {cout << "got one type 12." << endl;}
			Domain domain = design.domains[e];
			vector<int> possible_crossovers = domain.crossover_ids;
			Edge_desc myedge = id_to_edge(e);
			int e2, v1, v2;
			Edge_desc cross_edge;
			Crossover c;
			for (vector<int>::iterator it = possible_crossovers.begin(); it != possible_crossovers.end(); ++it) {
				c = design.crossovers[*it];
				if ( c.d_a == e ) { e2 = c.d_b; } else { e2 = c.d_a; } 
				cross_edge = id_to_edge(e2);
				if (g[cross_edge].type == 'd') {
					if (g[myedge].type == 'd') {
						if (edge(c.v_a, c.v_b, g).second == true || edge(c.v_b, c.v_a, g).second == true){
							remove_crossover(*it);
						}
					}
					else if (g[myedge].type == 's') {
						if (add == true){ 
							add_crossover(*it);
						}
					}
					else {printf ("Error! MyGraph::make_tansition: domain is neither s nor d. \n"); exit (EXIT_FAILURE);}
				}
			}
			if (g[myedge].type == 'd') {
				g[myedge].type = 's';
			}
			else if (g[myedge].type == 's') {
				g[myedge].type = 'd';
			}
			else {printf ("Error! MyGraph::make_tansition: domain is neither s nor d. \n"); exit (EXIT_FAILURE);}
			set_edge_weights();
		}
		void update_embedding() {
			emb.resize(num_vertices(g));
			cout << "Testing for planarity: ";
			if (boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g,boyer_myrvold_params::embedding = &emb[0]))
				cout << "input graph is planar." << endl;
			else
				cout << "input graph is not planar." << endl;
		}
		void print_embedding() {
			face_counter count_visitor;
			planar_face_traversal(g, &emb[0], count_visitor, get(&EdgeProperty::id, g));
			cout << "Traversing faces of planar embedding... the planar embedding of the graph has " << count_visitor.count << " faces." << endl;
			cout << endl << "Vertices on the faces: " << endl;
			vertex_output_visitor v_vis;
			planar_face_traversal(g, &emb[0], v_vis, get(&EdgeProperty::id, g));
			cout << endl << "Edges on the faces: " << endl;
			edge_output_visitor e_vis;
			planar_face_traversal(g, &emb[0], e_vis, get(&EdgeProperty::id, g));
		}
		int get_bound_count(){
			int result = 0;
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].type == 'd'){result++;}
			}
			return result;
		}
		void test_transition(int e, bool add) {
			Domain domain = design.domains[e];
			vector<int> possible_crossovers = domain.crossover_ids;
			Edge_desc myedge = id_to_edge(e);
			int e2, v1, v2;
			Edge_desc cross_edge;
			Crossover c;
			for (vector<int>::iterator it = possible_crossovers.begin(); it != possible_crossovers.end(); ++it) {
				c = design.crossovers[*it];
				if ( c.d_a == e ) { e2 = c.d_b; } else { e2 = c.d_a; } 
				cross_edge = id_to_edge(e2);
				if (g[cross_edge].type == 'd') {
					if (g[myedge].type == 'd') {
						if (edge(c.v_a, c.v_b, g).second == true || edge(c.v_b, c.v_a, g).second == true){
							//remove_crossover(*it);
						}
					}
					else if (g[myedge].type == 's') {
						if (add == true){ 
							//add_crossover(*it);
						}
					}
					else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				}
			}
			if (g[myedge].type == 'd') {
				g[myedge].type = 's';
			}
			else if (g[myedge].type == 's') {
				g[myedge].type = 'd';
			}
			else {printf ("Error!\n"); exit (EXIT_FAILURE);}
			set_edge_weights();
		}
};

