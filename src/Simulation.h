class Simulation{
	public:
		//Simulation(Constants *constants_, MyGraph *G_, TempRamp *ramp_);
		Simulation(){}
		~Simulation(){}

		Constants *constants;
		MyGraph *G;
		TempRamp *ramp;
		vector< vector<double> > rates;

		double dG_duplex(Domain &domain);
		double dG_stack();
		void print_rates();
		double calc_tot_rate();
};
double Simulation::dG_duplex(Domain &domain) {
	double result;
	double T = ramp->get_T();
	double terminal = ( ( constants->dH_termAT - T*constants->dS_termAT ) + ( constants->dH_termCG - T*constants->dS_termCG ) ); // 2 * 1/2
	double dH = (domain.length - 1) * constants->dH_average;
	double dS = (domain.length - 1) * constants->dS_average;
	//double dS_salt = (domain.length-1) * 0.368 * log( 0.5*constants->conc_Tris + 3.3*sqrt(constants->conc_Mg) );
	//double dS_salt = (domain.length-1) * 0.368 * 3.45528339927;  //using 12.5 and 40.
	//double dS_salt = (domain.length - 1) * 0.368 * -0.944; // using 0.001 * 12.5 and 0.001 * 40.  (Frits)
	double dS_salt = (domain.length - 1) * 0.368 * constants->salt_per_phosphate; // using 0.001 * 12.5 and 0.001 * 40.  (Frits)
	//double dS_salt = 0.;
	result = constants->dH_init - T*constants->dS_init + dH - T*dS + terminal - dS_salt;
	return result;
}
double Simulation::dG_stack() {
	double result;
	double dG_average = constants->dH_average - ramp->get_T()*constants->dS_average; //Not sure if this is what he means in paper.
	result = constants->n_parameter * dG_average;
	//cout << result << endl;
	return result;
}	
void Simulation::print_rates(){
	print_matrix(rates);
}
double Simulation::calc_tot_rate(){
	double result = 0;
    for(vector< vector<double> >::iterator d = rates.begin(); d != rates.end(); ++d){
		for(vector<double>::iterator j = d->begin(); j != d->end(); ++j){
			result += *j;
		}
	}
	return result;
}


class Local: public Simulation{
	public:
		Local(Constants *constants_, MyGraph *G_, TempRamp *ramp_);
		~Local(){}

		double dG_shape(Crossover &crossover);
		vector<double> calc_rate(int d);
		void fill_rates();
		void run(string filename_, int seed);
		void test(int seed, double T);
		void test(int seed, double T, int d);
};
Local::Local(Constants *constants_, MyGraph *G_, TempRamp *ramp_){
	ramp = ramp_;
	constants = constants_;
	G = G_;
	rates.resize(G->design.domains.size());
}
double Local::dG_shape(Crossover &crossover) {
	double result;
	double E;
	E = constants->lambda_ss * constants->lambda_ss + G->total_weight(crossover.v_a, crossover.v_b);
	result = -(constants->gas_constant * ramp->get_T()) * constants->gamma_parameter * log(constants->C_parameter / E);
	//cout << crossover.v_a << "\t" << crossover.v_b << "\t" << G->total_weight(crossover.v_a, crossover.v_b) << "\t" << ramp->get_T() << "\t" << E << "\t" << result << endl;
	return result;
}
vector<double> Local::calc_rate(int d){
	vector<double> result;
	double dG = 0;
	Domain domain = G->design.domains[d];
	Edge_desc edge = G->id_to_edge(d);
	Edge_desc cross_edge;
	Edge_desc stack_edge;
	// Binding
	if (G->g[edge].type == 's') {
		if(domain.cross_domains.empty()){ // 0 --> 1
			result.push_back(constants->k_plus * constants->conc_staple);
		}
		else{
			for (vector<int>::iterator it = domain.cross_domains.begin(); it != domain.cross_domains.end(); ++it) {
				cross_edge = G->id_to_edge(*it);
				if (G->g[cross_edge].type == 's') { // 00 --> 01
					result.push_back(constants->k_plus * constants->conc_staple);
				}
				else if (G->g[cross_edge].type == 'd') { // 01 --> 11 or 12   
					//Can check s22 case by checking whether the crossover exists.
					int c = domain.crossover_ids[0]; //needs updating for multiple crossovers
					Crossover crossover = G->design.crossovers[c];
					dG += dG_shape(crossover);
					result.push_back( (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( -dG / (constants->gas_constant * ramp->get_T()) ) );
					result.push_back(constants->k_plus * constants->conc_staple); //12 case (different staples)
					//result = constants->k_plus * exp( -dG / (constants->gas_constant * ramp->get_T()) );
				}
				else{printf ("Error!\n"); exit (EXIT_FAILURE);}
			}
		}
	}
	// Unbinding
	else if (G->g[edge].type == 'd') {
		if(domain.cross_domains.empty()){ // 1 --> 0
			dG += dG_duplex(domain); 
		}
		else{
			for (vector<int>::iterator it = domain.cross_domains.begin(); it != domain.cross_domains.end(); ++it) {
				cross_edge = G->id_to_edge(*it);
				if (G->g[cross_edge].type == 's') { // 01 --> 00
					dG += dG_duplex(domain);
				}
				else if (G->g[cross_edge].type == 'd') { // 11 --> 01
					//Two different rates here as well. DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! Actually, the rates are the same in local model.
					dG += dG_duplex(domain);
				}
				else{printf ("Error!\n"); exit (EXIT_FAILURE);}
			}
		}
		for (vector<int>::iterator it = domain.stack_domains.begin(); it != domain.stack_domains.end(); ++it){
			stack_edge = G->id_to_edge(*it);
			if (G->g[stack_edge].type == 'd'){
				dG += dG_stack();
			}
		}
		result.push_back( (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( dG / (constants->gas_constant * ramp->get_T() ) ) );
		//result = constants->k_plus * exp( dG / (constants->gas_constant * ramp->get_T() ) );
	}
	else{printf ("Error!\n"); exit (EXIT_FAILURE);}
	
	return result;
}
void Local::fill_rates() {
	//rates.clear();
	//for (int d=0; d<G->design.domains.size(); d++){
		//cout << calc_rate(d).size() << endl;
	//	rates.push_back(calc_rate(d));
	//}
	for (int d=0; d<rates.size(); d++){
		//cout << calc_rate(d).size() << endl;
		rates[d] = calc_rate(d);
	}
}
void Local::run(string filename_, int seed) {
	ofstream change_file;
	string filename = "../Output/"+filename_+".txt";
	change_file.open(filename);
	constants->write_params(change_file);
	typedef boost::minstd_rand base_generator_type;
	base_generator_type generator(seed);
	uniform_real<> uni_dist(0,1);
	variate_generator<base_generator_type&, uniform_real<> > uni(generator, uni_dist);
    double r1, r2, tau, min, max, total_rate;
	int next_edge, k, reaction_index, reaction_type;
	ramp->set_time(0.);
	while (ramp->current_t < ramp->t_max){
		bool add = true;
		fill_rates();
		r1 = uni(); r2 = uni();
		total_rate = calc_tot_rate(); 
		min = 0.; //check whether this can be outside this loop.
		max = 0.;
		k=0; //reaction index
		for (int i=0; i<rates.size(); i++){ 
			for (int j=0; j<rates[i].size(); j++){ 
				max = max + rates[i][j]/total_rate; 
				if (r2 >= min && r2 < max){
					next_edge = i;
					reaction_index = k;
					reaction_type = j;
					break; //need to break twice
				}
				min = max;
				if (j!=0)
				   	k++;
			}
			if (r2 >= min && r2 < max){
				break;
			}
			k++;
		}
		if (reaction_type == 1) {add = false;}	
		//G->set_edge_weights(); 
		tau = (1./total_rate)*log(1./r1); //Total rate is very small when mostly bound at low T (upto 60C)! This is causing problems by giving large tau.
		if (tau > ramp->cool_rate){continue;}
		change_file << ramp->current_t << "\t" << centigrade(ramp->get_T()) << "\t" << next_edge << "\t" << G->get_bound_count() << "\n";
		G->make_transition(next_edge,add);
		ramp->move_time(tau); //this causes segmentation fault if the current time after this is longer that total time. But fixes it in the function itself.
	}
	change_file.close();
}
void Local::test(int seed, double T) {
	typedef boost::minstd_rand base_generator_type;
	base_generator_type generator(seed);
	uniform_real<> uni_dist(0,1);
	variate_generator<base_generator_type&, uniform_real<> > uni(generator, uni_dist);
    double r1, r2, tau, min, max, total_rate;
	int next_edge, k, reaction_index, reaction_type;
	ramp->set_temp(T);
	bool add = true;
	fill_rates();
	//print_rates();
	r1 = uni(); r2 = uni();
	total_rate = calc_tot_rate(); 
	min = 0.; //check whether this can be outside this loop.
	max = 0.;
	k=0; //reaction index
	for (int i=0; i<rates.size(); i++){ 
		for (int j=0; j<rates[i].size(); j++){ 
			max = max + rates[i][j]/total_rate; 
			//cout << "i: " << i << "\t" << "j: " << j << "\t" << "k: " << k << "\t" << "min: " << min << "\t" << "max: " << max << endl;
			if (r2 >= min && r2 < max){
				next_edge = i;
				reaction_index = k;
				reaction_type = j;
				break; //need to break twice
			}
			//cout << i << "\t" << j << "\t" << k << "\t" << min << "\t" << max << endl;
			min = max;
			if (j!=0)
				k++;
		}
		if (r2 >= min && r2 < max){
			break;
		}
		k++;
	}
	if (reaction_type == 1) {add = false;}	
	tau = (1./total_rate)*log(1./r1); //Total rate is very small when mostly bound at low T (upto 60C)! This is causing problems by giving large tau.
	cout << "T: " << centigrade(ramp->get_T()) << "C\tbound: " << G->get_bound_count() << "\tr1: " << r1 << "\tr2: " << r2 << "\ttot_rate: " << total_rate << "\ttau: " << tau << "\tnext_edge: " << next_edge << "\treaction_type: " << reaction_type << endl;
	int d = next_edge;
	vector<double> result;
	double dG = 0;
	Domain domain = G->design.domains[d];
	Edge_desc edge = G->id_to_edge(d);
	Edge_desc cross_edge;
	Edge_desc stack_edge;
	// Binding
	if (G->g[edge].type == 's') {
		if(domain.cross_domains.empty()){ // 0 --> 1
			result.push_back(constants->k_plus * constants->conc_staple);
			cout << "0->1:\t";	
		}
		else{
			for (vector<int>::iterator it = domain.cross_domains.begin(); it != domain.cross_domains.end(); ++it) {
				cross_edge = G->id_to_edge(*it);
				if (G->g[cross_edge].type == 's') { // 00 --> 01
					result.push_back(constants->k_plus * constants->conc_staple);
					cout << "00->01:\t";	
				}
				else if (G->g[cross_edge].type == 'd') { // 01 --> 11 or 12   
					//Can check s22 case by checking whether the crossover exists.
					int c = domain.crossover_ids[0]; //needs updating for multiple crossovers
					Crossover crossover = G->design.crossovers[c];
					dG += dG_shape(crossover);
					result.push_back( (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( -dG / (constants->gas_constant * ramp->get_T()) ) );
					result.push_back(constants->k_plus * constants->conc_staple); //12 case (different staples)
					cout << "01->11:\t" << "dG_shape= " << dG_shape(crossover) << "\t";	
					//result = constants->k_plus * exp( -dG / (constants->gas_constant * ramp->get_T()) );
				}
				else{printf ("Error!\n"); exit (EXIT_FAILURE);}
			}
		}
	}
	// Unbinding
	else if (G->g[edge].type == 'd') {
		if(domain.cross_domains.empty()){ // 1 --> 0
			dG += dG_duplex(domain); 
			cout << "1->0:\t" << "dG_duplex= " << dG_duplex(domain) << "\t";	
		}
		else{
			for (vector<int>::iterator it = domain.cross_domains.begin(); it != domain.cross_domains.end(); ++it) {
				cross_edge = G->id_to_edge(*it);
				if (G->g[cross_edge].type == 's') { // 01 --> 00
					dG += dG_duplex(domain);
					cout << "01->00:\t" << "dG_duplex= " << dG_duplex(domain) << "\t";	
				}
				else if (G->g[cross_edge].type == 'd') { // 11 --> 01
					//Two different rates here as well. DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! Actually, the rates are the same in local model.
					dG += dG_duplex(domain);
					cout << "11->01:\t" << "dG_duplex= " << dG_duplex(domain) << "\t";	
				}
				else{printf ("Error!\n"); exit (EXIT_FAILURE);}
			}
		}
		for (vector<int>::iterator it = domain.stack_domains.begin(); it != domain.stack_domains.end(); ++it){
			stack_edge = G->id_to_edge(*it);
			if (G->g[stack_edge].type == 'd'){
				dG += dG_stack();
				cout << "dG_stack= " << dG_stack() << "\t";	
			}
		}
		cout << "dG= " << dG << "\t";	
		result.push_back( (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( dG / (constants->gas_constant * ramp->get_T() ) ) );
	}
	else{printf ("Error!\n"); exit (EXIT_FAILURE);}
	cout << "rate(s): ";
	for (int i=0; i<result.size(); i++){
		cout << result[i] << "\t";
	}
	cout << "\n";
}
void Local::test(int seed, double T, int d) {
	ramp->set_temp(T);
	fill_rates();
	double total_rate = calc_tot_rate(); 
	cout << "T: " << centigrade(ramp->get_T()) << "\td: " << d << "\ttot_rate: " << total_rate << "\t";
	vector<double> result;
	double dG = 0;
	Domain domain = G->design.domains[d];
	Edge_desc edge = G->id_to_edge(d);
	Edge_desc cross_edge;
	Edge_desc stack_edge;
	// Binding
	if (G->g[edge].type == 's') {
		if(domain.cross_domains.empty()){ // 0 --> 1
			result.push_back(constants->k_plus * constants->conc_staple);
			cout << "0->1:\t";	
		}
		else{
			for (vector<int>::iterator it = domain.cross_domains.begin(); it != domain.cross_domains.end(); ++it) {
				cross_edge = G->id_to_edge(*it);
				if (G->g[cross_edge].type == 's') { // 00 --> 01
					result.push_back(constants->k_plus * constants->conc_staple);
					cout << "00->01:\t";	
				}
				else if (G->g[cross_edge].type == 'd') { // 01 --> 11 or 12   
					//Can check s22 case by checking whether the crossover exists.
					int c = domain.crossover_ids[0]; //needs updating for multiple crossovers
					Crossover crossover = G->design.crossovers[c];
					dG += dG_shape(crossover);
					result.push_back( (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( -dG / (constants->gas_constant * ramp->get_T()) ) );
					result.push_back(constants->k_plus * constants->conc_staple); //12 case (different staples)
					cout << "01->11:\t" << "dG_shape= " << dG_shape(crossover) << "\t";	
					//result = constants->k_plus * exp( -dG / (constants->gas_constant * ramp->get_T()) );
				}
				else{printf ("Error!\n"); exit (EXIT_FAILURE);}
			}
		}
	}
	// Unbinding
	else if (G->g[edge].type == 'd') {
		if(domain.cross_domains.empty()){ // 1 --> 0
			dG += dG_duplex(domain); 
			cout << "1->0:\t" << "dG_duplex= " << dG_duplex(domain) << "\t";	
		}
		else{
			for (vector<int>::iterator it = domain.cross_domains.begin(); it != domain.cross_domains.end(); ++it) {
				cross_edge = G->id_to_edge(*it);
				if (G->g[cross_edge].type == 's') { // 01 --> 00
					dG += dG_duplex(domain);
					cout << "01->00:\t" << "dG_duplex= " << dG_duplex(domain) << "\t";	
				}
				else if (G->g[cross_edge].type == 'd') { // 11 --> 01
					//Two different rates here as well. DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! DANGER! Actually, the rates are the same in local model.
					dG += dG_duplex(domain);
					cout << "11->01:\t" << "dG_duplex= " << dG_duplex(domain) << "\t";	
				}
				else{printf ("Error!\n"); exit (EXIT_FAILURE);}
			}
		}
		for (vector<int>::iterator it = domain.stack_domains.begin(); it != domain.stack_domains.end(); ++it){
			stack_edge = G->id_to_edge(*it);
			if (G->g[stack_edge].type == 'd'){
				dG += dG_stack();
				cout << "dG_stack= " << dG_stack() << "\t";	
			}
		}
		cout << "dG= " << dG << "\t";	
		result.push_back( (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( dG / (constants->gas_constant * ramp->get_T() ) ) );
	}
	else{printf ("Error!\n"); exit (EXIT_FAILURE);}
	cout << "rate(s): ";
	for (int i=0; i<result.size(); i++){
		cout << result[i] << "\t";
	}
	cout << "\n";
}
