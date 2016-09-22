#include "Common.h"
#include "Constants.h"
#include "Design.h"
#include "MyGraph.h"
#include "TempRamp.h"
//#include "bundled.h"
//#include "SSA.h"
//#include "rate.h"
#include "Simulation.h"

int main() {
	Constants *constants = new Constants(1.5,0.,20.,9);

	MyGraph *G = new MyGraph(Frits(), constants);
	MyGraph *G_half = new MyGraph(Frits(), constants);
	for (int i=0; i<166; i++){
		if (i%2==0){G_half->make_transition(i,true);}
	}
	MyGraph *G_full = new MyGraph(Frits(), constants);
	for (int i=0; i<166; i++){G_full->make_transition(i,true);}
	//Gb->print_edges();
	//G->design.print_domains();
	//G->design.print_crossovers();
	//G->reset_edge_index();
	//G->bind_all_domains();
	//G->reset_edge_index();
	//G->add_all_crossovers();
	//G->set_edge_weights();
	//G->print_edges();
	//G->make_transition(1);
	//G->make_transition(2);
	//G->make_transition(3);
	//G->make_transition(1);
	//G->make_transition(8);
	//G->make_transition(3);
	//G->make_transition(6);
	//Gb->make_transition(10,true);
	//G->update_embedding();
	//G->print_embedding();
	//G->write_gv("bound");
	Melt *melt = new Melt(40.,100.,60.);
	Anneal *anneal = new Anneal(100.,40.,60.);
	
	Local local_a = Local(constants, G, anneal);
	Local local_mb = Local(constants, G_full, melt);
	local_a.run("Anneal", 10);
	local_mb.run("Melt", 10);
	delete constants;
	delete G;
	delete anneal;
	delete melt;

	return 0;
}















