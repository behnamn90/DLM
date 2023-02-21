/*
 *
 *    Node.h
 *     Author: Behnam
 *
 */

#ifndef NODE_H_
#define NODE_H_

#include "OrderParameter.h"
#include "../Headers/Graph.h"

class Node {
public:
    Node();
    virtual ~Node(){}
    
    int pool_id;
    pair<int,int> pooldomain_ids;
    pair<int,int> nucs;
    int id;  // id in design
    int idp; // id in pool
    pair<int,int> subdomain_ids;
    
    StaplePool* pool;
    pair<PDOM,PDOM> pooldomains;
    pair<SDOM,SDOM> subdomains;
};



#endif /* Vertex_hpp */
