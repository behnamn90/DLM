/*
 *
 *	Scaffold.h
 * 	Author: Behnam
 *
 */

#ifndef SCAFFOLD_H_
#define SCAFFOLD_H_

#include "Strand.h"


class Scaffold: public Strand {
public:
	Scaffold();
	Scaffold(const string&);
	virtual ~Scaffold(){}

	void read_sequence_file(const string&);
};

#endif
