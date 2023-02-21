/*
 *
 *	Input.cpp
 * 	Author: Behnam
 *
 */

#include "Input.h"

input_file::input_file() {
	true_values.insert("true");
	true_values.insert("1");
	true_values.insert("yes");
	true_values.insert("yup");
	true_values.insert("sure");

	false_values.insert("false");
	false_values.insert("0");
	false_values.insert("no");
	false_values.insert("nope");
	false_values.insert("nah");
}

Inputs::Inputs (int argc, char * argv[]) { 
    if (argc == 1){
        file_name = "input.txt";
    }
    else if (argc == 2){
        file_name = argv[1];
	}
	else{
        cout << "Usage: ./DLM input_file" << endl;
        exit (EXIT_FAILURE);
	}
	loadInputFile();
	loadOptions();
    if(this->config_generator) this->umbrella_sampling = true;
}
Inputs::Inputs () {
    file_name = "input.txt";
    loadInputFile();
    loadOptions();
    //print_params();
}

void Inputs::print_map(){
	cout << "\n______________________________Printing Input Map_____________________________\n";
	for(auto it = input.keys.cbegin(); it != input.keys.cend(); ++it){
    std::cout << it->first << " " << it->second.value << "\n";
	}
	cout << "_______________________________________________________________________________\n";
	cout << "\n";
}
void Inputs::print_params(){
	cout << "\n___________________________Printing Input Parameters_________________________\n";
	cout << "seed: " << seed << "\n";
	cout << "gamma: " << gamma << "\n";
	cout << "n_param: " << n_param << "\n";
	cout << "concentration: " << concentration << "\n";
	cout << "sim_type: " << sim_type << "\n";
	cout << "anneal: " << anneal << "\n";
	cout << "melt: " << melt << "\n";
	cout << "isothermal: " << isothermal << "\n";
	
	cout << "max_time: " << max_time << "\n";
	cout << "max_steps: " << max_steps << "\n";
	cout << "burn_steps: " << burn_steps << "\n";
	cout << "temp: " << temp << "\n";
	
	cout << "cool_rate: " << cool_rate << "\n";
	cout << "n_cycles: " << n_cycles << "\n";
	cout << "max_temp: " << max_temp << "\n";
	cout << "min_temp: " << min_temp << "\n";
	cout << "const_T_interval: " << const_T_interval << "\n";
	cout << "domain_energies: " << domain_energies << "\n";
	cout << "break_seams: " << break_seams << "\n";
	
	cout << "statistics_file: " << statistics_file_name << "\n";
	cout << "scaffold_file: " << scaffold_file_name << "\n"; 
	cout << "staple_file: " << staple_file_name << "\n"; 
	cout << "domain_file: " << domain_file_name << "\n"; 
	cout << "write_state_every: " << write_state_every << "\n";
    cout << "trajectory_file: " << trajectory_file_name << "\n";
    cout << "write_transtion_every: " << write_transition_every << "\n";
    cout << "transition_file: " << transition_file_name << "\n";
	cout << "hist_file_name: " << hist_file_name << "\n";
	cout << "w_file_name: " << w_file_name << "\n";
    cout << "conc_file_name: " << conc_file_name << "\n";

	cout << "staple_species: ";
	for (vector<int>::iterator it = staple_species.begin(); it!=staple_species.end(); ++it){
		cout << *it << "\t";
	}
	cout << "\n";
	cout << "_______________________________________________________________________________\n";
	cout << "\n";
}
void Inputs::loadInputFile() {
	FILE *inp_file = fopen(file_name, "r");
	if(inp_file == NULL) {
		fprintf(stderr, "Input file '%s' not found\n", file_name);
			input.state = -2;
			return;
	}
	loadInput(inp_file);
	fclose(inp_file);
	return;
}
void Inputs::loadInput(FILE *inp_file) {
	input.state = UNPARSED;
	addInput(inp_file);
	input.state = PARSED;
}
void Inputs::addInput(FILE *inp_file) {
	size_t alloc_size;
	char *c_option = NULL;
	string file_contents("");

	int count = 0;
	while(count != -1) {
		count = getline(&c_option, &alloc_size, inp_file);
		if(count != -1) file_contents += string(c_option);
		free(c_option);
		c_option = NULL;
	}

	addInput(file_contents);
}
void Inputs::addInput(std::string s_inp) {
	vector<string> tot_lines;
	boost::split(tot_lines, s_inp, boost::is_any_of("\n"));
	vector<string> lines;

	for(vector<string>::iterator it = tot_lines.begin(); it != tot_lines.end(); it++) {
		// remove in-line comments
		size_t comment_start = it->find('#');
		if(comment_start != string::npos) it->erase (comment_start, it->size() - comment_start);

		// split the string using ; as a delimiter
		std::vector<string> to_add;
		boost::split(to_add, *it, boost::is_any_of(";"));

		lines.insert(lines.end(), to_add.begin(), to_add.end());
	}

	std::vector<string>::iterator l_end = lines.end();
	for(std::vector<string>::iterator it = lines.begin(); it != lines.end(); it++) {
		//cout << *it << endl;
		string key, value;
		int res = readLine(it, l_end, key, value);

		if(res == KEY_READ){
			input_value new_value(value);

			//input_map::iterator old_val = input.keys.find(key);
			input.keys[key] = value;
		}
	}
}
int readLine(std::vector<string>::iterator &it, std::vector<string>::iterator &end, string &key, string &value) {
	string option(*it);
	option = trim(option);

	if (option.size() > 0) {
		std::vector<string> words;
		boost::split(words, option, boost::is_any_of("="));;
		
		if (words.size() == 1) {
			return NOTHING_READ;
		}

		if (words.size() > 2) {
			for (unsigned int i = 2; i < words.size(); i ++) words[1] += string("=") + words[i];
		}

		string my_key = trim(words[0]);
		string my_value = trim(words[1]);

		if(my_value[0] == '{') {
			// counts the number of open and closed curly brackets 
			size_t open = std::count (my_value.begin(), my_value.end(), '{');
			size_t close = std::count (my_value.begin(), my_value.end(), '}');

			int sum = (int)open - (int)close; 

			if (sum < 0) {
				return NOTHING_READ;
			}

			if (sum > 0) my_value += string("\n");

			while (sum > 0) {
				it++;
				
				string new_line = string(*it);
				new_line = trim(new_line);
			
				sum += std::count (new_line.begin(), new_line.end(), '{');
				sum -= std::count (new_line.begin(), new_line.end(), '}');

				my_value += new_line;
				if(sum != 0) my_value += string("\n");
			}
		}

		key = my_key;
		value = my_value;
	}
	else return NOTHING_READ;

	return KEY_READ;
}

void Inputs::loadOptions(){
    getInputInt("target_pool", &target_pool);
    getInputDouble("target_bound_fraction", &target_bound_fraction);
    getInputString("info_file", info_file_name);
    getInputString("rate_model", rate_model);

    getInputDouble("gamma", &gamma);
	getInputDouble("n_param", &n_param);
	getInputDouble("concentration", &concentration);
    getInputBool("custom_conc", &custom_conc);

	getInputString("domain_energies", domain_energies);

	getInputString("sim_type", sim_type);
    //getInputBool("scale_rates", &scale_rates);
    getInputString("rate_method", rate_method);

    getInputBool("miss_staples", &miss_staples);
    getInputBool("extra_ops", &extra_ops);

    getInputDouble("k_bi", &k_bi);
    getInputDouble("k_uni", &k_uni);
    getInputDouble("k_f", &k_f);
	
	getInputDouble("temp", &temp);
	getInputDouble("max_time", &max_time);
	getInputInt("max_steps", &max_steps);
	getInputInt("burn_steps", &burn_steps);
	
	getInputDouble("max_temp", &max_temp);
	getInputDouble("min_temp", &min_temp);
	getInputDouble("cool_rate", &cool_rate);
    getInputDouble("const_T_interval", &const_T_interval);
	//getInputBool("anneal", &anneal);
	//getInputBool("melt", &melt);
	getInputInt("n_cycles", &n_cycles);
	
	getInputInt("seed", &seed);
	getInputVectorInt("staple_species", &staple_species);

	getInputString("scaffold_file", scaffold_file_name);
	getInputString("domain_file", domain_file_name);
	getInputString("staple_file", staple_file_name);
    getInputString("topology_file", topology_file_name);
    getInputString("configuration_file", configuration_file_name);
    getInputString("initialise_as", initialise_as);
    getInputString("last_conf_file", last_conf_file_name);

    getInputString("energy_file", energy_file_name);

    
    getInputBool("umbrella_sampling", &umbrella_sampling);
    getInputString("weight_file", w_file_name);
    getInputString("op_file", op_file_name);

	getInputBool("break_seams", &break_seams);
	
    getInputInt("write_simple_every", &write_simple_every);
    getInputInt("write_state_every", &write_state_every);
    getInputString("trajectory_file", trajectory_file_name);
    getInputString("simple_trajectory_file", simple_trajectory_file_name);

    getInputString("missing_staple_file", missing_staple_file_name);
    
    getInputString("burn_file", burn_file_name);
    
    getInputString("statistics_file", statistics_file_name);
    getInputString("staple_stats_file", staple_stats_file_name);
    getInputString("staple_temp_file", staple_temp_file_name);
    getInputString("domain_stats_file", domain_stats_file_name);
    getInputString("domain_temp_file", domain_temp_file_name);
    getInputString("crossover_stats_file", cross_stats_file_name);
    getInputString("crosspair_stats_file", crosspair_stats_file_name);
    getInputString("helix_stats_file", helix_stats_file_name);
    getInputInt("write_stat_every", &write_stat_every);

    getInputInt("write_hist_every", &write_hist_every);
    getInputString("traj_hist_file", traj_hist_file_name);
    getInputString("hist_file", hist_file_name);
    
    getInputString("staple_hist_file", staple_hist_file_name);
    getInputString("domain_hist_file", domain_hist_file_name);
    getInputString("crossover_hist_file", cross_hist_file_name);
    getInputString("crosspair_hist_file", crosspair_hist_file_name);
    getInputString("helix_hist_file", helix_hist_file_name);

    
    
    getInputInt("write_transition_every", &write_transition_every);
    getInputString("transition_file", transition_file_name);
    
    getInputString("conc_file", conc_file_name);
    
    getInputBool("make_movie", &make_movie);
    getInputBool("track_clusters", &track_clusters);

    getInputInt("coat_with", &coat_with);
    getInputInt("rotated_pool", &rotated_pool);
    getInputInt("rotate_by", &rotate_by);
    getInputInt("scaff_rot", &scaff_rot);

    
	if (sim_type == "isothermal"){
		isothermal = true;
		anneal = false;
		melt = false;
        test = false;
        convert = false;
        config_generator = false;
        exact = false;
    }
	else if (sim_type == "anneal"){
		isothermal = false;
		anneal = true;
		melt = false;
        test = false;
        convert = false;
        config_generator = false;
        exact = false;
    }
	else if (sim_type == "melt"){
		isothermal = false;
		anneal = false;
		melt = true;
        test = false;
        convert = false;
        config_generator = false;
        exact = false;
	}
    else if (sim_type == "test"){
        isothermal = false;
        anneal = false;
        melt = false;
        test = true;
        convert = false;
        config_generator = false;
        exact = false;
    }
    else if (sim_type == "convert"){
        isothermal = false;
        anneal = false;
        melt = false;
        test = false;
        convert = true;
        config_generator = false;
        exact = false;
    }
    else if (sim_type == "config_generator"){
        isothermal = false;
        anneal = false;
        melt = false;
        test = false;
        convert = false;
        config_generator = true;
        exact = false;
    }
    else if (sim_type == "exact"){
        isothermal = false;
        anneal = false;
        melt = false;
        test = false;
        convert = false;
        config_generator = false;
        exact = true;
    }
	else {
		cout << "Please select a valid sim_type.\n";
	}
}

input_map::iterator Inputs::getInputValue(const char *skey)  {
	std::map<string, input_value>::iterator it = input.keys.find(string(skey));
	if(it != input.keys.end()) it->second.read++;
	return it;
}

int Inputs::getInputString(const char *skey, std::string &dest) {
	input_map::iterator it = getInputValue(skey);
	if(it == input.keys.end()) return KEY_NOT_FOUND;
		
	dest = it->second.value;

	return KEY_FOUND;
}

int Inputs::getInputString(const char *skey, char *dest) {
	string s_dest;
	int res = getInputString(skey, s_dest);
	if(res != KEY_FOUND) return res;
	strncpy(dest, s_dest.c_str(), sizeof(char) * (s_dest.size()+1));

	return KEY_FOUND;
}

int Inputs::getInputInt(const char *skey, int *dest) {
	input_map::iterator it = getInputValue(skey);
	if(it == input.keys.end()) return KEY_NOT_FOUND;

	*dest = (int) floor(atof(it->second.value.c_str())+0.1);

	return KEY_FOUND;
}

int Inputs::getInputBool(const char *skey, bool *dest) {
	input_map::iterator it = getInputValue(skey);
	if(it == input.keys.end()) return KEY_NOT_FOUND;

	// make it lower case
	string val = it->second.value;
	std::transform(val.begin(), val.end(), val.begin(), ::tolower);

	set<string>::iterator res = input.true_values.find(val);
	if(res != input.true_values.end()) *dest = true;
	else {
		res = input.false_values.find(val);
		if(res != input.false_values.end()) *dest = false;
	}

	return KEY_FOUND;
}

int Inputs::getInputDouble(const char *skey, double *dest) {
	input_map::iterator it = getInputValue(skey);
	if(it == input.keys.end()) return KEY_NOT_FOUND;

	*dest = atof(it->second.value.c_str());

	return KEY_FOUND;
}

int Inputs::getInputChar(const char *skey, char *dest) {
	input_map::iterator it = getInputValue(skey);
	if(it == input.keys.end()) return KEY_NOT_FOUND;

	*dest = it->second.value[0];

	return KEY_FOUND;
}
int Inputs::getInputVectorInt(const char *skey, vector<int> *dest) {
	input_map::iterator it = getInputValue(skey);
	if(it == input.keys.end()) return KEY_NOT_FOUND;

	int dummy;
	vector<string> str_vec;
	boost::split(str_vec, it->second.value, boost::is_any_of(" "));
	for (vector<string>::iterator nit = str_vec.begin(); nit!=str_vec.end(); ++nit){
		//cout << *nit << endl;
		dummy = (int) floor(atof(nit->c_str())+0.1);
		dest->push_back(dummy);
	}

	return KEY_FOUND;
}
