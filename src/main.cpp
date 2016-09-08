#include "common.h"
#include "constants.h"
#include "design.h"
#include "graph.h"
//#include "bundled.h"
//#include "SSA.h"
//#include "rates.h"


int main() {
	Design Frits;
	MyGraph G(Frits);
	//G.design.print_domains();
	//G.design.print_crossovers();
	G.add_all_crossovers();
	G.reset_edge_index();
	G.bind_all_domains();
	G.set_edge_weights();
	G.print_edges();
	G.write_gv("bound");
}















