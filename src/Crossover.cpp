/*
 *
 *	Crossover.cpp
 * 	Author: Behnam
 *
 */

#include "Crossover.h"

Crossover::Crossover () {
    state = false;
}

void Crossover::bind(){
    prev_state = state;
    this->state = true;
    if (state != prev_state){
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->type == OP_t(BOUND_CROSSOVERS))
                (*op)->state++;
        }
    }
}
void Crossover::unbind(){
    prev_state = state;
    this->state = false;
    if (state != prev_state){
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->type == OP_t(BOUND_CROSSOVERS))
                (*op)->state--;
        }
    }
}



CrossPair::CrossPair(){}

void CrossPair::update_state(){
    prev_state = state;
    if (this->crossovers.first->state == true && this->crossovers.second->state == true)
        this->state = true;
    else
        this->state = false;
    if (state != prev_state){
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->type == OP_t(BOUND_CROSSPAIRS)){
                if (this->state == true)
                    (*op)->state++;
                else
                    (*op)->state--;
            }
        }
    }
}
