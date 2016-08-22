#include "common.h"
#include "constants.h"
#include "design.h"
#include "bundled.h"
//#include "SSA.h"
#include "rates.h"


int main() {
	cout << dH_average << "\t" << dS_average << endl;
	int n_vertices = 166; //including zeroth vertex
	//int n_domains = n_vertices + 1;
	Tile tile;
	//tile.print_vertex_pairs();	
	Graph bound_graph(n_vertices-1); //need to exclude zeroth vertex but num_vertices(g) includes it.
	add_domains(bound_graph);
	bind_all_domains(bound_graph);
	add_all_crossovers(bound_graph,tile);
	add_all_pairs(bound_graph,tile);
	reset_vertex_index(bound_graph);	
	reset_edge_index(bound_graph);	
	//print(bound_graph);
	set_edge_weights(bound_graph);	
	//print_edge_properties(bound_graph);
	//Embedding embedding(num_vertices(bound_graph));
	//planar_embedding(bound_graph,embedding);
	//print_embedding_faces(bound_graph,embedding);	
	write_gv(bound_graph,"0_bound");
	//my_dijkstra(bound_graph,1,50);
	
	Graph g(n_vertices-1);	
	add_domains(g);
	//bind_all_domains(g);
	//add_all_crossovers(g,tile);
	//add_all_pairs(g,tile);
	reset_vertex_index(g);	
	reset_edge_index(g);	
	
	
	set_edge_weights(g);	
	write_gv(g,"0_unbound");
	int e1, e2;
	for (vector< crossover >::iterator it = tile.crossovers.begin(); it != tile.crossovers.end(); ++it){
		if ((*it).type == "m") {
			e1 = (*it).d_a; e2 = (*it).d_b; 	
			single_reaction(g,bound_graph,e1);   single_reaction(g,bound_graph,e2);
		}
	}
	single_reaction(g,bound_graph,1);   single_reaction(g,bound_graph,10);
	single_reaction(g,bound_graph,11);  single_reaction(g,bound_graph,14);
	single_reaction(g,bound_graph,15);  single_reaction(g,bound_graph,24);
	single_reaction(g,bound_graph,25);  single_reaction(g,bound_graph,28);
	single_reaction(g,bound_graph,29);  single_reaction(g,bound_graph,38);
	single_reaction(g,bound_graph,39);  single_reaction(g,bound_graph,42);
	single_reaction(g,bound_graph,43);  single_reaction(g,bound_graph,52);
	single_reaction(g,bound_graph,53);  single_reaction(g,bound_graph,56);
	single_reaction(g,bound_graph,57);  single_reaction(g,bound_graph,66);
	single_reaction(g,bound_graph,67);  single_reaction(g,bound_graph,70);
	single_reaction(g,bound_graph,71);  single_reaction(g,bound_graph,80);
	single_reaction(g,bound_graph,84);  single_reaction(g,bound_graph,93);
	single_reaction(g,bound_graph,94);  single_reaction(g,bound_graph,97);
	single_reaction(g,bound_graph,98);  single_reaction(g,bound_graph,107);
	single_reaction(g,bound_graph,108); single_reaction(g,bound_graph,111);
	single_reaction(g,bound_graph,112); single_reaction(g,bound_graph,121);
	single_reaction(g,bound_graph,122); single_reaction(g,bound_graph,125);
	single_reaction(g,bound_graph,126); single_reaction(g,bound_graph,135);
	single_reaction(g,bound_graph,136); single_reaction(g,bound_graph,139);
	single_reaction(g,bound_graph,140); single_reaction(g,bound_graph,149);
	single_reaction(g,bound_graph,150); single_reaction(g,bound_graph,153);
	single_reaction(g,bound_graph,154); single_reaction(g,bound_graph,163);
	set_edge_weights(g);	
	reset_vertex_index(g);  
	reset_edge_index(g);    
	write_gv(g,"5_g");

	//pair<Graph::edge_descriptor, bool> edgePair = edge(12, 13, g);
	single_reaction(g,bound_graph,8);
	set_edge_weights(g);	
	reset_edge_index(g);	
	write_gv(g,"6_g");	
	single_reaction(g,bound_graph,edge(3,4,g));
	set_edge_weights(g);	
	reset_edge_index(g);	
	write_gv(g,"7_g");	
	single_reaction(g,bound_graph,edge(3,4,g));
	set_edge_weights(g);	
	reset_edge_index(g);	
	write_gv(g,"8_g");	
	
/*
	set_edge_weights(g);	
	//cout << Rate(g,bound_graph,3) << endl;
	//cout << Rate(g,bound_graph,2) << endl;
	//cout << Rate(g,bound_graph,5) << endl;
	//cout << Rate(g,bound_graph,8) << endl;
	//cout << Rate(g,bound_graph,49) << endl;
	
	const double k[] = {0.001,0.01,1.2,1.};
	const int A0[] = {0,0};
	const int A0_size = sizeof(A0)/sizeof(*A0);
	const int k_size = sizeof(k)/sizeof(*k);
	vector< vector<double> > result = Gillespie(4,A0,A0_size,k,k_size,2); 
	//print_matrix(result);
	
	double free = G_duplex();
	//cout << free << endl;
*/	
	//set_edge_weights(g);	
	//reset_edge_index(g);	
	//Simulate(g,bound_graph,1203);
	
	//reset_edge_index(g);	
	//write_gv(g,"1_g");	

  	return 0;
}


/*
	//Check if edge exists.
	pair<Graph::edge_descriptor, bool> edgePair = edge(64, 68, g);
	bool haha = edgePair.second;
	cout << haha << endl;
*/


/*
	pair<int,int> P_arr [5];
	P_arr[0] = make_pair(1,2);
	P_arr[1] = make_pair(2,3);
   	cout << P_arr[0].first << endl;	
	pair< pair<int,int>, pair<int,int> > d_pairs [5];
	d_pairs[0] = make_pair(P_arr[0],P_arr[1]);
	cout << d_pairs[0].first.second << endl;
	
	vector< pair<int,int> > P_vec;
	P_vec.push_back(make_pair(1,2));
	P_vec.push_back(make_pair(2,3));
   	cout << P_vec[0].first << endl;	
	vector < pair< pair<int,int>, pair<int,int> > > d_pairs2;
	d_pairs2.push_back(make_pair(P_vec[0],P_vec[1]));
	cout << d_pairs2[0].first.second << endl;
*/











































