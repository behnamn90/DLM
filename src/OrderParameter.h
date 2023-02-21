//
//  OrderParameter.hpp
//  DLM
//
//  Created by Behnam Najafi on 03/06/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#ifndef OrderParameter_h
#define OrderParameter_h

#include "Common.h"

class OrderParameter{
//private:
public:
    OrderParameter(){id = -1;}
    OrderParameter(int, OP_t, string);
    static int counter;
    int id;
    
    int pool_id;
    OP_t type;
    
    string name;
    
    int prev_prev_state;
    int prev_state;
    int state;
    
    bool biased;
    int fut_state;
    std::map<int,double> weight;
    
    
    std::set<int> group_ids;
    
    std::set<int> explored_vals;
    
    /*
     Temperature Dependent members:
     index of vector is current_index
     in TempRamp
     */
    vector<Stat<int>> stats;
};

class OrderParameter2D{
public:
    OrderParameter2D(){}
    OrderParameter2D(OrderParameter*, OrderParameter*);
    virtual ~OrderParameter2D(){}

    OrderParameter* OP1;
    OrderParameter* OP2;

    string name;

    pair<int,int> prev_state;
    pair<int,int> state;
    pair<int,int> fut_state;

    std::set<pair<int,int> > explored_vals;
    std::map<pair<int,int>, int> count;
    std::map<pair<int,int>, double> time;
    std::map<pair<int,int>, double> weight;

    void set_new_value();
    void update(const double);

    void print();

    void write();
};


 
#endif /* OrderParameter_h */
