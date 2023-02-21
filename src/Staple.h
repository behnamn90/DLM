/*
 *
 *	Staple.h
 * 	Author: Behnam
 *
 */

#ifndef STAPLE_H_
#define STAPLE_H_

#include "Strand.h"

class Staple: public Strand {
public:
	Staple();
	Staple(const string&);
	virtual ~Staple(){}

	int id;
    
    int pool_id;
    
    int num_domains;
    
    double concentration;
    
    pair<int,int> nucs;
    vector<int> helix_ids;
    vector<HX> helices;
	
    vector<PDOM> domains;
    vector<CR> crossovers;
    vector<int> domain_ids;
    vector<int> crossover_ids;
    
	bool is_seam;
	bool is_edge;
    
    char type; // 1domain, 2domain, or 3domain
    
    bool is_missing;
    
    pair<CR,bool> dp2cross(const PDOM, const PDOM);
    
    State_names state_names;
    State_t state;
    State_t prev_state;
    vector<State_t> possible_states;
    vector<string> possible_state_names;

    vector<OrderParameter*> OPs;
    
    void make_empty();
	void make_full();
    
    vector< Stat<State_t> > stats; //index is the index of temperate in TempRamp::unique_Ts
    
	void change_state(State_t);
    void initialise_state(State_t);
    
    void print();
    
    vector<double> entrance;
    
    vector<Transition*> transitions;
};

#endif
