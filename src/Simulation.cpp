/*
 *
 *	Simulation.cpp
 * 	Author: Behnam
 *
 */

#include "Simulation.h"

//General Simulation Methods
double Simulation::dG_duplex(Domain &domain) {
	double result;
	double dH = 0.;
	double dS = 0.;
	double terminal = 0;
	double T = ramp->get_T();
	string sequence = domain.seq;

	if (sequence[0] == 'A' || sequence[0] == 'T'){terminal += constants->dH_termAT - T*constants->dS_termAT;}
	if (sequence[sequence.size()-1] == 'A' || sequence[sequence.size()-1] == 'T'){terminal += constants->dH_termAT - T*constants->dS_termAT;}

	string sub;
	for (int i=0; i<sequence.size()-1; i++){
		sub.clear();
		sub += sequence[i];
		sub += sequence[i+1];
		if (sub == "AA" || sub == "TT") { dH+=constants->dH_AA; dS+=constants->dS_AA; }
		else if (sub == "CA" || sub == "TG") { dH+=constants->dH_CA; dS+=constants->dS_CA; }
		else if (sub == "GT" || sub == "AC") { dH+=constants->dH_GT; dS+=constants->dS_GT; }
		else if (sub == "CT" || sub == "AG") { dH+=constants->dH_CT; dS+=constants->dS_CT; }
		else if (sub == "GA" || sub == "TC") { dH+=constants->dH_GA; dS+=constants->dS_GA; }
		else if (sub == "GG" || sub == "CC") { dH+=constants->dH_GG; dS+=constants->dS_GG; }
		else if (sub == "AT") { dH+=constants->dH_AT; dS+=constants->dS_AT; }
		else if (sub == "TA") { dH+=constants->dH_TA; dS+=constants->dS_TA; }
		else if (sub == "CG") { dH+=constants->dH_CG; dS+=constants->dS_CG; }
		else if (sub == "GC") { dH+=constants->dH_GC; dS+=constants->dS_GC; }
		else {printf ("Error! sequence not in nn database. \n"); exit (EXIT_FAILURE);}
	}
	
	double dS_salt = ((domain.length - 1) * 0.368 * constants->salt_per_phosphate) / 1000.; // using 0.001 * 12.5 and 0.001 * 40.  (Frits)
	result = constants->dH_init - T*constants->dS_init + dH - T*dS + terminal - T * dS_salt;
	return result;
}
double Simulation::dG_duplex_average(Domain &domain) {
	double result;
	double T = ramp->get_T();
	double terminal = ( ( constants->dH_termAT - T*constants->dS_termAT ) + ( constants->dH_termCG - T*constants->dS_termCG ) ); // 2 * 1/2
	double dH = (domain.length - 1) * constants->dH_average;
	double dS = (domain.length - 1) * constants->dS_average;
	//double dS_salt = (domain.length-1) * 0.368 * log( 0.5*constants->conc_Tris + 3.3*sqrt(constants->conc_Mg) );
	//double dS_salt = (domain.length-1) * 0.368 * 3.45528339927;  //using 12.5 and 40.
	//double dS_salt = (domain.length - 1) * 0.368 * -0.944; // using 0.001 * 12.5 and 0.001 * 40.  (Frits)
	double dS_salt = ((domain.length - 1) * 0.368 * constants->salt_per_phosphate) / 1000.; // using 0.001 * 12.5 and 0.001 * 40.  (Frits)
	//double dS_salt = 0.;
	result = constants->dH_init - T*constants->dS_init + dH - T*dS + terminal - T * dS_salt;
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


//Local Model Methods
Local::Local(Constants *constants_, MyGraph *G_, TempRamp *ramp_){
	ramp = ramp_;
	constants = constants_;
	G = G_;
	rates.resize(G->design.domains.size());
	affected.resize(rates.size());
	for (int i = 0; i < rates.size(); i++){
		affected[i] = i;
	}
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
	//for (int d=0; d<rates.size(); d++){
		//cout << calc_rate(d).size() << endl;
		//cout << d << endl;
	//	rates[d] = calc_rate(d);
	//}
	int d;
	for (int i=0; i<affected.size(); i++){
		//cout << calc_rate(d).size() << endl;
		//cout << d << endl;
		d = affected[i];
		rates[d] = calc_rate(d);
	}
}
void Local::run(string filename_, int seed) {
	ofstream change_file;
	string filename = "../Output/"+filename_+".txt";
	change_file.open(filename);
	change_file << "seed= " << seed << endl;
	change_file << "cool_rate[s/K]= " << ramp->cool_rate << endl;
	constants->write_params(change_file);
	typedef boost::minstd_rand base_generator_type;
	base_generator_type generator(seed);
	uniform_real<> uni_dist(0,1);
	variate_generator<base_generator_type&, uniform_real<> > uni(generator, uni_dist);
    double r1, r2, tau, min, max, total_rate, T_now, T_past;
	int next_edge, reaction_type;
	ramp->set_time(0.);
	while (ramp->current_t < ramp->t_max){
		T_now = ramp->get_T();
		bool add = true;
		if ( abs(T_now-T_past) > 0.00000000001){ 
			affected.clear();
			affected.resize(rates.size());
			for (int i = 0; i < rates.size(); i++){
				affected[i] = i;
			}
		}
		sort( affected.begin(), affected.end() );
		affected.erase( unique( affected.begin(), affected.end() ), affected.end() );
		//for (int i=0; i<affected.size(); i++){
			//cout << affected[i] << " ";
		//}
		//cout << "\n\n";
		//print_rates();
		fill_rates();
		affected.clear();
		r1 = uni(); r2 = uni();
		total_rate = calc_tot_rate(); 
		min = 0.; //check whether this can be outside this loop.
		max = 0.;
		for (int i=0; i<rates.size(); i++){ 
			for (int j=0; j<rates[i].size(); j++){ 
				max = max + rates[i][j]/total_rate; 
				if (r2 >= min && r2 < max){
					next_edge = i;
					reaction_type = j;
					break; //need to break twice
				}
				min = max;
			}
			if (r2 >= min && r2 < max){
				break;
			}
		}
		if (reaction_type == 1) {add = false;}
		//G->set_edge_weights(); 
		tau = (1./total_rate)*log(1./r1); //Total rate is very small when mostly bound at low T (upto 60C)! This is causing problems by giving large tau.		
		if (tau > ramp->cool_rate){continue;}
		change_file << ramp->current_t << "\t" << centigrade(ramp->get_T()) << "\t" << next_edge << "\t" << G->get_bound_count() << "\n";
		G->make_transition(next_edge,add,affected);
		ramp->move_time(tau); //this causes segmentation fault if the current time after this is longer that total time. But fixes it in the function itself.
		T_past = T_now;
	}
	change_file.close();
}
