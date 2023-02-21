//
//  TransitionManager.cpp
//  DLM
//
//  Created by Behnam Najafi on 23/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#include "TransitionManager.h"

TransitionManager::TransitionManager(){
}

TransitionManager::TransitionManager(Constants *constants_, MyGraph *G_, TempRamp *ramp_, FileIO* ofiles_) :
constants(constants_), G(G_), ramp(ramp_), ofiles(ofiles_){
    inputs = constants->inputs;
    design = G->design;
    initialise_transitions();
    initialise_reverse_transitions();
    add_transtion_pairs();
    add_tr_to_objects();
    //print_possibles();
    step = 0;
    if (inputs->rate_model == "local"){this->local = true;}
    else if (inputs->rate_model == "global"){this->local = false;}
    else {printf ("Error:\t Select local or global rate model!\n"); exit (EXIT_FAILURE);}
    custom_rates = false; mixed_rates = false; kawasaki_rates = false; metropolis_rates = false;
    if (inputs->rate_method == "custom"){this->custom_rates = true;}
    else if (inputs->rate_method == "mixed"){this->mixed_rates = true;}
    else if (inputs->rate_method == "kawasaki"){this->kawasaki_rates = true;}
    else if (inputs->rate_method == "metropolis"){this->metropolis_rates = true;}
    else {printf ("Error:\t Select custom, mixed, kawasaki or metropolis rate method!\n"); exit (EXIT_FAILURE);}
}

void TransitionManager::initialise_transitions(){
    TransitionMap M;
    TransitionProperty TP;
    int id = 0;
    for (auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
        for (auto st = pool->staples.begin(); st!=pool->staples.end(); ++st){
            for (auto i = st->possible_states.begin(); i!= st->possible_states.end(); ++i){
                for (auto j = st->possible_states.begin(); j!= st->possible_states.end(); ++j){
                    TP = M.FullMap[make_pair(State_t(*i),State_t(*j))];
                    if (TP.allowed){
                        transitions.push_back(Transition(design, st, State_t(*i),State_t(*j), TP, id));
                        id++;
                    }
                }
            }
        }
    }

    if(design->num_staple_pools > 1){
        TransitionProperty TP1, TP2;
        PDOM domain1, domain2;
        ST st1, st2;
        SDOM walkdom, mdom;
        int pool1, pool2;
        for (auto sdom = design->domains.begin(); sdom!= design->domains.end(); ++sdom){
            for (auto dummy1 = sdom->staples.begin(); dummy1!= sdom->staples.end(); ++dummy1){
                st1 = dummy1->second;
                pool1 = dummy1->first;
                if (pool1 == 0) {pool2 = 1;} else {pool2 = 0;}
                walkdom = sdom->bind_pairs[pool1];
                mdom = walkdom->bind_pairs[pool2];
                for (auto dummy2 = walkdom->staples.begin(); dummy2!= walkdom->staples.end(); ++dummy2){
                    if (dummy2->first != dummy1->first)
                        st2 = dummy2->second;
                }
                for (auto i = st1->possible_states.begin(); i!= st1->possible_states.end(); ++i){
                    for (auto j = st1->possible_states.begin(); j!= st1->possible_states.end(); ++j){
                        TP1 = M.FullMap[make_pair(State_t(*i),State_t(*j))];
                        if (TP1.allowed && TP1.bind){
                            domain1 = st1->domains[TP1.domain];
                            for (auto check1 = domain1->children_ids.begin(); check1 != domain1->children_ids.end(); ++check1){
                                if (*check1 == sdom->id){
                                    for (auto p = st2->possible_states.begin(); p!= st2->possible_states.end(); ++p){
                                        for (auto q = st2->possible_states.begin(); q!= st2->possible_states.end(); ++q){
                                            TP2 = M.FullMap[make_pair(State_t(*p),State_t(*q))];
                                            if (TP2.allowed && !TP2.bind){
                                                domain2 = st2->domains[TP2.domain];
                                                for (auto check2 = domain2->children_ids.begin(); check2 != domain2->children_ids.end(); ++check2){
                                                    if (*check2 == mdom->id){
                                                        transitions.push_back(Transition(design, sdom, st1, State_t(*i),State_t(*j), TP1, st2, State_t(*p),State_t(*q), TP2, id));
                                                        id++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // Note: done in a way such that staple always binds and affected_staple always unbinds.
    // So no duplicates exist.
}

void TransitionManager::initialise_reverse_transitions(){
    for (auto tr1 = transitions.begin(); tr1!=transitions.end(); ++tr1){
        for (auto tr2 = transitions.begin(); tr2!=transitions.end(); ++tr2){
            if (tr1->type == Transition_t(normal) && tr2->type == Transition_t(normal)){
                if (tr1->initial_state == tr2->final_state && tr1->final_state == tr2->initial_state){
                    if (tr1->pool == tr2->pool && tr1->staple->id == tr2->staple->id){
                        tr1->reverse_id = tr2->id;  tr1->reverse = tr2;
                        tr2->reverse_id = tr1->id;  tr2->reverse = tr1;
                    }
                }
            }
            if (tr1->type == Transition_t(invasion) && tr2->type == Transition_t(invasion)){
                if (tr1->toehold->id == tr2->mdom->id && tr1->mdom->id == tr2->toehold->id){
                    if (tr1->pool == tr2->other_pool && tr1->other_pool == tr2->pool){
                        if (tr1->initial_state == tr2->affected_final_state && tr1->final_state == tr2->affected_initial_state){
                            if (tr2->initial_state == tr1->affected_final_state && tr2->final_state == tr1->affected_initial_state){
                                tr1->reverse_id = tr2->id;  tr1->reverse = tr2;
                                tr2->reverse_id = tr1->id;  tr2->reverse = tr1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void TransitionManager::add_transtion_pairs(){
    for (auto tr = transitions.begin(); tr!=transitions.end(); ++tr){
        if(tr->forward){
            transition_pairs.push_back(make_pair(&(*tr), &(*(tr->reverse))));
        }
    }
}


void TransitionManager::reset_possibles(){
    for (auto it = transitions.begin(); it != transitions.end(); ++it){
        //if (it->staple->id == next->staple->id){
            it->decide_possible();
        //}
    }
}
void TransitionManager::print_possibles(){
    cout << "--------------------------------- " << step << endl;
    for (auto tr = transitions.begin(); tr != transitions.end(); ++tr){
        if (tr->possible){
            State_names names;
            cout << tr->id;
            cout << "\t --- \t";
            cout << "P: " << tr->pool << " --- ";
            cout << "ST: " << tr->staple->id << " --- ";
            cout << "DOM: " << tr->domain->id << " --- ";
            cout << "B?: " << tr->properties.bind << " --- ";
            cout << "F?: " << tr->forward << " --- ";
            cout << names.array(tr->initial_state) << " --> " << names.array(tr->final_state);
            cout << " --- " << tr->rate;
            cout << " --- " << tr->reverse_id;
            cout << endl;
            if (tr->type == invasion){
                cout << "\t\t --- \t";
                cout << "P: " << tr->other_pool << " --- ";
                cout << "ST: " << tr->affected_staple->id << " --- ";
                cout << "DOM: " << tr->affected_domain->id << " --- ";
                cout << "B?: " << tr->affected_properties.bind << " --- ";
                cout << names.array(tr->affected_initial_state) << " --> " << names.array(tr->affected_final_state);
                cout << " --- " << tr->rate;
                cout << endl;
            }
        }
    }
}

void TransitionManager::add_tr_to_objects(){
    for (auto& tr : this->transitions){
        for (auto& pool : design->staple_pools){
            for (auto& dom : pool.domains){
                if (tr.domain->id == dom.id &&
                    tr.pool == pool.id)
                    dom.transitions.push_back(&tr);
                if (tr.type == Transition_t(invasion) &&
                    tr.affected_domain->id == dom.id &&
                    tr.other_pool == pool.id)
                    dom.transitions.push_back(&tr);
            }
            for (auto& st : pool.staples){
                if (tr.staple->id == st.id &&
                    tr.pool == pool.id)
                    st.transitions.push_back(&tr);
                if (tr.type == Transition_t(invasion) &&
                    tr.affected_staple->id == st.id &&
                    tr.other_pool == pool.id)
                    st.transitions.push_back(&tr);
            }
        }
    }
}

void TransitionManager::reset_recalculates(){
    for (auto& tr : this->transitions){
        tr.recalculate = true;
    }
}

void TransitionManager::set_dG_duplex(TR& tr) {
    tr->dG_duplex = tr->domain->dH - ramp->get_T()*tr->domain->dS;
}
void TransitionManager::set_dG_stack(TR& tr) {
    if (tr->type == normal){
        for (auto stack = tr->domain->stack_domains.begin(); stack != tr->domain->stack_domains.end(); ++stack){
            if( (*stack)->state){
                tr->dG_stack += constants->n_parameter * (dH_average - ramp->get_T()*dS_average);
            }
        }
    }
    else if (tr->type == invasion){
        for (auto stack = tr->domain->stack_domains.begin(); stack != tr->domain->stack_domains.end(); ++stack){
            if( (*stack)->state){
                tr->dG_stack += constants->n_parameter * (dH_average - ramp->get_T()*dS_average);
            }
        }
        for (auto stack = tr->affected_domain->stack_domains.begin(); stack != tr->affected_domain->stack_domains.end(); ++stack){
            if( (*stack)->state){
                tr->dG_stack -= constants->n_parameter * (dH_average - ramp->get_T()*dS_average);
            }
        }
    }
    else {printf ("Error! TransitionManager::set_dG_stack(): transition type not recognised!\n"); exit (EXIT_FAILURE);}
}

void TransitionManager::set_dG_shape(TR& tr, bool based_on_affected) {
    if (this->local){
        double E;
        if (based_on_affected && tr->affected_crossover.second) {
            E = cs_hack + G->shortest_path(tr->affected_crossover.first);
            tr->dG_shape = -(gas_constant * ramp->get_T()) * constants->gamma_parameter * log(constants->C_parameter / E);
        }
        else if (!based_on_affected && tr->crossover.second) {
            E = cs_hack + G->shortest_path(tr->crossover.first);
            tr->dG_shape = -(gas_constant * ramp->get_T()) * constants->gamma_parameter * log(constants->C_parameter / E);
        }
        else{
            tr->dG_shape = 0;
            //std::cout << "TransitionManager::set_dG_shape (local) should not be called." << std::endl;
        }
    }
    else{
        // Have to run G->faces_weight() before to set numFaces.
        double facesWeight;
        facesWeight = G->faces_weight();
        double logsum0 = ( (G->numFaces + G->numLong) * log(constants->C_parameter) ) + facesWeight;
        tr->apply(G,true);
        facesWeight = G->faces_weight();
        double logsum1 = ( (G->numFaces + G->numLong) * log(constants->C_parameter) ) + facesWeight;
        tr->dG_shape = -(gas_constant * ramp->get_T() * constants->gamma_parameter ) *( logsum1 - logsum0 );
        tr->undo_apply(G);
    }
}

void TransitionManager::fill_rates_custom(){ //Custom
    double k_bi = inputs->k_bi;
    double k_uni = inputs->k_uni;
    double RT = gas_constant * ramp->get_T();
    double kf, kb;
    kf = inputs->k_f;
    double kr_in, kr_out, dG_in, dG_out;
    //if (design->staple_pools[inputs->target_pool].OPs[4].state > 0){
        //cout << step << "\t" << design->staple_pools[inputs->target_pool].OPs[3].state << "\t" << design->staple_pools[inputs->target_pool].OPs[4].state << endl;
    //}
    for (auto tr = transitions.begin(); tr!=transitions.end(); ++tr){
        //if (tr->check_seam_stack) {
            //cout << step << "\tshould check seam stack \t" << tr->staple->is_seam << "\t" << endl;
        //}
        
        if(tr->possible){
            if (tr->recalculate){
                tr->rate = 0;
                tr->dG = 0; tr->dG_duplex = 0; tr->dG_stack = 0; tr->dG_shape = 0;
                if (tr->type == normal){
                    if(tr->uni){
                        if (this->local){
                            if (tr->forward){ // 10->11
                                set_dG_shape(tr);
                                tr->dG = tr->dG_shape;
                                tr->rate = k_uni * exp( -tr->dG / RT );
                            }
                            else{ // 11->10
                                set_dG_duplex(tr); set_dG_stack(tr);
                                tr->dG = tr->dG_duplex+tr->dG_stack;
                                tr->rate = k_uni * exp( tr->dG / RT );
                            }
                        }
                        else{
                            if (tr->forward){
                                set_dG_shape(tr);
                                tr->dG = tr->dG_shape;
                                tr->rate = k_uni * exp( -tr->dG / RT );
                            }
                            else{
                                set_dG_duplex(tr); set_dG_stack(tr);
                                tr->dG = tr->dG_duplex+tr->dG_stack;
                                tr->rate = k_uni * exp( tr->dG / RT );
                            }
                        }
                    }
                    else{
                        if (tr->forward) { // 00->10
                            tr->rate = k_bi * tr->staple->concentration;
                            
                        }
                        else{ // 10->11
                            set_dG_duplex(tr); set_dG_stack(tr);
                            if (!this->local){ set_dG_shape(tr);}
                            tr->dG = tr->dG_duplex+tr->dG_stack-tr->dG_shape;
                            tr->rate = 1000. * k_bi * exp( tr->dG / RT );
                        }
                    }
                }
                else if (tr->type == invasion){
                    kb = 400. / (1.*(tr->b - tr->m)*(tr->b - tr->m));
                    dG_in = tr->toehold->dH - ramp->get_T()*tr->toehold->dS;
                    dG_out = tr->mdom->dH - ramp->get_T()*tr->mdom->dS;
                    kr_in = kf * (2./(tr->b-tr->m)) * exp(dG_in/RT);
                    kr_out = kf * (2./(tr->b-tr->m)) * exp(dG_out/RT);
                    tr->k_BM = (kf*kb*kr_in) / ((kr_in*kr_out)+(kr_in*kb)+(kr_out*kb));
                    if (tr->final_state == s11){
                        set_dG_shape(tr);
                        tr->dG = tr->dG_shape;
                        tr->rate = tr->k_BM * 1000. * exp( -tr->dG / RT );
                    }
                    else if (tr->affected_initial_state == s11){
                        set_dG_stack(tr);
                        tr->dG = tr->dG_stack;
                        tr->rate = tr->k_BM * tr->staple->concentration * exp( -tr->dG / RT );
                    }
                    else{
                        set_dG_stack(tr);
                        if(!this->local) set_dG_shape(tr);
                        tr->dG = tr->dG_stack + tr->dG_shape;
                        tr->rate = tr->k_BM * tr->staple->concentration * exp( -(0.5*tr->dG) / RT );
                    }
                }
                else {}
            }
        }
    }
}

void TransitionManager::fill_rates(){
    if (this->custom_rates) fill_rates_custom();
    else cout << "custom rate model only in this release." << endl;
    //else if (this->mixed_rates) fill_rates_mixed();
    //else if (this->kawasaki_rates) fill_rates_kawasaki();
    //else if (this->metropolis_rates) fill_rates_metropolis();
    //else{}
    total_rate = 0;
    for (auto tr = transitions.begin(); tr!=transitions.end(); ++tr){
        if(tr->possible){
            total_rate += tr->rate;
        }
    }
}

void TransitionManager::select_transition(Uni& uni){
    if (step>0) prev_rev_id = next->reverse->id;
	previous_tau = tau;
	r1 = uni(); r2 = uni();
	min = 0.;
	max = 0.;
	for (auto tr = transitions.begin(); tr!=transitions.end(); ++tr){
		if (tr->possible){
            if (inputs->umbrella_sampling){
                max += tr->rate_w/total_rate_w;
            }
			else {
                max += tr->rate/total_rate;
            }
			if (r2 >= min && r2 < max){
				next = tr;
				break;
			}
			min = max;
		}
	}
    if (inputs->umbrella_sampling){
        tau = (1./total_rate_w)*log(1./r1);
    }
    else{
        tau = (1./total_rate)*log(1./r1);
    }
    ramp->correct_overflow(tau);
    next->tau = this->tau;
}

void TransitionManager::apply_next() {
    if (!ramp->T_was_changed) {
        next->apply(G);
        G->fill_components();
    }
    step++;
}

void TransitionManager::append_trajectory(){
    if (step % inputs->write_simple_every == 0){
        ofiles->simple_traj_file << step << "\t";
        for (auto dom = design->domains.begin(); dom!= design->domains.end(); ++dom){
            if (dom->state == -1){
                ofiles->simple_traj_file << "-" << " ";
            }
            else {
                ofiles->simple_traj_file << dom->state << " ";
            }
        }
        ofiles->simple_traj_file << "\n";
        ofiles->simple_traj_file << std::flush;
    }
    if (step % inputs->write_state_every == 0){
        ofiles->trajectory_file << "time = " << ramp->current_t << "\n";
        ofiles->trajectory_file << "step = " << step << "\n";
        for (auto dom = design->domains.begin(); dom!= design->domains.end(); ++dom){
            ofiles->trajectory_file << dom->state;
            if(dom!= design->domains.end()-1) ofiles->trajectory_file << " ";
        }   ofiles->trajectory_file << "\n";
        for (auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
            ofiles->trajectory_file << "{ " << pool->name << "\n";
            for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                ofiles->trajectory_file << st->state;
                if(st!= pool->staples.end()-1) ofiles->trajectory_file << " ";
            }   ofiles->trajectory_file << "\n";
            for (auto dom = pool->domains.begin(); dom!= pool->domains.end(); ++dom){
                ofiles->trajectory_file << dom->state;
                if(dom!= pool->domains.end()-1) ofiles->trajectory_file << " ";
            }   ofiles->trajectory_file << "\n";
            for (auto cr = pool->crossovers.begin(); cr != pool->crossovers.end(); ++cr){
                ofiles->trajectory_file << cr->state;
                if(cr!= pool->crossovers.end()-1) ofiles->trajectory_file << " ";
            }   ofiles->trajectory_file << "\n";
            ofiles->trajectory_file << "}\n";
        }
        ofiles->trajectory_file << std::flush;
        retruncate(ofiles->last_conf_file,inputs->last_conf_file_name);
        ofiles->last_conf_file << "time = " << ramp->current_t << "\n";
        ofiles->last_conf_file << "step = " << step << "\n";
        for (auto dom = design->domains.begin(); dom!= design->domains.end(); ++dom){
            ofiles->last_conf_file << dom->state;
            if(dom!= design->domains.end()-1) ofiles->last_conf_file << " ";
        }   ofiles->last_conf_file << "\n";
        for (auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
            ofiles->last_conf_file << "{ " << pool->name << "\n";
            for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                ofiles->last_conf_file << st->state;
                if(st!= pool->staples.end()-1) ofiles->last_conf_file << " ";
            }   ofiles->last_conf_file << "\n";
            for (auto dom = pool->domains.begin(); dom!= pool->domains.end(); ++dom){
                ofiles->last_conf_file << dom->state;
                if(dom!= pool->domains.end()-1) ofiles->last_conf_file << " ";
            }   ofiles->last_conf_file << "\n";
            for (auto cr = pool->crossovers.begin(); cr != pool->crossovers.end(); ++cr){
                ofiles->last_conf_file << cr->state;
                if(cr!= pool->crossovers.end()-1) ofiles->last_conf_file << " ";
            }   ofiles->last_conf_file << "\n";
            ofiles->last_conf_file << "}\n";
        }
        ofiles->last_conf_file << std::flush;
    }
}
void TransitionManager::write_transition(){
    if (step % inputs->write_transition_every == 0){
        ofiles->transition_file << "# ";
        ofiles->transition_file << step << "\t";
        ofiles->transition_file << ramp->current_t << "\t";
        ofiles->transition_file << centigrade(ramp->get_T()) << "\t";
        ofiles->transition_file << total_rate << "\t";
        //transition_file << opManager->OPs["Num_bound_U"].current << "\n";
        ofiles->transition_file << "\n";
        next->write(ofiles->transition_file);
        ofiles->transition_file << std::flush;
    }
}

TR TransitionManager::manual_select_normal(int pool_id, int staple_id, int domain_id, State_t initial_state, State_t final_state){
    TR result;
    for (auto tr= transitions.begin(); tr!= transitions.end(); ++tr){
        if (tr->type == normal){
            if (tr->pool == pool_id){
                if (tr->staple->id == staple_id){
                    if (tr->domain->id == domain_id){
                        if (tr->initial_state == initial_state && tr->final_state == final_state){
                            result = tr;
                        }
                    }
                }
            }
        }
    }
    return result;
}
TR TransitionManager::manual_select_invasion(int pool1_id, int staple1_id, int domain1_id, State_t initial_state1, State_t final_state1,
                                            int pool2_id, int staple2_id, int domain2_id, State_t initial_state2, State_t final_state2){
    TR result;
    for (auto tr= transitions.begin(); tr!= transitions.end(); ++tr){
        if (tr->type == invasion){
            if (tr->pool == pool1_id && tr->other_pool == pool2_id){
                if (tr->staple->id == staple1_id && tr->affected_staple->id == staple2_id){
                    if (tr->domain->id == domain1_id && tr->affected_domain->id == domain2_id){
                        if (tr->initial_state == initial_state1 && tr->final_state == final_state1 &&
                            tr->affected_initial_state == initial_state2 && tr->affected_final_state == final_state2){
                            result = tr;
                        }
                    }
                }
            }
        }
    }
    return result;
}

