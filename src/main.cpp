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
	Constants *constants = new Constants(2.5,2.,20.);
	MyGraph *G = new MyGraph(Frits(), constants);
	MyGraph *Gb = new MyGraph(Frits(), constants);
	for (int i=0; i<166; i++){Gb->make_transition(i);}
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
	//G->update_embedding();
	//G->print_embedding();
	//G->write_gv("bound");
	//delete frits;
	//delete G;
	/*	
	Melt *melt = new Melt(40.,100.,60.);
	Local local_m = Local(constants, G, melt);
	local_m.fill_rates();
	local_m.print_rates();
	cout << sum_elements(local_m.rates) << endl;
	//local_m.run("Melt", 20);
	*/
	Anneal *anneal = new Anneal(100.,40.,60.);
	Melt *melt = new Melt(40.,100.,60.);
	
	Local local_a = Local(constants, G, anneal);
	local_a.fill_rates();
	cout << "Anneal, empty: " << sum_elements(local_a.rates) << endl;
	Local local_ab = Local(constants, Gb, anneal);
	local_ab.fill_rates();
	cout << "Anneal, full: " << sum_elements(local_ab.rates) << endl;
	
	Local local_m = Local(constants, G, melt);
	local_m.fill_rates();
	cout << "Melt, empty: " << sum_elements(local_m.rates) << endl;
	Local local_mb = Local(constants, Gb, melt);
	local_mb.fill_rates();
	cout << "Melt, full: " << sum_elements(local_mb.rates) << endl;

	//local_a.run("Anneal", 10);
	//local_m.run("Melt", 20);
	delete constants;
	delete G;
	//delete anneal;
	//delete melt;

}















