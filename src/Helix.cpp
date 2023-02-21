/*
 *
 *    Helix.cpp
 *     Author: Behnam
 *
 */

#include "Helix.h"

Part::Part(){}

void Part::update_state(){
    this->bound = false;
    this->num_bound = 0;
    for (auto pdom = this->domains.begin(); pdom!= this->domains.end(); ++pdom){
        if((*pdom)->state)
            this->num_bound++;
    }
    if (this->num_bound == this->length)
        this->bound = true;
}

Helix::Helix(){}

void Helix::update_state(){
    prev_state = state;
    for (auto part = parts.begin(); part!= parts.end(); ++part){
        part->update_state();
    }
    if (this->type == 'n'){
        if (parts[0].bound)
            this->state = 'F';
        else
            this->state = 'N';
    }
    if (this->type == 's'){
        if (!parts[0].bound && !parts[1].bound)
            this->state = 'N';
        if (parts[0].bound && !parts[1].bound)
            this->state = 'L';
        if (!parts[0].bound && parts[1].bound)
            this->state = 'R';
        if (parts[0].bound && parts[1].bound)
            this->state = 'F';
    }
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        if ((*op)->type == OP_t(BOUND_HELICES)){
            if (state == 'F')
               (*op)->state++;
            if (prev_state == 'F')
                (*op)->state--;
        }
        if ((*op)->type == OP_t(LBOUND_HELICES)){
            if (state == 'L')
                (*op)->state++;
            if (prev_state == 'L')
                (*op)->state--;
        }
        if ((*op)->type == OP_t(RBOUND_HELICES)){
            if (state == 'R')
                (*op)->state++;
            if (prev_state == 'R')
                (*op)->state--;
        }
    }
}

/*
void Helix::check_bound(){
    is_bound = false;
    num_bound = 0;
    for (auto dom = domains.begin(); dom != domains.end(); ++dom){
        if ((*dom)->state){
            this->num_bound++;
        }
    }
    if (num_bound == domains.size())
    {
        is_bound = true;
    }
    state = num_bound;
}
*/



