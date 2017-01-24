#include <iostream>
#include <utility>                  
#include <algorithm>               
#include <fstream>
#include <vector>
#include <stdio.h>      /* printf, fopen */
#include <stdlib.h>     /* exit, EXIT_FAILURE */

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/ref.hpp>
#include <boost/graph/copy.hpp>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>

using namespace std;
using namespace boost;



template <class Graph> void print(Graph& g) {
	typename Graph::vertex_iterator i, end;
	typename Graph::out_edge_iterator ei, edge_end;
	for(boost::tie(i,end) = vertices(g); i != end; ++i) {
		cout << *i << " --> ";
		for (boost::tie(ei,edge_end) = out_edges(*i, g); ei != edge_end; ++ei)
		    cout << target(*ei, g) << "  ";
		cout << endl;
	}
};
struct face_counter : public planar_face_traversal_visitor {
	face_counter() : count(0) {}
	void begin_face() { ++count; }
	int count;
};
struct output_visitor : public planar_face_traversal_visitor {
	void begin_face() { cout << "New face: "; }
	void end_face() { cout << endl; }
};
struct vertex_output_visitor : public output_visitor {
	template <typename Vertex> 
	void next_vertex(Vertex v) 
	{ 
		cout << v << " "; 
	}
};
struct edge_output_visitor : public output_visitor {
	template <typename Edge> 
	void next_edge(Edge e) 
	{ 
		cout << e << " "; 
	}
};

template <class T> void print_vec(vector<T> &vec){
	for (int i=0; i<vec.size(); i++){
			cout << i << "\t" << vec[i] << "\n";
	}
}
void print_matrix(vector< vector<double> >& matrix){
	for (int i=0; i<matrix.size(); i++){
		for (int j=0; j<matrix[i].size(); j++){
			cout << matrix[i][j] << "\t";
		}
		cout << "\n";
	}
}
void write_matrix(vector< vector<double> >& matrix){
	ofstream myfile;
    myfile.open ("State.txt");
	for (int i=0; i<matrix.size(); i++){
		for (int j=0; j<matrix[0].size(); j++){
			myfile << matrix[i][j] << "\t";
		}
		myfile << "\n";
	}
	myfile.close();
}
void copy_vec(int *a, const int *b, int b_size){
     if(b_size == 0) return;    
     *a = *b;
     copy_vec(++a, ++b, b_size-1);
}

double sum_elements(vector<double>& v) {
	double sum_of_elems = 0.;
	for(vector<double>::iterator it = v.begin(); it != v.end(); ++it)
		sum_of_elems += *it;
	return sum_of_elems;
}

double kelvin(double C) {
	return C + 273.15;
}
double centigrade(double K) {
	return K - 273.15;
}

double seconds(double minutes) {
	return minutes * 60.;
}
double minutes(double seconds) {
	return seconds / 60.;
}
double Cpm2spC (double Cpm){
	return 1. / (Cpm / 60.);
}








/*
struct face_counter : public planar_face_traversal_visitor {
	face_counter() : count(0) {}
	void begin_face() { ++count; }
	int count;
};
struct output_visitor : public planar_face_traversal_visitor {
	void begin_face() { cout << "New face: "; }
	void end_face() { cout << endl; }
};
struct vertex_output_visitor : public output_visitor {
	template <typename Vertex> 
	void next_vertex(Vertex v) 
	{ 
		cout << v << " "; 
	}
};
struct edge_output_visitor : public output_visitor {
	template <typename Edge> 
	void next_edge(Edge e) 
	{ 
		cout << e << " "; 
	}
};
*/
