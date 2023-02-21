/*
 *
 *	Domain.h
 * 	Author: Behnam
 *
 */

#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "Node.h"

class Domain {
public:
	Domain();
	virtual ~Domain(){}
		
	int id;
    
    int length;
    double dH;
    double dS;
        
	pair<int,int> nucs;
    
    vector<OrderParameter*> OPs;
    
    pair<VX,VX> nodes;
    pair<int,int> node_idps;    // The nodes' ids in the pool
	pair<int,int> node_ids;     // The nodes' ids in design (actual id)
        
    int helix_id;
    HX helix;

    bool is_seam;
    bool is_edge;
    bool is_body;

    string seq;
};


class SubDomain: public Domain {
public:
    SubDomain();
    virtual ~SubDomain(){}
    
    std::map<int,int> parent_ids; //(pool_id, id in pool)
    std::map<int,vector<PoolDomain>::iterator> parents; //(pool_id, parent)
     
    map<int,int> bind_pair_ids; //(pool_id, id in mixed)
    map<int,vector<SubDomain>::iterator> bind_pairs; //(pool_id, DOM in mixed)
    
    map<int,int> staple_ids;
    map<int,vector<Staple>::iterator> staples;
    
    int state;
    int prev_state;
    vector<int> possible_states;
    vector<string> possible_state_names;

    
    vector< Stat<int> > stats; //index is the index of temperate in TempRamp::unique_Ts
    
    edge_t edge;

    pair<edge_t,bool> edge2;
    //edge_t pseudo_edge;
    double ss_weight;
    double ds_weight;
};

class PoolDomain: public Domain {
public:
    PoolDomain();
    virtual ~PoolDomain(){}
    
    bool is_middle;
    int s_index;
    
    int pool_id;
    string pool_name;
    
    int staple_id;
    vector<Staple>::iterator staple;
    
    vector<int> cross_dom_ids;
    vector<vector<PoolDomain>::iterator> cross_domains;
    
    vector<int> crossover_ids;
    vector<vector<Crossover>::iterator> crossovers;
    
    vector<int> stack_ids;
    vector<vector<PoolDomain>::iterator> stack_domains;

    //std::map<State_t, std::vector<int>> stackIdxs;
    //std::map<State_t, std::vector<PDOM>> stackDomains;
    
    vector<int> children_ids;
    vector<SDOM> children;
    
    bool state;
    bool prev_state;
    vector<bool> possible_states;
    vector<string> possible_state_names;
    
    void bind();
    void unbind();
    
    pair<edge_t,bool> edge_p;
    
    vector< Stat<bool> > stats; //index is the index of temperate in TempRamp::unique_Ts
    
    vector<double> entrance;
    vector<Transition*> transitions;
};




#endif
