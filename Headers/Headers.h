/*
 *
 *	Headers.h
 * 	Author: Behnam
 *
 */

#ifndef HEADERS_H_
#define HEADERS_H_

#include <iostream>
#include <utility>                  
#include <algorithm>               
#include <fstream>
#include <vector>
#include <stdio.h>      /* printf, fopen */
#include <stdlib.h>     /* exit, EXIT_FAILURE, system */
#include <sstream> //for double to string
#include <iomanip>
#include <streambuf>

#include <time.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
//#include <boost/property_map/property_map.hpp>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>
//#include <boost/ref.hpp>
#include <boost/graph/copy.hpp>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
//#include <boost/generator_iterator.hpp>

#include <boost/random/normal_distribution.hpp>
#include <boost/random/triangle_distribution.hpp>
#include <boost/random.hpp>


#include <boost/algorithm/string.hpp> //For splitting strings boost::split()

//For make_vector
//#include <boost/fusion/container/generation/make_vector.hpp>
//#include <boost/fusion/include/make_vector.hpp>

#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/graphml.hpp>

//#include <boost/filesystem.hpp>

#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().


//For parse_input
#include <cstring>
#include <ctype.h>
#include <cmath>
#include <string>
#include <map>
#include <set>
//For getline
#include <stddef.h>
#include <limits.h>
#include <errno.h>

#define EPSILON 0.000000000000000000001

using namespace std;
using namespace boost;

class Node;
typedef vector<Node>::iterator VX;
class Domain;
typedef vector<Domain>::iterator DOM;
class PoolDomain;
typedef vector<PoolDomain>::iterator PDOM;
class SubDomain;
typedef vector<SubDomain>::iterator SDOM;
class Crossover;
typedef vector<Crossover>::iterator CR;
class CrossPair;
typedef vector<CrossPair>::iterator CRP;
class Staple;
typedef vector<Staple>::iterator ST;
class Helix;
typedef vector<Helix>::iterator HX;

class StaplePool;

class Transition;
typedef vector<Transition>::iterator TR;

/* Possible states of staples with one, two, and
 * three domains.
 * */
enum State_t { s0, s1,
        s00, s10, s01, s12, s11,
        s000, s001, s010, s100, s011, s110, s101, s012, s120, s102, s112, s211, s121, s123, s111,
        State_MAX };
class State_names {
public:
    static string array(int index) {
        static string a[] = { "s0", "s1",
                              "s00", "s10", "s01", "s12", "s11",
                              "s000", "s001", "s010", "s100", "s011", "s110", "s101",
                              "s012", "s120", "s102", "s112", "s211", "s121", "s123", "s111"};
        return a[index];
    } 
};

/* Possible Order parameters in the model.
 * The Order parameter file should use an integer
 * corresponding to the position in this array.
 * */
enum OP_t {
    BOUND_BPS,
    BOUND_DOMAINS,
    BOUND_CROSSOVERS,
    BOUND_STAPLES,
    BOUND_HELICES,
    LBOUND_HELICES,
    RBOUND_HELICES,
    BOUND_CROSSPAIRS,
    NUM_CLUSTERS,
    LARGE_CLUSTER,
    MISBOUND_STAPLES,
    OP_MAX
};
class OP_names {
public:
    static string array(int index) {
        static string a[] = {
            "BOUND_BPS",
            "BOUND_DOMAINS",
            "BOUND_CROSSOVERS",
            "BOUND_STAPLES",
            "BOUND_HELICES",
            "LBOUND_HELICES",
            "RBOUND_HELICES",
            "BOUND_CROSSPAIRS",
            "NUM_CLUSTERS",
            "LARGE_CLUSTER",
            "MISBOUND_STAPLES",
        };
        return a[index];
    }
};



/* These are used to calculate connected components.
 * Similar to order parameters.
 * */
enum GraphStat_t {
    num_clusters,
    large_cluster1, large_cluster2, large_cluster3,
    GraphStat_MAX};
class GraphStat_names {
public:
    static string array(int index) {
        static string a[] = {
            "num_clusters",
            "large_cluster1", "large_cluster2", "large_cluster3"
        };
        return a[index];
    }
};

/* Whether the transition is invasion type.
 * */
enum Transition_t { normal, invasion, Tr_MAX };
class Transition_names {
public:
    static string array(int index) {
        static string a[] = { "normal", "invasion"};
        return a[index];
    }
};

/* General statistics collector.
 * Used for both order parameters and
 * for objects (domains, crossovers, staples)
 * */
template <class T>
class Stat{
public:
    Stat(){
        for (int i= 0; i< Tr_MAX; i++){
            n_tr.push_back(0);
        }
    }
    vector<int> n_tr;
    std::map<T, int> count;
    std::map<T, double> time;
};
template <>
class Stat <int> {
public:
    Stat(){
        n = 0;
        rolling_average = 0;
        for (int i= 0; i< Tr_MAX; i++){
            n_tr.push_back(0);
        }
    }
    vector<int> n_tr;
    std::map<int, int> count;
    std::map<int, double> time;
    
    //Special Members
    int n;
    double rolling_average;
    void update(const int& new_state){
        n++;
        rolling_average += (new_state - rolling_average) / (n*1.);
    }
};

typedef boost::mt19937 base_generator_type;
typedef boost::variate_generator<base_generator_type&, uniform_real<> > Uni;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (int i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << " ";
    }
    os << "]";
    return os;
}

#endif
