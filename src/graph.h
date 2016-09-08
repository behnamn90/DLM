struct VertexProperty{
	int id;
};
struct EdgeProperty{
	int id;
	int length;
	double weight;
	string type; //s: single-stranded, d: double-stranded, i: inside-crossover, o: outside-crossover, m: seam-crossover
	string colour;
	string thickness;
};
typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty > Graph;
typedef vector< vector< graph_traits<Graph>::edge_descriptor > > Embedding; //for embeddings 
typedef graph_traits<Graph>::vertex_iterator Vertex_iter;
typedef graph_traits<Graph>::edge_iterator Edge_iter;
typedef graph_traits<Graph>::vertex_descriptor Vertex_desc;
typedef graph_traits<Graph>::edge_descriptor Edge_desc;

class MyGraph{
	public:
		Design design;
		Graph g;
		Embedding emb;
		void read_design(Design& dummy){design = dummy;}
		void add_vertices(){
			for(int i=0; i<design.n_vertices; i++){
				add_vertex(g);
			}
		}
		void add_domains(){
			EdgeProperty EP; EP.type = 's';
		    for (vector<Domain>::iterator d = design.domains.begin(); d != design.domains.end(); ++d) {	
				EP.length = (*d).length;
				EP.id = distance(design.domains.begin(), d);
				add_edge((*d).source, (*d).target, EP, g);
			}
		}
		MyGraph(Design& dummy){
			read_design(dummy);
			add_vertices();
			add_domains();
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
		void add_crossover(pair<int,int> vpair){
			int cross_id = design.vpair_to_crossover(vpair);
			EdgeProperty EP; 
			EP.type = design.crossovers[cross_id].type;
			EP.length = default_crossover_size;
			add_edge(design.crossovers[cross_id].v_a, design.crossovers[cross_id].v_b, EP, g);
		}
		void add_crossover(int d){
			int cross_id = design.domain_to_crossover(d);
			EdgeProperty EP; 
			EP.type = design.crossovers[cross_id].type;
			EP.length = default_crossover_size;
			add_edge(design.crossovers[cross_id].v_a, design.crossovers[cross_id].v_b, EP, g);
		}
		void add_all_crossovers(){
			EdgeProperty EP; 
			EP.length = default_crossover_size;
			for (vector<Crossover>::iterator c = design.crossovers.begin(); c != design.crossovers.end(); ++c) {
				EP.type = (*c).type;
				add_edge((*c).v_a, (*c).v_b, EP, g);
			}
		}
		void bind_all_domains() {
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].type == "s"){g[*ei].type = "d";}
			}
		}
		void set_edge_weights(){	
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].type == "d"){g[*ei].weight = g[*ei].length * l_ds * g[*ei].length * l_ds;}
				else if(g[*ei].type == "s"){g[*ei].weight = g[*ei].length * l_ss * lambda_ss;}
				else if(g[*ei].type == "i" || g[*ei].type == "o" || g[*ei].type == "m"){g[*ei].weight = lambda_ss * lambda_ss;}
				else {cout << "no domain type found for edge" << endl;}
			}
		}
		void print_edges(){
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				cout << g[*ei].id << ":\t";
				cout << *ei << ":\t";
				cout << g[*ei].type << "\t";
				cout << g[*ei].length << "\t";
				cout << g[*ei].weight << "\n";
			}	
		}
		void write_gv(string filename) {
			reset_vertex_index();
			reset_edge_index();
			Edge_iter ei,ef;
			tie( ei, ef) = edges(g);
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
			outfile.open("../Output/bound.gv");
			write_graphviz_dp(outfile, g, dp);
			outfile.close();
		}
};

