class Simulation{
	public:
		//Simulation(Constants *constants_, MyGraph *G_, TempRamp *ramp_);
		Simulation(){}
		~Simulation(){}

		Constants *constants;
		MyGraph *G;
		TempRamp *ramp;
		vector<double> rates;

		double dG_duplex(Domain &domain);
		double dG_stack();
		void print_rates();
};
double Simulation::dG_duplex(Domain &domain) {
	double result;
	double T = ramp->get_T();
	double terminal = ( ( constants->dH_termAT - T*constants->dS_termAT ) + ( constants->dH_termCG - T*constants->dS_termCG ) ); // 2 * 1/2
	double dH = (domain.length-1) * constants->dH_average;
	double dS = (domain.length-1) * constants->dS_average;
	//double dS_salt = (domain.length-1) * 0.368 * log( 0.5*constants->conc_Tris + 3.3*sqrt(constants->conc_Mg) );
	//double dS_salt = (domain.length-1) * 0.368 * -0.944;
	double dS_salt = 0.;
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
	print_vec(rates);
}


class Local: public Simulation{
	public:
		Local(Constants *constants_, MyGraph *G_, TempRamp *ramp_);
		~Local(){}

		double dG_shape(Crossover &crossover);
		double calc_rate(int d);
		void fill_rates();
		void run(string filename_, int seed);
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
double Local::calc_rate(int d){
	double result;
	double dG = 0;
	Domain domain = G->design.domains[d];
	Edge_desc edge = G->id_to_edge(d);
	Edge_desc cross_edge;
	Edge_desc stack_edge;
	// Binding
	if (G->g[edge].type == 's') {
		if(domain.cross_domains.empty()){ // 0 --> 1
			result = constants->k_plus * constants->conc_staple;
		}
		else{
			for (vector<int>::iterator it = domain.cross_domains.begin(); it != domain.cross_domains.end(); ++it) {
				cross_edge = G->id_to_edge(*it);
				if (G->g[cross_edge].type == 's') { // 00 --> 01
					result = constants->k_plus * constants->conc_staple;
				}
				else if (G->g[cross_edge].type == 'd') { // 01 --> 11   
					//Can check s22 case by checking whether the crossover exists.
					int c = domain.crossover_ids[0]; //needs updating for multiple crossovers
					Crossover crossover = G->design.crossovers[c];
					dG += dG_shape(crossover);
					result = (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( -dG / (constants->gas_constant * ramp->get_T()) );
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
					//Can check s22 case by checking whether the crossover exists.
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
		result = (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( dG / (constants->gas_constant * ramp->get_T() ) );
		//result = constants->k_plus * exp( dG / (constants->gas_constant * ramp->get_T() ) );
	}
	else{printf ("Error!\n"); exit (EXIT_FAILURE);}

	return result;
}
void Local::fill_rates() {
	for (int d=0; d<G->design.domains.size(); d++){
		rates[d] = calc_rate(d);
	}
}
void Local::run(string filename_, int seed) {
	ofstream change_file;
	string filename = "../Output/"+filename_+".txt";
	change_file.open(filename);
	typedef minstd_rand base_generator_type;
	base_generator_type generator(seed);
	uniform_real<> uni_dist(0,1);
	variate_generator<base_generator_type&, uniform_real<> > uni(generator, uni_dist);
    double r1, r2;
	double tau;
	double min;
 	double max;
	int next_edge;
	double total_rate;
	ramp->set_time(0.);
	while (ramp->current_t < ramp->t_max){
		fill_rates();
		r1 = uni(); r2 = uni();
		total_rate = sum_elements(rates); 
		min = 0.; //check whether this can be outside this loop.
		max = 0.;
		for (int i=0; i<G->design.domains.size(); i++){ 
			max = max + rates[i]/total_rate; 
			if (r2 >= min && r2 < max){
				next_edge = i;
				break;
			}
			min = max;
		}
		G->make_transition(next_edge);
		//G->set_edge_weights(); 
		change_file << ramp->current_t << "\t" << centigrade(ramp->get_T()) << "\t" << next_edge << "\t" << G->get_bound_count() << "\n";
		tau = (1./total_rate)*log(1./r1); //Total rate is very small when mostly bound at low T (upto 60C)! This is causing problems by giving large tau.
		ramp->move_time(tau); //this causes segmentation fault if the current time after this is longer that total time. But fixes it in the function itself.
	}
	change_file.close();
}
/*
void Local::run(string filename_, int seed) {
	ofstream change_file;
	string filename = "../Output/"+filename_+".txt";
	change_file.open(filename);
	typedef minstd_rand base_generator_type;
	base_generator_type generator(seed);
	uniform_real<> uni_dist(0,1);
	variate_generator<base_generator_type&, uniform_real<> > uni(generator, uni_dist);
    double r1, r2;
	double tau;
	double min=0.;
 	double max;
	int next_edge;
	double total_rate;
	ramp->set_time(0.);
	while (ramp->current_t < ramp->t_max){
		fill_rates();
		r1 = uni(); r2 = uni();
		total_rate = sum_elements(rates); 
		max = rates[0]/total_rate;
		for (int i=0; i<G->design.domains.size(); i++){ //PROBLEM HERE with rates index
			if (i!=0){	
				max = max + rates[i]/total_rate; 
			}
			if (r2 >= min && r2 < max){
				next_edge = i;
				break;
			}
			min = max;
		}
		G->make_transition(next_edge);
		G->set_edge_weights(); 
		change_file << ramp->current_t << "\t" << centigrade(ramp->get_T()) << "\t" << next_edge << "\t" << G->get_bound_count() << "\n";
		tau = (1./total_rate)*log(1./r1);
		ramp->move_time(tau);
	}
	change_file.close();
}
*/
