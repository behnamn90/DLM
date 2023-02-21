//
//  StatManager.hpp
//  DLM
//
//  Created by Behnam Najafi on 18/01/2018.
//  Copyright © 2018 Behnam Najafi. All rights reserved.
//

#ifndef StatManager_h
#define StatManager_h

#include "TransitionManager.h"

template <class T>
void fill_times(vector<T>& container,
                const int& idx,
                const double& tau){
    for (auto it = container.begin(); it!= container.end(); ++it){
        it->stats[idx].time[it->state] += tau;
    }
}

template <class obj>
void write_obj_stat_headers(ofstream& myfile,
                            const vector<obj>& container){
    myfile << "step" << "\t";
    myfile << "time" << "\t";
    myfile << "T" << "\t";
    for (auto& it : container){
        for (auto& state_name : it.possible_state_names){
            myfile << it.id << "_" << state_name << "\t";
        }
        for (int i = 0; i< Tr_MAX; i++){
            myfile << it.id << "_" << "n_tr" << i << "\t";
        }
    }
    myfile << "\n";
    myfile << std::flush;
}
template <class obj>
void write_obj_stat(ofstream& myfile,
                    const int& step,
                    const double& time,
                    const string& Temp,
                    const int& Ti,
                    vector<obj>& container){
    myfile << step << "\t";
    myfile << time << "\t";
    myfile << Temp << "\t";
    for (auto it= container.begin(); it!= container.end(); ++it){
        for (auto state = it->possible_states.begin(); state!= it->possible_states.end(); ++state){
            myfile << it->stats[Ti].time[*state] << "\t";
        }
        for (auto ntr = it->stats[Ti].n_tr.begin(); ntr!= it->stats[Ti].n_tr.end(); ntr++){
            myfile << *ntr << "\t";
        }
    }
    myfile << "\n";
    myfile << std::flush;
}



template <class Object>
void check_entrance(vector<Object>& container,
                    const int& Ti,
                    const double& time,
                    const double& dt,
                    const bool& iso,
                    int& num_done,
                    const bool& melt){
    for(auto& obj : container){
        double ratio;
        if (iso) {ratio = obj.stats[Ti].time[*(obj.possible_states.end()-1)] / time;}
        else if (melt) {ratio = obj.stats[Ti].time[obj.possible_states[0]] / dt;}
        else {ratio = obj.stats[Ti].time[*(obj.possible_states.end()-1)] / dt;}
        if (melt){
            if (obj.state == obj.possible_states[0] && obj.entrance.size() == 0)
                obj.entrance.push_back(time);
            if (ratio > 0.05 && obj.entrance.size() == 1)
                obj.entrance.push_back(time);
            if (ratio > 0.1 && obj.entrance.size() == 2)
                obj.entrance.push_back(time);
            if (ratio > 0.2 && obj.entrance.size() == 3)
                obj.entrance.push_back(time);
            if (ratio > 0.3 && obj.entrance.size() == 4)
                obj.entrance.push_back(time);
            if (ratio > 0.4 && obj.entrance.size() == 5)
                obj.entrance.push_back(time);
            if (ratio > 0.5 && obj.entrance.size() == 6){
                obj.entrance.push_back(time);
                num_done++;
            }
        }
        else{
            if (obj.state == *(obj.possible_states.end()-1) && obj.entrance.size() == 0)
                obj.entrance.push_back(time);
            if (ratio > 0.05 && obj.entrance.size() == 1)
                obj.entrance.push_back(time);
            if (ratio > 0.1 && obj.entrance.size() == 2)
                obj.entrance.push_back(time);
            if (ratio > 0.2 && obj.entrance.size() == 3)
                obj.entrance.push_back(time);
            if (ratio > 0.3 && obj.entrance.size() == 4)
                obj.entrance.push_back(time);
            if (ratio > 0.4 && obj.entrance.size() == 5)
                obj.entrance.push_back(time);
            if (ratio > 0.5 && obj.entrance.size() == 6){
                obj.entrance.push_back(time);
                num_done++;
            }
        }
    }
}


class StatManager{
public:
    StatManager(){}
    StatManager(TransitionManager*);
    virtual ~StatManager(){}
    
    FileIO* ofiles;
    
    Inputs* inputs;
    Design* design;
    MyGraph* G;
    TempRamp* ramp;
    TransitionManager* trManager;

    void setup();
    void update_times();
    void update_counts();
    
    void write_op_stats();
    void write_object_stats();

    string str;
    
    static bool headers_written;
    bool dummy;
    
    void write_all();

    void write_in_times();
    
    int target_num_done;
    int num_done = 0;
    bool all_in_times_done = false;
    

    
    void print();
    
    void test();
    
};




#endif /* StatManager_hpp */
