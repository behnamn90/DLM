//
//  Transition.hpp
//  DLM
//
//  Created by Behnam Najafi on 23/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#ifndef Transition_h
#define Transition_h

#include <ostream>
#include "MyGraph.h"
#include "TransitionProperty.h"


class Transition {
public:
	Transition();
    Transition(Design*, ST, State_t, State_t, TP, int, Transition_t = normal);
    Transition(Design*, SDOM,
               ST, State_t, State_t, TP,
               ST, State_t, State_t, TP,
               int, Transition_t = invasion);
	virtual ~Transition(){}
	
    Design* design;
    
    bool rate_already_scaled; //for scaling rates to avoid double scaling.
    
    int id;
    int reverse_id;
    Transition_t type;
    bool forward;
    vector<Transition>::iterator reverse;
    
    bool uni;
    
    vector<SDOM> sdoms;
    SDOM toehold;
    SDOM walkdom;
    SDOM mdom;
    
    State_names state_names;
    
    int toe;
    int n, m, b;
    //double k_ex;
    
    ST staple;
    State_t initial_state;
    State_t final_state;
    pair<CR, bool> crossover;
    TP properties;
    int pool;
    PDOM domain; //In pool
    
    ST affected_staple;
    State_t affected_initial_state;
    State_t affected_final_state;
    pair<CR, bool> affected_crossover;
    TP affected_properties;
    int other_pool;
    PDOM affected_domain;

	bool possible;
	bool recalculate;
    
    bool recalc_duplex;
    bool recalc_stack;
    bool recalc_shape;
	
    double dG = 0;
	double dG_duplex = 0;
	double dG_stack = 0;
	double dG_shape = 0;
	double rate = 0;
	double rate_w = 0;
    double tau = 0; //duplicate of trManager tau.
    double k_BM = 0;
    
	void decide_possible();
	void apply(MyGraph*, bool pseudo = false);

    void undo_apply(MyGraph*);

	void write(ofstream&);
    void print();
    
    bool block_unblock;
    bool block_unblock_affected;
    
    bool check_seam_stack;
    bool check_affected_seam_stack;

    friend ostream &operator<<(ostream &os, const Transition &transition);

};



#endif /* Transition_hpp */
