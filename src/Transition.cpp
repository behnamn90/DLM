//
//  Transition.cpp
//  DLM
//
//  Created by Behnam Najafi on 23/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#include "Transition.h"

//Transition
Transition::Transition(){}

Transition::Transition(Design* design_, ST staple_, State_t init, State_t fin, TP props, int id_, Transition_t type_) :
design(design_), staple(staple_), initial_state(init), final_state(fin), properties(props), id(id_), type(type_) {
    domain = staple->domains[properties.domain];
    crossover.second = false;
    pool = staple->pool_id;
    sdoms = domain->children;
    if (properties.cross){
        crossover = staple->dp2cross(domain, staple->domains[properties.cross_dom]);
    }
    other_pool = -1;
    decide_possible();
    recalculate = true;
    rate = 0;
    rate_w = 0;
    rate_already_scaled = false;
    if (properties.bind) {forward = true;}
    else {forward = false;}
    if (crossover.second) {uni = true;}
    else {uni = false;}
    
    block_unblock = false;
    if (initial_state == s12 || final_state == s12) block_unblock = true;
        /*
        ||
        initial_state == s012 || final_state == s012 ||
        initial_state == s120 || final_state == s120 ||
        initial_state == s102 || final_state == s102 ||
        initial_state == s112 || final_state == s112 ||
        initial_state == s211 || final_state == s211 ||
        initial_state == s121 || final_state == s121 ||
        initial_state == s123 || final_state == s123) */
    check_seam_stack = false;
    if (staple->is_seam == true && block_unblock == true) check_seam_stack = true;
}

Transition::Transition(Design* design_, SDOM toehold_, ST staple_, State_t init, State_t fin, TP props, ST staple2_, State_t init2, State_t fin2, TP props2, int id_, Transition_t type_) :
design(design_), toehold(toehold_),
staple(staple_), initial_state(init), final_state(fin), properties(props),
affected_staple(staple2_), affected_initial_state(init2), affected_final_state(fin2), affected_properties(props2),
id(id_), type(type_) {
    domain = staple->domains[properties.domain];
    affected_domain = affected_staple->domains[affected_properties.domain];
    crossover.second = false;
    pool = staple->pool_id;
    sdoms = domain->children;
    if (properties.cross){
        crossover = staple->dp2cross(domain, staple->domains[properties.cross_dom]);
    }
    if (affected_properties.cross){
        affected_crossover = affected_staple->dp2cross(affected_domain, affected_staple->domains[affected_properties.cross_dom]);
    }
    if (pool == 0) {other_pool = 1; forward = true;}
    if (pool == 1) {other_pool = 0; forward = false;}
    walkdom = toehold->bind_pairs[pool];
    mdom = walkdom->bind_pairs[other_pool];
    n = toehold->length;
    m = mdom->length;
    //b = affected_domain->length;
    b = walkdom->length + m;
    decide_possible();
    recalculate = true;
    rate = 0;
    rate_w = 0;
    rate_already_scaled = false;
    
    
    
    block_unblock = false;
    if (initial_state == s12 || final_state == s12) block_unblock = true;
    check_seam_stack = false;
    if (staple->is_seam == true && block_unblock == true) check_seam_stack = true;
    
    block_unblock_affected = false;
    if (affected_initial_state == s12 || affected_final_state == s12) block_unblock_affected  = true;
    check_affected_seam_stack = false;
    if (affected_staple->is_seam == true && block_unblock_affected == true) check_affected_seam_stack = true;
    
}

void Transition::decide_possible(){
    possible = false;
    if(design->num_staple_pools == 1){
        if (staple->state == initial_state){
            possible = true;
        }
    }
    else{
        if (type == normal){
            if (staple->state == initial_state){
                possible = true;
                if (properties.bind){
                    for (auto sdom = sdoms.begin(); sdom!= sdoms.end(); ++sdom){
                        if ((*sdom)->state != -1) possible = false;
                    }
                }
            }
        }
        else if (type == invasion){
            if (staple->state == initial_state && affected_staple->state == affected_initial_state){
                if (toehold->state == -1 && walkdom->state == other_pool)
                    possible = true;
            }
        }
        else{ cout << "Error: Transition type not known.\n";}
    }
    if (staple->is_missing) possible = false;
    if (type == invasion && affected_staple->is_missing) possible = false;
}

void Transition::apply(MyGraph* G, bool pseudo){
    if(type == normal){
        if (!pseudo) design->change_state(staple,final_state);
        if (properties.bind){
            G->bind_domain(domain);
            if(crossover.second){
                G->add_crossover(crossover.first);
            }
        }
        else{
            G->unbind_domain(domain);
            if(crossover.second){
                G->remove_crossover(crossover.first);
            }
        }
    }
    else if (type == invasion){
        if (properties.bind && !affected_properties.bind){
            if (!pseudo) design->change_state(affected_staple,affected_final_state);
            G->unbind_domain(affected_domain);
            if(affected_crossover.second){
                G->remove_crossover(affected_crossover.first);
            }
            G->bind_domain(domain);
            if(crossover.second){
                G->add_crossover(crossover.first);
            }
            if (!pseudo) design->change_state(staple,final_state);
        }
        else if (!properties.bind && affected_properties.bind){
            if (!pseudo) design->change_state(staple,final_state);
            G->unbind_domain(domain);
            if(crossover.second){
                G->remove_crossover(crossover.first);
            }
            G->bind_domain(affected_domain);
            if(affected_crossover.second){
                G->add_crossover(affected_crossover.first);
            }
            if (!pseudo) design->change_state(affected_staple,affected_final_state);
        }
        else{
            cout << "Error in Transition::apply: invasion reaction is neither bind-unbind or unbind-bind!";
        }
    }
    else{}
}
void Transition::undo_apply(MyGraph* G){
    if(type == normal){
        if (properties.bind){
            G->unbind_domain(domain);
            if(crossover.second){
                G->remove_crossover(crossover.first);
            }
        }
        else{
            G->bind_domain(domain);
            if(crossover.second){
                G->add_crossover(crossover.first);
            }
        }
    }
    else if (type == invasion){
        if (properties.bind && !affected_properties.bind){
            G->unbind_domain(domain);
            if(crossover.second){
                G->remove_crossover(crossover.first);
            }
            G->bind_domain(affected_domain);
            if(affected_crossover.second){
                G->add_crossover(affected_crossover.first);
            }
        }
        else if (!properties.bind && affected_properties.bind){
            G->unbind_domain(affected_domain);
            if(affected_crossover.second){
                G->remove_crossover(affected_crossover.first);
            }
            G->bind_domain(domain);
            if(crossover.second){
                G->add_crossover(crossover.first);
            }
        }
        else{
            cout << "Error in Transition::apply: invasion reaction is neither bind-unbind or unbind-bind!";
        }
    }
    else{}
}


void Transition::write(ofstream& transition_file){
    transition_file << "\t";
    transition_file << design->staple_pools[pool].name << "\t";
    transition_file << staple->id << "\t";
    transition_file << domain->id << "(";
    for (auto dom = sdoms.begin(); dom!= sdoms.end(); ++dom){
        transition_file << (*dom)->id;
        if (dom!= sdoms.end()-1) transition_file << ",";
    }  transition_file << ")\t";
	transition_file << "[";
	transition_file << state_names.array(initial_state);
	transition_file << "->";
	transition_file << state_names.array(final_state);
    transition_file << "]\t";
    transition_file << dG_duplex << "\t";
    transition_file << dG_stack << "\t";
    transition_file << dG_shape << "\t";
    transition_file << dG << "\t";
    transition_file << rate << "\n";
    
    if (this->type == invasion){
        transition_file << "\t";
        transition_file << design->staple_pools[other_pool].name << "\t";
        transition_file << affected_staple->id << "\t";
        transition_file << affected_domain->id << "(";
        transition_file << affected_domain->children_ids[0] << "," << affected_domain->children_ids[1] << ")\t";
        transition_file << "[";
        transition_file << state_names.array(affected_initial_state);
        transition_file << "->";
        transition_file << state_names.array(affected_final_state);
        transition_file << "]\n";
    }
}

void Transition::print(){
    cout << "\t";
    cout << design->staple_pools[pool].name << "\t";
    cout << "st " << staple->id << "\t";
    cout << "dom " << domain->id << "\t";
    cout << "len " << domain->length << "\t";
    cout << "[";
    cout << state_names.array(initial_state);
    cout << "->";
    cout << state_names.array(final_state);
    cout << "]";
    cout << "\n";

    if (this->type == invasion){
        cout << "\t\t";
        cout << design->staple_pools[other_pool].name << "\t";
        cout << "st " << affected_staple->id << "\t";
        cout << "dom " << affected_domain->id << "\t";
        cout << "dom " << affected_domain->length << "\t";
        cout << "[";
        cout << state_names.array(affected_initial_state);
        cout << "->";
        cout << state_names.array(affected_final_state);
        cout << "]\t";
        cout << "toe " << toehold->id << "\t";
        cout << "walk " << walkdom->id << "\t";
        cout << "m " << mdom->id << "\t";
        cout << "k_BM " << k_BM << "\t";
        cout << "\n";
    }
    cout << "\t\t\t";
    cout << "duplex " << dG_duplex << "\t";
    cout << "stack " << dG_stack << "\t";
    cout << "shape " << dG_shape << "\t";
    cout << "dG " << dG << "\t";
    cout << "rate " << rate << "\n";
}

ostream &operator<<(ostream &os, const Transition &transition) {
    State_names names;
    os << "st: " << transition.staple->id << " (";
    os << names.array(transition.initial_state) << "->";
    os << names.array(transition.final_state) << ")";
    return os;
}

