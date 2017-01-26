/*
 *
 *	Common.cpp
 * 	Author: Behnam
 *
 */

#include "Common.h"

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
	return (seconds / 60.);
}

double Cpm2spC(double Cpm){
	return 1. / (Cpm / 60.);
}
