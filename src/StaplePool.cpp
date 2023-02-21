/*
 *
 *    StaplePool.cpp
 *     Author: Behnam
 *
 */

#include "StaplePool.h"

//Constructors
StaplePool::StaplePool(){}
StaplePool::StaplePool(Inputs* inputs_, string& input_string, int id_, Scaffold* scaffold_) :
inputs(inputs_), scaffold(scaffold_), id(id_){
    read_string(input_string);
    add_links();
    add_crosspairs();
    split_helices();
    add_possible_states();
    add_stacks();
    //add_state_stacks();
    add_sequences();
    add_concentrations();
    add_energies();
    this->num_objects = num_domains+num_staples+num_crossovers+num_helices+num_crosspairs;
}

void StaplePool::read_string(string& input_string_){
    vector<std::string> All;
    boost::split(All, input_string_, boost::is_any_of("["));
    string Initial=     All[0];
    string Specs=       All[1];
    string Domains=     All[2];
    string Staples=     All[3];
    string Crossovers=  All[4];
    string Helices=     All[5];
    
    //Initial
    Initial.erase(std::remove(Initial.begin(), Initial.end(), '{'), Initial.end());
    Initial.erase(std::remove(Initial.begin(), Initial.end(), '\t'), Initial.end());
    Initial.erase(std::remove(Initial.begin(), Initial.end(), '\n'), Initial.end());
    this->name = Initial;
    
    vector<string> Lines;
    vector<string> vline;
    
    //Specifications
    boost::split(Lines, Specs, boost::is_any_of("\n"));
    Lines.erase(Lines.begin());
    Lines.erase(Lines.end()-1);
    Lines.erase(Lines.end()-1);
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        line->erase(0,2);
    }
    boost::split(vline, Lines[0], boost::is_any_of("="));
    this->num_nucs = stoi(vline[1]);
    
    //Domains
    boost::split(Lines, Domains, boost::is_any_of("\n"));
    Lines.erase(Lines.begin());
    Lines.erase(Lines.end()-1);
    Lines.erase(Lines.end()-1);
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        line->erase(0,2);
    }
    vector<string> dummy;
    PoolDomain domain;
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        boost::split(vline, *line, boost::is_any_of("\t"));
        domain.id = stoi(vline[0]);
        domain.pool_id = this->id;
        domain.pool_name = this->name;
        boost::split(dummy, vline[1], boost::is_any_of("-"));
        domain.nucs = make_pair(stoi(dummy[0]),stoi(dummy[1]));
        if(this->id == inputs->rotated_pool){
            if (inputs->rotate_by >=0){
                domain.nucs.first = circ_add(stoi(dummy[0]),inputs->rotate_by,num_nucs);
                domain.nucs.second = circ_add(stoi(dummy[1]),inputs->rotate_by,num_nucs);
            }
            else {
                domain.nucs.first = circ_substract(stoi(dummy[0]),abs(inputs->rotate_by),num_nucs);
                domain.nucs.second = circ_substract(stoi(dummy[1]),abs(inputs->rotate_by),num_nucs);
            }
        }
        if (vline[2].c_str()[0] == 'b') domain.is_body = true;
        else domain.is_body = false;
        if (vline[2].c_str()[0] == 's') domain.is_seam = true;
        else domain.is_seam = false;
        if (vline[2].c_str()[0] == 'e') domain.is_edge = true;
        else domain.is_edge = false;
        this->domains.push_back(domain);
    }
    
    
    //Staples
    boost::split(Lines, Staples, boost::is_any_of("\n"));
    Lines.erase(Lines.begin());
    Lines.erase(Lines.end()-1);
    Lines.erase(Lines.end()-1);
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        line->erase(0,2);
    }
    Staple staple;
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        boost::split(vline, *line, boost::is_any_of("\t"));
        staple.id = stoi(vline[0]);
        staple.pool_id = this->id;
        staple.is_missing = false;
        boost::split(dummy, vline[1], boost::is_any_of(","));
        staple.domains.clear();
        staple.domain_ids.clear();
        for (auto d = dummy.begin(); d!= dummy.end(); ++d){
            staple.domains.push_back(domains.begin()+stoi(*d)); //POINTER
            staple.domain_ids.push_back(stoi(*d));
        }
        this->staples.push_back(staple);
    }
    
    //Crossovers
    boost::split(Lines, Crossovers, boost::is_any_of("\n"));
    Lines.erase(Lines.begin());
    Lines.erase(Lines.end()-1);
    Lines.erase(Lines.end()-1);
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        line->erase(0,2);
    }
    Crossover crossover;
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        boost::split(vline, *line, boost::is_any_of("\t"));
        crossover.id = stoi(vline[0]);
        crossover.pool_id = this->id;
        boost::split(dummy, vline[1], boost::is_any_of("-"));
        crossover.domain_ids = make_pair(stoi(dummy[0]),stoi(dummy[1]));
        crossover.domains.first = domains.begin()+stoi(dummy[0]); //POINTER
        crossover.domains.second = domains.begin()+stoi(dummy[1]); //POINTER
        crossover.type = vline[2].c_str()[0];
        if (crossover.type == 's') crossover.is_seam = true;
        else crossover.is_seam = false;
        if (crossover.type == 'l') crossover.is_long = true;
        else crossover.is_long = false;
        this->crossovers.push_back(crossover);
    }
    
    //Helices
    boost::split(Lines, Helices, boost::is_any_of("\n"));
    Lines.erase(Lines.begin());
    Lines.erase(Lines.end()-1);
    Lines.erase(Lines.end()-1);
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        line->erase(0,2);
        //cout << *line << endl;
    }
    Helix helix;
    int hel_id = 0;
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        boost::split(dummy, *line, boost::is_any_of(":"));
        boost::algorithm::trim(dummy[0]);
        boost::algorithm::trim(dummy[1]);
        boost::split(vline, dummy[1], boost::is_any_of(" "));
        //hel_id = stoi(dummy[0]); //If we want to read from topology file
        helix.id = hel_id;
        helix.pool_id = this->id;
        helix.domain_ids.clear();
        helix.domains.clear();
        for (auto d = vline.begin(); d!= vline.end(); ++d){
            helix.domain_ids.push_back(stoi(*d));
            helix.domains.push_back(this->domains.begin()+stoi(*d)); //POINTER
            (this->domains.begin()+stoi(*d))->helix_id = hel_id; //REV
        }
        helix.pool = &(*this);
        this->helices.push_back(helix);
        hel_id++;
    }
    this->num_domains = domains.size();
    this->num_staples = staples.size();
    this->num_crossovers = crossovers.size();
    this->num_helices = helices.size();
}

void StaplePool::add_links(){
    for (auto dom = domains.begin(); dom!=domains.end(); ++dom){
        dom->length = circ_dist(dom->nucs.second, dom->nucs.first, this->num_nucs);
        dom->node_idps.first = dom->id;
        dom->node_idps.second = circ_add(dom->id, 1, num_domains);
        dom->is_middle = false;
    }
    for (auto st = staples.begin(); st!=staples.end(); ++st){
        st->nucs.first = (*(st->domains.begin()))->nucs.first;
        st->nucs.second = (*(st->domains.end()-1))->nucs.second;
        st->is_edge = false;
        st->is_seam = false;
        st->helix_ids.clear();
        st->length = 0;
        st->num_domains = 0;
        for (auto dom = st->domains.begin(); dom!= st->domains.end(); ++dom){
            if ((*dom)->is_edge) st->is_edge = true;
            if ((*dom)->is_seam) st->is_seam = true;
            st->length += (*dom)->length;
            st->num_domains++;
            st->helix_ids.push_back((*dom)->helix_id);
            (*dom)->s_index = dom-st->domains.begin(); //REV
            (*dom)->staple_id = st->id;
            (*dom)->staple = st; //REV //POINTER
        }
        if (st->num_domains == 3){
            for (auto dom = st->domains.begin(); dom!= st->domains.end(); ++dom){
                if ((*dom)->s_index == 1)
                    (*dom)->is_middle = true; //REV
            }
        }
    }
    for (auto cross = crossovers.begin(); cross!= crossovers.end(); ++cross){
        cross->nucs.first = (cross->domains.first)->nucs.second;
        cross->nucs.second = (cross->domains.second)->nucs.first;
        cross->length = circ_dist(cross->nucs.first, cross->nucs.second, this->num_nucs);
        cross->node_idps.first = (cross->domains.first)->node_idps.second;
        cross->node_idps.second = (cross->domains.second)->node_idps.first;
        (cross->domains.first)->crossover_ids.push_back(cross->id); //REV
        (cross->domains.second)->crossover_ids.push_back(cross->id); //REV
        (cross->domains.first)->crossovers.push_back(cross); //REV //POINTER
        (cross->domains.second)->crossovers.push_back(cross); //REV //POINTER
        cross->staple_id = (cross->domains.first)->staple_id;
        cross->staple = (cross->domains.first)->staple; //POINTER
        if(cross->domains.first->is_seam && cross->domains.second->is_seam && !cross->is_seam){
            cout << "Both domains of crossover are seams but crossover is not!\n";
        }
        cross->helix_ids.first = cross->domains.first->helix_id;
        cross->helix_ids.second = cross->domains.second->helix_id;
        if(abs(cross->domains.first->s_index - cross->domains.second->s_index) > 1 && !cross->is_long){
            cout << "Domains of crossover " << cross->id << " give long cross but cross is not long!\n";
        }
        (cross->staple)->crossover_ids.push_back(cross->id);
        (cross->staple)->crossovers.push_back(cross); //POINTER
        (cross->domains.first)->cross_dom_ids.push_back((cross->domains.second)->id);
        (cross->domains.second)->cross_dom_ids.push_back((cross->domains.first)->id);
        (cross->domains.first)->cross_domains.push_back(cross->domains.second);
        (cross->domains.second)->cross_domains.push_back(cross->domains.first);
    }
}

void StaplePool::add_crosspairs(){
    for (auto cross=this->crossovers.begin(); cross!=this->crossovers.end(); ++cross){
        cross->is_crosspair = false;
    }
    int counter = 0;
    for (auto cross1=this->crossovers.begin(); cross1!=this->crossovers.end(); ++cross1){
        for (auto cross2=this->crossovers.begin(); cross2!=this->crossovers.end(); ++cross2){
            if (cross1->node_idps.first == cross2->node_idps.second &&
                cross1->node_idps.second == cross2->node_idps.first &&
                cross1->id < cross2->id){
                CrossPair cpair = CrossPair();
                cpair.crossovers = make_pair(cross1,cross2);
                cpair.crossover_ids = make_pair(cross1->id,cross2->id);
                cpair.id = counter;
                cpair.pool_id = this->id;
                this->crosspairs.push_back(cpair);
                cross1->is_crosspair = true;
                cross1->crosspair_id = counter;
                cross2->is_crosspair = true;
                cross2->crosspair_id = counter;
                counter++;
            }
        }
    }
    this->num_crosspairs = crosspairs.size();
}

void StaplePool::split_helices(){
    // Find location (in the helix->domain vector) of first seam for each helix
    map<int,int> first_seam_locs;
    for (auto helix = this->helices.begin(); helix!= this->helices.end(); ++helix){
        helix->type = 'n';
        helix->is_seam = false;
        for (auto dom = helix->domains.begin(); dom!= helix->domains.end(); ++dom){
            if ((*dom)->is_seam){
                helix->is_seam = true; helix->type = 's';
                first_seam_locs[helix->id] = std::distance(helix->domains.begin(),dom);
                break;
            }
        }
    }
    // Add the index of domains for parts_ids and pointers to parts
    Part part, leftpart, rightpart;
    for (auto helix = this->helices.begin(); helix!= this->helices.end(); ++helix){
        part.domains.clear(); leftpart.domains.clear(); rightpart.domains.clear();
        part.domain_ids.clear(); leftpart.domain_ids.clear(); rightpart.domain_ids.clear();
        if (helix->is_seam){
            for (auto dom = helix->domains.begin(); dom!= helix->domains.end(); ++dom){
                if (std::distance(helix->domains.begin(),dom) <= first_seam_locs[helix->id]){
                    leftpart.domain_ids.push_back((*dom)->id);
                    leftpart.domains.push_back(this->domains.begin()+(*dom)->id);
                }
                else{
                    rightpart.domain_ids.push_back((*dom)->id);
                    rightpart.domains.push_back(this->domains.begin()+(*dom)->id);
                }
            }
            leftpart.length = leftpart.domain_ids.size();
            rightpart.length = rightpart.domain_ids.size();
            helix->parts.push_back(leftpart);
            helix->parts.push_back(rightpart);
        }
        else{
            for (auto dom = helix->domains.begin(); dom!= helix->domains.end(); ++dom){
                part.domain_ids.push_back((*dom)->id);
                part.domains.push_back(this->domains.begin()+(*dom)->id);
            }
            part.length = part.domain_ids.size();
            helix->parts.push_back(part);
        }
    }
}


void StaplePool::add_stacks(){
    PDOM stack_dom;
    for (auto dom = domains.begin(); dom!= domains.end(); ++dom){
        if (dom!=domains.end()-1)   {stack_dom = dom+1;}
        else                        {stack_dom = domains.begin();}
        if (dom->is_seam){
            if (stack_dom->is_seam){
                for (auto cross_dom = dom->cross_domains.begin(); cross_dom!= dom->cross_domains.end(); ++cross_dom){
                    if ((*cross_dom)->is_seam){
                        dom->stack_domains.push_back(domains.begin()+(*cross_dom)->id);
                        dom->stack_ids.push_back((*cross_dom)->id);
                    }
                }
            }
            else{
                dom->stack_domains.push_back(stack_dom);
                dom->stack_ids.push_back(stack_dom->id);
            }
        }
        else{
            if (!(dom->is_edge && stack_dom->is_edge)){
                dom->stack_domains.push_back(stack_dom);
                dom->stack_ids.push_back(stack_dom->id);
            }
        }
        if (dom!=domains.begin())   {stack_dom = dom-1;}
        else                        {stack_dom = domains.end()-1;}
        if (dom->is_seam){
            if (stack_dom->is_seam){
                for (auto cross_dom = dom->cross_domains.begin(); cross_dom!= dom->cross_domains.end(); ++cross_dom){
                    if ((*cross_dom)->is_seam){
                        dom->stack_domains.push_back(domains.begin()+(*cross_dom)->id);
                        dom->stack_ids.push_back((*cross_dom)->id);
                    }
                }
            }
            else{
                dom->stack_domains.push_back(stack_dom);
                dom->stack_ids.push_back(stack_dom->id);
            }
        }
        else{
            if (!(dom->is_edge && stack_dom->is_edge)) {
                dom->stack_domains.push_back(stack_dom);
                dom->stack_ids.push_back(stack_dom->id);
            }
        }
    }
}

void StaplePool::add_possible_states(){
    State_names state_names;
    for (auto st = staples.begin(); st!= staples.end(); ++st){
        if (st->num_domains == 1){
            for(int i = 0; i< s00; i++){
                st->possible_states.push_back(State_t(i));
                st->possible_state_names.push_back(state_names.array(State_t(i)));
            }
        }
        else if (st->num_domains == 2){
            for(int i = s00; i< s000; i++){
                st->possible_states.push_back(State_t(i));
                st->possible_state_names.push_back(state_names.array(State_t(i)));
            }
        }
        else if (st->num_domains == 3){
            for(int i = s000; i< State_MAX; i++){
                st->possible_states.push_back(State_t(i));
                st->possible_state_names.push_back(state_names.array(State_t(i)));
            }
        }
        else{cout << "Error:\t Only staples with up to 3 domains supported\n"; exit (EXIT_FAILURE);}
    }
    for (auto dom = domains.begin(); dom!= domains.end(); ++dom){
        dom->possible_states.push_back(false);
        dom->possible_states.push_back(true);
        dom->possible_state_names.push_back("N");
        dom->possible_state_names.push_back("B");
    }
    for (auto cross = crossovers.begin(); cross!= crossovers.end(); ++cross){
        cross->possible_states.push_back(false);
        cross->possible_states.push_back(true);
        cross->possible_state_names.push_back("N");
        cross->possible_state_names.push_back("B");
    }
    for (auto cpair = crosspairs.begin(); cpair!= crosspairs.end(); ++cpair){
        cpair->possible_states.push_back(false);
        cpair->possible_states.push_back(true);
        cpair->possible_state_names.push_back("N");
        cpair->possible_state_names.push_back("B");
    }
    for (auto helix = helices.begin(); helix!= helices.end(); ++helix){
        helix->possible_states.push_back('N');
        helix->possible_state_names.push_back("N");
        if (helix->type == 'n'){
            helix->possible_states.push_back('F');
            helix->possible_state_names.push_back("F");
        }
        if (helix->type == 's'){
            helix->possible_states.push_back('L');
            helix->possible_states.push_back('R');
            helix->possible_states.push_back('F');
            helix->possible_state_names.push_back("L");
            helix->possible_state_names.push_back("R");
            helix->possible_state_names.push_back("F");
        }
    }
    pdom_poss_states.push_back(false);
    pdom_poss_states.push_back(true);
    cross_poss_states.push_back(false);
    cross_poss_states.push_back(true);
    crosspair_poss_states.push_back(false);
    crosspair_poss_states.push_back(true);
    helix_poss_states.push_back('N');
    helix_poss_states.push_back('L');
    helix_poss_states.push_back('R');
    helix_poss_states.push_back('F');
    set<int> possible_num_domains;
    for (auto st = this->staples.begin(); st!= this->staples.end(); ++st){
        possible_num_domains.insert(st->num_domains);
    }
    if (possible_num_domains.find(1) != possible_num_domains.end()){
        for (int i = 0; i < s00; i++){
            staple_poss_states.push_back(State_t(i));
        }
    }
    if (possible_num_domains.find(2) != possible_num_domains.end()){
        for (int i = s00; i < s000; i++){
            staple_poss_states.push_back(State_t(i));
        }
    }
    if (possible_num_domains.find(3) != possible_num_domains.end()){
        for (int i = s000; i < State_MAX; i++){
            staple_poss_states.push_back(State_t(i));
        }
    }
    pdom_poss_state_names.push_back("N");
    pdom_poss_state_names.push_back("B");
    cross_poss_state_names.push_back("N");
    cross_poss_state_names.push_back("B");
    crosspair_poss_state_names.push_back("N");
    crosspair_poss_state_names.push_back("B");
    helix_poss_state_names.push_back("N");
    helix_poss_state_names.push_back("L");
    helix_poss_state_names.push_back("R");
    helix_poss_state_names.push_back("F");
    for (auto it = staple_poss_states.begin(); it!= staple_poss_states.end(); ++it){
        staple_poss_state_names.push_back(state_names.array(*it));
    }
    
}

void StaplePool::add_sequences(){
    //Adding sequences for pool domains and staples
    if (inputs->domain_energies == "seq"){
        for (auto dom = this->domains.begin(); dom!= this->domains.end(); ++dom){
            if (dom->nucs.second > dom->nucs.first){
                dom->seq = this->scaffold->seq.substr(dom->nucs.first,dom->length); //all domains except end of scaffold
            }
            else{
                dom->seq = this->scaffold->seq.substr(dom->nucs.first,scaffold->length-dom->nucs.first);
                dom->seq += this->scaffold->seq.substr(0,dom->nucs.second+1);
            }
        }
        for (auto st = this->staples.begin(); st!= this->staples.end(); ++st){
            for (auto dom = st->domains.begin(); dom!= st->domains.end(); ++dom){
                st->seq += (*dom)->seq;
            }
            st->set_reverse_comp();
        }
    }
}

void StaplePool::add_concentrations(){
    for (auto st = this->staples.begin(); st!= this->staples.end(); ++st){
        st->concentration = inputs->concentration * pow(10,-6);
    }
    if (inputs->custom_conc){
        string line_in;
        vector<std::string> line; //To split line_in
        ifstream myfile (inputs->conc_file_name);
        if (myfile.is_open()) {
            while(getline(myfile, line_in)) {
                boost::split(line, line_in, boost::is_any_of("\t"));
                this->staples[stoi(line[0])].concentration = stod(line[1]) * pow(10,-6);
            }
            myfile.close();
        }
        else {printf ("Error:\t Could not read concentration file\n"); exit (EXIT_FAILURE);}
    }
}

void StaplePool::add_energies(){
    double dH,dS;
    if (inputs->domain_energies == "average"){
        for (auto dom = domains.begin(); dom!=domains.end(); ++dom){
            dH = 0.;
            dS = 0.;
            dH += (dom->length - 1) * dH_average;
            dS += (dom->length - 1) * dS_average;
            dH += dH_init;
            dS += dS_init;
            dH += dH_termAT;
            dS += dS_termAT;
            dS += (dom->length - 1) * salt_per_phosphate_hack;
            dom->dH = dH;
            dom->dS = dS;
        }
    }
    else if (inputs->domain_energies == "seq"){
        for (auto dom = domains.begin(); dom!=domains.end(); ++dom){
            dH = 0.;
            dS = 0.;
            string sub;
            for (int i=0; i<dom->length-1; i++){
                sub.clear();
                sub += dom->seq[i];
                sub += dom->seq[i+1];
                if (sub == "AA" || sub == "TT") { dH+=dH_AA; dS+=dS_AA; }
                else if (sub == "CA" || sub == "TG") { dH+=dH_CA; dS+=dS_CA; }
                else if (sub == "GT" || sub == "AC") { dH+=dH_GT; dS+=dS_GT; }
                else if (sub == "CT" || sub == "AG") { dH+=dH_CT; dS+=dS_CT; }
                else if (sub == "GA" || sub == "TC") { dH+=dH_GA; dS+=dS_GA; }
                else if (sub == "GG" || sub == "CC") { dH+=dH_GG; dS+=dS_GG; }
                else if (sub == "AT") { dH+=dH_AT; dS+=dS_AT; }
                else if (sub == "TA") { dH+=dH_TA; dS+=dS_TA; }
                else if (sub == "CG") { dH+=dH_CG; dS+=dS_CG; }
                else if (sub == "GC") { dH+=dH_GC; dS+=dS_GC; }
                else {
                    cout << "Error! dom->seq not in nn database. \n" << dom->seq << "\n";
                    exit (EXIT_FAILURE);
                }
            }
            dH+= dH_init;
            dS+= dS_init;
            if (dom->seq[0] == 'A' || dom->seq[0] == 'T'){
                dH += dH_termAT;
                dS += dS_termAT;
            }
            if (dom->seq[dom->length-1] == 'A' || dom->seq[dom->length-1] == 'T'){
                dH += dH_termAT;
                dS += dS_termAT;
            }
            dS+= (dom->length - 1) * salt_per_phosphate_hack;
            dom->dH = dH;
            dom->dS = dS;
        }
    }
    else if (inputs->domain_energies == "custom"){
        string line_in;
        vector<std::string> line; //To split line_in
        ifstream myfile (inputs->energy_file_name);
        if (myfile.is_open()) {
            while(getline(myfile, line_in)) {
                boost::split(line, line_in, boost::is_any_of("\t"));
                this->domains[stoi(line[0])].dH = stod(line[1]);
                this->domains[stoi(line[0])].dS = stod(line[2]);
            }
            myfile.close();
        }
        else {printf ("Error:\t Could not read energy file\n"); exit (EXIT_FAILURE);}
    }
    else{
        printf ("Error:\t Specify either average, seq or custom energies.\n"); exit (EXIT_FAILURE);
    }
}


//Printers
void StaplePool::print_staples(){
    cout << "\n------- Printing Staples -------\n";
    for (auto st = staples.begin(); st != staples.end(); ++st){
        cout << st->id << "\t";
        //cout << st->state_names.array(st->state) << "\t";
        cout << st->num_domains << " doms: ";
        for (const auto& dom : st->domains){
            cout << dom->s_index << "(";
            cout << dom->id << ")";
            cout << "\t";
        }
        cout << "cross:\t";
        for (const auto& cross : st->crossovers){
            cout << cross->id << "(";
            cout << cross->domains.first->s_index << "->";
            cout << cross->domains.second->s_index << ")";
            cout << "\t";
        }
        /*
        cout << "len: " << st->length << "\t";
        cout << "conc: " << st->concentration << "\t";
        cout << "s?" << st->is_seam << "\t";
        cout << "e?" << st->is_edge << "\t";
         */
        cout << endl;
    }
}
void StaplePool::print_domains(){
    cout << "\n------- Printing Domains -------\n";
    for (auto dom = domains.begin(); dom != domains.end(); ++dom){
        cout << dom->id << "\t";
        
        //cout << dom->state << "\t";
        //cout << "len: " << dom->length << "\t";
        cout << "nucs: (" << dom->nucs.first << "->" << dom->nucs.second << ")\t";
        cout << "s? " << dom->is_seam << "\t";
        /*
        cout << "nodes: (" << dom->nodes.first->id << "->" << dom->nodes.second->id << ")\t";
        cout << "st " << dom->staple->id << "\t";
        cout << "s_index " << dom->s_index << "\t";
        cout << "s? " << dom->is_seam << "\t";
        cout << "e? " << dom->is_edge << "\t";
        cout << "dG " << dom->dH << " " << dom->dS << "\t";
        cout << "helix " << dom->helix->id << "\t";
        cout << "cross: ";
        for (auto cross = dom->crossovers.begin(); cross != dom->crossovers.end(); ++cross){
            cout << "(" << (**cross).id << ",";
        }   cout << ")\t";
        cout << "cr_dom: ";
        for (auto crd = dom->cross_domains.begin(); crd != dom->cross_domains.end(); ++crd){
            cout << "(" << (*crd)->id << ",";
        }   cout << ")\t";
        */
        /*
        cout << "st_dom: (";
        for (auto std = dom->stack_domains.begin(); std != dom->stack_domains.end(); ++std){
            cout << (*std)->id << ",";
        }   cout << ")\t";
        */
        
        cout << "child: ";
        for (auto cid = dom->children.begin(); cid != dom->children.end(); ++cid){
            cout << "(" << (*cid)->id << ",";
        }   cout << ")\t";
        /*
        cout << dom->dH << " , ";
        cout << dom->dS << "\t";
        cout << dom->seq << "\t";
        for (auto op = dom->OPs.begin(); op!= dom->OPs.end(); ++op){
            cout << (*op)->pool_id << "(" << (*op)->name << ")" << "\t";
        }
         */
        cout << endl;
    }
}
void StaplePool::print_crossovers(){
    cout << "\n------- Printing Crossovers -------\n";
    for (auto cross = crossovers.begin(); cross != crossovers.end(); ++cross){
        cout << cross->id << "\t";
        cout << cross->type << "\t";
        cout << "nodesP (" << cross->node_idps.first << "->" << cross->node_idps.second << ")\t";
        cout << "nodes (" << cross->nodes.first->id << "->" << cross->nodes.second->id << ")\t";
        if (cross->is_crosspair){
            cout << "crosspair: " << cross->crosspair->id << "\t";
        }
        /*
        cout << "nucs (" << cross->nucs.first << "->" << cross->nucs.second << ")\t";
        cout << "doms (" << cross->domains.first->id << "->" << cross->domains.second->id << ")\t";
        cout << "h(" << (cross->helices.first)->id << "->" << (cross->helices.second)->id << ")\t";
        cout << "seam? " << cross->is_seam << "\t";
        cout << "long? " << cross->is_long << "\t";
        //cout << "5->3? " << cross->is_53 << "\t";
        //cout << "Type: " << cross->type << "\t";
        cout << "staple " << cross->staple->id << "\t";
         */
        for (auto op = cross->OPs.begin(); op!= cross->OPs.end(); ++op){
            cout << (*op)->pool_id << "(" << (*op)->name << ")" << "\t";
        }
        cout << endl;
    }
}
void StaplePool::print_crosspairs(){
    cout << "\n------- Printing CrossPairs -------\n";
    for (auto cpair = crosspairs.begin(); cpair != crosspairs.end(); ++cpair){
        cout << cpair->id << "\t";
        cout << cpair->crossovers.first->id << "\t";
        cout << cpair->crossovers.second->id << "\t";
        for (auto op = cpair->OPs.begin(); op!= cpair->OPs.end(); ++op){
            cout << (*op)->pool_id << "(" << (*op)->name << ")" << "\t";
        }
        cout << endl;
    }
}
void StaplePool::print_helices(){
    cout << "\n------- Printing Helices " << name <<  " -------\n";
    for (auto helix = helices.begin(); helix != helices.end(); ++helix){
        cout << helix->id << "\t";
        cout << "Pool: " << helix->pool->id << "\t";
        cout << "Type: " << helix->type << " -----\t";
        for (auto part = helix->parts.begin(); part!= helix->parts.end(); ++part){
            for (auto dom = part->domains.begin(); dom!= part->domains.end(); ++dom){
                cout << std::distance(helix->parts.begin(),part);
                cout << "(";
                cout << (*dom)->id;
                cout << ")";
                cout << " ";
            }
        }
        cout << endl;
    }
}
void StaplePool::print_OPs(){
    OP_names names;
    cout << "\n---------------- Printing OPs " << this->name << " ----------------\n\n";
    for (auto op = OPs.begin(); op != OPs.end(); ++op){
        cout << op->name << "\t";
        cout << names.array(op->type) << "\t";
        cout << this->name << "\t";
        cout << op->group_ids.size() << "\t";
        cout << "\n";
    }
}
void StaplePool::print(){
    cout << "\n---------------- Printing Staple Pool " << name <<  " ----------------\n\n";
    print_domains();
    print_crossovers();
    print_staples();
    print_helices();
}


