/*
 *
 *	EdgeProperty.h
 * 	Author: Behnam
 *
 */

#ifndef EDGEPROPERTY_H_
#define EDGEPROPERTY_H_

#include "../Headers/Headers.h"

struct EdgeProperty{
	int id;
	std::pair<std::vector<Domain>::iterator, bool> domain;
    std::pair<std::vector<SubDomain>::iterator, bool> domain_s;
    std::pair<std::vector<PoolDomain>::iterator, bool> domain_p;
    std::pair<std::vector<Crossover>::iterator, bool> crossover;
	double weight;

	//For drawing
    std::string colour; //for drawing.
    std::string thickness; //for drawing.
    //int length; //for drawing.
	//char type; 	
    /*
     * s: single-stranded
     * d: double-stranded
     * i: inside-crossover
     * o: outside-crossover
     * m: seam-crossover
     * l: long-crossover
     */
};

#endif
