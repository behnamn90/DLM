/*
 *
 *	Common.h
 * 	Author: Behnam
 *
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "../Headers/Headers.h"

template <class T> void print_vec(vector<T> &vec){
	for (int i=0; i<vec.size(); i++){
		cout << i << "\t" << vec[i] << "\n";
	}
}

//template <class T> void print_vec(vector<T> &);
void copy_vec(int*, const int*, int);
double sum_elements(vector<double>&);
int sum_elements(vector<int>&);

double kelvin(double);
double centigrade(double);

double seconds(double);
double minutes(double);

double Cpm2spC(double);

int circ_dist(int, int, int);

string reverse_comp(string);

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 3)
{ //rounded to n significant digits
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}
template <typename T>
std::string to_string_with_dec(const T a_value, const int n = 3)
{ //rounded to n decimal points
    std::ostringstream out;
    out << std::fixed << std::setprecision(n) << a_value;
    return out.str();
}

int circ_add(int,int,int);
int circ_substract(int,int,int);

bool sort_descending (int,int);
bool sort_ascending (int,int);




 
#endif

