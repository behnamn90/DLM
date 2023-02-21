//
//  StatManager.cpp
//  DLM
//
//  Created by Behnam Najafi on 18/01/2018.
//  Copyright © 2018 Behnam Najafi. All rights reserved.
//

#include "StatManager.h"


StatManager::StatManager(TransitionManager* trManager_): trManager(trManager_) {
    inputs = trManager->inputs;
    design = trManager->design;
    G = trManager->G;
    ramp = trManager->ramp;
    ofiles = trManager->ofiles;
    this->setup();
    
    this->target_num_done = design->staple_pools[inputs->target_pool].num_objects;
    
    //this->initialise();
}
bool StatManager::headers_written = false;

void StatManager::setup(){
    for (int i = 0; i< ramp->Temps.size(); ++i){
        for (auto op = design->OPs.begin(); op!= design->OPs.end(); ++op){
            Stat<int> opstat;
            op->stats.push_back(opstat);
        }
        for (auto sdom = design->domains.begin(); sdom!= design->domains.end(); ++sdom){
            Stat<int> sdomstat = Stat<int>();
            sdom->stats.push_back(sdomstat);
        }
        for(auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
            for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
                Stat<int> opstat;
                op->stats.push_back(opstat);
            }
            for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
                Stat<bool> pdomstat = Stat<bool>();
                pdom->stats.push_back(pdomstat);
            }
            for (auto cross = pool->crossovers.begin(); cross!= pool->crossovers.end(); ++cross){
                Stat<bool> crossstat = Stat<bool>();
                cross->stats.push_back(crossstat);
            }
            for (auto cpair = pool->crosspairs.begin(); cpair!= pool->crosspairs.end(); ++cpair){
                Stat<bool> crossstat = Stat<bool>();
                cpair->stats.push_back(crossstat);
            }
            for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                Stat<State_t> staplestat = Stat<State_t>();
                st->stats.push_back(staplestat);
            }
            for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
                Stat<char> helixstat = Stat<char>();
                helix->stats.push_back(helixstat);
            }
        }
        if (inputs->track_clusters){
            for (auto op = G->cg.OPs.begin(); op!= G->cg.OPs.end(); ++op){
                Stat<int> opstat;
                op->stats.push_back(opstat);
            }
            for (auto cg = G->p_cg.begin(); cg!= G->p_cg.end(); ++cg){
                for (auto op = cg->OPs.begin(); op!= cg->OPs.end(); ++op){
                    Stat<int> opstat;
                    op->stats.push_back(opstat);
                }
            }
        }
    }
}

void StatManager::update_counts(){
    int T = ramp->prev_idx; //State has not changed yet.
    ramp->Temps[T].num_tr[trManager->next->type]++;
    
    for (auto op = design->OPs.begin(); op!= design->OPs.end(); ++op){
        op->stats[T].update(op->state);
        op->stats[T].n_tr[trManager->next->type]++;
        op->stats[T].count[op->state]++;
    }
    auto pool = design->staple_pools.begin()+trManager->next->pool;
    for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
        op->stats[T].update(op->state);
        op->stats[T].n_tr[trManager->next->type]++;
        op->stats[T].count[op->state]++;
    }
    if (trManager->next->type == invasion){
        pool = design->staple_pools.begin()+trManager->next->other_pool;
        for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
            op->stats[T].update(op->state);
            op->stats[T].n_tr[trManager->next->type]++;
            op->stats[T].count[op->state]++;
        }
    }
    if (inputs->track_clusters){
        int pool_id;
        for (auto op = G->cg.OPs.begin(); op!= G->cg.OPs.end(); ++op){
            op->stats[T].update(op->state);
            op->stats[T].n_tr[trManager->next->type]++;
            op->stats[T].count[op->state]++;
        }
        if (design->num_staple_pools > 1){
        pool_id = trManager->next->pool;
            for (auto op = G->p_cg[pool_id].OPs.begin(); op!= G->p_cg[pool_id].OPs.end(); ++op){
                op->stats[T].update(op->state);
                op->stats[T].n_tr[trManager->next->type]++;
                op->stats[T].count[op->state]++;
            }
            if (trManager->next->type == invasion){
                pool_id = trManager->next->other_pool;
                for (auto op = G->p_cg[pool_id].OPs.begin(); op!= G->p_cg[pool_id].OPs.end(); ++op){
                    op->stats[T].update(op->state);
                    op->stats[T].n_tr[trManager->next->type]++;
                    op->stats[T].count[op->state]++;
                }
            }
        }
    }
    for (auto sdom = trManager->next->sdoms.begin(); sdom!= trManager->next->sdoms.end(); ++sdom){
        (*sdom)->stats[T].count[(*sdom)->state]++;
        (*sdom)->stats[T].n_tr[trManager->next->type]++;
    }
    trManager->next->staple->stats[T].count[trManager->next->staple->state]++;
    trManager->next->staple->stats[T].n_tr[trManager->next->type]++;
    trManager->next->domain->stats[T].count[trManager->next->domain->state]++;
    trManager->next->domain->stats[T].n_tr[trManager->next->type]++;
    trManager->next->domain->helix->stats[T].count[trManager->next->domain->helix->state]++;
    trManager->next->domain->helix->stats[T].n_tr[trManager->next->type]++;
    if(trManager->next->crossover.second == true){
        trManager->next->crossover.first->stats[T].count[trManager->next->crossover.first->state]++;
        trManager->next->crossover.first->stats[T].n_tr[trManager->next->type]++;
        if(trManager->next->crossover.first->is_crosspair){
            trManager->next->crossover.first->crosspair->stats[T].count[trManager->next->crossover.first->crosspair->state]++;
            trManager->next->crossover.first->crosspair->stats[T].n_tr[trManager->next->type]++;
        }
    }
    if (trManager->next->type == invasion){
        for (auto sdom = trManager->next->affected_domain->children.begin(); sdom!= trManager->next->affected_domain->children.end(); ++sdom){
            (*sdom)->stats[T].count[(*sdom)->state]++;
            (*sdom)->stats[T].n_tr[trManager->next->type]++;
        }
        trManager->next->affected_staple->stats[T].count[trManager->next->affected_staple->state]++;
        trManager->next->affected_staple->stats[T].n_tr[trManager->next->type]++;
        trManager->next->affected_domain->stats[T].count[trManager->next->affected_domain->state]++;
        trManager->next->affected_domain->helix->stats[T].n_tr[trManager->next->type]++;
        trManager->next->affected_domain->helix->stats[T].count[trManager->next->affected_domain->helix->state]++;
        trManager->next->affected_domain->stats[T].n_tr[trManager->next->type]++;
        if(trManager->next->affected_crossover.second == true){
            trManager->next->affected_crossover.first->stats[T].count[trManager->next->affected_crossover.first->state]++;
            trManager->next->affected_crossover.first->stats[T].n_tr[trManager->next->type]++;
            if (trManager->next->affected_crossover.first->is_crosspair){
                trManager->next->affected_crossover.first->crosspair->stats[T].count[trManager->next->affected_crossover.first->crosspair->state]++;
                trManager->next->affected_crossover.first->crosspair->stats[T].n_tr[trManager->next->type]++;
            }
        }
    }
}
void StatManager::update_times(){
    int idx;
    idx = ramp->prev_idx;
    fill_times<OrderParameter> (design->OPs,idx,trManager->next->tau);
    fill_times<SubDomain> (design->domains,idx,trManager->next->tau);
    for(auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
        fill_times<OrderParameter> (pool->OPs,idx,trManager->next->tau);
        fill_times<PoolDomain> (pool->domains,idx,trManager->next->tau);
        fill_times<Crossover> (pool->crossovers,idx,trManager->next->tau);
        fill_times<CrossPair> (pool->crosspairs,idx,trManager->next->tau);
        fill_times<Staple> (pool->staples,idx,trManager->next->tau);
        fill_times<Helix> (pool->helices,idx,trManager->next->tau);
    }
    if (inputs->track_clusters){
        fill_times<OrderParameter> (G->cg.OPs,idx,trManager->next->tau);
        for (auto cg = G->p_cg.begin(); cg!= G->p_cg.end(); ++cg){
            fill_times<OrderParameter> (cg->OPs,idx,trManager->next->tau);
        }
    }
    double time;
    if (inputs->isothermal){
        idx = ramp->idx;
        time = ramp->current_t;
    }
    else{
        idx = ramp->prev_idx;
        time = ramp->previous_t;
    }
    auto pool = design->staple_pools.begin()+inputs->target_pool;
    check_entrance(pool->domains, idx, time, ramp->dt, inputs->isothermal, this->num_done, inputs->melt);
    check_entrance(pool->crossovers, idx, time, ramp->dt, inputs->isothermal, this->num_done, inputs->melt);
    check_entrance(pool->crosspairs, idx, time, ramp->dt, inputs->isothermal, this->num_done, inputs->melt);
    check_entrance(pool->staples, idx, time, ramp->dt, inputs->isothermal, this->num_done, inputs->melt);
    check_entrance(pool->helices, idx, time, ramp->dt, inputs->isothermal, this->num_done, inputs->melt);
    if (num_done == target_num_done) this->all_in_times_done = true;
}

void StatManager::write_op_stats(){
    if (!headers_written){
        ofiles->stats_file << "step" << "\t";
        ofiles->stats_file << "time" << "\t";
        ofiles->stats_file << "Temp" << "\t";
        for (auto op = design->OPs.begin(); op!= design->OPs.end(); ++op){
            ofiles->stats_file << op->name << "\t";
        }
        for (auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
            for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
                ofiles->stats_file << op->name << "\t";
            }
        }
        if (inputs->track_clusters){
            for (auto op = G->cg.OPs.begin(); op!= G->cg.OPs.end(); ++op){
                ofiles->stats_file << op->name << "\t";
            }
            for (auto cg = G->p_cg.begin(); cg!= G->p_cg.end(); ++cg){
                for (auto op = cg->OPs.begin(); op!= cg->OPs.end(); ++op){
                    ofiles->stats_file << op->name << "\t";
                }
            }
        }
        ofiles->stats_file << "\n";
        ofiles->stats_file << std::flush;
    }
    if (dummy){
        if (inputs->isothermal){
            int Ti = ramp->idx;
            ofiles->stats_file << trManager->step << "\t";
            ofiles->stats_file << ramp->current_t << "\t";
            ofiles->stats_file << ramp->Temps[Ti].str_T << "\t";
            for (auto op = design->OPs.begin(); op!= design->OPs.end(); ++op){
                ofiles->stats_file << op->state << "\t";
            }
            for (auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
                for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
                    ofiles->stats_file << op->state << "\t";
                }
            }
            if (inputs->track_clusters){
                for (auto op = G->cg.OPs.begin(); op!= G->cg.OPs.end(); ++op){
                    ofiles->stats_file << op->state << "\t";
                }
                for (auto cg = G->p_cg.begin(); cg!= G->p_cg.end(); ++cg){
                    for (auto op = cg->OPs.begin(); op!= cg->OPs.end(); ++op){
                        ofiles->stats_file << op->state << "\t";
                    }
                }
            }
        }
        else{
            int prev_Ti = ramp->prev_idx;
            ofiles->stats_file << trManager->step-1 << "\t";
            ofiles->stats_file << ramp->previous_t << "\t";
            ofiles->stats_file << ramp->Temps[prev_Ti].str_T << "\t";
            for (auto op = design->OPs.begin(); op!= design->OPs.end(); ++op){
                ofiles->stats_file << op->stats[prev_Ti].rolling_average << "\t";
            }
            for (auto pool = design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
                for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
                    ofiles->stats_file << op->stats[prev_Ti].rolling_average << "\t";
                }
            }
            if (inputs->track_clusters){
                for (auto op = G->cg.OPs.begin(); op!= G->cg.OPs.end(); ++op){
                    ofiles->stats_file << op->stats[prev_Ti].rolling_average << "\t";
                }
                for (auto cg = G->p_cg.begin(); cg!= G->p_cg.end(); ++cg){
                    for (auto op = cg->OPs.begin(); op!= cg->OPs.end(); ++op){
                        ofiles->stats_file << op->stats[prev_Ti].rolling_average << "\t";
                    }
                }
            }
        }
        ofiles->stats_file << "\n";
        ofiles->stats_file << std::flush;
    }
}
void StatManager::write_object_stats(){
    if (!headers_written){
        write_obj_stat_headers(ofiles->sdom_stats_file,design->domains);
        for (auto pool= design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
            write_obj_stat_headers(ofiles->pdom_stats_files[pool->id],pool->domains);
            write_obj_stat_headers(ofiles->cr_stats_files[pool->id],pool->crossovers);
            write_obj_stat_headers(ofiles->crp_stats_files[pool->id],pool->crosspairs);
            write_obj_stat_headers(ofiles->st_stats_files[pool->id],pool->staples);
            write_obj_stat_headers(ofiles->hlx_stats_files[pool->id],pool->helices);
        }
    }
    if (dummy){
        int step, Ti; double t; string T;
        if (inputs->isothermal){
            step = trManager->step;
            t = ramp->current_t;
            Ti = ramp->idx;
            T = ramp->Temps[Ti].str_T;
        }
        else {
            step = trManager->step-1;
            t = ramp->previous_t;
            Ti = ramp->prev_idx;
            T = ramp->Temps[Ti].str_T;
        }
        write_obj_stat(ofiles->sdom_stats_file, step, t, T, Ti, design->domains);
        for (auto pool= design->staple_pools.begin(); pool!= design->staple_pools.end(); ++pool){
            write_obj_stat(ofiles->pdom_stats_files[pool->id], step, t, T, Ti, pool->domains);
            write_obj_stat(ofiles->cr_stats_files[pool->id], step, t, T, Ti, pool->crossovers);
            write_obj_stat(ofiles->crp_stats_files[pool->id], step, t, T, Ti, pool->crosspairs);
            write_obj_stat(ofiles->st_stats_files[pool->id], step, t, T, Ti, pool->staples);
            write_obj_stat(ofiles->hlx_stats_files[pool->id], step, t, T, Ti, pool->helices);
        }
    }
}

void StatManager::write_all(){
    dummy = false;
    if (inputs->isothermal){
        if (!inputs->umbrella_sampling){
            if (trManager->next->id != trManager->prev_rev_id) dummy = true;
            if (!design->target_reached && trManager->step % 1000 == 0) dummy = true;
            if (design->target_reached) dummy = true;
        }
        if (trManager->step % inputs->write_stat_every == 0) dummy = true;
    }
    else {if (ramp->T_was_changed) dummy = true;}
    
    this->write_op_stats();
    if (!inputs->isothermal) this->write_object_stats();

    this->headers_written = true;
}

void StatManager::write_in_times(){
    StaplePool* pool = &design->staple_pools[inputs->target_pool];
    for (const auto& obj : pool->domains){
        ofiles->times_file << pool->name << "\t";
        ofiles->times_file << "dom" << "\t";
        ofiles->times_file << obj.id << "\t";
        for (const auto& time : obj.entrance){
            ofiles->times_file << time << "\t";
        }
        ofiles->times_file << "\n";
    }
    for (const auto& obj : pool->crossovers){
        ofiles->times_file << pool->name << "\t";
        ofiles->times_file << "cr" << "\t";
        ofiles->times_file << obj.id << "\t";
        for (const auto& time : obj.entrance){
            ofiles->times_file << time << "\t";
        }
        ofiles->times_file << "\n";
    }
    for (const auto& obj : pool->crosspairs){
        ofiles->times_file << pool->name << "\t";
        ofiles->times_file << "crp" << "\t";
        ofiles->times_file << obj.id << "\t";
        for (const auto& time : obj.entrance){
            ofiles->times_file << time << "\t";
        }
        ofiles->times_file << "\n";
    }
    for (const auto& obj : pool->staples){
        ofiles->times_file << pool->name << "\t";
        ofiles->times_file << "st" << "\t";
        ofiles->times_file << obj.id << "\t";
        for (const auto& time : obj.entrance){
            ofiles->times_file << time << "\t";
        }
        ofiles->times_file << "\n";
    }
    for (const auto& obj : pool->helices){
        ofiles->times_file << pool->name << "\t";
        ofiles->times_file << "hlx" << "\t";
        ofiles->times_file << obj.id << "\t";
        for (const auto& time : obj.entrance){
            ofiles->times_file << time << "\t";
        }
        ofiles->times_file << "\n";
    }
    ofiles->times_file << std::flush;
}
