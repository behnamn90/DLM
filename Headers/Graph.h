/*
 *
 *	Graph.h
 * 	Author: Behnam
 *
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "../Headers/VertexProperty.h"
#include "../Headers/EdgeProperty.h"

typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::undirectedS,
        VertexProperty, EdgeProperty
        > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_iterator_t;
typedef boost::property_map<Graph, int VertexProperty::* >::type vertex_map_t;

typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator_t;
typedef boost::property_map<Graph, int EdgeProperty::* >::type edge_map_t;

typedef std::vector<edge_t> edge_path_t;
typedef std::vector<edge_path_t> planar_embedding_storage_t; //for embeddings
typedef boost::iterator_property_map< planar_embedding_storage_t::iterator,
        boost::property_map<Graph, int VertexProperty::*>::type>
        planar_embedding_t;

typedef double Weight;
typedef property_map <Graph,vertex_index_t>::type IndexMap;
typedef iterator_property_map <Weight*,IndexMap,Weight,Weight&> DistanceMap;

struct face_counter : public planar_face_traversal_visitor {
    face_counter() : count(0) {}
    void begin_face() { ++count; }
    int count;
};
struct output_visitor : public planar_face_traversal_visitor {
    void begin_face() { cout << "New face: "; }
    void end_face() { cout << endl; }
};
struct vertex_output_visitor : public output_visitor {
    template <typename Vertex>
    void next_vertex(Vertex v)
    {
        cout << v << " ";
    }
};
struct edge_output_visitor : public output_visitor {
    template <typename Edge>
    void next_edge(Edge e)
    {
        cout << e << " ";
    }
};



#endif
