/*
 *
 *	Common.h
 * 	Author: Behnam
 *
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "Headers.h"

template <class T> void print_vec(vector<T> &vec){
	for (int i=0; i<vec.size(); i++){
			cout << i << "\t" << vec[i] << "\n";
	}
}
void print_matrix(vector< vector<double> >&);
void write_matrix(vector< vector<double> >&);
void copy_vec(int*, const int*, int);

double sum_elements(vector<double>&);

double kelvin(double);
double centigrade(double);

double seconds(double);
double minutes(double);
double Cpm2spC(double);

#endif




/*
template <class T> void print_vec(vector<T> &vec);
void print_matrix(vector< vector<double> >& matrix);
void write_matrix(vector< vector<double> >& matrix);
void copy_vec(int *a, const int *b, int b_size);

double sum_elements(vector<double>& v);

double kelvin(double C);
double centigrade(double K);

double seconds(double minutes);
double minutes(double seconds);
double Cpm2spC (double Cpm);
*/






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
