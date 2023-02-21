//
//  OrderParameter.cpp
//  DLM
//
//  Created by Behnam Najafi on 03/06/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#include "OPManager.h"


OPManager::OPManager(StatManager* statManager_): statManager(statManager_){
    inputs = statManager->inputs;
    design = statManager->design;
    G = statManager->G;
    ramp = statManager->ramp;
    trManager = statManager->trManager;
    ofiles = statManager->ofiles;

    this->initialise();
    this->read_weights();
    ofstream a;
    for (const auto& op : this->OPs_2D){
        files_2D.push_back(std::move(a));
        burn_2D.push_back(std::move(a));
    }
    
    set_values();
}
void OPManager::initialise(){
    for (auto op = design->OPs.begin(); op!= design->OPs.end(); ++op){
        this->OPs.push_back(&(*op));
    }
    for (auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
        for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
            this->OPs.push_back(&(*op));
        }
    }
    if (inputs->track_clusters){
        for (auto op = G->cg.OPs.begin(); op!= G->cg.OPs.end(); ++op){
            this->OPs.push_back(&(*op));

        }
        for (auto cg = G->p_cg.begin(); cg!= G->p_cg.end(); ++cg){
            for (auto op = cg->OPs.begin(); op!= cg->OPs.end(); ++op){
                this->OPs.push_back(&(*op));
            }
        }
    }
}
void OPManager::read_weights(){
    if (inputs->umbrella_sampling){
        ifstream myfile;
        myfile.open(inputs->w_file_name);
        std::string all;
        myfile.seekg(0, std::ios::end);
        all.reserve(myfile.tellg());
        myfile.seekg(0, std::ios::beg);
        all.assign((std::istreambuf_iterator<char>(myfile)),std::istreambuf_iterator<char>());
        vector<std::string> lines;
        boost::split(lines, all, boost::is_any_of("\n"));
        vector<string> first_line;
        boost::split(first_line, lines[0], boost::is_any_of("\t"));
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->name == first_line[0]){
                (*op)->biased = true;
                this->biased.first = true;
                this->biased.second = *op;
            }
        }
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->name!="BPs" && (*op)->name!="Domains" && (*op)->name!=biased.second->name){
                OPs_2D.push_back(OrderParameter2D(biased.second, *op));
            }
        }
        if (inputs->config_generator){
            int min, max;
            double weight = 1.;
            if (biased.second->type == OP_t(BOUND_DOMAINS)){
                min = 0;
                max = design->staple_pools[biased.second->pool_id].num_domains;
            }
            else if (biased.second->type == OP_t(BOUND_STAPLES)){
                min = 0;
                max = design->staple_pools[biased.second->pool_id].num_staples;
            }
            else if (biased.second->type == OP_t(BOUND_CROSSOVERS)){
                min = 0;
                max = design->staple_pools[biased.second->pool_id].num_crossovers;
            }
            else if (biased.second->type == OP_t(BOUND_HELICES)){
                min = 0;
                max = design->staple_pools[biased.second->pool_id].num_helices;
            }
            else{
                cout << "Error! OPManager::read_weights(): OP_t not recognised!" << endl;
                exit (EXIT_FAILURE);
            }
            for (int i = 0; i <= max; i++){
                biased.second->weight[i] = weight;
                weight = weight * 10.;
            }
        }
        else{
            vector<string> newline;
            for (auto line = lines.begin()+1; line!= lines.end(); ++line){
                boost::split(newline, *line, boost::is_any_of("\t"));
                if (newline.size() > 1) biased.second->weight[stoi(newline[0])] = stod(newline[1]);
            }
        }
        myfile.close();
    }
}

void OPManager::set_values(){
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        (*op)->explored_vals.insert((*op)->state);
        all_vals.insert((*op)->state);
    }
    for (auto op = this->OPs_2D.begin(); op!= this->OPs_2D.end(); ++op){
        op->set_new_value();
        op->explored_vals.insert(op->state);
    }
}
void OPManager::update_times(){
    for (auto op = this->OPs_2D.begin(); op!= this->OPs_2D.end(); ++op){
        op->update(trManager->next->tau);
    }
}


void OPManager::set_future_value(const TR& tr){
    biased.second->fut_state = biased.second->state;
    if (biased.second->type == OP_t(BOUND_BPS)){
        for (auto& op : tr->domain->OPs){
            if (op->biased){
                if (tr->properties.bind)
                    op->fut_state = op->state + tr->domain->length;
                else
                    op->fut_state = op->state - tr->domain->length;
            }
        }
    }
    if (biased.second->type == OP_t(BOUND_DOMAINS)){
        for (auto& op : tr->domain->OPs){
            if (op->id == biased.second->id){
                if (tr->properties.bind)
                    op->fut_state = op->state + 1;
                else
                    op->fut_state = op->state - 1;
            }
        }
    }
    if (biased.second->type == OP_t(BOUND_CROSSOVERS) &&
        tr->crossover.second){
        for (auto& op : tr->crossover.first->OPs){
            if (op->biased){
                if (tr->properties.bind)
                    op->fut_state = op->state + 1;
                else
                    op->fut_state = op->state - 1;
            }
        }
    }
    if (biased.second->type == OP_t(BOUND_STAPLES)){
        for (auto& op : tr->staple->OPs){
            if (op->biased){
                if (tr->properties.bind)
                    op->fut_state = op->state + 1;
                else
                    op->fut_state = op->state - 1;
            }
        }
    }
    if (tr->type == Transition_t(invasion)){
        if (biased.second->type == OP_t(BOUND_BPS)){
            for (auto op = tr->affected_domain->OPs.begin(); op!= tr->affected_domain->OPs.end(); ++op){
                if ((*op)->biased){
                    if (tr->affected_properties.bind)
                        (*op)->fut_state = (*op)->state + tr->affected_domain->length;
                    else
                        (*op)->fut_state = (*op)->state - tr->affected_domain->length;
                }
            }
        }
        if (biased.second->type == OP_t(BOUND_DOMAINS)){
            for (auto op = tr->affected_domain->OPs.begin(); op!= tr->affected_domain->OPs.end(); ++op){
                if ((*op)->biased){
                    if (tr->affected_properties.bind)
                        (*op)->fut_state = (*op)->state + 1;
                    else
                        (*op)->fut_state = (*op)->state - 1;
                }
            }
        }
        if (biased.second->type == OP_t(BOUND_CROSSOVERS) &&
            tr->affected_crossover.second){
            for (auto op = tr->affected_crossover.first->OPs.begin(); op!= tr->affected_crossover.first->OPs.end(); ++op){
                if ((*op)->biased){
                    if (tr->affected_properties.bind)
                        (*op)->fut_state = (*op)->state + 1;
                    else
                        (*op)->fut_state = (*op)->state - 1;
                }
            }
        }
        if (biased.second->type == OP_t(BOUND_STAPLES)){
            for (auto op = tr->affected_staple->OPs.begin(); op!= tr->affected_staple->OPs.end(); ++op){
                if ((*op)->biased){
                    if (tr->affected_properties.bind)
                        (*op)->fut_state = (*op)->state + 1;
                    else
                        (*op)->fut_state = (*op)->state - 1;
                }
            }
        }
    }
}
void OPManager::fill_rates_w(){
    trManager->total_rate_w = 0.;
    for (auto tr = trManager->transitions.begin(); tr!= trManager->transitions.end(); ++tr){
        tr->rate_w = 0.;
        if(tr->possible){
            if (tr->recalculate){
                this->set_future_value(tr);
                tr->rate_w = tr->rate * (biased.second->weight[biased.second->fut_state] / biased.second->weight[biased.second->state]);
                trManager->total_rate_w += tr->rate_w;
            }
        }
    }
}

void OPManager::write_last(){
    ofiles->hist_file << "Val\t";
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        ofiles->hist_file << (*op)->name << "_Count\t";
        ofiles->hist_file << (*op)->name << "_Time\t";
        ofiles->hist_file << (*op)->name << "_Weight\t";
    }
    ofiles->hist_file << "\n";
    int Ti = ramp->idx;
    if (Ti >= ramp->Temps.size()) Ti = ramp->Temps.size()-1;
    for (auto val= all_vals.begin(); val!= all_vals.end(); ++val){
        ofiles->hist_file << *val << "\t";
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->stats[Ti].count.find(*val)!= (*op)->stats[Ti].count.end())
                ofiles->hist_file << (*op)->stats[Ti].count[*val];
            else ofiles->hist_file << "0";
            ofiles->hist_file << "\t";
            if ((*op)->stats[Ti].time.find(*val)!= (*op)->stats[Ti].time.end()) ofiles->hist_file << (*op)->stats[Ti].time[*val];
            else ofiles->hist_file << "0";
            ofiles->hist_file << "\t";
            if ((*op)->weight.find(*val)!= (*op)->weight.end())
                ofiles->hist_file << (*op)->weight[*val];
            else ofiles->hist_file << "0";
            ofiles->hist_file << "\t";
        }
        ofiles->hist_file << "\n";
    }
    ofiles->hist_file << std::flush;
    int i = 0;
    for (auto& op : this->OPs_2D){
        string str = "Output/"; str+="2D-"; str+= op.name;
        files_2D[i].open(str, std::ofstream::out | std::ofstream::trunc);
        files_2D[i] << "OP1\t";
        files_2D[i] << "OP2\t";
        files_2D[i] << op.name << "_Count\t";
        files_2D[i] << op.name << "_Time\t";
        files_2D[i] << op.name << "_Weight\t";
        files_2D[i] << "\n";
        for (const auto& val : op.explored_vals){
            files_2D[i] << val.first << "\t";
            files_2D[i] << val.second << "\t";
            files_2D[i] << op.count[val];
            files_2D[i] << "\t";
            files_2D[i] << op.time[val];
            files_2D[i] << "\t";
            files_2D[i] << op.OP1->weight[val.first];
            files_2D[i] << "\n";
        }
        files_2D[i].close();
        i++;
    }
}
void OPManager::write_burn(){
    ofiles->burn_file << "Val\t";
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        ofiles->burn_file << (*op)->name << "_Count\t";
        ofiles->burn_file << (*op)->name << "_Time\t";
        ofiles->burn_file << (*op)->name << "_Weight\t";
    }
    ofiles->burn_file << "\n";
    int Ti = ramp->idx;
    if (Ti >= ramp->Temps.size()) Ti = ramp->Temps.size()-1;
    for (auto val= all_vals.begin(); val!= all_vals.end(); ++val){
        ofiles->burn_file << *val << "\t";
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->stats[Ti].count.find(*val)!= (*op)->stats[Ti].count.end())
                ofiles->burn_file << (*op)->stats[Ti].count[*val];
            else ofiles->burn_file << "0";
            ofiles->burn_file << "\t";
            if ((*op)->stats[Ti].time.find(*val)!= (*op)->stats[Ti].time.end()) ofiles->burn_file << (*op)->stats[Ti].time[*val];
            else ofiles->burn_file << "0";
            ofiles->burn_file << "\t";
            if ((*op)->weight.find(*val)!= (*op)->weight.end()) ofiles->burn_file << (*op)->weight[*val];
            else ofiles->burn_file << "0";
            ofiles->burn_file << "\t";
        }
        ofiles->burn_file << "\n";
    }
    ofiles->burn_file << std::flush;
    int i = 0;
    for (auto& op : this->OPs_2D){
        string str = "Output/"; str+="Burn2D-"; str+= op.name;
        burn_2D[i].open(str, std::ofstream::out | std::ofstream::trunc);
        burn_2D[i] << "OP1\t";
        burn_2D[i] << "OP2\t";
        burn_2D[i] << op.name << "_Count\t";
        burn_2D[i] << op.name << "_Time\t";
        burn_2D[i] << op.name << "_Weight\t";
        burn_2D[i] << "\n";
        for (const auto& val : op.explored_vals){
            burn_2D[i] << val.first << "\t";
            burn_2D[i] << val.second << "\t";
            burn_2D[i] << op.count[val];
            burn_2D[i] << "\t";
            burn_2D[i] << op.time[val];
            burn_2D[i] << "\t";
            burn_2D[i] << op.OP1->weight[val.first];
            burn_2D[i] << "\n";
        }
        burn_2D[i].close();
        i++;
    }
}

void OPManager::write_object_hist(){
    int Ti = ramp->prev_idx;
    write_obj_hist(ofiles->sdom_hist_file, Ti, design->domains, design->sdom_poss_state_names, design->sdom_poss_states);
    for (auto pool= design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
        write_obj_hist(ofiles->pdom_hist_files[pool->id], Ti, pool->domains, pool->pdom_poss_state_names, pool->pdom_poss_states);
        write_obj_hist(ofiles->cr_hist_files[pool->id], Ti, pool->crossovers, pool->cross_poss_state_names, pool->cross_poss_states);
        write_obj_hist(ofiles->crp_hist_files[pool->id], Ti, pool->crosspairs, pool->crosspair_poss_state_names, pool->cross_poss_states);
        write_obj_hist(ofiles->st_hist_files[pool->id], Ti, pool->staples, pool->staple_poss_state_names, pool->staple_poss_states);
        write_obj_hist(ofiles->hlx_hist_files[pool->id], Ti, pool->helices, pool->helix_poss_state_names, pool->helix_poss_states);
    }
}

void OPManager::write(){
    ofiles->traj_hist_file << "#\t" << trManager->step << "\t" << ramp->current_t << endl;
    ofiles->traj_hist_file << "Val\t";
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        ofiles->traj_hist_file << (*op)->name << "_Count\t";
        ofiles->traj_hist_file << (*op)->name << "_Time\t";
        ofiles->traj_hist_file << (*op)->name << "_Weight\t";
    }
    ofiles->traj_hist_file << "\n";
    int Ti = ramp->idx;
    for (auto val= all_vals.begin(); val!= all_vals.end(); ++val){
        ofiles->traj_hist_file << *val << "\t";
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->stats[Ti].count.find(*val)!= (*op)->stats[Ti].count.end())
                ofiles->traj_hist_file << (*op)->stats[Ti].count[*val];
            else ofiles->traj_hist_file << "0";
            ofiles->traj_hist_file << "\t";
            if ((*op)->stats[Ti].time.find(*val)!= (*op)->stats[Ti].time.end()) ofiles->traj_hist_file << (*op)->stats[Ti].time[*val];
            else ofiles->traj_hist_file << "0";
            ofiles->traj_hist_file << "\t";
            if ((*op)->weight.find(*val)!= (*op)->weight.end()) ofiles->traj_hist_file << (*op)->weight[*val];
            else ofiles->traj_hist_file << "0";
            ofiles->traj_hist_file << "\t";
        }
        ofiles->traj_hist_file << "\n";
    }
    ofiles->traj_hist_file << std::flush;
}

void OPManager::generate_config(){
    int min, max;
    string dummy;
    int value;
    if (biased.second->type == OP_t(BOUND_DOMAINS)){
        min = 0;
        max = design->staple_pools[biased.second->pool_id].num_domains;
    }
    else if (biased.second->type == OP_t(BOUND_STAPLES)){
        min = 0;
        max = design->staple_pools[biased.second->pool_id].num_staples;
    }
    else if (biased.second->type == OP_t(BOUND_CROSSOVERS)){
        min = 0;
        max = design->staple_pools[biased.second->pool_id].num_crossovers;
    }
    else if (biased.second->type == OP_t(BOUND_HELICES)){
        min = 0;
        max = design->staple_pools[biased.second->pool_id].num_helices;
    }
    else{
        cout << "Error! OPManager::generate_conf(): OP_t not recognised!" << endl;
        exit (EXIT_FAILURE);
    }
    string str;
    if (done_configs.size() > max){
        design->target_reached = true;
    }
    else{
        value = biased.second->state;
        if (std::find(done_configs.begin(), done_configs.end(),value)!=done_configs.end()){ //already 
        }
        else{
            str = "Output/"; str+= design->staple_pools[0].name;
            for (int i = 1; i<design->num_staple_pools; i++){
                str+= "_";
                str+= design->staple_pools[i].name;
            }
            str+= "_";
            str+= to_string(value);
            str+= "_";
            str+= to_string(dummy);
            str+= ".conf";
            ofstream config_file;
            config_file.open(str, std::ofstream::out | std::ofstream::trunc);
            config_file << "time = " << 0. << "\n";
            config_file << "step = " << 0 << "\n";
            for (auto dom = design->domains.begin(); dom!= design->domains.end(); ++dom){
                config_file << dom->state;
                if(dom!= design->domains.end()-1) config_file << " ";
            }   config_file << "\n";
            for (auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
                config_file << "{ " << pool->name << "\n";
                for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                    config_file << st->state;
                    if(st!= pool->staples.end()-1) config_file << " ";
                }   config_file << "\n";
                for (auto dom = pool->domains.begin(); dom!= pool->domains.end(); ++dom){
                    config_file << dom->state;
                    if(dom!= pool->domains.end()-1) config_file << " ";
                }   config_file << "\n";
                for (auto cr = pool->crossovers.begin(); cr != pool->crossovers.end(); ++cr){
                    config_file << cr->state;
                    if(cr!= pool->crossovers.end()-1) config_file << " ";
                }   config_file << "\n";
                config_file << "}\n";
            }
            config_file.close();
            done_configs.push_back(value);
            cout << value << "\n";
        }
    }
}
