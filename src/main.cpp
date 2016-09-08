#include "common.h"
#include "constants.h"
#include "design.h"
#include "graph.h"
//#include "bundled.h"
//#include "SSA.h"
//#include "rates.h"


int main() {
	Tile tile;
	tile.print_domains();
	MyGraph g(tile);

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











































