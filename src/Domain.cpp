/*
 *
 *	Domain.cpp
 * 	Author: Behnam
 *
 */

#include "Domain.h"

Domain::Domain () {
}

SubDomain::SubDomain () {
}

PoolDomain::PoolDomain () {
}

void PoolDomain::bind(){
    prev_state = this->state;
    this->state = true;
    if (state != prev_state){
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->type == OP_t(BOUND_BPS))
                (*op)->state+= this->length;
            if ((*op)->type == OP_t(BOUND_DOMAINS))
                (*op)->state++;
        }
    }
    for(auto child = children.begin(); child!= children.end(); ++child){
        (*child)->prev_state = (*child)->state;
        (*child)->state = this->pool_id;
        if ((*child)->state != (*child)->prev_state){
            for (auto op = (*child)->OPs.begin(); op!= (*child)->OPs.end(); ++op){
                if ((*op)->type == OP_t(BOUND_BPS))
                    (*op)->state+= (*child)->length;
                if ((*op)->type == OP_t(BOUND_DOMAINS))
                    (*op)->state++;
            }
        }
    }
}
void PoolDomain::unbind(){
    prev_state = this->state;
    this->state = false;
    if (state != prev_state){
        for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
            if ((*op)->type == OP_t(BOUND_BPS))
                (*op)->state-= this->length;
            if ((*op)->type == OP_t(BOUND_DOMAINS))
                (*op)->state--;
        }
    }
    for(auto child = children.begin(); child!= children.end(); ++child){
        (*child)->prev_state = (*child)->state;
        (*child)->state = -1;
        if ((*child)->state != (*child)->prev_state){
            for (auto op = (*child)->OPs.begin(); op!= (*child)->OPs.end(); ++op){
                if ((*op)->type == OP_t(BOUND_BPS))
                    (*op)->state-= (*child)->length;
                if ((*op)->type == OP_t(BOUND_DOMAINS))
                    (*op)->state--;
            }
        }
    }
}

