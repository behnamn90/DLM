#include "Common.h"
#include "Constants.h"
#include "Design.h"
#include "MyGraph.h"
#include "TempRamp.h"
#include "Simulation.h"

int main(int argc, const char * argv[]) {
	if (argc != 6){
		cout << "Arguments: gamma n conc cool_rate[Cpm] seed." << endl;
		exit (EXIT_FAILURE);	
	}
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
	Melt *melt = new Melt(40.,100.,Cpm2spC(arguments[4]),1);
	Anneal *anneal = new Anneal(100.,40.,Cpm2spC(arguments[4]),1);
	
	Local local_a = Local(constants, G, anneal);
	Local local_mb = Local(constants, G_full, melt);
	//string file_params = "_"+to_string(arguments[1])+"_"+to_string(arguments[2])+"_"+to_string(arguments[3])+"_"+to_string(arguments[4])+"_"+to_string(seed);
	string file_params;
	for (int i=1; i<argc; i++){
		file_params.append("_");
		file_params.append(argv[i]);
	}
	local_a.run("Anneal"+file_params, seed);
	//local_mb.run("Melt"+file_params, seed);
	/*
	G_half->update_embedding();
	G_half->update_faces();
	Embedding my_emb = G_half->faces;

	//Embedding my_emb = G_half->emb;
	int i = 0;
	for (Embedding::iterator f = my_emb.begin(); f != my_emb.end(); ++f) {
		cout << "face " << i << " has size " << (*f).size() << endl;  
		for (Path::iterator e = (*f).begin(); e != (*f).end(); ++e) {
			cout << (*e) << " ";
		}
		cout << endl;
		i++;
	}
	Path myloop = G_half->select_face(17);
	for (Path::iterator e = myloop.begin(); e != myloop.end(); ++e) {
		cout << (*e) << " ";
	}
	cout << endl;
	*/
	delete constants;
	delete G;
	delete anneal;
	delete melt;

	return 0;
}

