/*
 *
 *	Design.h
 * 	Author: Behnam
 *
 */

#ifndef DESIGN_H_
#define DESIGN_H_

#include "StaplePool.h"



bool sort_domains(Domain,Domain);
bool sort_nodes(Node,Node);

class Design {
public:
	Design();
    Design(Inputs*);
	virtual ~Design(){}
	Inputs* inputs;
    
    Scaffold scaffold;
    
    void add_pools();
    vector<StaplePool> staple_pools;
    unsigned long num_staple_pools;
    unsigned long num_nucs;
    unsigned long num_staple;
    unsigned long num_domains;
    unsigned long num_crossovers;
    unsigned long num_crosspairs;
    void refill_pool_pointers();
    
    void add_nodes();
    vector<Node> nodes;
    
    void add_subdoms();
    vector<SubDomain> domains;
    void calc_subdom_energies();
    
    void add_parents();
    
    void add_pointers();
    
    void add_possible_states();

    void add_missing();
    
    void initialise_obj_states();

    void initialise_OP_states();
    
    void add_OPs_to_objects();
    
    void change_state(const ST&,
                      const State_t);

    void print_domains();
    void print_nodes();
    void print_OPs();
    void print();
        
    vector<OrderParameter> OPs; //index is the index of temperate in TempRamp::unique_Ts
    void add_OPs();
    void read_OPs();
    
    State_names state_names;
    bool target_reached;
    pair<int,double> target_reached_at;
    
    
    vector<int> sdom_poss_states;
    vector<bool> pdom_poss_states;
    vector<bool> cross_poss_states;
    vector<bool> crosspair_poss_states;
    vector<State_t> staple_poss_states;
    vector<char> helix_poss_states;
    
    vector<string> sdom_poss_state_names;
    vector<string> pdom_poss_state_names;
    vector<string> cross_poss_state_names;
    vector<string> crosspair_poss_state_names;
    vector<string> staple_poss_state_names;
    vector<string> helix_poss_state_names;

    std::map< pair<int,int>, pair<bool,int> > _vmap;

    const map<pair<int, int>, pair<bool, int>> &getVmap() const;

    void setVmap(const map<pair<int, int>, pair<bool, int>> &vmap);

};




#endif
