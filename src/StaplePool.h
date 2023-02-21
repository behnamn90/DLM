/*
 *
 *    StaplePool.h
 *     Author: Behnam
 *
 */

#ifndef STAPLEPOOL_H_
#define STAPLEPOOL_H_

#include "Constants.h" //For dH,dS
#include "Input.h"
#include "Helix.h"
#include "Scaffold.h"

bool formsCrossover(PDOM a, PDOM b, State_t state);
bool neitherFormCrossover(PDOM a, PDOM b, State_t sA, State_t sB);

class StaplePool {
public:
    StaplePool();
    StaplePool(Inputs*, string&, int, Scaffold*);
    virtual ~StaplePool(){}
    
    Inputs* inputs;
    Scaffold* scaffold;
    int id;

    void read_string(string&);
    string name;
    int num_nucs;
    vector<Staple> staples;
    vector<PoolDomain> domains;
    vector<Crossover> crossovers;
    vector<CrossPair> crosspairs;
    vector<Helix> helices;
    unsigned long num_domains;
    unsigned long num_staples;
    unsigned long num_crossovers;
    unsigned long num_crosspairs;
    unsigned long num_helices;
    unsigned long num_objects;
    
    void add_links();
    void add_crosspairs();
    void split_helices();
    void add_stacks();
    void add_state_stacks();
    void add_possible_states();
    void add_sequences();
    void add_concentrations();
    void add_energies();
    
    void print_staples();
    void print_domains();
    void print_crossovers();
    void print_crosspairs();
    void print_helices();
    void print_OPs();
    void print();
    
    vector<OrderParameter> OPs;
    
    vector<bool> pdom_poss_states;
    vector<bool> cross_poss_states;
    vector<bool> crosspair_poss_states;
    vector<State_t> staple_poss_states;
    vector<char> helix_poss_states;
    
    vector<string> pdom_poss_state_names;
    vector<string> cross_poss_state_names;
    vector<string> crosspair_poss_state_names;
    vector<string> staple_poss_state_names;
    vector<string> helix_poss_state_names;


};




#endif

