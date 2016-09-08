struct VertexProperty{
	int id;
	vector<int> pair_id; // eg 12 is connected by two crossovers
	vector<string> pair_type; //i: inside-crossover, o: outside-crossover
};
struct EdgeProperty{
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
	Vertex_iter vi,vf;
	int i = 0;
	for(tie(vi,vf) = vertices(g); vi != vf; ++vi) {
		g[*vi].id = i;
		i++;
	}
}
template <class Graph> void reset_edge_index(Graph& g) {
	Edge_iter ei,ef;
	boost::tie( ei, ef) = boost::edges(g);
	int i = 0;
	for ( ; ei != ef ; ++ei ){
		g[*ei].id = i;
		i++;
	}
}
template <class Graph> void add_domains(Graph& g) { 
	Vertex_iter vi,vf;
	EdgeProperty EP, EP_long; 
	EP.type = "s"; EP_long.type = "s";
	EP.length = default_domain_size; EP_long.length = default_domain_size * 2;
	for(boost::tie(vi,vf) = vertices(g); vi != vf; ++vi) {
		add_edge(*vi, *vi + 1, EP, g);
	}
	add_edge(num_vertices(g)-1, 0, EP_long, g);
	pair<Edge_desc, bool> edgePair = edge(82, 83, g);
	Edge_desc en = edgePair.first;
	g[en].length = default_domain_size * 2;
}
double Temp(double& time) {
	double result;
	int n_intervals = t_max;
	double current_Temp;
	if (time < t_max){
		current_Temp = T_max;
		//current_Temp = T_min;
		for (int i=0; i<n_intervals; i = i + cool_rate){
			if (time < i){
				result = current_Temp;
				break;
			}
			current_Temp = current_Temp - 1.;	
			//current_Temp = current_Temp + 1.;	
		}
	}
	/*
	if (time < t_max){
		current_Temp = T_max;
		for (int i=0; i<n_intervals; i = i + cool_rate){
			if (time < i){
				result = current_Temp;
				break;
			}
			current_Temp = current_Temp - 1.;	
		}
	}
	if (time >= t_max && time < 2.* t_max){
		current_Temp = T_min;
		for (int i=n_intervals; i<2*n_intervals; i = i + cool_rate){
			if (time < i){
				result = current_Temp;
				break;
			}
			current_Temp = current_Temp + 1.;	
		}
	}
	*/
	else {printf ("Time > 2*t_max passed to Temp function!.\n");}
	return result;
}
double G_duplex(double& T, int& n_bp) {
	double result;
	double terminal = ( ( dH_termAT - T*dS_termAT ) + ( dH_termCG - T*dS_termCG ) ); // 2 * 1/2
	double dH = (n_bp-1) * dH_average;
	double dS = (n_bp-1) * dS_average;
	double salt = (n_bp-1) * 0.368 * log( 0.5*conc_Tris + 3.3*sqrt(conc_Mg) );
	result = dH_init - T*dS_init + dH - T*dS + terminal + salt;
	return result;
}
double G_stack(double& T) {
	double result;
	double dG_average = dH_average - T*dS_average; //Not sure if this is what he means in paper.
	result = n_parameter * dG_average;
	return result;
}	
template <class Graph> double G_shape(Graph& g, Graph& bound_graph, int& e, double& T) {
	double result;
	double E;
	Edge_desc eg; //descriptor for edge in current graph.
	Edge_iter ei,ef;
	tie( ei, ef) = edges(g);
	for ( ; ei != ef ; ++ei ){
		if(g[*ei].id == e){eg = *ei;}
	}
	Edge_desc eb = edge(source(eg,g),target(eg,g),bound_graph).first; //descriptor for edge in bound graph.
	vector< pair<int,int> >::iterator id_it; //iterator for pair_id.
	int pt_it; //iterator for the pair_types.
	Edge_desc e2; //descriptor for pair (on current graph).
	if (g[eg].type == "d"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g).first; //the second domain.
				if (g[e2].type == "d"){
					printf ("This type of transition does not need loop cost.\n"); exit (EXIT_FAILURE);
				}
				else if (g[e2].type == "s"){
					printf ("This type of transition only needs loop cost in Glabal Model.\n"); exit (EXIT_FAILURE);
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
	}
	else if (g[eg].type == "s"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g).first; //the second domain.
				if (g[e2].type == "d"){
					if( (bound_graph[eb].pair_type)[pt_it] == "i" || (bound_graph[eb].pair_type)[pt_it] == "m") {
						if ( source(eg,g) < source(e2,g) )
							E = lambda_ss * lambda_ss + Er2(g, source(eg,g), target(e2,g));
						else
							E = lambda_ss * lambda_ss + Er2(g, target(eg,g), source(e2,g));
					}
					if( (bound_graph[eb].pair_type)[pt_it] == "o" ) {
						if ( source(eg,g) < source(e2,g) )
							E = lambda_ss * lambda_ss + Er2(g, target(eg,g) , source(e2,g));
						else
							E = lambda_ss * lambda_ss + Er2(g, source(eg,g) , target(e2,g));
					}
				}
				else if (g[e2].type == "s"){
					printf ("This type of transition does not need loop cost.\n"); exit (EXIT_FAILURE);
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
	}
	result = - (gas_constant * T) * gamma_parameter * log(C_parameter/E);
	return result;
}

template <class Graph> double Rate(Graph& g, Graph& bound_graph, int e, double T) {
	double result;
	double E;
	double dG_shape = 0;
	double dG_stack = 0;
	double dG_duplex = 0;

	Edge_desc eb;
	pair<int,int> seam_pair; //iterator for the pair_types.

	Edge_desc eg, eleft, eright; //descriptor for edge in current graph.
	Edge_iter ei,ef;
	tie( ei, ef) = edges(g);
	for ( ; ei != ef ; ++ei ){
		if(g[*ei].id == e){
			eg = *ei; 
			eb = edge(source(eg,g),target(eg,g),bound_graph).first;
			if (!((bound_graph[eb].pair_id).empty())){
				if( (bound_graph[eb].pair_type)[0] == "m" ) { //the seam case 
					if (e==12 || e ==26 || e==40 || e ==54 || e==68 || e ==95 || e==109 || e ==123 || e==137 || e ==151){
						--ei; eleft = *ei; ++ei;
						seam_pair = (bound_graph[eb].pair_id)[0];
						eright = edge(seam_pair.first,seam_pair.second,g).first;
					}
					else if (e==13 || e ==27 || e==41 || e ==55 || e==69 || e ==96 || e==110 || e ==124 || e==138 || e ==152){
						++ei; eright = *ei; --ei; 
						seam_pair = (bound_graph[eb].pair_id)[0];
						eleft = edge(seam_pair.first,seam_pair.second,g).first;
					}
					else{
						printf ("A non-seam got through in stack testing.\n");
						exit (EXIT_FAILURE);
					}
				}
				else {
					--ei; eleft = *ei; ++ei; 
					++ei; eright = *ei; --ei;
				}
			}
			else if (e == 165){ //the last domain case
				eleft = edge(164,165,g).first;
				eright = edge(0,1,g).first;  
			}
			else {
				--ei; eleft = *ei; ++ei; 
				++ei; eright = *ei; --ei;
			}
			break;
		}
		//Problems will occur at ends! Fix this!
	}

	eb = edge(source(eg,g),target(eg,g),bound_graph).first; //descriptor for edge in bound graph.
	
	vector< pair<int,int> >::iterator id_it; //iterator for pair_id.
	Edge_desc e2; //descriptor for pair (on current graph).
	int domain_length = g[eg].length;
	if (g[eg].type == "d"){
		if (!((bound_graph[eb].pair_id).empty())){
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g).first; //the second domain.
				if (g[e2].type == "d"){ // s11 --> s01 (7)
					dG_duplex = G_duplex(T,domain_length);
					if (g[eleft].type == "d")
						dG_stack += G_stack(T);
					if (g[eright].type == "d")
						dG_stack += G_stack(T);
					result = k_plus * exp( (dG_duplex+dG_stack) / (gas_constant*T) );  
				}
				else if (g[e2].type == "s"){ // s10 --> s00 (5)  *need shape only in global model
					dG_duplex = G_duplex(T,domain_length);
					if (g[eleft].type == "d")
						dG_stack += G_stack(T);
					if (g[eright].type == "d")
						dG_stack += G_stack(T);
					result = k_plus * exp( (dG_duplex+dG_stack) / (gas_constant*T) );  
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
			}
		}
		else { // s1 --> s0
			dG_duplex = G_duplex(T,domain_length);
			if (g[eleft].type == "d")
				dG_stack += G_stack(T);
			if (g[eright].type == "d")
				dG_stack += G_stack(T);
			result = k_plus * exp( (dG_duplex+dG_stack) / (gas_constant*T) );  
		}	
	}
	else if (g[eg].type == "s"){
		if (!((bound_graph[eb].pair_id).empty())){
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g).first; //the second domain.
				if (g[e2].type == "d"){ // s01 --> s11 (8)
					dG_shape = G_shape(g,bound_graph,e,T);
					result = k_plus * exp(-dG_shape / (gas_constant*T) );
				}
				else if (g[e2].type == "s"){ // s00 --> s10 (4)
					result = k_plus * conc_staple;	
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
			}
		}	
		else { // s0 --> s1
			result = k_plus * conc_staple;
		}	
	}

	return result;
}
template <class Graph> void Simulate(Graph& g, Graph& bound_graph, int seed) {
	typedef minstd_rand base_generator_type;
	base_generator_type generator(seed);
	uniform_real<> uni_dist(0,1);
	variate_generator<base_generator_type&, uniform_real<> > uni(generator, uni_dist);
    double r1, r2;
	double tau;
	
	int num_domains = num_vertices(g);
	Edge_iter ei,ef;

	double min=0.;
 	double max;
	int next_edge;
	double total_rate;
	vector<double> rates;

	ofstream change_file;
    string filename = "../Output/Changes_";
	string n_str = to_string(n_parameter);
	string dummy_str (n_str.begin(), n_str.begin()+3);
	filename.append(dummy_str);
	filename.append("_");
	string gamma_str = to_string(gamma_parameter);
	dummy_str.clear();
	string dummy_str2 (gamma_str.begin(), gamma_str.begin()+3);
	filename.append(dummy_str2);
	filename.append(".txt");
	change_file.open (filename);

	int num_bound_edges;
	double time = 0.;
	double T = Temp(time);
	int n_max = 100000000;
	for (int n=0; n<n_max; n++){
		num_bound_edges = 0;
		rates.clear();
		for (int i=0; i<num_domains; i++){
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].id == i){
					rates.push_back(Rate(g,bound_graph,i,T));
					if (g[*ei].type == "d") {num_bound_edges++;} 
				}
			}
		}	
		r1 = uni(); r2 = uni();
		total_rate = sum_elements(rates); 
		tau = (1./total_rate)*log(1./r1);
		time = time + tau;
		if (time > t_max) {
			cout << time << "\t";
			printf ("Error! Simulation time has exeeded!\n"); 
			break;
		}
		T = Temp(time);
		max = rates[0]/total_rate;
		
		for (int i=0; i<num_domains; i++){
			if (i!=0)	
				max = max + rates[i]/total_rate; 
			if (r2 >= min && r2 < max){
				next_edge = i;
				break;
			}
			min = max;
		}
		single_reaction(g,bound_graph,next_edge);
		set_edge_weights(g); 
		change_file << time << "\t" << Centigrade(T) << "\t" << next_edge << "\t" << num_bound_edges << "\n";
	}
	change_file.close();
	//return result;
}

/*
double Temp(double time) {
	double result;
	int n_intervals = t_max;
	double current_Temp = T_max;
	for (int i=0; i<n_intervals; i = i + cool_rate){
		if (time < i){
			result = current_Temp;
			break;
		}
		current_Temp = current_Temp - 1.;	
	}
	return result;
}
template <class Graph> double G_shape_old(Graph& g, Graph& bound_graph, int e, double T) {
	double result;
	double E;
	Graph g2; copy_graph(g,g2);
	
	Edge_desc eg; //descriptor for edge in current graph.
	Edge_iter ei,ef;
	tie( ei, ef) = edges(g2);
	for ( ; ei != ef ; ++ei ){
		if(g2[*ei].id == e){eg = *ei;}
	}

	Edge_desc eb = edge(source(eg,g2),target(eg,g2),bound_graph).first; //descriptor for edge in bound graph.
	
	vector< pair<int,int> >::iterator id_it; //iterator for pair_id.
	int pt_it; //iterator for the pair_types.
	Edge_desc e2; //descriptor for pair (on current graph).
	if (g2[eg].type == "d"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g2).first; //the second domain.
				if (g2[e2].type == "d"){
					printf ("This type of transition does not need loop cost.\n"); exit (EXIT_FAILURE);
				}
				else if (g2[e2].type == "s"){
					printf ("This type of transition only needs loop cost in Glabal Model.\n"); exit (EXIT_FAILURE);
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
	}
	else if (g2[eg].type == "s"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g2).first; //the second domain.
				if (g2[e2].type == "d"){
					if( (bound_graph[eb].pair_type)[pt_it] == "i" ) {
						if ( source(eg,g2) < source(e2,g2) )
							E = lambda_ss * lambda_ss * Er2(g2, source(eg,g2), target(e2,g2));
						else
							E = lambda_ss * lambda_ss * Er2(g2, target(eg,g2), source(e2,g2));
					}
					if( (bound_graph[eb].pair_type)[pt_it] == "o" ) {
						if ( source(eg,g2) < source(e2,g2) )
							E = lambda_ss * lambda_ss * Er2(g2, target(eg,g2) , source(e2,g2));
						else
							E = lambda_ss * lambda_ss * Er2(g2, source(eg,g2) , target(e2,g2));
					}
					
				}
				else if (g2[e2].type == "s"){
					printf ("This type of transition does not need loop cost.\n"); exit (EXIT_FAILURE);
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
		g2[eg].type = "d";
	}


	//E = 220.;
	result = - (gas_constant * T) * gamma_parameter * log(C_parameter/E);
	return result;
}
template <class Graph> double Rate_old(Graph& g, Graph& bound_graph, int e, double T) {
	double result;
	double E;
	Graph g2; copy_graph(g,g2);
	double dG_shape = 0;
	double dG_stack = 0;
	double dG_duplex = 0;

	Edge_desc eg, eleft, eright; //descriptor for edge in current graph.
	Edge_iter ei,ef;
	tie( ei, ef) = edges(g2);
	for ( ; ei != ef ; ++ei ){
		if(g2[*ei].id == e){eg = *ei; --ei; eleft = *ei; ++ei; ++ei; eright = *ei; break;}
		//Problems will occur at ends! Fix this!
	}

	Edge_desc eb = edge(source(eg,g2),target(eg,g2),bound_graph).first; //descriptor for edge in bound graph.
	
	vector< pair<int,int> >::iterator id_it; //iterator for pair_id.
	int pt_it; //iterator for the pair_types.
	Edge_desc e2; //descriptor for pair (on current graph).
	int domain_length = g2[eg].length;
	if (g2[eg].type == "d"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g2).first; //the second domain.
				if (g2[e2].type == "d"){
					dG_duplex = G_duplex(T,domain_length);
					if (g2[eleft].type == "d")
						dG_stack = G_stack(T);
					if (g2[eright].type == "d")
						dG_stack += G_stack(T);
					result = k_plus * exp( (dG_duplex+dG_shape) / (gas_constant*T) );  
				}
				else if (g2[e2].type == "s"){
					dG_duplex = G_duplex(T,domain_length);
					if (g2[eleft].type == "d")
						dG_stack = G_stack(T);
					if (g2[eright].type == "d")
						dG_stack += G_stack(T);
					result = k_plus * exp( (dG_duplex+dG_shape) / (gas_constant*T) );  
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}
		else { 
			dG_duplex = G_duplex(T,domain_length);
			if (g2[eleft].type == "d")
				dG_stack = G_stack(T);
			if (g2[eright].type == "d")
				dG_stack += G_stack(T);
			result = k_plus * exp( (dG_duplex+dG_shape) / (gas_constant*T) );  
		}	
	}
	else if (g2[eg].type == "s"){
		if (!((bound_graph[eb].pair_id).empty())){
			pt_it = 0;
			for (id_it = bound_graph[eb].pair_id.begin(); id_it != bound_graph[eb].pair_id.end(); ++id_it){
				e2 = edge((*id_it).first, (*id_it).second, g2).first; //the second domain.
				if (g2[e2].type == "d"){
					dG_shape = G_shape(g,bound_graph,e,T);
					result = k_plus * exp(-dG_shape / (gas_constant*T) );
				}
				else if (g2[e2].type == "s"){
					result = k_plus;	
				}
				else {printf ("Error!\n"); exit (EXIT_FAILURE);}
				pt_it++;
			}
		}	
		else { 
			result = k_plus;
		}	
	}

	return result;
}
template <class Graph> void Simulate_old(Graph& g, Graph& bound_graph, int seed) {
	typedef minstd_rand base_generator_type;
	base_generator_type generator(seed);
	uniform_real<> uni_dist(0,1);
	variate_generator<base_generator_type&, uniform_real<> > uni(generator, uni_dist);
    double r1, r2;
	double tau;
	
	int num_domains = num_vertices(g);
	vector<double> row(num_domains+1);
	vector< vector<double> > result;
	Edge_desc eg; 
	Edge_iter ei,ef;

	double min=0.;
 	double max;
	int next_edge;
	double total_rate;
	vector<double> rates;

	ofstream change_file;
    change_file.open ("Changes.txt");

	int num_bound_edges;
	double time = 0.;
	double T = Temp(time);
	int n_max = 100000000;
	for (int n=0; n<n_max; n++){
		if (time>t_max)
			break;
		rates.clear();
		row[0] = time; 
		num_bound_edges = 0;
		for (int i=0; i<num_domains; i++){
			tie( ei, ef) = edges(g);
			for ( ; ei != ef ; ++ei ){
				if(g[*ei].id == i){
					eg = *ei;
					rates.push_back(Rate(g,bound_graph,i,T));
					if (g[eg].type == "d") {row[i+1]=1; num_bound_edges++;} 
					if (g[eg].type == "s") row[i+1]=0;
				}
			}
		}	
		result.push_back(row);
		r1 = uni(); r2 = uni();
		total_rate = sum_elements(rates); 
		tau = (1./total_rate)*log(1./r1);
		time = time + tau;
		if (time > t_max) {
			cout << time << "\t";
			printf ("Error! Simulation time has exeeded!\n"); 
			break;
		}
		T = Temp(time);
		max = rates[0]/total_rate;
		
		for (int i=0; i<num_domains; i++){
			if (i!=0)	
				max = max + rates[i]/total_rate; 
			if (r2 >= min && r2 < max){
				next_edge = i;
				break;
			}
			min = max;
		}
		change_file << time << "\t" << T << "\t" << next_edge << "\t" << num_bound_edges << "\n";
		single_reaction(g,bound_graph,next_edge);
	}

	change_file.close();
	write_matrix(result);
	//print_matrix(result);
	
	//return result;
}
*/
