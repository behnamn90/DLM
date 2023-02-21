/*
 *
 *    Helix.h
 *     Author: Behnam
 *
 */

#ifndef HELIX_H_
#define HELIX_H_


#include "Staple.h"

class Part{
public:
    Part();
    vector<int> domain_ids;
    vector<PDOM> domains;
    int length;
    int num_bound;
    bool bound;
    void update_state();
};

class Helix{
public:
    Helix();
    virtual ~Helix(){}

    //Initialised when read
    int id;
    vector<int> domain_ids;
    vector<PDOM> domains;
    int pool_id;
    StaplePool* pool;

    //Initialised when filled
    vector<char> possible_states;
    vector<string> possible_state_names;

    
    //Initialised when split
    bool is_seam;
    char type; // 'n' 's'
    vector<Part> parts;
    
    char prev_state;
    char state; //'N' 'L' 'R' 'F'
    
    void update_state();
 
    vector< Stat<char> > stats; //index is the index of temperate in TempRamp::unique_Ts
 
    vector<OrderParameter*> OPs;

    vector<double> entrance;
 };


#endif

