//
//  OPManager.h
//  DLM
//
//  Created by Behnam Najafi on 03/06/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#ifndef OPManager_h
#define OPManager_h


#include "StatManager.h"
//#include "OrderParameter.h"


template <class obj, class state_t>
void write_obj_hist(ofstream& myfile,
                    const int& Ti,
                    vector<obj>& container,
                    const vector<string>& poss_state_names,
                    const vector<state_t>& poss_states){
    myfile << "id" << "\t";
    for (auto state_name = poss_state_names.begin(); state_name!= poss_state_names.end(); ++state_name){
        myfile << "t_" << *state_name << "\t";
    }
    for (auto state_name = poss_state_names.begin(); state_name!= poss_state_names.end(); ++state_name){
        myfile << "c_" << *state_name << "\t";
    }
    for (int i = 0; i < Tr_MAX; i++){
        myfile << "n_tr" << i << "\t";
    }
    myfile << "\n";
    for (auto it= container.begin(); it!= container.end(); ++it){
        myfile << it->id << "\t";
        for (auto state = poss_states.begin(); state!= poss_states.end(); ++state){
            myfile << it->stats[Ti].time[*state] << "\t";
        }
        for (auto state = poss_states.begin(); state!= poss_states.end(); ++state){
            myfile << it->stats[Ti].count[*state] << "\t";
        }
        for (auto ntr = it->stats[Ti].n_tr.begin(); ntr!= it->stats[Ti].n_tr.end(); ntr++){
            myfile << *ntr << "\t";
        }
        myfile << "\n";
    }
    myfile << std::flush;
}

class OPManager{
public:
    OPManager(){}
    OPManager(StatManager*);
    virtual ~OPManager(){}
    
    Inputs* inputs;
    Design* design;
    MyGraph* G;
    TempRamp* ramp;
    TransitionManager* trManager;
    StatManager* statManager;
    FileIO* ofiles;

    vector<OrderParameter*> OPs;
    
    void initialise();
    void read_weights();
    void initialise_2D();
    
    void set_values();
    void update_times();
    
    std::set<int> all_vals;
    
    pair<bool,OrderParameter*> biased;
    
    vector<OrderParameter2D> OPs_2D;
    vector<ofstream> files_2D;
    vector<ofstream> burn_2D;
    
    void write();
    void write_last();
    void write_burn();

    void write_object_hist();
    
    void set_future_value(const TR& tr);
    void fill_rates_w();
    
    void generate_config();
    vector<int> done_configs;
    
    void print();
};


#endif
