//
//  TransitionManager.hpp
//  DLM
//
//  Created by Behnam Najafi on 23/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#ifndef TransitionManager_h
#define TransitionManager_h

#include "TempRamp.h"
#include "Transition.h"
#include "TransitionMap.h"
#include "FileIO.h"


class TransitionManager {
public:
	TransitionManager();
	TransitionManager(Constants*, MyGraph*, TempRamp*, FileIO*);
	virtual ~TransitionManager(){}
	
    FileIO* ofiles;
	Constants* constants;
	Design* design;
	MyGraph* G;
	TempRamp* ramp;
	Inputs* inputs;
    
    bool local;

	vector<Transition> transitions;
    
    void add_transtion_pairs();
    vector<std::pair<Transition*,Transition*> > transition_pairs;
    
    void initialise_transitions();
    void initialise_reverse_transitions();
    
	void reset_possibles();
	void reset_recalculates();
	
	void set_dG_duplex(TR&);
	void set_dG_stack(TR&);
	
    void set_dG_shape(TR&, bool = false);
	
	double r1, r2, min, max;
	double total_rate, tau, previous_tau;
	double total_rate_w;
	
    int prev_rev_id = -1;
	TR next;
	
	int step;
	
	//void fill_transitions();
    void fill_rates();
    void fill_rates_custom();
    void fill_rates_mixed();
    void fill_rates_kawasaki();
    void fill_rates_metropolis();
    bool kawasaki_rates, metropolis_rates, custom_rates, mixed_rates;
    
    
	void select_transition(Uni&);
	
	void apply_next();
	
    //vector<int> pool_bound;
    
    void print_possibles();
    void add_tr_to_objects();
    
    void rewrite_last_conf();
    void append_trajectory();
    void write_transition();
    
    TR manual_select_normal(int, int, int, State_t, State_t);
    TR manual_select_invasion(int, int, int, State_t, State_t,
                              int, int, int, State_t, State_t);

};


#endif /* TransitionManager_hpp */
