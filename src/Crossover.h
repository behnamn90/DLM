/*
 *
 *	Crossover.h
 * 	Author: Behnam
 *
 */

#ifndef CROSSOVER_H_
#define CROSSOVER_H_

#include "Domain.h"

class Crossover {
public:
	Crossover();
	virtual ~Crossover(){}

    // position in StaplePool::vector<Crossover>
	int id;
    // position of StaplePool in Design::vector<StaplePool>
    int pool_id;

    // domains connected by this
    std::pair<int,int> domain_ids;
    std::pair<PDOM,PDOM> domains;

    // staple containing this
    int staple_id;
    ST staple;

    // nucleotides on the scaffold strand
    std::pair<int,int> nucs;
    // nodes in the graph
    std::pair<VX,VX> nodes;
    std::pair<int,int> node_ids; // in Design::vector<Node>
    // in nodes of graph representing the staple pool
    std::pair<int,int> node_idps;

    /* For crossovers that are paired in
     * double-crossovers
     * */
    bool is_crosspair;
    int crosspair_id;
    CRP crosspair;

    bool is_seam; // for convenience;
    bool is_long; // for convenience;
    char type;
    /*
     * i: inside the embedding
     * o: outside the embedding
     * l: long crossover of three domain staple
     * s: seam
     */


    bool state;
    bool prev_state;
    std::vector<bool> possible_states;
    std::vector<std::string> possible_state_names;

    std::vector<OrderParameter*> OPs;

    std::pair<int,int> helix_ids;
    std::pair<HX,HX> helices;
	
    double length;

    std::vector< Stat<bool> > stats; //index is the index of temperate in TempRamp::unique_Ts
    
    void bind();
    void unbind();



    std::pair<edge_t,bool> edge;
    std::pair<edge_t,bool> edge2; // for connected components
    std::pair<edge_t,bool> edge_p;

    std::vector<double> entrance;
};

class CrossPair{
public:
    CrossPair();
    virtual ~CrossPair(){}
    
    //Initialised when read
    int id;
    pair<int,int> crossover_ids;
    pair<CR,CR> crossovers;
    int pool_id;
    
    //Initialised when filled
    vector<bool> possible_states;
    vector<string> possible_state_names;
    
    
    //Initialised when split
    bool is_seam;
    
    bool prev_state;
    bool state;
    
    void update_state();
    
    vector< Stat<bool> > stats; //index is the index of temperate in TempRamp::unique_Ts
    
    vector<OrderParameter*> OPs;
    
    vector<double> entrance;
};



#endif
