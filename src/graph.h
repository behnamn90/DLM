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
		Tile design;
		Graph g;
		Embedding emb;
		void read_tile(Tile& dummy){
			design = dummy;
		}
		void add_vertices(){
			for(int i=0; i<design.n_vertices; i++){
				add_vertex(g);
			}
		}
		void add_domains(){
			EdgeProperty EP;
		    for (vector<Domain>::iterator d = design.domains.begin(); d != design.domains.end(); ++d) {	
				EP.length = (*d).length;
				EP.id = distance(domains.begin(), d);
				add_edge((*d).source, (*d).target, EP, g);
			}
		}
		MyGraph(Tile& dummy){
			read_tile(dummy);
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
};

