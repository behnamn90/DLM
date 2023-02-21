/*
 *
 *	Input.h
 * 	Author: Behnam
 *
 */

#ifndef INPUT_H_
#define INPUT_H_

#include "Common.h"
//#include "Common.h"
//#include "Constants.h"

#define UNPARSED 0
#define PARSED 1
#define ERROR 2
#define KEY_NOT_FOUND -1
#define KEY_FOUND 0
#define KEY_INVALID 1

#define INP_EOF -1
#define KEY_READ 0
#define NOTHING_READ 1



int readLine(std::vector<string>::iterator &it, std::vector<string>::iterator &end, string &key, string &value);

/* depretiated in C++17
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}
*/
// trim from start (in place)
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

// trim from end (in place)
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// trim from both ends (in place)
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

struct input_value {
	std::string value;
	int read;
	input_value() : value(""), read(0) {}
	input_value(std::string v) : value(v), read(0) {}
};

typedef std::map<std::string, input_value> input_map;
struct input_file {
    input_map keys;
    std::vector<std::string> unread_keys;
    int state;
    std::set<std::string> true_values;
    std::set<std::string> false_values;
    input_file();
};

class Inputs{
public:
    Inputs(int argc, char * argv[]);
    Inputs();
    virtual ~Inputs(){}

    
    
    const char * file_name;
    input_file input;

    void print_map();
    void print_params();
    
    void loadInputFile();
    void loadInput(FILE *);
    void addInput(string);
    void addInput(FILE *);
    
    void loadOptions();
    input_map::iterator getInputValue(const char *skey);
    int getInputString(const char *skey, std::string &dest);
    int getInputString(const char *skey, char *dest);
    int getInputBool(const char *skey, bool *dest);
    int getInputDouble(const char *skey, double *dest);
    int getInputInt(const char *skey, int *dest);
    int getInputVectorInt(const char *skey, vector<int> *dest);
    int getInputChar(const char *skey, char *dest); 
    
    double target_bound_fraction;
    int target_pool;
    
    string info_file_name;
    
    string rate_method;
    //bool scale_rates;

    
    double gamma;
    double n_param;
    double concentration;
    
    string domain_energies;
	
    int seed;
    
    double k_bi;
    double k_uni;
    double k_f;
    
    vector<int> staple_species;
    
    string rate_model;

    
    string scaffold_file_name;
    string domain_file_name;
    string staple_file_name;
    
    string topology_file_name;
    string configuration_file_name; //Initial conf
    string initialise_as;

    string statistics_file_name;
    string staple_stats_file_name;
    string staple_temp_file_name;
    string domain_stats_file_name;
    string domain_temp_file_name;
    string cross_stats_file_name;
    string crosspair_stats_file_name;
    string helix_stats_file_name;
    int write_stat_every;
    
    int write_hist_every;
    string traj_hist_file_name;
    string hist_file_name;
    string burn_file_name;
    
    string staple_hist_file_name;
    string domain_hist_file_name;
    string cross_hist_file_name;
    string crosspair_hist_file_name;
    string helix_hist_file_name;

    string transition_file_name;
    int write_transition_every;
    
    int write_simple_every;
    string last_conf_file_name;
    int write_state_every;
    string trajectory_file_name;
    string simple_trajectory_file_name;

    string missing_staple_file_name;
    
    string energy_file_name;
    
    bool make_movie;
	
    bool miss_staples;

    
    bool umbrella_sampling;
    
	bool break_seams;
    //string occupancy_file_name;
	
	string sim_type;
	bool anneal;
	bool melt;
	bool isothermal;
    bool test;
    bool convert;
    bool config_generator;
    bool exact;
	
    bool track_clusters;

    
    bool custom_conc;
    
	double temp;
	double max_time;
	int max_steps;
	int burn_steps;

	double max_temp;
	double min_temp;
	double cool_rate;
	double const_T_interval;
	int n_cycles;
	
    int coat_with;
    int rotated_pool;
    int rotate_by;
    int scaff_rot = 0;

    bool extra_ops;
    string op_file_name;
	
    string w_file_name;
    string conc_file_name;
};

#endif
