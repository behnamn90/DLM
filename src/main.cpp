#include "Common.h"
#include "Constants.h"
#include "Design.h"
#include "MyGraph.h"
#include "TempRamp.h"
#include "Simulation.h"

int main(int argc, const char * argv[]) {
	double arguments[argc-1]; //gamma, n, conc, cool_rate, seed
	for(int i = 1; i < argc-1; i++) { 
		arguments[i] = strtod(argv[i], NULL);
	}
	int seed = stoi(argv[5]);

	Constants *constants = new Constants(arguments[1],arguments[2],arguments[3],9);

	MyGraph *G = new MyGraph(Frits(), constants);
	MyGraph *G_half = new MyGraph(Frits(), constants);
	MyGraph *G_full = new MyGraph(Frits(), constants);
	for (int i=0; i<166; i++){
		if (i%2==0){G_half->make_transition(i,true);}
	}
	for (int i=0; i<166; i++){G_full->make_transition(i,true);}
	Melt *melt = new Melt(40.,100.,Cpm2spC(arguments[4]));
	Anneal *anneal = new Anneal(100.,40.,Cpm2spC(arguments[4]));
	
	Local local_a = Local(constants, G, anneal);
	Local local_mb = Local(constants, G_full, melt);
	//string file_params = "_"+to_string(arguments[1])+"_"+to_string(arguments[2])+"_"+to_string(arguments[3])+"_"+to_string(arguments[4])+"_"+to_string(seed);
	string file_params;
	for (int i=1; i<argc; i++){
		file_params.append("_");
		file_params.append(argv[i]);
	}
	local_a.run("Anneal"+file_params, seed);
	local_mb.run("Melt"+file_params, seed);
	delete constants;
	delete G;
	delete anneal;
	delete melt;
/*
	int length = 16;
	double dH = (length - 1) * constants->dH_average;
	double dS = (length - 1) * constants->dS_average;
	//double dS_salt = (length - 1) * 0.368 * constants->salt_per_phosphate; // using 0.001 * 12.5 and 0.001 * 40.  (Frits)
	double dS_salt = 0.;
	double terminal, dG_duplex, dG_stack, dG_shape, T, r, r2, E;
	E =  constants->lambda_ss * constants->lambda_ss + 4. * length *constants->l_ds * length * constants->l_ds  +  8.* length * constants->l_ss * constants->lambda_ss; 
	cout << "TC" << "\t" << "dG_duplex" << "\t" <<"dG_shape" << "\t" << "k+[i]" << "\t" << "r(duplex)" << "\t" << "r(shape)" << endl;
	for (double TC=0; TC<100.; TC+=0.1){
		dG_shape = -(constants->gas_constant * T) * constants->gamma_parameter * log(constants->C_parameter / E);
		T = kelvin(TC);
		terminal = ( ( constants->dH_termAT - T*constants->dS_termAT ) + ( constants->dH_termCG - T*constants->dS_termCG ) ); // 2 * 1/2
		dG_duplex = constants->dH_init - T*constants->dS_init + dH - T*dS + terminal - T*dS_salt;
		dG_stack = constants->n_parameter * (constants->dH_average - T*constants->dS_average); //Not sure if this is what he means in paper.
		r = (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( dG_duplex / (constants->gas_constant * T ) );
		r2 = (pow(10,3)*(1/(constants->scale*constants->scale*constants->scale))) * constants->k_plus * exp( -dG_shape / (constants->gas_constant *T) );
		cout << TC << "\t" << dG_duplex << "\t" << dG_shape << "\t" << constants->k_plus * constants->conc_staple << "\t" << r << "\t" << r2 <<endl;
	}
	//Design test = Frits();
	//test.print_domains();
*/
	/*
	for (unsigned i = test.domains.size(); i-- > 0; ) {
		cout << i << "\t" << test.domains[i].seq << endl;
	}
	int scaff_size = test.scaffold_sequence.size();
	int staple_size = 0; 
	for (int i = 0; i < test.domains.size(); i++){
		staple_size += test.domains[i].seq.size();
	}
	cout << scaff_size << "\t" << staple_size << endl;
	*/

	return 0;
}















