/*
 *
 *	Design.h
 * 	Author: Behnam
 *
 */

#ifndef DESIGN_H_
#define DESIGN_H_

#include "Headers.h"
#include "Common.h"

class Domain {
	public:
		int source; int target;
		vector<int> cross_domains; //no need for this
		vector<int> crossover_ids;
		vector<int> stack_domains;
		int length;
		string seq;
};
class Crossover {
	public:
		int v_a; int v_b;
		int d_a; int d_b;
		int d_a1; int d_a2; int d_b1; int d_b2; //no need for this
		char type;  //i: inside, o: outside, m: seam
};
class Design {
	public:
		int n_vertices;
		int n_domains;
		vector<Domain> domains;
		vector<Crossover> crossovers;
		
		void fill_crossover_ids();
		Design();
		void print_domains();
		void print_crossovers();

		vector<int> domain_to_crossovers(int);
		vector<int> vertex_to_crossovers(int);
		int vpair_to_crossover( pair<int,int> );
		int dpair_to_crossover( pair<int,int> );
		bool check_single_domain_staple(int);
		bool check_double_domain_staple(int);
		bool check_triple_domain_staple(int);

		string scaffold_sequence;
};


class Frits: public Design{
	public:
		Frits();
		void fill_domains();
		void fill_crossovers();
};

class Simple: public Design{
	public:
		Simple();
		void fill_domains();
		void fill_crossovers();
};



#endif

