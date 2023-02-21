//
//  Loop.h
//  DLM_XC
//
//  Created by Behnam Najafi on 03/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#ifndef LOOP_h
#define LOOP_h

#include "Design.h"

void print_embedding_storage(Graph&, planar_embedding_storage_t&);

//const double C_parameter = 2.8 * pow(10,-18);


struct traversal_visitor : public boost::planar_face_traversal_visitor {

    const Graph& graph;
    int numFaces;
    double total_weight;
    double face_weight;
    double logsum;

    explicit traversal_visitor(const Graph& graph);

    void begin_traversal();
    void begin_face();

    void next_vertex(vertex_t v);

    void next_edge(edge_t e);

    void end_face();
    void end_traversal();

};

void set_face_data(Graph& g, planar_embedding_storage_t& emb, traversal_visitor& outvis);


#endif /* Loop_h */
