/*
 *
 *	Strand.h
 * 	Author: Behnam
 *
 */

#ifndef STRAND_H_
#define STRAND_H_

#include "Crossover.h"

class Strand {
public:
	Strand();
	virtual ~Strand(){}

	int length;
	
	string seq;  
	string reverse_comp; 
	
	void set_seq(const string&);
	void set_length();
	void set_reverse_comp();
	void print_seq();
};

#endif
