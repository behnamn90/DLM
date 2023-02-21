/*
 *
 *	MyGraph.h
 * 	Author: Behnam
 *
 */

#ifndef MYGRAPH_H_
#define MYGRAPH_H_

#include "Loop.h"

void remove_long_crossovers(Graph& graph, const Design* design);
void readd_long_crossovers(Graph& graph, Design* design);


bool sort_decending (int,int);
void reset_edge_index_single(Graph&);

class Comp_Graph{
public:
    Comp_Graph ();
    Comp_Graph (int, string, int);
    virtual ~Comp_Graph(){}

    int pool_id; //-1 for main component graph.
    string pool_name;
    std::size_t num_vertices;

    Graph g;
    std::size_t num_components;
    vector<int> component; //component[i] = the index of the component that vertex i is part of.
    vector<int> comp_sizes; //comp_sizes[i] = size of i'th largest cluster
    vector<int> num_comps; //num_comps[i] = number of components of size i;

    void fill_components();

    void add_OPs();
    vector<OrderParameter> OPs; //index is the index of temperate in TempRamp::unique_Ts

};

class MyGraph{
public:
    MyGraph (Design*);
	virtual ~MyGraph(){}

    Inputs* inputs;
	Design* design;
	Graph g; //Graph of design subdomains
    bool local;

    Comp_Graph cg; //Component graph for design (of g)
    vector<Comp_Graph> p_cg; //Component graphs for each pool

	void add_vertices();
    void add_layouts();
	void add_domains();

    void initialise_state();
    void bind_domain(const PDOM);
    void unbind_domain(const PDOM);
    void bind_domains(vector<SDOM>&);
    void unbind_domains(vector<SDOM>&);
	void add_crossover(CR);
	void remove_crossover(CR);
    void reset_vertex_index();
    void reset_edge_index();

    double shortest_path(const CR);
    double long_pathweight(const CR);
    vector<Weight> distances; // To store distances for djkastra


    planar_embedding_storage_t embedding_storage;
    void add2embedding(const CR& cross);
    void removeFromEmbedding(const CR& cross);
    double faces_weight();
    int numLong = 0;
    int numFaces; // not including long crossovers.

    void fill_components();

	void print_edges();
    void print_embedding();
    void write_single(const string&,Graph&,const string&);
    void write(const string&);


    //Redundant
    edge_t id_to_edge(int);


};

#endif


